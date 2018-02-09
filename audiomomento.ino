#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFMiniMp3.h>

int flipPin = A0; 
int ledPin = 13;   
float flipVal = 0.0;
bool flipState = false;
int flipCounter = 0;  
int previousFlipCounterValue = 1;


class Mp3Notify
{
public:
  static void OnError(uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }
  static void OnPlayFinished(uint16_t track)
  {
    Serial.print("Play finished for #");
    Serial.println(track);  
  }
  static void OnCardOnline(uint16_t code)
  {
    Serial.println("Card online ");
  }
  static void OnCardInserted(uint16_t code)
  {
    Serial.println("Card inserted ");
  }
  static void OnCardRemoved(uint16_t code)
  {
    Serial.println("Card removed ");
  }
};

SoftwareSerial secondarySerial(10, 11); // RX, TX
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(secondarySerial);

void setup() {
  
  Serial.begin(115200);

  Serial.println("initializing...");

  mp3.begin();
  mp3.setVolume(40);
  mp3.setRepeatPlay(true);
  
  pinMode(flipPin, INPUT);  
  pinMode(ledPin, OUTPUT); 

  Serial.println("starting...");
}


void loop() {
  
  digitalWrite(ledPin, HIGH); 
  Serial.print("Audio is ON"); 

  flipVal = analogRead (flipPin);
  
  if (flipVal < 200){
    flipVal = 0;
  }
    
  if (flipVal > 200) {
    if(flipState){
      flipCounter++;
      flipState = false;
    }
    else {
      flipState = true;
    }
  }

    Serial.print("; flipVal is ");
    Serial.print(flipVal);
    Serial.print("; flipCounter is ");
    Serial.print(flipCounter); 
    Serial.print("; flipState is ");
    Serial.print(flipState); 
    
    if (flipCounter == previousFlipCounterValue) {
      if (flipCounter % 2 != 0){
        mp3.start();
        Serial.println ("; Track 2 is playing");
        delay(1000);
        mp3.pause(); 
      }
      else{
        mp3.start();
        Serial.println("; Track 1 is playing");
        delay(1000);
        mp3.pause();
        }
    }
    else {
      if (flipCounter % 2 != 0){
        mp3.playMp3FolderTrack(2);
        Serial.println("; Track 2 is playing");
        previousFlipCounterValue = flipCounter;
        mp3.pause(); 
      }
      else{
        mp3.playMp3FolderTrack(1);
        Serial.println("; Track 1 is playing");
        previousFlipCounterValue = flipCounter;
        mp3.pause();
      }
   }
 }
