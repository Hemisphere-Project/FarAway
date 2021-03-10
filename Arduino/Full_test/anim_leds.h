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
        int& size = data[1];
        
        // Forward
        if (pos > lastPos) 
            for(int k=max(0, pos-size); k <= pos; k++)
                this->pixel( k, CRGBW::GhostWhite);
        
        // Backward
        else
            for(int k=max(0, pos+size); k <= pos; k++)
                this->pixel( k, CRGBW::GhostWhite);


        lastPos = pos;
    }
};