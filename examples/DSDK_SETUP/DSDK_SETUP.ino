/*
  DSDK_SETUP - Example use of library for reading KWJ Engineering with SPEC Sensors on Digital SDK.
  Created by David E. Peaslee, March 29, 2018.
  Updated by David Peaslee, March 29, 2021.
  Created for SPEC/KWJ Digital Gas Sensor (DGS, or also known as DULPSM)


  !!! RX, TX Must be on 3.3 volt communication, or using level shifters to get to 3.3V UART!!!

  Wiring:
  Connect the TX of Digital Sensor to the RX of 3.3 V Arduino
  Connect the RX of Digital Sensor to the TX of 3.3 V Arduino
  Connect the power of Digital Sensor to the 3.3V power of the Arduino
  Connect the ground of Digital Sensor to the ground of the Arduino

  Serial terminal settings: 9600 baud, 8N1N, "no line endings"

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
const char* Barcode = "042617040460 110102 CO 1705 2.72";
char input;
void setup() //Setup intended for first time setup of SDK. Must initialize both serial ports:
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.flush();
  Serial1.flush();
  Serial.println("Begin Setup");

  mySensor.DEBUG = true;


  //Remainder of setup() code is only necessary for initial setup of DULP

  Serial.println();
  Serial.println(mySensor.getFW());  // Retruns FW date
  Serial.println(mySensor.setBC(Barcode));

  Serial.println(mySensor.setToff(1.03)); delay(500);

  Serial.println(mySensor.getLMP());  // Retruns LMP Settings
  Serial.println(mySensor.setLMP(mySensor.LMP[0], mySensor.LMP[1], mySensor.LMP[2]));
  delay(1000);
  mySensor.getEEPROM();


  Serial.println("Enter 'Z' When Sensor is Stabilized");
  while (Serial.read() != 'Z') {}
  if (mySensor.zero()) Serial.println("Finished Setting Zero"); //zeros returns 1/0

  Serial.println("Finished Setup");
  Serial.println("1/0 (read): raw counts, concentration (ppb), integer temp (degC), float temp (degF), float temp (degC), int humid (%), float humid (%)");

}

void loop()
{
  if (Serial.available()) {
    input = Serial.read();
    switch (input) {
      case 'e':
        mySensor.getEEPROM(); //print eeprom
        break;
      case 's':
        //sleep for 30 seconds, need call to wake to see if time elapsed
        mySensor.sleepSensor(30);
        //for open-ended sleep, mySensor.sleepSensor(); then 2 calls to mySensor.getData('\r') to wake
        break;
      case 'z':
        mySensor.zero();  //rezero sensor
        break;
      case 'a':
        mySensor.setAve(100);
        break;
      default:
        break;
    }
  }

  if (mySensor.wakeSensor()) {// check if awake, wake if sleep time elapsed


    delay(1000);
    Serial.print(mySensor.getData('\r'));
    Serial.print(": ");
    Serial.print(mySensor.getConc('c'));
    Serial.print(", ");
    Serial.print(mySensor.getConc());
    Serial.print(", ");
    Serial.print((int) mySensor.getTemp()); //same as getTemp('c')
    Serial.print(", ");
    Serial.print(mySensor.getTemp('F'));
    Serial.print(", ");
    Serial.print(mySensor.getTemp('C'));
    Serial.print(", ");
    Serial.print((int) mySensor.getRh()); //same as getRh('r')
    Serial.print(", ");
    Serial.println(mySensor.getRh('R'));
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
