#include <execution>
#include <algorithm>

namespace CPU{
    Vec3 *accumulation;
    u32 frameCount;
    //for multi-threading
    std::vector<u32> imageY;
    Scene *scene;
    u8 *frameBuffer;

    void init(){
        accumulation = (Vec3*)malloc(sizeof(Vec3) * IMG_X * IMG_Y);
        frameCount = 1;
        imageY.resize(IMG_Y);
        for(u32 y=0; y<IMG_Y; y++) imageY[y] = y;
    };
    void uninit(){free(accumulation);};
    inline f32 random(){return (f32)rand()/RAND_MAX;};
    HitPayload miss(Scene *scene, Ray &ray){
        HitPayload payload;
        payload.objectIndex = -1;
        return payload;
    };
    HitPayload hit(Scene *scene, Ray &ray, f32 hitDistance, s32 objectIndex){
        HitPayload payload;
        payload.hitDistance = hitDistance;
        payload.objectIndex = objectIndex;

        Sphere &sphere = scene->spheres[objectIndex];
        Vec3 origin = ray.origin - sphere.pos;
        payload.pos = origin + (ray.direction * hitDistance);
        payload.normal = normalize(payload.pos);
        payload.pos = payload.pos + sphere.pos;

        return payload;
    };
    HitPayload TraceRay(Scene *scene, Ray &ray){
        s32 closestS = -1;
        f32 closestT = FLT_MAX;
        for(u32 x=0; x<scene->spheres.size(); x++){
            Sphere &sphere = scene->spheres[x];
            Vec3 spherePos = sphere.pos;    
            f32 radius = sphere.radius;
            Vec3 pos = ray.origin - spherePos;
            Vec3 dir = ray.direction;

            f32 a = dot(dir, dir);
            f32 b = 2.0 * dot(pos, dir);
            f32 c = dot(pos, pos) - radius*radius;

            f32 discriminant = (b*b) - (4.0*a*c);
            if(discriminant > 0){
                f32 t = (-b - (f32)sqrt(discriminant))/((f32)2.0*a);
                if(t > 0 && t < closestT){
                    closestT = t;
                    closestS = x;
                };
            };
        };
        if(closestS != -1) return hit(scene, ray, closestT, closestS);
        return miss(scene, ray);
    };
    void draw(Scene *sceneArg, u8 *frameBufferArg){
        if(frameCount == 1) memset(accumulation, 0, sizeof(Vec3)*IMG_X*IMG_Y);
        scene = sceneArg;
        frameBuffer = frameBufferArg;
        std::for_each(std::execution::par, imageY.begin(), imageY.end(),
            [&](u32 y){
                const u32 antialiasing = 1;
                const u32 bounces = 2;
                for(u32 xPixel=0; xPixel<IMG_X*4; xPixel+=4){
                    f32 red = 0;
                    f32 green = 0;
                    f32 blue = 0;
                    u32 x = xPixel/4;
                    for(u32 anti=0; anti<antialiasing; anti++){
                        f32 r = random();
                        f32 v = (f32)(y+r)/(float)IMG_Y;
                        v = 1 - v*2;
                        f32 u = (f32)(x+r)/(float)IMG_X;
                        u = u*2 - 1;
                        Vec3 camPos(0, 0, 2);
                        Vec3 dir = Vec3(u, v, -1);
                        dir = normalize(dir);
                        Ray ray;
                        ray.origin = camPos;
                        ray.direction = dir;
                        f32 multiplier = 1.0f;
                        for(u32 bounce=0; bounce<bounces; bounce++){    
                            HitPayload payload = TraceRay(scene, ray);
                            if(payload.objectIndex == -1){
                                red += multiplier * 0.7;
                                green += multiplier * 0.6;
                                blue += multiplier * 0.9;
                                break;
                            };
                            Sphere &sphere = scene->spheres[payload.objectIndex];
                            Vec3 lightSrc(-1, -1, -1);
                            lightSrc = normalize(lightSrc);
                            f32 alpha = dot(payload.normal, lightSrc*-1);
                            if(alpha < 0.0) alpha = 0;
                            alpha *= multiplier;
                            red += alpha * sphere.mat.col.x;
                            green += alpha * sphere.mat.col.y;
                            blue += alpha * sphere.mat.col.z;
                            multiplier *= 0.7f;

                            ray.origin = payload.pos + (payload.normal*0.0001f);
                            Vec3 offsetNormal(random() - 0.5, random() - 0.5, random() - 0.5);
                            ray.direction = reflect(ray.direction, payload.normal + offsetNormal*sphere.mat.roughness);
                        };
                    };

                    red /= antialiasing;
                    green /= antialiasing;
                    blue /= antialiasing;
                    accumulation[x + y*IMG_X] = accumulation[x + y*IMG_X] + Vec3(red, green, blue);
                    Vec3 col = accumulation[x + y*IMG_X];
                    col = col / (f32)frameCount;
                    if(col.x < 0) col.x = 0;
                    if(col.y < 0) col.y = 0;
                    if(col.z < 0) col.z = 0;
                    if(col.x > 1) col.x = 1;
                    if(col.y > 1) col.y = 1;
                    if(col.z > 1) col.z = 1;
                    frameBuffer[(y*IMG_X*4) + xPixel] = col.x*255;
                    frameBuffer[(y*IMG_X*4) + xPixel+1] = col.y*255;
                    frameBuffer[(y*IMG_X*4) + xPixel+2] = col.z*255;
                    frameBuffer[(y*IMG_X*4) + xPixel+3] = 255;
                };
          }
        );
        frameCount++;
    };
};