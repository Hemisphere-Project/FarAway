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
        this->mod("chase", new K32_mod_isawtooth)->maxi(this->size()-1)->play();
        this->mod("bleep", new K32_mod_pulse)->mini(0)->maxi(255)->period(1000)->param(0,0)->param(1,10)->play();
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


        int red = 100;
        CRGBW color(red,0,0,0);

        this->all(color);

        // BLACK CHASER
        int amp = sizez/2;
        for (int k=0; k < amp; k++) {
            CRGBW colorChase( max( red,  255 * (amp-k)/amp ) ,0,0,0);
            this->pixel( pos-k, colorChase);
            this->pixel( pos+k, colorChase);
        }

        // WHITE HEAD
        // CRGBW dot(mod("bleep")->value(), 0, 0, 0);
        // this->pixel( 0, dot);
        // this->pixel( 1, dot);
        // this->pixel( 2, CRGBW::Black);


        lastPos = pos;
    }
};