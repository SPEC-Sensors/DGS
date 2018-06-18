/*
  DSDK_SETUP - Example use of library for reading KWJ Engineering with SPEC Sensors on Digital SDK.
  Created by David E. Peaslee, March 29, 2018.
  Created for SPEC/KWJ Digital Gas Sensor (DGS, or also known as DULPSM)
  Not yet released into the public domain.


  !!! RX, TX Must be on 3.3 volt communication, or using level shifters to get to 3.3V UART!!!

  Wiring:
  Connect the TX of Digital Sensor to the RX of 3.3 V Arduino
  Connect the RX of Digital Sensor to the TX of 3.3 V Arduino
  Connect the power of Digital Sensor to the 3.3V power of the Arduino
  Connect the ground of Digital Sensor to the ground of the Arduino

  This code is optimized for the Arduino Due, but can be modified to work with any other Arduino.
  The DGS is a 3.3V device, which means you must use level shifters if using with a 5V Arduino.

  SoftwareSerial is needed for any Arduino compatible device that does not have a native alternative serial port (Serial1)
  DGS runs at 9600 baud rate.
*/

/*This example requires the DGS libraries, if it does not compile look to the library.properties file and add your board to the architectures.
   The purpose of this example is to help you set up the DGS. After the first setup, loading the barcode into the DGS, you do not need to
   repeat the setup portion of the DGS commands. You only need to initialize the serial ports in Setup.
   You can re-zero at anytime by sending Z through the serial terminal, once the continuous output starts. If you require technical assistance, please grab a
   screen shot of the BarCode text, as well as the output of the EEPROM, and the Firmware Date. To turn off verbose setup, change DEBUG to false.

*/
#include "DGS.h"

//#include "SoftwareSerial.h" //Include the next lines if using Arduino without Serial1 support, and replace refernces to Serial1 with mySerial
//
//SoftwareSerial mySerial(10, 11); // !!! RX, TX Must be on 3.3 volt communication, or using level shifters to get to 3.3V UART!!!

DGS mySensor(&Serial1);

void setup() //Setup intended for first time setup of SDK. Must initialize both serial ports:
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.flush();
  Serial1.flush();
  Serial.println("Begin Setup");

  mySensor.DEBUG = false;

  //Remainder of setup() code is only necessary for initial setup of DULP
  Serial.println();
  Serial.println(mySensor.getFW());  // Retruns FW date
  Serial.println();

  if (mySensor.setBC("071615011247 110102 CO 1507 4.05")) Serial.println("Finished Setting BC");  //Enter your barcode string. Returns 1/0.

  mySensor.getEEPROM();  //Read eeprom into array and/or list view if DEBUG is TRUE. See end of sketch for outline of values.
  Serial.println();

  if (mySensor.getLMP()) { // Loads LMP values into variable returns 1/0
    Serial.println("LMP91000 Values loaded into array");
    if (mySensor.setLMP(mySensor.LMP[0], mySensor.LMP[1], mySensor.LMP[2])) Serial.println("Finished Setting New Values For LMP"); //Sets LMP variables into register returns 1/0
  }
  Serial.println();

  if (mySensor.setToff(2.3)) Serial.println("Finished Setting T Offset"); //Set T offset in deg C returns 1/0
  Serial.println();

  Serial.println("Enter 'Z' When Sensor is Stabilized");
  while (Serial.read() != 'Z') {}
  if (mySensor.zero()) Serial.println("Finished Setting Zero"); //zeros returns 1/0
  Serial.println();

  Serial.println("Finished Setup");
}

void loop()
{
  mySensor.getData('\r');
  Serial.print("Concentration = ");
  Serial.print(mySensor.getConc());     //default is 'p' for temperature compensated ppb, any other cahracter for raw counts
  Serial.print(", Temperature = ");
  Serial.print(mySensor.getTemp('F'));  //'F' or 'C' for units of temperature, default is 'C', any other cahracter for raw counts
  Serial.print(", Humidity = ");
  Serial.println(mySensor.getRh());     //default is 'r' for %Rh, any other character for raw counts

  while (Serial.available()) // read from Serial port, send to port SDK port
  {
    char U = Serial.read();
    if (U == 'Z') {
      if (mySensor.zero()) Serial.println("Finished Setting Zero"); //zeros returns 1/0
    }
    if (U == 'f') {
      Serial.println(mySensor.getFW());
    }
    if (U == 'e') {
      mySensor.DEBUG = true;
      mySensor.getEEPROM();
    }
  }
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
