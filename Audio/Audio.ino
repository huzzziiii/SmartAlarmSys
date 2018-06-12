#include <SD.h>
#include <SPI.h>
#include<math.h>


//--------DAC-------------
const int PIN_CS = 10;
const int GAIN_1 = 0x1;
const int GAIN_2 = 0x0;

File file;
const int SD_CS = 9;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);

  if (!SD.begin()){
    Serial.println("Initialization Failed!");
    while(true);
  }
  Serial.println("----------Initialization Successful!--------");

  file = SD.open("Sixteen.wav"); //Eight, Sixteen

  if (!file){
    Serial.println("File didn't OPEN!");
  }
  else{
    Serial.println("OPENED!");
  }
  //SDCard();
  //analogOutput(0, GAIN_1, 1, 4000);
}

void SDCard(){

   int sampleNumber;  
   file.seek(44);
    while (file.available()){
      byte LSB = file.read();
      byte MSB = file.read(); 
      //LSB is in the MSB spot and MSB in the LSB spot as the bytes need to be swapped
      uint16_t sample = (MSB << 8) | LSB;
      int16_t signed_sample = (int16_t) sample;
      
      Serial.print(" Sample Read: ");
      Serial.print(LSB, DEC);  //print the data that was jsut read from the sd card, 16 bit decimal 
      Serial.print("----");
      Serial.print(MSB, DEC);
      Serial.print("----> ");
      Serial.print(signed_sample, DEC);
      Serial.print("----> ");
      Serial.print(signed_sample, HEX);
      
  //convert 16 to 12-bit
    uint16_t final_val = sample >>4; //fd47 -> 0fd4 (16 -> 12 bit)
        
    Serial.print(" ---->12 bit ---> ");
    Serial.println(final_val);
    analogOutput(0, GAIN_1, 1, 4000);
      
      delay(2000);
    }
}

void analogOutput(byte channel, byte gain, byte shut, unsigned int val){
  byte lowByte = val & 0xff;
  byte highByte = ( (val >> 8) & 0xff) | channel << 7 | gain << 5 | shut << 4;

  PORTB &= 0xfb; // 1111 1011 (CS -> LOW) [PORT B: 8-13]
  //digitalWrite(PIN_CS,LOW);
  
  SPI.transfer(highByte);
  SPI.transfer(lowByte);
  PORTB |= 0x04;  //0000 0100 (CS -> HIGH)
}

void loop() {

  
  // put your main code here, to run repeatedly:
  for (int i=100; i<4000; i+=10){
    Serial.println(i);
    analogOutput(0, GAIN_1, 1, i);
    delay(300);
  }
}

