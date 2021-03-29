/*
DGS.h - Library for reading KWJ Engineering with SPEC Sensors on Digital SDK.
Created by David E. Peaslee, NOV 27, 2016.
Updated by David Peaslee, MAR 24, 2021
*/

#ifndef _DGS_h
#define _DGS_h
//#include "application.h"
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#elif defined(SPARK)
#include "application.h"
#else
//#include "WProgram.h"
#endif

class DGS
{
  private:
    Stream *_mySerial;
    long dataArray[11]; //Multipurpose array
    void DEBUG_PRINTLN(long x);
    void DEBUG_PRINTLN(float x);
    void DEBUG_PRINTLN(String x);
    void DEBUG_PRINT(long x);
    void DEBUG_PRINT(float x);
    void DEBUG_PRINT(String x);
    void unlock(void);
	
	unsigned long sleepTime;


  public:
    DGS(Stream &mySerial); //Class that represents DGS with Stream as class for Serial
    DGS(Stream *mySerial); //Class that represents DGS with Stream as class for Serial

    bool DEBUG;

	bool isSleeping;
	void sleepSensor(unsigned long sleepy = 0);
	bool wakeSensor(void);
	
	int setAve(int t);
	
	int setToff(float offset);  //Sets a temperature offset
    int setBC(const char *BC);  //Initializes DGS with all information from barcode, only way to set Span
    String getFW(void); //Returns the Firmware Date
	
	
    int getLMP(void); //Loads LMP[3] with register values from LMP91000
    int LMP[3];	//LMP Registers, needs to be loaded with getLMP
    int setLMP(int R1, int R2, int R3); //Sets new LMP registers and reintializes LMP

    int zero(void); //Zeros to current reading, Zero only in a clean air environment
    int setXSpan(float); //ReCalibrates device, only should be used after zero, and using a calibrated gas

    int getData(char c); //Gets the data and loads into variables
    long getConc(char u = 'p');  //Reads concentration from current data array 'p' ppb or 'c' counts
    float getTemp(char u = 'c');  //Reads temperature from current data array 'C' for Celsius or 'F' for Fahrenheit
    float getRh(char u = 'r'); //Reads Rh from current data array

    void getEEPROM(void); //Loads variables into EEPROM E[5] and e[14] arrays, outputs EEPROM if DEBUG is true
    String eepromStr[5]; //Stores the character based EEPROM data
    long eepromInt[13];  //Stores the integer based EEPROM data
    float Sensitivity_Code;  //Stores the sensitivity coefficient in nA/PPM
};


/*
EEPROM Values can be called using the following addresses (Ex. CO.e[1] is int and CO.E[1] is String)
nA_per_PPM_x100 = e[1]
ADC_OC = e[2]
ADC_Zero = e[3]
ADC_Span = e[4]
PPM_Span_x100 = e[5]
Temperature_Offset_x1000 = e[6]
T_Zero = e[7]
RH_Zero = e[8]
T_Span = e[9]
RH_Span = e[10]
LMP91000 Register 0x10 = e[11]
LMP91000 Register 0x11 = e[12]
LMP91000 Register 0x12 = e[13]
Barcode = E[0]
Serial_Number_ = E[1]
Part_Number = E[2]
Gas = E[3]
Date_Code = E[4]
Sensitivity_Code = Sf
*/

#endif
