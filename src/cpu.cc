namespace CPU{
    inline f32 random(){return ((double)rand()/(RAND_MAX));};
    HitPayload miss(Scene &scene, Ray &ray){
        HitPayload payload;
        payload.objectIndex = -1;
        return payload;
    };
    HitPayload hit(Scene &scene, Ray &ray, f32 hitDistance, s32 objectIndex){
        HitPayload payload;
        payload.hitDistance = hitDistance;
        payload.objectIndex = objectIndex;

        Sphere &sphere = scene.spheres[objectIndex];
        Vec3 origin = ray.origin - sphere.pos;
        payload.pos = origin + (ray.direction * hitDistance);
        payload.normal = normalize(payload.pos);
        payload.pos = payload.pos + sphere.pos;

        return payload;
    };
    HitPayload TraceRay(Scene &scene, Ray &ray){
        s32 closestS = -1;
        f32 closestT = FLT_MAX;
        for(u32 x=0; x<scene.spheres.size(); x++){
            Sphere &sphere = scene.spheres[x];
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
    void draw(Scene &scene, u8 *frameBuffer){
        const u32 antialiasing = 1;
        const u32 bounces = 2;
        for(u32 y=0; y<IMG_Y; y++){
            for(u32 x=0; x<IMG_X*4; x+=4){
                f32 red = 0;
                f32 green = 0;
                f32 blue = 0;
                for(u32 anti=0; anti<antialiasing; anti++){
                    f32 r = random();
                    f32 v = (f32)(y+r)/(float)IMG_Y;
                    v = 1 - v*2;
                    f32 u = (f32)((x/4)+r)/(float)IMG_X;
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
                            red += multiplier * 0.0;
                            green += multiplier * 0.0;
                            blue += multiplier * 0.0;
                            break;
                        };
                        Sphere &sphere = scene.spheres[payload.objectIndex];
                        Vec3 lightSrc(-1, -1, -1);
                        lightSrc = normalize(lightSrc);
                        f32 alpha = dot(payload.normal, lightSrc*-1);
                        if(alpha < 0.0) alpha = 0;
                        alpha *= multiplier;
                        red += alpha * sphere.col.x;
                        green += alpha * sphere.col.y;
                        blue += alpha * sphere.col.z;
                        multiplier *= 0.7f;

                        ray.origin = payload.pos + (payload.normal*0.0001f);
                        ray.direction = reflect(ray.direction, payload.normal);
                    };
                };

                red /= antialiasing;
                green /= antialiasing;
                blue /= antialiasing;
                if(red < 0) red = 0;
                if(green < 0) green = 0;
                if(blue < 0) blue = 0;
                if(red > 1) red = 1;
                if(green > 1) green = 1;
                if(blue > 1) blue = 1;
                frameBuffer[(y*IMG_X*4) + x] = red*255;
                frameBuffer[(y*IMG_X*4) + x+1] = green*255;
                frameBuffer[(y*IMG_X*4) + x+2] = blue*255;
                frameBuffer[(y*IMG_X*4) + x+3] = 255;
            };
        };
    };
};