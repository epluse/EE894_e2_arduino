/***************************************************************************/
/* sourcefile for "ee894E2.h" module */
/***************************************************************************/
/*
Read functions for measurement values of the EE894 Sensor via E2 interface.

Copyright 2021 E+E Elektronik Ges.m.b.H.

Disclaimer:
This application example is non-binding and does not claim to be complete with regard
to configuration and equipment as well as all eventualities. The application example
is intended to provide assistance with the EE894 sensor module design-in and is provided "as is".
You yourself are responsible for the proper operation of the products described.
This application example does not release you from the obligation to handle the product safely
during application, installation, operation and maintenance. By using this application example,
you acknowledge that we cannot be held liable for any damage beyond the liability regulations
described.

We reserve the right to make changes to this application example at any time without notice.
In case of discrepancies between the suggestions in this application example and other E+E
publications, such as catalogues, the content of the other documentation takes precedence.
We assume no liability for the information contained in this document.
*/

// Includes
//-----------------------------------------------------------------------------
#include "ee894E2.h"
#include <Arduino.h>
#include <e2Interface.h>

// Definitions
//-----------------------------------------------------------------------------
#define ACK 1
#define NAK 0

extern e2Interface e2(18,19);

ee894E2::ee894E2(int _SDA, int _SCL)
{
   e2 = e2Interface(_SDA,_SCL);
}

ee894E2::ee894E2(void)
{
   e2 = e2Interface(18,19);
}


uint8_t ee894E2::readMeasValue(float &value ,unsigned char controlBytes[], int scaleFactor)
{
  stE2Return e2Return;
  e2Return.status = readStatus();
  if(e2Return.status == E2_OK)
  {
	e2Return  = e2.readShortFromSlave(controlBytes);
  }
  if(e2Return.status != E2_OK)
  {
	return e2Return.status;
  }
  
  value = (float)e2Return.data / scaleFactor;
  return E2_OK;
}

uint8_t ee894E2::readRh(float &humidity) 					//read measurement value 1 (relative humidity [%RH])
{  
  unsigned char controlBytes[2] = {CB_MV1LO,CB_MV1HI};
  return readMeasValue(humidity, controlBytes, 100);
}


uint8_t ee894E2::readTemp(float &temperature) 					//read measurement value 2 (temperature [K])
{ 
  unsigned char controlBytes[2] = {CB_MV2LO,CB_MV2HI};
  return readMeasValue(temperature, controlBytes, 100);
}


uint8_t ee894E2::readPres(float &pressure) 				//read measurement value 3 (ambient pressure [mbar])
{
  unsigned char controlBytes[2] = {CB_MV3LO,CB_MV3HI};
  return readMeasValue(pressure, controlBytes, 10);
}


uint8_t ee894E2::readCo2Mean(float &Co2) 			//read measurement value 4 (CO2 mean [ppm])
{ 
  unsigned char controlBytes[2] = {CB_MV4LO,CB_MV4HI};
  return readMeasValue(Co2, controlBytes, 1);
}


uint8_t ee894E2::readStatus(void)	//read statusbyte from E2-Interface
{ 
  stE2Return e2Return;
  e2Return = e2.readByteFromSlave(CB_STATUS);
  if (e2Return.status == E2_OK)
  { 
	if(e2Return.data != 0)
	{ 
	  return E2_ERR_MEAS;
	}
  }
  return e2Return.status;
}


uint8_t ee894E2::readSensorType(unsigned int &sensorType) //read sensortype from E2-Interface
{ 
  stE2Return e2Return;
  unsigned char controlBytes[2] = {CB_TYPELO,CB_TYPEHI};
  e2Return = e2.readShortFromSlave(controlBytes);
  if (e2Return.status != E2_OK)
  {
	return e2Return.status;
	 
  }
  sensorType = e2Return.data;
  return E2_OK;
}


uint8_t ee894E2::readSensorSubType(uint8_t &sensorSubType) 					//read sensor subtype from E2-Interface
{ 
  stE2Return e2Return;
  e2Return = e2.readByteFromSlave(CB_TYPESUB);
  if (e2Return.status != E2_OK)
  {
	return e2Return.status;
  }
  sensorSubType = e2Return.data;
  return E2_OK;
}


uint8_t ee894E2::readAvailablePhysicalMeasurements(uint8_t &availablePhysicalMeasurements) 	//read available physical measurements from E2-Interface
{ 
  stE2Return e2Return;
  e2Return = e2.readByteFromSlave(CB_AVPHMES);
  if (e2Return.status != E2_OK)
  {
	return e2Return.status;
  }
  availablePhysicalMeasurements = e2Return.data;
  return E2_OK;
}


void ee894E2::getErrorString(uint8_t e2Status, char errorString[])
{
  memset(errorString, '\0', sizeof(errorString));
  switch(e2Status)
  {
  case E2_OK: 
    strncpy(errorString,"Success",30); 
    break;  
  case E2_ERR_NAK: 
    strncpy(errorString,"Not acknowledge error",30); 
    break;
  case E2_ERR_CKSUM: 
    strncpy(errorString,"Checksum error",30);
    break;
	case E2_ERR_MEAS: 
    strncpy(errorString,"Measurment error",30);
    break;
  default:
    strncpy(errorString,"unknown error",30);
    break;
  }	
}
