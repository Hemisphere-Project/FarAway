#include "SPIFFS.h"
#include "AudioFileSourceSPIFFS.h"
// #include "AudioGeneratorWAV.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"

// AudioGeneratorWAV *wav = NULL;
AudioGeneratorMP3 *wav = NULL;
AudioFileSourceSPIFFS *file  = NULL;
AudioOutputI2S *out = NULL;

long lastStart = 0;
bool lastLong = false;

void audio_setup() {
  Serial.println("AUDIO: Mounting SPIFFS...");
  
  // First try to mount without formatting
  if (!SPIFFS.begin(false)) {
    Serial.println("SPIFFS: Mount failed, formatting...");
    // Format the partition
    if (!SPIFFS.format()) {
      Serial.println("SPIFFS: Format failed!");
    } else {
      Serial.println("SPIFFS: Format OK, remounting...");
      if (!SPIFFS.begin(false)) {
        Serial.println("SPIFFS: Still failed after format!");
      } else {
        Serial.println("SPIFFS: Mounted after format (empty)");
      }
    }
  } else {
    Serial.println("SPIFFS: Mounted successfully");
    
    // List files
    File root = SPIFFS.open("/");
    if (root) {
      File file = root.openNextFile();
      Serial.println("SPIFFS Files:");
      while(file) {
        Serial.print("  ");
        Serial.print(file.name());
        Serial.print(" - ");
        Serial.print(file.size());
        Serial.println(" bytes");
        file = root.openNextFile();
      }
    }
  }

  audioLogger = &Serial;
  out = new AudioOutputI2SNoDAC();
  out->SetPinout(26,25,22);
  out->SetGain(1.9);
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

void audio_play( bool longNote ) {
  // if (wav != NULL) return;
  if (wav != NULL) {
    // if (millis()-lastStart > 6000) audio_stop();
    if (!lastLong) return;  // SHORT IS PLAYING
    else if (millis()-lastStart < 1500) return; // WAS PLAYING LONG for less than 1500
  }
  lastStart = millis();
  lastLong = longNote;
  
  audio_stop();

  Serial.println("AUDIO: start playing beep.wav");
  // String media = "/"+String(random(4,7))+"nofx.wav";
  String media; 
  media = "/"+String(random(1,4)); 
  if (longNote) media += "long";
  else media += "small";
  media += ".mp3";

  file = new AudioFileSourceSPIFFS(media.c_str());
  wav = new AudioGeneratorMP3();
  wav->begin(file, out);
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