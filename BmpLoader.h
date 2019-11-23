#ifdef BMPLOADER_H
#define BMPLOADER_H

#include<ios.h>
#include<iostream.h>


class BmpLoader {
    
    public:
        unsigned char* textureData;
        int iWidth, iHeigth;
        BmpLoader(const char*);
        ~BmpLoader();
    };

    private:
        BITMAPINFO
}

#endif
