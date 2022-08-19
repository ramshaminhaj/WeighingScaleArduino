#include "HX711.h"
#include <EEPROM.h>
// HX711 circuit wiring
//defining Dout and SCK pins of each HX711
const int LOADCELL1_DOUT_PIN = 2;
const int LOADCELL1_SCK_PIN = 3;
const int LOADCELL2_DOUT_PIN = 4;
const int LOADCELL2_SCK_PIN = 5;
const int LOADCELL3_DOUT_PIN = 6;
const int LOADCELL3_SCK_PIN = 7;
const int LOADCELL4_DOUT_PIN = 8;
const int LOADCELL4_SCK_PIN = 9;

//original calibration factors for each loadcell
int calibrationfactor = 406;
int calibrationfactor2 = 406;
int calibrationfactor3 = 915;
int calibrationfactor4 = 966;

//2 bytes to store the calibration factors to EEPROM 
long upperbyte = 0; //signed 32 bit int
long lowerbyte = 0;

int units;
int units2;
int units3;
int units4;

HX711 scale;
HX711 scale2;
HX711 scale3;
HX711 scale4;

void setup() {
  Serial.begin(57600);
  
  if(EEPROM.read(0)==0) //Check if there is memory stored before
         {
         upperbyte=EEPROM.read(1);//if yes, read cf1 upperbyte in 1 and lower byte in 2
         upperbyte = upperbyte << 8; 
         lowerbyte = EEPROM.read(2);
         calibrationfactor = (int) upperbyte | lowerbyte;
         upperbyte=EEPROM.read(3);
         upperbyte = upperbyte << 8;
         lowerbyte = EEPROM.read(4);
         calibrationfactor2 = (int) upperbyte | lowerbyte;
         upperbyte=EEPROM.read(5);
         upperbyte = upperbyte << 8;
         lowerbyte = EEPROM.read(6);
         calibrationfactor3 = (int) upperbyte | lowerbyte;
         upperbyte=EEPROM.read(7);
         upperbyte = upperbyte << 8;
         lowerbyte = EEPROM.read(8);
         calibrationfactor4 = (int) upperbyte | lowerbyte;
      
         }

  scale.begin(LOADCELL1_DOUT_PIN, LOADCELL1_SCK_PIN);
  scale.set_scale(calibrationfactor);
  scale.tare();

  scale2.begin(LOADCELL2_DOUT_PIN, LOADCELL2_SCK_PIN);
  scale2.set_scale(calibrationfactor2);
  scale2.tare();
  
  scale3.begin(LOADCELL3_DOUT_PIN, LOADCELL3_SCK_PIN);
  scale3.set_scale(calibrationfactor3);
  scale3.tare();
  
  scale4.begin(LOADCELL4_DOUT_PIN, LOADCELL4_SCK_PIN);
  scale4.set_scale(calibrationfactor3);
  scale4.tare();
         
}

void loop() {

  if (1) {
    scale.set_scale(calibrationfactor);
    units = scale.get_units(5); //get average of 5 readings ( reading = DO output/calibration factor)

    scale2.set_scale(calibrationfactor2);
    units2 = scale2.get_units(5);

    scale3.set_scale(calibrationfactor3);
    units3 = scale3.get_units(5);

    scale4.set_scale(calibrationfactor4);
    units4 = scale4.get_units(5);


    //send string of readings and calibration factors to computer
    Serial.print(units);
    Serial.print(',');
    Serial.print(units2);
    Serial.print(',');
    Serial.print(units3);
    Serial.print(',');
    Serial.print(units4);
    Serial.print(',');
    Serial.print(calibrationfactor);
    Serial.print(',');
    Serial.print(calibrationfactor2);
    Serial.print(',');
    Serial.print(calibrationfactor3);
    Serial.print(',');
    Serial.println(calibrationfactor4);
    }

    //if there is serial message from the computer
    if(Serial.available())
    {
      char temp = Serial.read();
      if(temp == 'a' ) // incr cf by 1
      {         
         calibrationfactor += 1;
         UpdateMemory(); //update cf value in EEPROM
      }
      else if(temp == 'b')
      {
        calibrationfactor -= 1; 

         UpdateMemory();
      }
      else if(temp == 'c')
      {
        calibrationfactor2 += 1;
        UpdateMemory();
      }
       else if(temp == 'd')
      {
        calibrationfactor2 -= 1;
        UpdateMemory();
      }
      else if(temp == 'e')
      {
        calibrationfactor3 += 1;
        UpdateMemory();
      }  
      else if(temp == 'f')
      {
        calibrationfactor3 -= 1;
        UpdateMemory();
      }
       else if(temp == 'g')
      {
        calibrationfactor4 += 1;
         UpdateMemory();
      }
       else if(temp == 'h')
      {
        calibrationfactor4 -= 1;
        UpdateMemory();
      }    
      else if(temp == 'i') 
      {
        
        scale.tare();//tare loadcell 1     
      }
      else if(temp == 'j')
      {
        
        scale2.tare();//tare loadcell 2     
      }
      else if(temp == 'k')
      {
        
        scale3.tare();//tare loadcell 3   
      }
      else if(temp == 'l')
      {
        
        scale4.tare();//tare loadcell 4      
      }
      else if(temp == 'm') // tare whole weighing scale
      {
        scale.tare();
        scale2.tare();
        scale3.tare();
        scale4.tare();      
      }
      
    } 
  

}


void UpdateMemory(){ //update EEPROM
         upperbyte = calibrationfactor >> 8; // converting cf to two bytes
         lowerbyte = calibrationfactor & 0xff;
         EEPROM.update(1,upperbyte);
         EEPROM.update(2,lowerbyte);
         
         upperbyte = calibrationfactor2 >> 8;
         lowerbyte = calibrationfactor2 & 0xff;
         EEPROM.update(3,upperbyte);
         EEPROM.update(4,lowerbyte);
         
         upperbyte = calibrationfactor3 >> 8;
         lowerbyte = calibrationfactor3 & 0xff;
         EEPROM.update(5,upperbyte);
         EEPROM.update(6,lowerbyte);
         
         upperbyte = calibrationfactor4 >> 8;
         lowerbyte = calibrationfactor4 & 0xff;
         EEPROM.update(7,upperbyte);
         EEPROM.update(8,lowerbyte);
         EEPROM.update(0,0); //let arduino know that value is now in EEPROM
}

//calibration factor will be the (DO output)/(weight reading)
