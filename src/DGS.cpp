/*
DGS.cpp - Library for reading KWJ Engineering with SPEC Sensors on Digital SDK.
Created by David E. Peaslee, NOV 27, 2016.
Updated by David Peaslee, MAR 24, 2021
*/

/*
User inputs are 
'\r'
'c' set to continuous approx. 4 mA to ground (not including uart consumption)
'S' set span accepts a PPM float above 1.0 "\r\nEnter span gas value in PPM:" atof() "\r\nSetting span..." then:
                      either "done\r\n" or "Temperature error, " or "ADC range error, " or "Serial number error, " & "no changes made\r\n"
'Z' zero sensor "\r\nSetting zero..." either "done\r\n", "Temperature error, " or "ADC range error, " and "no changes made\r\n"
'f' output firmware should be "15SEP17\r\n"
'l' get LMP values: "                               LMP91000 Values\r\n", "LMP91000 Register 0x10= 3\r\n","LMP91000 Register 0x11= 145\r\n","LMP91000 Register 0x12= 3\r\n"
'L' set LMP values: "\r\nEnter LMP91000 Register 0x10:",atoi(),"\r\nEnter LMP91000 Register 0x11:",atoi(),"\r\nEnter LMP91000 Register 0x12:",atoi(),"\r\n"
'T' set Temperature offset: "\r\nEnter Temperature_Offset: ", atof(), "\r\n"
'A' set average: "\r\nEnter Average Total: ", atoi(), "\r\n"
'B' set Barcode: "\r\nRemove Sensor and Scan: ", BC,"\r\nSetting OC...done\r\nSetting zero...done\r\n"
'e' get eeprom: "                               EEPROM Values\r\n"
        nA_per_PPM_x100= 340
        ADC_OC= 13035
        ADC_Zero= 13035
        ADC_Span= 16299
        Temperature_Offset_x1000= 1000
        T_Zero= 25336
        RH_Zero= 27602
        T_Span= 25052
        RH_Span= 27710
        LMP91000 Register 0x10= 3
        LMP91000 Register 0x11= 145
        LMP91000 Register 0x12= 3
        Average_Total= 1
        Barcode= 042617040460 110102 CO 1705 2.72
        Serial_Number= 042617040460
        Part_Number= 110102
        Gas= CO
        Date_Code= 1705
        Sensitivity_Code= 2.72\r\n
'r' reset: locks to reset WDT
's' sleep approx. 30 uA to ground
*/

#include "DGS.h"

DGS::DGS(Stream *mySerial) : _mySerial(mySerial) {
	isSleeping = false;
}

DGS::DGS(Stream &mySerial) : _mySerial(&mySerial) {
	isSleeping = false;
}

void DGS::unlock(void)
{
  DEBUG_PRINT("Unlocking");
  _mySerial->write("12345\r");
  delay(500);
}

void DGS::DEBUG_PRINTLN(long x)
{
  if (DEBUG) {
    delay(10);
    Serial.println(x);
  }
}

void DGS::DEBUG_PRINTLN(String x)
{
  if (DEBUG) {
    delay(10);
    Serial.println(x);
  }
}

void DGS::DEBUG_PRINTLN(float x)
{
  if (DEBUG) {
    delay(10);
    Serial.println(x);
  }
}

void DGS::DEBUG_PRINT(long x)
{
  if (DEBUG) {
    delay(10);
    Serial.print(x);
  }
}

void DGS::DEBUG_PRINT(float x)
{
  if (DEBUG) {
    delay(10);
    Serial.print(x);
  }
}

void DGS::DEBUG_PRINT(String x)
{
  if (DEBUG) {
    delay(10);
    Serial.print(x);
  }
}

int DGS::getData(char c)
{
  delay(500);
  String dataString;
  _mySerial->write(c);
  while (!_mySerial->available()) {
  }
  dataString = _mySerial->readStringUntil('\n');
  for (int i = 0; i < 11; i++) {
    String subS = dataString.substring(0, dataString.indexOf(','));
    if (subS.length() == 0) return 0;
    dataArray[i] = subS.toInt();
    dataString = dataString.substring(dataString.indexOf(',') + 2);
  }
  return 1;
}
void DGS::sleepSensor(unsigned long sleepy)
{
  isSleeping = true;
  sleepTime = (1000 * sleepy) + millis();
  DEBUG_PRINTLN("Going to Sleep");
  while (_mySerial->available()) _mySerial->read();
  _mySerial->write('s');  
}

bool DGS::wakeSensor(void)
{
  if (isSleeping && (millis() > sleepTime)){
	  isSleeping = false;
	  DEBUG_PRINTLN("Waking");
	  while (_mySerial->available()) _mySerial->read();
	  _mySerial->write('r');
	  delay(2000);
  } 
  return !isSleeping;
}

int DGS::setToff(float offset)
{
  String commandString;
  DEBUG_PRINTLN("Starting T Offset");
  while (_mySerial->available()) _mySerial->read();
  _mySerial->write('T');
  delay(10);
  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);
  
  delay(100);
  if (commandString == "\r\nEnter Temperature_Offset: ") {
    _mySerial->println(offset);
//    _mySerial->write('\r');
  }
  else {
    DEBUG_PRINTLN("Failed Entering Temperature");

    delay(2000);
    while (_mySerial->available()) _mySerial->read();
    return 0;

  }

  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  DEBUG_PRINTLN(commandString);
  return 1;

}

int DGS::setBC(const char *BC)
{
  String commandString;
  DEBUG_PRINTLN("Starting BC Setup");
  while (_mySerial->available()) _mySerial->read();
  _mySerial->write('B');
  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);
  
  if (commandString == "\r\nRemove Sensor and Scan: ") {

    for (int i = 0; i < (int) strlen(BC); i++) {
      _mySerial->write(BC[i]);

      delay(10);
    }
    _mySerial->write('\r');
  }
  else {
    DEBUG_PRINTLN("Failed Entering BC");
    return 0;
  }
  while (!_mySerial->available()) {}
  commandString = _mySerial->readStringUntil('\n');
  DEBUG_PRINTLN(commandString); //should be \r\n
  
  while (!_mySerial->available()) {}
  commandString = _mySerial->readStringUntil('\n');
  DEBUG_PRINTLN(commandString); 
  
  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString); 
  
  if (commandString == "Setting zero...done\r\n") {
    delay(2000);
    DEBUG_PRINTLN("Success");
    return 1;
  }
  delay(2000);
  return 0;
}

void DGS::getEEPROM(void)
{
  DEBUG_PRINTLN("Starting EEPROM Read");
  while (_mySerial->available()) _mySerial->read();
  _mySerial->flush();
  _mySerial->write('e');

  while (!_mySerial->available()) {}

  String data = _mySerial->readStringUntil('\n');
  DEBUG_PRINTLN(data);
  while (!_mySerial->available()) {}
  for (int i = 0; i < 13; i++) {
    data = _mySerial->readStringUntil('\n');
    String subS1 = data.substring(0, data.indexOf('='));
    String subS2 = data.substring(data.indexOf('=') + 2);
    eepromInt[i] = subS2.toInt();
    DEBUG_PRINT(subS1);
    DEBUG_PRINT("= ");
    DEBUG_PRINTLN(eepromInt[i]);
  }
  for (int i = 0; i < 5; i++) {
    data = _mySerial->readStringUntil('\n');
    String subS1 = data.substring(0, data.indexOf('='));
    String subS2 = data.substring(data.indexOf('=') + 2);
    eepromStr[i] = subS2;
    DEBUG_PRINT(subS1);
    DEBUG_PRINT("= ");
    DEBUG_PRINTLN(eepromStr[i]);
  }
  data = _mySerial->readStringUntil('\n');
  String subS1 = data.substring(0, data.indexOf('='));
  String subS2 = data.substring(data.indexOf('=') + 2);
  Sensitivity_Code = subS2.toFloat();
  DEBUG_PRINT(subS1);
  DEBUG_PRINT("= ");
  DEBUG_PRINTLN(Sensitivity_Code);
  while (_mySerial->available()) _mySerial->read();

}

String DGS::getFW(void)
{
  String dataString;
  DEBUG_PRINTLN("Getting FW");
  while (_mySerial->available()) _mySerial->read();
  _mySerial->flush();
  _mySerial->write('f');

  while (!_mySerial->available()) {}
  dataString = _mySerial->readStringUntil('\r');
  if (dataString != "15SEP17") DEBUG_PRINTLN("Unsupported FW");
  while (_mySerial->available()) _mySerial->read();
  return dataString;
}

int DGS::setLMP(int R1, int R2, int R3)
{
  String commandString;
  DEBUG_PRINTLN("Setting LMP Registers");
  while (_mySerial->available()) _mySerial->read();
  _mySerial->flush();
  _mySerial->write('L');
  
  while (!_mySerial->available()) {}
  commandString = _mySerial->readStringUntil('\n');
  commandString = _mySerial->readStringUntil('\n');
  commandString.trim();
  DEBUG_PRINT(commandString);
  if (commandString == "Enter LMP91000 Register 0x10:") {
    _mySerial->print(R1);
	_mySerial->print("\r");
  }
  else {
    DEBUG_PRINTLN("Setting Failed");
    return 0;
  }
  while (!_mySerial->available()) {}
  commandString = _mySerial->readStringUntil('\n');
  DEBUG_PRINTLN(commandString);
  if (commandString.toInt() != R1) {
    DEBUG_PRINTLN("Failure");
    return 0;
  }

  while (!_mySerial->available()) {}
  commandString = _mySerial->readStringUntil('\n');
  commandString.trim();
  DEBUG_PRINT(commandString);
  if (commandString == "Enter LMP91000 Register 0x11:") {
    _mySerial->print(R2);
	_mySerial->print("\r");
  }
  else {
    DEBUG_PRINTLN("Setting Failed");
    return 0;
  }
  while (!_mySerial->available()) {}
  commandString = _mySerial->readStringUntil('\n');
  DEBUG_PRINTLN(commandString);
  if (commandString.toInt() != R2) {
    DEBUG_PRINTLN("Failure");
    return 0;
  }
  
while (!_mySerial->available()) {}
  commandString = _mySerial->readStringUntil('\n');
  commandString.trim();
  DEBUG_PRINT(commandString);
  if (commandString == "Enter LMP91000 Register 0x12:") {
    _mySerial->print(R3); 
	_mySerial->print("\r");
  }
  else {
    DEBUG_PRINTLN("Setting Failed");
    return 0;
  }
  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  commandString.trim();
  DEBUG_PRINTLN(commandString);
  if (commandString.toInt() != R3) {
    DEBUG_PRINTLN("Failure");
    return 0;
  }
  
  else {
    while (_mySerial->available()) _mySerial->read();
    DEBUG_PRINTLN("Success");
    delay(2000);
    return 1;
  }
}

int DGS::getLMP(void)
{
  String dataString;
  DEBUG_PRINTLN("Getting LMP Registers");
  while (_mySerial->available()) _mySerial->read();
  _mySerial->flush();
  _mySerial->write('l');
  
  while (!_mySerial->available()) {}
  dataString = _mySerial->readStringUntil('\n');
  DEBUG_PRINTLN(dataString);
  
  while (!_mySerial->available()) {}
  //"LMP91000 Register 0x1X= 3\r\n"
  for (int i = 0; i < 3; i++) {
    dataString = _mySerial->readStringUntil('\n');
    String subS = dataString.substring(dataString.indexOf('=') + 1, dataString.length());
    LMP[i] = subS.toInt();
    DEBUG_PRINT("LMP R");
    DEBUG_PRINT((long)i);
    DEBUG_PRINT(" = ");
    DEBUG_PRINTLN((long)LMP[i]);
    if (LMP[i] < 3 || LMP[i] > 200) return 0;
  }
  return 1;
}

int DGS::zero(void)
{
  String commandString;
  DEBUG_PRINTLN("Starting Zero");
  while (_mySerial->available()) _mySerial->read();
  _mySerial->flush();
  
  _mySerial->write('Z');

  //should be \r\nSetting zero..." either "done\r\n
  while (!_mySerial->available()) {}
  commandString = _mySerial->readStringUntil('\n');
  DEBUG_PRINTLN(commandString);

  delay(10);
 
  while (!_mySerial->available()) {}
  commandString = _mySerial->readStringUntil('\n');
  DEBUG_PRINTLN(commandString); 
  
  if (commandString == "Setting zero...done\r") {
    DEBUG_PRINTLN("Success");
    return 1;
  }
  else DEBUG_PRINTLN("Failed Zero");
  return 0;
}

int DGS::setXSpan(float X)
{
  String commandString;
  float inSf;
  DEBUG_PRINTLN("Starting Span Setup");
  while (_mySerial->available()) _mySerial->read();
  _mySerial->write('S');

  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);
  delay(10);
  if (commandString == "Enter Unlock Code: ") {
    unlock();
  }
/*   else {
    DEBUG_PRINT("Failed Unlock");
    return 0;
  } */

  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);
  delay(10);
  if (commandString == "\r\nEnter span gas value in PPM: ") {
    _mySerial->print(X);
    _mySerial->write('\r');
  }
  else {
    DEBUG_PRINT("Failed Span");
    return 0;
  }
  while (!_mySerial->available()) {}
  inSf = _mySerial->parseFloat();
  DEBUG_PRINT(inSf);

  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);

  if (inSf == X)
  {

  }
  while (!_mySerial->available()) {
    delay(1);
  }
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);
  delay(10);
  if (commandString == "done\r\n") {
    DEBUG_PRINTLN("Success");
    return 1;
  }
  else DEBUG_PRINTLN("Failed Span");
  return 0;

  delay(10);
}

long DGS::getConc(char u)
{
  if (u == 'c') return dataArray[4];  //returns counts
  else return dataArray[1];
}

int DGS::setAve(int t)
{
	if (t>300) t = 300;
	else if (t<1) t = 1;
	String commandString;
	DEBUG_PRINTLN("Setting Average time (range: 1-300 seconds)");
	while (_mySerial->available()) _mySerial->read();
	_mySerial->write('A');

	while (!_mySerial->available()) {}
	commandString = _mySerial->readString();
	DEBUG_PRINT(commandString);

	if (commandString == "\r\nEnter Average Total: ") {
		_mySerial->print(t);
		_mySerial->write('\r');
		delay(100);
		
	}
	else {
		while (_mySerial->available()) _mySerial->read();
		DEBUG_PRINTLN("Failed setting Average");
		return 0;
	}
	while (!_mySerial->available()) {}
	commandString = _mySerial->readStringUntil('\n');
	DEBUG_PRINTLN(commandString); 
	return 1;
}
  
float DGS::getTemp(char u)
{	
  if (u == 'c') return dataArray[2]; //return integer in degC
  float temp = dataArray[5]/65536.0*175.72;
  temp = temp - 46.85;
  if (u == 'F') return temp * 9 / 5 + 32;
  else return temp; //return counts converted to C
}

float DGS::getRh(char u)
{
  if (u == 'r') return dataArray[3];
  float humid = dataArray[6]/65536.0*125.0;
  humid = humid - 6.0;
  return humid;
}
