/*
  DSDK_Photon - Example use of library for reading KWJ Engineering with SPEC Sensors on Digital SDK.
  Created by David E. Peaslee, March 29, 2018.
  Created for SPEC/KWJ Digital Gas Sensor (DGS, or also known as DULPSM)
  Not yet released into the public domain.

  !!! RX, TX Must be on 3.3 volt communication, or using level shifters to get to 3.3V UART!!!

  Wiring:
  Connect the TX of Digital Sensor to the RX of 3.3 V Arduino
  Connect the RX of Digital Sensor to the TX of 3.3 V Arduino
  Connect the power of Digital Sensor to the 3.3V power of the Arduino
  Connect the ground of Digital Sensor to the ground of the Arduino

  This code is optimized for the Particle Photon, but can be modified to work with any other Arduino.
  The DGS is a 3.3V device, which means you must use level shifters if using with a 5V Arduino.
  SoftwareSerial is needed for any Arduino compatible device that does not have a native alternative serial port (Serial1)
  DGS runs at 9600 baud rate.
*/


/* This program is intended to allow serial communication with the DGS. Using the Serial terminal you can send and recieve information to the DGS:
   This example requires DGS.h
   See the documentation page here for a list of commands:
   http://www.spec-sensors.com/wp-content/uploads/2017/01/DG-SDK-968-045_9-6-17.pdf
   When reseting be sure the continuous output has stopped.
*/

 #include "DGS.h"
 DGS mySensor(&Serial1);
 int ttime = 5;
 unsigned long now;
 double Conc;
 double Temp;
 double Humidity;
 String FWDate;
 double alphaX =1;


int zeroSensor(String average);

// setup() runs once, when the device is first turned on.
void setup() {
while (!Serial.isConnected()) Particle.process(); // wait for Host to open serial port
  Serial1.begin(9600);
  Serial.flush();
  Serial1.flush();
  Serial.println("Begin Setup");

  Particle.variable("xonc", Conc);
  Particle.variable("temp", Temp);
  Particle.variable("Rh", Humidity);
  Particle.variable("alpha", alphaX);
  Particle.function("zero", zeroSensor);

  mySensor.DEBUG = TRUE;

  //Remainder of setup() code is only necessary for initial setup of DULP
  Serial.println();
  Serial.println(FWDate=mySensor.getFW());  // Retruns FW date
  Serial.println();
  now = millis();
}

void loop() {
    if (millis()-now>ttime*1000){
    mySensor.getData('\r');
     Conc=mySensor.getConc()/1000*alphaX+(1-alphaX)*Conc;
     Temp=mySensor.getTemp('F');
     Humidity=mySensor.getRh();

    Serial.print("Concentration = ");
    Serial.print(Conc);     //default is 'p' for temperature compensated ppb, any other cahracter for raw counts
    Serial.print(", Temperature = ");
    Serial.print(Temp);  //'F' or 'C' for units of temperature, default is 'C', any other cahracter for raw counts
    Serial.print(", Humidity = ");
    Serial.println(Humidity);     //default is 'r' for %Rh, any other character for raw counts
    now = millis();
  }
}

int zeroSensor(String average){
  alphaX = average.toFloat();
  if (mySensor.zero()) return 1; //zeros returns 1/0
  return 0;
}

/*
    EEPROM Values can be called using the following addresses (Ex. mySensor.e[1] is int and mySensor.E[1] is String)

    nA_per_PPM_x100 = eepromInt[1]
    ADC_OC = eepromInt[2]
    ADC_Zero = eepromInt[3]
    ADC_Span = eepromInt[4]
    Temperature_Offset_x1000 = eepromInt[5]
    T_Zero = eepromInt[6]
    RH_Zero = eepromInt[7]
    T_Span = eepromInt[8]
    RH_Span = eepromInt[9]
    LMP91000 Register 0x10 = eepromInt[10]
    LMP91000 Register 0x11 = eepromInt[11]
    LMP91000 Register 0x12 = eepromInt[12]
    Average_Total = eepromInt[13]
    Barcode = eepromStr[0]
    Serial_Number_ = eepromStr[1]
    Part_Number = eepromStr[2]
    Gas = eepromStr[3]
    Date_Code = eepromStr[4]
    Sensitivity_Code = Sensitivity_Code
*/
