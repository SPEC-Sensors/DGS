/*
  DGS_25SEP17.h - Library for reading KWJ Engineering with SPEC Sensors on Digital SDK with firmware date 25SEP17.
  Created by David E. Peaslee, Mar 29, 2018.
*/

#ifndef _DGS_h
#define _DGS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#elif defined(SPARK)
//#include "application.h"
#else
#include "WProgram.h"
#endif

#include "Stream.h"

class DGS
{
  private:
    Stream *_mySerial;
    long dataArray[11]; //Multipurpose array
    //void unlock(void);
    void DEBUG_PRINTLN(long x);
    void DEBUG_PRINTLN(float x);
    void DEBUG_PRINTLN(String x);
    void DEBUG_PRINT(long x);
    void DEBUG_PRINT(float x);
    void DEBUG_PRINT(String x);

  public:
    DGS(Stream &mySerial); //Class that represents DULP with Stream as class for Serial
    DGS(Stream *mySerial); //Class that represents DULP with Stream as class for Serial

    bool DEBUG;

    int setToff(float offset);  //Sets a temperature offset
    int setBC(String BC);  //Initializes DGS with all information from barcode, only way to set Span
    String getFW(void); //Returns the Firmware Date

    int getLMP(void); //Loads LMP[3] with register values from LMP91000
    int LMP[3];  //LMP Registers, needs to be loaded with getLMP
    int setLMP(int R1, int R2, int R3); //Sets new LMP registers and reintializes LMP

    int zero(void); //Zeros to current reading, Zero only in a clean air environment
    int setXSpan(float); //ReCalibrates device, only should be used after zero, and using a calibrated gas

    int getData(char c); //Gets the data and loads into variables
    long getConc(char x = 'p');  //Reads concentration from current data array 'p' ppb or 'c' counts
    long getTemp(char t = 'C');  //Reads temperature from current data array 'C' for Celsius or 'F' for Fahrenheit
    long getRh(char r = 'r'); //Reads Rh from current data array 'r' for Rh and 'c' for counts

    void getEEPROM(void); //Loads variables into EEPROM E[5] and e[14] arrays, outputs EEPROM if DEBUG is true
    String eepromStr[5]; //Stores the character based EEPROM data
    long eepromInt[13];  //Stores the integer based EEPROM data
    float Sensitivity_Code;  //Stores the sensitivity coefficient in nA/PPM
};

#endif


