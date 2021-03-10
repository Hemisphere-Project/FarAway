/*
  K32_anim.h
  Created by Thomas BOHL, March 2020.
  Released under GPL v3.0
*/
#ifndef K32_anim_h
#define K32_anim_h

#define ANIM_DATA_SLOTS  32
#define ANIM_MOD_SLOTS  16

#include "K32_ledstrip.h"
#include "K32_modulator.h"


//
// BASE ANIM
//
class K32_anim {
  public:
    K32_anim()
    {
      this->_strip = NULL;
      memset(this->_data, 0, sizeof this->_data);

      for (int k=0; k<ANIM_MOD_SLOTS; k++)
        this->_modulators[k] = NULL;

      this->newData = xSemaphoreCreateBinary();
      this->bufferInUse = xSemaphoreCreateBinary();
      xSemaphoreTake(this->newData, 1);
      xSemaphoreGive(this->bufferInUse);

      this->wait_lock = xSemaphoreCreateBinary();
      xSemaphoreGive(this->wait_lock);
    }

    virtual ~K32_anim() {
      vQueueDelete(this->newData);
      vQueueDelete(this->bufferInUse);
      vQueueDelete(this->wait_lock);
    }

    K32_anim* setup(K32_ledstrip* strip, int size = 0, int offset = 0) {
      this->_strip = strip;
      this->_size = (size) ? size : strip->size();
      this->_offset = offset;
      return this;
    }
    
    // ANIM CONTROLS
    //

    // get/set name
    String name () {  return this->_name;  }
    void name (String n) {  this->_name = n; }

    int size() {
      return this->_size;
    }

    // loop get
    bool loop() { 
      return this->_loop;
    }

    // loop set
    K32_anim* loop(bool doLoop) { 
      this->_loop = doLoop;
      return this;
    }

    // is playing ?
    bool isPlaying() {
      return this->animateHandle != NULL;
    }

    // start animate thread
    K32_anim* play(int timeout = 0) 
    {
      if (this->_strip == NULL) {
        LOG("ERROR: animation, you need to call setup before play !");
        return this;
      }
      this->_stopAt = (timeout>0) ? millis() + timeout : 0;

      if (this->isPlaying()) return this;

      xSemaphoreTake(this->wait_lock, portMAX_DELAY);
      xSemaphoreTake(this->bufferInUse, portMAX_DELAY);
      xTaskCreate( this->animate,           // function
                    "anim_task",            // task name
                    5000,                   // stack memory
                    (void*)this,            // args
                    3,                      // priority
                    &this->animateHandle ); // handler
      LOGF("ANIM: %s play \n", this->name().c_str() );
      
      return this;
    }

    // stop animation
    void stop() { 
      this->_stopAt = 1; 
      this->wait(); // !!! stop might hang an entire frame (but it safer to prevent clear conflict)
    }

    // wait until anim end (or timeout)
    bool wait(int timeout = 0) 
    {
      TickType_t xTicksToWait = portMAX_DELAY;
      if (timeout > 0) xTicksToWait = pdMS_TO_TICKS(timeout);

      if ( xSemaphoreTake(this->wait_lock, xTicksToWait) == pdTRUE) {
        xSemaphoreGive(this->wait_lock);
        return true;
      }
      return false;
    }

    // register new modulator
    K32_modulator* mod(K32_modulator* modulator, bool playNow = true) 
    { 
      int i = -1;
      for (int k=0; k<ANIM_MOD_SLOTS; k++)
        if(this->_modulators[k] == NULL) {
          i = k;
          break;
        }

      if (i < 0) {
        LOG("ERROR: no more slot available to register new modulator");
        return modulator;
      }
      
      this->_modulators[i] = modulator;
      // LOGF2("ANIM: %s register mod %s \n", this->name(), modulator->name()); 
      
      if (playNow) modulator->play();

      return modulator;
    }

    // register new modulator (named)
    K32_modulator* mod( String modName, K32_modulator* modulator, bool playNow = false) 
    {
      modulator->name(modName);

      // delete already existing mod with this name (replace)
      for (int k=0; k<ANIM_MOD_SLOTS; k++)
        if(this->_modulators[k] != NULL) 
          if (this->_modulators[k]->name() == modName)
          {
            LOGF("ANIM: %s replaced !\n", this->_modulators[k]->name());
            this->_modulators[k]->stop();
            delete this->_modulators[k];
            this->_modulators[k] = NULL;
          }

      return this->mod( modulator, playNow );
    }

    // Get mod by index
    K32_modulator* mod( int k )  {
      if (k < ANIM_MOD_SLOTS && this->_modulators[k] != NULL) 
        return this->_modulators[k];

      LOGF2("ANIM: %s mod not found: %d \n", this->name(), k);
      return new K32_modulator();
    }

    // get registered modulator
    K32_modulator* mod( String modName) 
    {
      for (int k=0; k<ANIM_MOD_SLOTS; k++)
        if(this->_modulators[k] != NULL)
          if (this->_modulators[k]->name() == modName) {
            // LOGINL("LIGHT: "); LOG(name);
            return this->_modulators[k];
          }

      LOGF2("ANIM: %s mod not found: %s \n", this->name(), modName);
      return new K32_modulator();
    }

    // search registered modulator
    bool hasmod( String modName) 
    {
      for (int k=0; k<ANIM_MOD_SLOTS; k++)
        if(this->_modulators[k] != NULL)
          if (this->_modulators[k]->name() == modName)
            return true;
      return false;
    }

    // search registered modulator
    bool hasmod( int k) 
    {
      if (k < ANIM_MOD_SLOTS && this->_modulators[k] != NULL) return true;
      return false;
    }

    // get mod index from name
    int modindex( String modName) 
    {
      for (int k=0; k<ANIM_MOD_SLOTS; k++)
        if(this->_modulators[k] != NULL)
          if (this->_modulators[k]->name() == modName)
            return k;
      return -1;
    }

    // remove Anonym only / All modulators
    K32_anim* unmod(bool all=false)
    {
      for (int k=0; k<ANIM_MOD_SLOTS; k++)
        if(this->_modulators[k] != NULL) 
          if (all || this->_modulators[k]->name() == "?")
          {
            // LOGF("ANIM: %s unmoded !\n", this->_modulators[k]->name());
            this->_modulators[k]->stop();
            delete this->_modulators[k];
            this->_modulators[k] = NULL;
          }
      // LOGF("ANIM: %s unmoded !\n", this->name());
      return this;
    }


    // ANIM MASTER
    //
    K32_anim* master(uint8_t m) {
      this->_master = m;
      return this;
    }
    uint8_t master() {
      return this->_master;
    }

    // change one element in data
    K32_anim* set(int k, int value) { 
      xSemaphoreTake(this->bufferInUse, portMAX_DELAY);     // data can be modified only during anim waitData
      if (k < ANIM_DATA_SLOTS) this->_data[k] = value; 
      xSemaphoreGive(this->bufferInUse);
      return this;
    }

    // new data push (int[], size)
    K32_anim* push(int* frame, int size) {
      bool didChange = false;
      size = min(size, ANIM_DATA_SLOTS);
      xSemaphoreTake(this->bufferInUse, portMAX_DELAY);     // data can be modified only during anim waitData
      for(int k=0; k<size; k++) 
        if (this->_data[k] != frame[k]) {
          this->_data[k] = frame[k]; 
          didChange = true;
        }
      xSemaphoreGive(this->bufferInUse);
      if (didChange || !this->_firstDataReceived) xSemaphoreGive(this->newData);
      return this;
    }
    
    // new data push (uint8_t[], size)
    K32_anim* push(uint8_t* frame, int size) {
      int dframe[size];
      for(int i=0; i<size; i++) dframe[i] = frame[i];
      return this->push(dframe, size);
    }

    // refresh data 
    K32_anim* push() {
      xSemaphoreGive(this->newData);
      return this;
    }

    // data push simplified
    K32_anim* push(int d0) { int frame[1] = {d0}; return this->push(frame, 1); }
    K32_anim* push(int d0, int d1) { int frame[2] = {d0, d1}; return this->push(frame, 2); }
    K32_anim* push(int d0, int d1, int d2) { int frame[3] = {d0, d1, d2}; return this->push(frame, 3); }
    K32_anim* push(int d0, int d1, int d2, int d3) { int frame[4] = {d0, d1, d2, d3}; return this->push(frame, 4); }
    K32_anim* push(int d0, int d1, int d2, int d3, int d4) { int frame[5] = {d0, d1, d2, d3, d4}; return this->push(frame, 5); }
    K32_anim* push(int d0, int d1, int d2, int d3, int d4, int d5) { int frame[6] = {d0, d1, d2, d3, d4, d5}; return this->push(frame, 6); }
    K32_anim* push(int d0, int d1, int d2, int d3, int d4, int d5, int d6) { int frame[7] = {d0, d1, d2, d3, d4, d5, d6}; return this->push(frame, 7); }
    K32_anim* push(int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7) { int frame[8] = {d0, d1, d2, d3, d4, d5, d6, d7}; return this->push(frame, 8); }



  // PROTECTED
  //
  protected:

    // ANIM LOGIC
    //

    // init called when anim starts to play (can be overloaded by anim class)
    // this is a prototype, must be defined in specific anim class
    virtual void init() {}

    // generate frame from data, called by update
    // this is a prototype, must be defined in specific anim class
    virtual void draw (int data[ANIM_DATA_SLOTS]) { LOG("ANIM: nothing to do.."); };

    // pause is like delay, but allow strip refresh
    void pause(int ms) {
      this->_strip->unlock();
      delay(ms);
      this->_strip->lock();
    }

    // DRAW ON STRIP
    //

    // draw pix
    void pixel(int pix, CRGBW color)  {
      if (pix < this->_size)
        this->_strip->pix( pix + this->_offset, color % this->_master);
    }

    // draw multiple pix
    void pixel(int pixStart, int count, CRGBW color)  {
      this->_strip->pix( pixStart + this->_offset, count, color % this->_master);
    }

    // draw all
    void all(CRGBW color) {
      this->_strip->pix( this->_offset, this->_size, color % this->_master);
    }

    // clear
    void clear() {
      this->all({0,0,0,0});
    }
    
    unsigned long startTime = 0;
    uint32_t frameCount = 0;


  // PRIVATE
  //
  private:

    // input data
    int _data[ANIM_DATA_SLOTS];

    // THREAD: draw frame on new data
    static void animate( void * parameter ) 
    {
      K32_anim* that = (K32_anim*) parameter;
      TickType_t timeout;

      bool triggerDraw = false;
      int dataCopy[ANIM_DATA_SLOTS];

      that->startTime = millis();
      that->frameCount = 0;

      if (that->_firstDataReceived) xSemaphoreGive(that->newData);                  // Not first play -> data can be drawn now !
      
      that->init();                                                                 // Subclass init hook

      do 
      {
        that->frameCount++;

        xSemaphoreGive(that->bufferInUse);                                          // allow push & pushdata to modify data
        yield();      

        if (triggerDraw) timeout = pdMS_TO_TICKS( 1000/LEDS_ANIMATE_FPS );          // adapt FPS if an image has been drawn (optimized strobe)
        else timeout = pdMS_TO_TICKS( 500/LEDS_ANIMATE_FPS );

        triggerDraw = (xSemaphoreTake(that->newData, timeout) == pdTRUE);


        xSemaphoreTake(that->bufferInUse, portMAX_DELAY);                           // lock buffer to prevent external change

        if (triggerDraw) that->_firstDataReceived = true;                           // Data has been set at least one time since anim creation
        else if (!that->_firstDataReceived) continue;                               // Data has never been set -> we can't draw ! 
        if (that->_stopAt && millis() >= that->_stopAt) break;                      // check if we should stop now

        memcpy(dataCopy, that->_data, ANIM_DATA_SLOTS*sizeof(int));                 // copy buffer

        xSemaphoreGive(that->bufferInUse);                                          // let data be push by others
        
        for (int k=0; k<ANIM_MOD_SLOTS; k++)
          if (that->_modulators[k])       
            triggerDraw = that->_modulators[k]->run(dataCopy) || triggerDraw;       // run modulators on data

        
        if (triggerDraw) {
          that->_strip->lock();
          that->draw(dataCopy);                                      // Subclass draw hook
          that->_strip->unlock();
        }

      } 
      while(that->loop());

      
      // stop and clear
      xSemaphoreTake(that->newData, 1);
      xSemaphoreGive(that->bufferInUse);

      that->clear();
      that->animateHandle = NULL;
  
      xSemaphoreGive(that->wait_lock);

      LOGF("ANIM: %s end \n", that->name().c_str());
      vTaskDelete(NULL);
    }
    
    // identity
    String _name = "?"; 
    bool _loop = true;

    // output
    K32_ledstrip* _strip = NULL;
    uint8_t _master = 255;
    int _size = 0;
    int _offset = 0; 

    // internal logic
    SemaphoreHandle_t newData;  
    SemaphoreHandle_t bufferInUse;  
    bool _firstDataReceived = false;

    // Animation
    TaskHandle_t animateHandle = NULL;
    SemaphoreHandle_t wait_lock;
    uint32_t _stopAt = 0;

    // Modulator
    K32_modulator* _modulators[ANIM_MOD_SLOTS];
};



#endif