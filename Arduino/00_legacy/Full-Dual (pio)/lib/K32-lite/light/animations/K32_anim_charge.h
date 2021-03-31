/*
  K32_anim_charge.h
  Created by Thomas BOHL, february 2019.
  Released under GPL v3.0
*/
#ifndef K32_anim_charge_h
#define K32_anim_charge_h

//
// NOTE: to be available, add #include to this file in K32_light.h !
//


//
// DISCHARGE : Anim for battery gauge display
//
class K32_anim_discharge : public K32_anim {
  public:

    void draw (int data[ANIM_DATA_SLOTS])
    {
      int& stateCharge = data[0];   // State of charge
      int& powerValue  = data[1];   // Power value (0 - 300)

      int length = this->size();

      CRGBW color1 = CRGBW{0, 100, 0};
      CRGBW color2 = CRGBW{100, 75, 0};
      
      for (int i=0; i<length; i ++)
      {
        /* First color below SOC */
        if(i<((stateCharge*length/100)/4))
          this->pixel(i,color1);
  
        else if ((i>=length/2-(stateCharge*length/100)/4)&&(i<length/2 + (stateCharge*length/100)/4))
          this->pixel(i,color1);
         
        else if (i>=length-(stateCharge*length/100)/4)
          this->pixel(i,color1);
        
        else /* Second color */
          this->pixel(i,color2);
        
      }

      /* Fleche Mode */
      this->pixel( (stateCharge*length/100)/4, color1);
      this->pixel( length - (stateCharge*length/100)/4 - 1, color1);

      vTaskDelay(pdMS_TO_TICKS(max(400 - powerValue*2,50) ));

      /* Blinking */
      for (int i=0; i<=powerValue/100+3; i ++)
      {
        

        /* Normal mode */
        // this->pixel( (stateCharge*length/100)/4-1 - i , color2);
        // this->pixel( length - (stateCharge*length/100)/4+i, color2);

        /* Fleche mode */
        this->pixel( (stateCharge*length/100)/4 - i , color2);
        this->pixel( length - (stateCharge*length/100)/4-1+i, color2);
        this->pixel( length/2-(stateCharge*length/100)/4 + i, color2);
        this->pixel( length/2 + (stateCharge*length/100)/4-1-i, color2);

        vTaskDelay(pdMS_TO_TICKS(max(400 - powerValue*2,50) ));
      }

      /* Leds OFFs */
      if (stateCharge > 0) {
        if (millis() > (this->startTime+stateCharge*1000)) {
          this->clear();
          this->loop(false);
        }
      }

    };
};

//
// CHARGE : Anim for battery gauge display
//
class K32_anim_charge : public K32_anim {
  public:

    void draw (int data[ANIM_DATA_SLOTS])
    {
      int& stateCharge = data[0];   // State of charge
      int& powerValue  = data[1];   // Power value (0 - 300)

      int length = this->size();

      CRGBW color1 = CRGBW(0, 100, 0);
      CRGBW color2 = CRGBW(165, 110, 0);

      for (int i=0; i<length; i ++)
      {
        /* First color below SOC */
        if(i<(stateCharge*length/100)/4)
          this->pixel(i,color1);
        
        else if ((i>=length/2-(stateCharge*length/100)/4)&&(i<length/2 + (stateCharge*length/100)/4))
          this->pixel(i,color1);
        
        else if (i>=length-(stateCharge*length/100)/4)
          this->pixel(i,color1);
        
        else /* Second color */
          this->pixel(i,color2);
        
      }

      /* Fleche mode */
      this->pixel( (stateCharge*length/100)/4 -1, color2);
      this->pixel( length - (stateCharge*length/100)/4, color2);

      vTaskDelay(pdMS_TO_TICKS(max(800 - powerValue*2,50) ));

      /* Blinking */
      for (int i=0; i<=powerValue/100+1; i ++)
      {
        /* Normal mode */
        // this->pixel( (stateCharge*length/100)/4 + i, color1);
        // this->pixel( length - (stateCharge*length/100)/4 -1 - i, color1);

        /* Fleche mode */
        this->pixel( (stateCharge*length/100)/4 -1 + i, color1);
        this->pixel( length - (stateCharge*length/100)/4  - i, color1);
        this->pixel( length/2-(stateCharge*length/100)/4 - 1 - i, color1);
        this->pixel( length/2 + (stateCharge*length/100)/4 + i, color1);

        vTaskDelay(pdMS_TO_TICKS(max(800 - powerValue*2,50)));
      }

    };
};


#endif
