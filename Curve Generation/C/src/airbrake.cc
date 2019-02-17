
#include "constant_area_drag_calculator.h"
#include "density_calculator.h"
#include "acceleration_calculator.h"

#ifdef ARDUINO

#include <Arduino.h>

// Global flags to configure code
#define ACCEL_NUMBER  0  // a flag that contains the number code for which accelerometer chip we are using 
#define USING_LSM     0
#define USING_ADX     1
void updateAccelReadings();

#define FIFO_LENGTH       200
#define NUM_FIFOS          12
#define ACCEL_X_FIFO        0
#define ACCEL_Y_FIFO        1
#define ACCEL_Z_FIFO        2
#define GYRO_X_FIFO         3
#define GYRO_Y_FIFO         4
#define GYRO_Z_FIFO         5
#define MAG_X_FIFO          6
#define MAG_Y_FIFO          7
#define MAG_Z_FIFO          8
#define BARO_ALT_FIFO       9
#define BARO_PRESS_FIFO    10
#define BARO_TEMP_FIFO     11

float fifos[NUM_FIFOS][FIFO_LENGTH]; // One Fifo for each sensor
int fifoIndicies[NUM_FIFOS][2];      // 0 == getIndex, 1 == putIndex

void getMostRecentReadings(float values[]);

bool getFifo(int fifoNum, float* elem);
bool putFifo(int fifoNum, float newElem);

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>

// LSM9DS1 Includes and Variables Definitions
#include <SparkFunLSM9DS1.h>
LSM9DS1 imu;
#define LSM9DS1_M  0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW
#define DECLINATION 3.44 // http://www.ngdc.noaa.gov/geomag-web/#declination

// ADX Includes and Variable Definitions
int scale = 200; //can read from -200 to 200 G's
#define ADX_X_PIN A0
#define ADX_Y_PIN A1
#define ADX_Z_PIN A2

// Barometer Includes and Variable Definitions
#include <Adafruit_MPL3115A2.h>
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
void updateBaroReadings();

// Stratologer Includes and Variable Definitions

// SD card Includes and Variable Defintions
#include <SD.h>

// Timer Includes and Definitions
IntervalTimer stratoTimer;
IntervalTimer accelTimer;
IntervalTimer baroTimer;

void stratoHandler();
void baroHandler();
void accelHandler();

void updateGyroReadings();
void updateMagReadings();

void updateLSMreadings();
void updateADXreadings();

void setup() 
{
  Serial.begin(9600);
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  imu.begin();

  baro.begin();

  stratoTimer.begin(stratoHandler, 50000);
  accelTimer.begin(accelHandler, 12500);
  baroTimer.begin(baroHandler, 12500);
}

void updateSensorReadings()
{
  updateAccelReadings();
  updateGyroReadings();
  updateMagReadings();
  updateBaroReadings();
}

void loop()
{
  updateSensorReadings();
}

// Fifo functions
bool getFifo(int fifoNum, float* elem)
{
  if(fifoIndicies[fifoNum][0] == fifoIndicies[fifoNum][1]) 
  {
    *elem = 0;
    return false;
  }
  else
  {
    int getIndex = fifoIndicies[fifoNum][0];
    *elem = fifos[fifoNum][getIndex];
    fifoIndicies[fifoNum][0] = (getIndex + 1) % FIFO_LENGTH;
    return true;
  }
}

bool putFifo(int fifoNum, float newElem)
{
  if((fifoIndicies[fifoNum][1] + 1) % FIFO_LENGTH == fifoIndicies[fifoNum][0]) 
  {
     return false;
  }
  else
  {
     int putIndex = fifoIndicies[fifoNum][1];
     fifos[fifoNum][putIndex] = newElem;
     fifoIndicies[fifoNum][1] = (putIndex + 1) % FIFO_LENGTH;
     return true;
  }
}

// Accel functions
void accelHandler()
{
  
}

void updateAccelReadings()
{
  switch(ACCEL_NUMBER)
  {
   case USING_LSM:
   updateLSMreadings();
   break;

   case USING_ADX:
   updateADXreadings();   
   break;
  }
}

void updateLSMreadings()
{
   if (imu.accelAvailable())
   {
    imu.readAccel();
    putFifo(ACCEL_X_FIFO, imu.ax);
    putFifo(ACCEL_Y_FIFO, imu.ay);
    putFifo(ACCEL_Z_FIFO, imu.az);
   } 
}

void updateADXreadings()
{
  // Get raw accelerometer data for each axis
  int rawX = analogRead(ADX_X_PIN);
  int rawY = analogRead(ADX_Y_PIN);
  int rawZ = analogRead(ADX_Z_PIN);

  float ax = map(rawX, 0, 1023, -scale, scale); // maps readings from 0 to 5v to -200 to 200 G's 
  float ay = map(rawY, 0, 1023, -scale, scale);
  float az = map(rawZ, 0, 1023, -scale, scale);
  putFifo(ACCEL_X_FIFO, ax);
  putFifo(ACCEL_Y_FIFO, ay);
  putFifo(ACCEL_Z_FIFO, az);
}

void updateGyroReadings() {
  if (imu.gyroAvailable())
  {
    imu.readGyro();
    putFifo(GYRO_X_FIFO, imu.gx);
    putFifo(GYRO_Y_FIFO, imu.gy);
    putFifo(GYRO_Z_FIFO, imu.gz);
  }
}

void updateMagReadings()
{
  if (imu.magAvailable())
  {
    imu.readMag();
    putFifo(MAG_X_FIFO, imu.mx);
    putFifo(MAG_Y_FIFO, imu.my);
    putFifo(MAG_Z_FIFO, imu.mz);
  }
}

// Barometer functions
void baroHandler()
{
  
}

void updateBaroReadings()
{
  putFifo(BARO_ALT_FIFO, baro.getAltitude());
  putFifo(BARO_PRESS_FIFO, baro.getPressure());
  putFifo(BARO_TEMP_FIFO, baro.getTemperature());
}

void getMostRecentReadings(float values[])
{
  for(int i = 0; i < NUM_FIFOS; i++)
  {
    float elem;
    getFifo(i, &elem);
    values[i] = elem;
  }
}

// Strato functions
void stratoHandler()
{
  
}

#else

int main(int argc, char* argv[]) {}

#endif
