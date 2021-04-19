#include "src/K32-lite/light/K32_light.h"


// ANIM CHASER 
// 0: position
// 1: size
//

class Anim_chaser : public K32_anim
{
public:

    int lastPos = 0;

    // Internal modulators
    void init () 
    {
        this->mod("chase", new K32_mod_sinus)->maxi(this->size()-1)->play();
    }

    // Loop
    void draw(int data[ANIM_DATA_SLOTS])
    {   
        // Clear
        this->clear();

        // Set speed
        mod("chase")->period( data[0] );
        
        // Get current position
        int pos = mod("chase")->value();

        // Size
        int& sizez = data[1];

        CRGBW color(255,255,255,255);

        // this->pixel( pos, CRGBW::GhostWhite);
        this->all(color);

        int amp = sizez/2;
        for (int k=0; k < amp; k++) {
            this->pixel( pos-k, CRGBW::Black);
            this->pixel( pos+k, CRGBW::Black);
        }


        lastPos = pos;
    }
};