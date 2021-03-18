/*
  K32_mods.h
  Created by Thomas BOHL, March 2020.
  Released under GPL v3.0
*/
#ifndef K32_mods_h
#define K32_mods_h

#include "K32_anim.h"


/*
USAGE: 
Modulators needs to define at least the method   int value() 

This method returns a (int) value generated by the modulator based on internal parameters.
Warning: this value will be CLAMPED to 0->255 by modulator run() function

The common parameters (available for all modulators):

  int period()                    = period length  (default = 1000)
  int phase()                     = phase value    (default = 0)
  int maxi()                      = maximum value  (default = 255)
  int mini()                      = minimum value  (default = 0)
  int amplitude()                 = maxi-mini     

  uint32_t time()                 = current time if modulator is playing or freezeTime if mod is paused. 
                                      if using K32_modulator_periodic: time is corrected with phase (1/360 of period)
                                      if using K32_modulator_trigger: time is based on play() time and corrected with phase as fixed delay (ms)
                                      
  float progress()                = % of progress in period between 0.0 and 1.0 
  int periodCount()               = count the number of period iteration since time()


Modulator can also access / modify those generic parameters:

  int params[MOD_PARAMS_SLOTS]    = modulator generic parameters, set by external users, can be renamed for convenience using local int& attribute

*/


/////////////////////////////////////////////////////////////////////////////////
////////////////////////  PERIODIC TYPE MODULATOS ////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


//
// SINUS
//
class K32_mod_sinus : public K32_modulator_periodic {
  public:  
    
    int value()
    {
      return (0.5f + 0.5f * sin(2 * PI * progress())) * amplitude() + mini();
    };
  
};

//
// TRIANGLE
//
class K32_mod_triangle : public K32_modulator_periodic {
  public:  

    int value()
    { 
      float percent = progress();
      if (percent > 0.5) percent = 1 - percent;

      // LOGF5("tri %d %d %d %d %d\n", mini(), maxi(), percent, time(), period());
      return 2*percent * amplitude() + mini();
    };
  
};

//
// SAW TOOTH
//
class K32_mod_sawtooth : public K32_modulator_periodic {
  public:  

    int value()
    {
      return progress() * amplitude() + mini();
    };
  
};

//
// SAW TOOTH INVERTED
//
class K32_mod_isawtooth : public K32_modulator_periodic {
  public:  

    int value()
    {
      return maxi() - progress() * amplitude();
    };

};

//
// PULSE
//
class K32_mod_pulse : public K32_modulator_periodic {
  public:  

    // named link to params 
    int& widthMS  = params[0];  // pulse ON width in milliseconds
    int& widthPCT = params[1];  // pulse ON width in percentage (activated if widthMS = 0)

    int value()
    {  
      int width = widthMS;
      if (widthMS == 0) width = period()*widthPCT/100;
      if ( time() % period() < width) return maxi();
      else return mini();
    };

};

//
// MULTI PULSE
//
class K32_mod_multipulse : public K32_modulator_periodic {
  public:  

    // named link to params 
    int& widthMS  = params[0];  // pulse ON width in milliseconds
    int& widthPCT = params[1];  // pulse ON width in percentage (activated if widthMS = 0)

    int value()
    {  
      int width = widthMS;
      if (widthMS == 0) width = period()*widthPCT/100;
      if ( time() % period() < width) return maxi();
      else return mini();
    };

};


//
// RANDOM
//
class K32_mod_random : public K32_modulator_periodic {
  public:  

    // internal attribute
    int lastPeriod = -1;
    uint8_t lastValue = 255;

    int value()
    {
      int newPeriod = periodCount();
      if (newPeriod != lastPeriod) {
        lastPeriod = newPeriod;
        lastValue = random(mini(), maxi());
      }
      return lastValue;
    };
  
};


/////////////////////////////////////////////////////////////////////////////////
////////////////////////  TRIGGER TYPE MODULATOS ////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

//
// FADE IN
//
class K32_mod_fadein : public K32_modulator_trigger {
  public:  
    
    int value()
    {   
      // not yet ready
      if (time() < 0) return 255; 

      // end of modulation
      if (time() >= period()) 
      {
        stop();
        return maxi();
      }
      
      return progress() * amplitude() + mini();

    };
};

//
// FADE OUT
//
class K32_mod_fadeout : public K32_modulator_trigger {
  public:  
    
    int value()
    { 
      // not yet ready
      if (time() < 0) return 255; 

      // end of modulation
      if (time() >= period()) 
      {
        stop();
        return mini();
      }
      
      return maxi() - progress() * amplitude();
      
    };
};



#endif
