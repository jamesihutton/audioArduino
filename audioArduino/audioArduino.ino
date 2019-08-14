#include <SD.h>                      // need to include the SD library
//#define SD_ChipSelectPin 53  //example uses hardware SS pin 53 on Mega2560
#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328, can use other pins
#include <TMRpcm.h>           //  also need to include this library...
#include <SPI.h>

TMRpcm audio;   // create an object for use in this sketch

int32_t playStart = 0;
uint8_t vol = 3;
uint32_t file = 1;
char fileName[] = "1.wav"; 
void setup(){

  audio.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
  audio.quality(1);

  Serial.begin(9600);
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  } else {
    Serial.print("PLAYING ");
    Serial.println(fileName);
    audio.play(fileName);  
    playStart = millis();
  }
}



void loop(){  

  if(Serial.available()){    
    char c = Serial.read();
    if (c == 'p') {
      audio.pause();
      Serial.println("PAUSE");
    } else if (c == 'u') {
      if (vol < 4) {
        vol++;
        audio.setVolume(vol);
        Serial.print("volume ");
        Serial.println(vol);
      } else {
        Serial.println("VOLUME MAX");
      }
    } else if (c == 'd') {
      if (vol > 1) {
        vol--;
        audio.setVolume(vol);
        Serial.print("volume ");
        Serial.println(vol);
      } else {
        Serial.println("VOLUME MIN");
      }
    } else if (c == 'n') {
      if (file >= 4) {file = 1;} else {file ++;}
      fileName[0] = file + '0';
      
      playStart = millis();
      audio.play(fileName);
      Serial.print("PLAYING ");
      Serial.println(fileName);
      
    } else if (c == 'f') {
      uint32_t trackTime = (millis() - playStart)/1000;
      audio.play(fileName,trackTime + 10);
      playStart -= 10000;
      
      
    } else if (c == 'b') {
      uint32_t trackTime = (millis() - playStart)/1000;
      if (trackTime < 10) {audio.play(fileName);}
      else { 
        audio.play(fileName,trackTime - 10);
        playStart += 10000;
      }
      
    }
    
  }

}
