/*
  K32_anim_basics.h
  Created by Thomas BOHL, february 2019.
  Released under GPL v3.0
*/
#ifndef K32_anim_basics_h
#define K32_anim_basics_h

//
// NOTE: to be available, add #include to this file in K32_light.h !
//


//
// TEST
//


class K32_anim_test : public K32_anim {
  public:
    
    // Setup
    void init() {
      this->loop(false);
    }

    // Loop
    void draw (int data[ANIM_DATA_SLOTS])
    {
      int stepMS = data[0];

      this->all( CRGBW{255,0,0} );
      this->pause(stepMS);

      this->all( CRGBW{0,255,0} );
      this->pause(stepMS);

      this->all( CRGBW{0,0,255} );
      this->pause(stepMS);

      this->all( CRGBW{0,0,0,255} );
      this->pause(stepMS);

      this->clear();
    };
};


//
// FULLCOLOR
//
class K32_anim_color : public K32_anim {
  public:

    // Setup
    void init() {}

    // Loop
    void draw(int data[ANIM_DATA_SLOTS])
    {
      CRGBW color {data[0], data[1], data[2], data[3]};
      this->all( color );
    };
};




#endif
