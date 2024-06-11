namespace CPU{
    void draw(u8 *frameBuffer){
        for(u32 y=0; y<IMG_Y; y++) {
            for(u32 x=0; x<IMG_X*4; x+=4){
                frameBuffer[(y*IMG_X*4) + x] = 0;
                frameBuffer[(y*IMG_X*4) + x+1] = 255;
                frameBuffer[(y*IMG_X*4) + x+2] = 0;
                frameBuffer[(y*IMG_X*4) + x+3] = 255;
            };
        };
    };
};