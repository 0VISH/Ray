namespace CPU{
    void draw(u8 *frameBuffer){
        for(u32 y=0; y<IMG_Y; y++){
            f32 v = (f32)y/(float)IMG_Y;
            v = 1 - v*2;
            for(u32 x=0; x<IMG_X*4; x+=4){
                f32 red = 0;
                f32 green = 0;
                f32 blue = 0;
                f32 u = (f32)(x/4)/(float)IMG_X;
                u = u*2 - 1;

                Vec3 spherePos(0, 0, 0);
                f32 radius = 0.5;
                Vec3 camPos(0, 0, 2);
                Vec3 dir = Vec3(u, v, -1);
                dir = normalize(dir);

                f32 a = dot(dir, dir);
                f32 b = 2.0 * dot(camPos, dir);
                f32 c = dot(camPos, camPos) - radius*radius;

                f32 discriminant = (b*b) - (4.0*a*c);
                if(discriminant > 0){
                    f32 t = (-b - (f32)sqrt(discriminant))/((f32)2.0*a);
                    Vec3 hitPoint = camPos + (dir*t);
                    hitPoint = normalize(hitPoint);
                    Vec3 lightSrc(-1, -1, -1);
                    lightSrc = normalize(lightSrc);
                    f32 alpha = dot(hitPoint, lightSrc*-1);
                    red = alpha;
                    blue = alpha;
                };

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