/*
DGS.cpp - Library for reading KWJ Engineering with SPEC Sensors on Digital SDK.
Created by David E. Peaslee, NOV 27, 2016.
Updated by David Peaslee, MAR 24, 2021
*/

#include "DGS.h"

DGS::DGS(Stream *mySerial) : _mySerial(mySerial) {
  

}
DGS::DGS(Stream &mySerial) : _mySerial(&mySerial) {
  

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

int DGS::setToff(float offset)
{
  String commandString;
  DEBUG_PRINTLN("Starting T Offset");
  while (_mySerial->available()) _mySerial->read();
  _mySerial->write('T');
  getData('T');
  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);
  delay(10);
  if (commandString == "Enter Unlock Code: ") {
    unlock();
  }
  else {
    DEBUG_PRINTLN("Failed Unlock");
    return 0;
  }

  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);
  delay(10);
  if (commandString == "\r\nEnter Temperature_Offset: ") {
    _mySerial->print(offset);
    _mySerial->write('\r');
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
  if (commandString.toFloat() == offset) {
    DEBUG_PRINTLN("Success");
    getData('\r');
    return 1;
  }

  else
  {
    getData('\r');
    delay(3000);
    return 0;
  }
}

int DGS::setBC(char * BC)
{
  String commandString;
  DEBUG_PRINTLN("Starting BC Setup");
  while (_mySerial->available()) _mySerial->read();
  _mySerial->write('B');
  getData('B');
  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);

  delay(10);
  if (commandString == "Enter Unlock Code: ") {
    unlock();
  }
  else {
    DEBUG_PRINT("Failed Unlock");
    return 0;
  }

  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);
  delay(10);
  if (commandString == "\r\nRemove Sensor and Scan: ") {

    for (int i = 0; i < strlen(BC) + 1; i++) {
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
  DEBUG_PRINTLN(commandString);
  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);
  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);
  if (commandString == "done\r\nSetting zero...done\r\n") {
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
  _mySerial->write('e');
  getData('e');

  String data = _mySerial->readStringUntil('\n');
  DEBUG_PRINTLN(data);
  while (!_mySerial->available()) {}
  for (int i = 1; i < 14; i++) {
    data = _mySerial->readStringUntil('\n');
    String subS1 = data.substring(0, data.indexOf('='));
    String subS2 = data.substring(data.indexOf('=') + 2);
    eepromInt[i] = subS2.toInt();
    DEBUG_PRINT(subS1);
    DEBUG_PRINT("= ");
    DEBUG_PRINTLN(eepromInt[i]);
  }
  for (int i = 14; i < 19; i++) {
    data = _mySerial->readStringUntil('\n');
    String subS1 = data.substring(0, data.indexOf('='));
    String subS2 = data.substring(data.indexOf('=') + 2);
    eepromStr[i - 14] = subS2;
    DEBUG_PRINT(subS1);
    DEBUG_PRINT("= ");
    DEBUG_PRINTLN(eepromStr[i - 14]);
  }
  data = _mySerial->readStringUntil('\n');
  String subS1 = data.substring(0, data.indexOf('='));
  String subS2 = data.substring(data.indexOf('=') + 2);
  Sensitivity_Code = subS2.toFloat();
  DEBUG_PRINT(subS1);
  DEBUG_PRINT("= ");
  DEBUG_PRINTLN(Sensitivity_Code);
}

String DGS::getFW(void)
{
  String dataString;
  DEBUG_PRINTLN("Getting FW");
  while (_mySerial->available()) _mySerial->read();
  _mySerial->write('f');
  getData('f');

  while (!_mySerial->available()) {}
  dataString = _mySerial->readStringUntil('\r');
  _mySerial->flush();
  return dataString;
  while (_mySerial->available()) _mySerial->read();
}

int DGS::setLMP(int R1, int R2, int R3)
{
  String commandString;
  DEBUG_PRINTLN("Setting LMP Registers");
  while (_mySerial->available()) _mySerial->read();
  _mySerial->write('L');


  getData('L');
  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  DEBUG_PRINTLN(commandString);
  delay(10);
  if (commandString == "Enter Unlock Code: ") {
    unlock();
  }
  else {
    DEBUG_PRINTLN("Failed Unlock");
    return 0;
  }

  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);
  if (commandString == "\r\nEnter LMP91000 Register 0x10: ") {
    _mySerial->print(R1);
    _mySerial->write('\r');
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
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);
  if (commandString == "Enter LMP91000 Register 0x11: ") {
    _mySerial->print(R2);
    _mySerial->write('\r');
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
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);
  if (commandString == "Enter LMP91000 Register 0x12: ") {
    _mySerial->print(R3);
    _mySerial->write('\r');
  }
  else {
    DEBUG_PRINTLN("Setting Failed");
    return 0;
  }
  while (!_mySerial->available()) {}
  commandString = _mySerial->readStringUntil('\n');
  DEBUG_PRINTLN(commandString);
  if (commandString.toInt() != R3) {
    DEBUG_PRINTLN("Failure");
    delay(2000);
    return 0;
  }
  else {
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
  _mySerial->write('l');
  getData('l');

  while (!_mySerial->available()) {}
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
  _mySerial->write('Z');
  getData('Z');

  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();


  DEBUG_PRINTLN(commandString);

  delay(10);
  if (commandString == "Enter Unlock Code: ") {
    unlock();
  }
  else {
    DEBUG_PRINTLN("Failed Unlock");
    return 0;
  }

  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);

  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);
  commandString.remove(commandString.length() - 2);
  if (commandString == "done") {
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
  getData('S');

  while (!_mySerial->available()) {}
  commandString = _mySerial->readString();
  DEBUG_PRINT(commandString);
  delay(10);
  if (commandString == "Enter Unlock Code: ") {
    unlock();
  }
  else {
    DEBUG_PRINT("Failed Unlock");
    return 0;
  }

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

long DGS::getConc(char c)
{
  if (c == 'c')
    return dataArray[4];
  else return dataArray[1];
}

long DGS::getTemp(char u)
{
  if (u == 'F')
    return dataArray[2] * 9 / 5 + 32;
  else return dataArray[2];
}

long DGS::getRh(void)
{
  return dataArray[3];
}
