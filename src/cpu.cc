namespace CPU{
    void draw(u8 *frameBuffer){
        for(u32 y=0; y<IMG_Y; y++){
            f32 v = (f32)y/(float)IMG_Y;
            v = v*2 - 1;
            for(u32 x=0; x<IMG_X*4; x+=4){
                u32 red = 0;
                u32 green = 0;
                u32 blue = 0;
                f32 u = (f32)(x/4)/(float)IMG_X;
                u = u*2 - 1;

                Vec3 spherePos(0, 0, 0);
                f32 radius = 0.5;
                Vec3 camPos(0, 0, 2);
                Vec3 dir = Vec3(u, v, -1);

                f32 a = dot(dir, dir);
                f32 b = 2.0 * dot(camPos, dir);
                f32 c = dot(camPos, camPos) - radius*radius;

                f32 discriminant = (b*b) - (4.0*a*c);
                //printf("%f %f %f %f %f %f %f\n", (b*b) , (4.0*a*c), a, b, c, u, v);
                if(discriminant > 0){
                    red = 255;
                    blue = 255;
                };

                frameBuffer[(y*IMG_X*4) + x] = red;
                frameBuffer[(y*IMG_X*4) + x+1] = green;
                frameBuffer[(y*IMG_X*4) + x+2] = blue;
                frameBuffer[(y*IMG_X*4) + x+3] = 255;
            };
        };
    };
};