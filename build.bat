@echo off

if not exist bin\ (
    mkdir bin\
)
if not exist bin\imgui.obj (
    cl /nologo /Iimgui/ src/imgui.cc -c /Z7 /Fo:bin/imgui.obj
)

cl /nologo /Iimgui/ src/main.cc -c /Z7 /Fo:bin/ray.obj
link /NOLOGO bin/ray.obj bin/imgui.obj /OUT:bin/ray.exe

if %errorlevel% equ 0 (
    bin\ray.exe
)