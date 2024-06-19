#include <stdio.h>
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_win32.h"
#include "window.cc"
#include "basic.hh"
#include "math.cc"
#include "world.cc"

#define IMG_X 500
#define IMG_Y 500

#include "cpu.cc"

s32 main(){
    WNDCLASSEXW wc = { sizeof(wc), CS_OWNDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Ray", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    if (!CreateDeviceWGL(hwnd, &g_MainWindow)){
        CleanupDeviceWGL(hwnd, &g_MainWindow);
        ::DestroyWindow(hwnd);
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }
    wglMakeCurrent(g_MainWindow.hDC, g_hRC);

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_InitForOpenGL(hwnd);
    ImGui_ImplOpenGL3_Init();

    u8 *frameBuffer = (u8*)malloc(IMG_X*IMG_Y*4);
    GLuint frameBufferTexture;
    glGenTextures(1, &frameBufferTexture);
    glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, IMG_X, IMG_Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    Scene scene;
    Sphere sph1;
    Sphere sph2;
    sph2.radius = 100;
    sph2.pos.x = 1.0;
    sph2.pos.y = -100.6;
    sph2.pos.z = 0.0;
    sph2.col.x = 1.0;
    sph2.col.y = 0.0;
    sph2.col.z = 0.5;
    scene.spheres.push_back(sph1);
    scene.spheres.push_back(sph2);

    while(true){
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)){
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if(msg.message == WM_QUIT) goto CLEAN_UP;
        }
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Scene");
            ImGui::Text("frame_rate: %f    time: %f", ImGui::GetIO().Framerate, 1/ImGui::GetIO().Framerate);
            CPU::draw(scene, frameBuffer);
            glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, IMG_X, IMG_Y, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer);
            ImGui::Image((ImTextureID)frameBufferTexture, ImVec2(IMG_X, IMG_Y));
            ImGui::End();
        }
        {
            ImGui::Begin("Scene Settings");
            for(u32 x=0; x<scene.spheres.size(); x++){
                Sphere &sphere = scene.spheres[x];
                ImGui::PushID(x);
                ImGui::DragFloat("Radius", &sphere.radius, 0.1f);
                ImGui::DragFloat("Pos_X", &sphere.pos.x, 0.1f);
                ImGui::DragFloat("Pos_Y", &sphere.pos.y, 0.1f);
                ImGui::DragFloat("Pos_Z", &sphere.pos.z, 0.1f);
                ImGui::DragFloat("Col_R", &sphere.col.x, 0.1f, 0.0, 1.0);
                ImGui::DragFloat("Col_G", &sphere.col.y, 0.1f, 0.0, 1.0);
                ImGui::DragFloat("Col_B", &sphere.col.z, 0.1f, 0.0, 1.0);
                ImGui::Separator();
                ImGui::PopID();
            };
            ImGui::End();
        }

        ImGui::Render();
        glViewport(0, 0, g_Width, g_Height);
        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ::SwapBuffers(g_MainWindow.hDC);
    }
CLEAN_UP:
    free(frameBuffer);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceWGL(hwnd, &g_MainWindow);
    wglDeleteContext(g_hRC);
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}
