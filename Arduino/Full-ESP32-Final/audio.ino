#include "SPIFFS.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2SNoDAC.h"

AudioGeneratorWAV *wav = NULL;
AudioFileSourceSPIFFS *file  = NULL;
AudioOutputI2S *out = NULL;

void audio_setup() {
  SPIFFS.begin();

  audioLogger = &Serial;
  out = new AudioOutputI2SNoDAC();
  out->SetPinout(26,25,22);
  out->SetGain(1.5);
}

void audio_play() {
  audio_stop();
  Serial.println("AUDIO: start playing beep.wav");
  file = new AudioFileSourceSPIFFS("/beep.wav");
  wav = new AudioGeneratorWAV();
  wav->begin(file, out);
}

void audio_stop() {
    if (wav != NULL) {
        if (wav->isRunning()) wav->stop();
        delete wav;
        wav = NULL;
        Serial.println("AUDIO: stopped decoder");
    }
    if (file != NULL) {
        delete file;
        file = NULL;
        Serial.println("AUDIO: stopped file");
    }
}

bool audio_loop()
{
  if (wav != NULL && wav->isRunning()) {
    if (!wav->loop()) {
        wav->stop();
        audio_stop();
    }
    else return true;
  }
  return false;
}