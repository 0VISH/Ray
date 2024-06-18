namespace CPU{
    void draw(Scene &scene, u8 *frameBuffer){
        for(u32 y=0; y<IMG_Y; y++){
            f32 v = (f32)y/(float)IMG_Y;
            v = 1 - v*2;
            for(u32 x=0; x<IMG_X*4; x+=4){
                f32 red = 0;
                f32 green = 0;
                f32 blue = 0;
                f32 u = (f32)(x/4)/(float)IMG_X;
                u = u*2 - 1;

                Sphere *closestS = nullptr;
                f32 closestT = FLT_MAX;
                Vec3 camPos(0, 0, 2);
                Vec3 dir = Vec3(u, v, -1);
                dir = normalize(dir);
                for(u32 x=0; x<scene.spheres.size(); x++){
                    Sphere &sphere = scene.spheres[x];
                    Vec3 spherePos = sphere.pos;    
                    f32 radius = sphere.radius;
                    Vec3 pos = camPos - spherePos;

                    f32 a = dot(dir, dir);
                    f32 b = 2.0 * dot(pos, dir);
                    f32 c = dot(pos, pos) - radius*radius;

                    f32 discriminant = (b*b) - (4.0*a*c);
                    if(discriminant > 0){
                        f32 t = (-b - (f32)sqrt(discriminant))/((f32)2.0*a);
                        if(t < closestT){
                            closestT = t;
                            closestS = &sphere;
                        };
                    };
                };
                if(closestS){
                    Vec3 pos = camPos - closestS->pos;
                    Vec3 hitPoint = pos + (dir*closestT);
                    hitPoint = normalize(hitPoint);
                    Vec3 lightSrc(-1, -1, -1);
                    lightSrc = normalize(lightSrc);
                    f32 alpha = dot(hitPoint, lightSrc*-1);
                    red = alpha * closestS->col.x;
                    green = alpha * closestS->col.y;
                    blue = alpha * closestS->col.z;
                }

                if(red < 0) red = 0;
                if(green < 0) green = 0;
                if(blue < 0) blue = 0;
                frameBuffer[(y*IMG_X*4) + x] = red*255;
                frameBuffer[(y*IMG_X*4) + x+1] = green*255;
                frameBuffer[(y*IMG_X*4) + x+2] = blue*255;
                frameBuffer[(y*IMG_X*4) + x+3] = 255;
            };
        };
    };
};