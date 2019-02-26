
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

#define LIFO_LENGTH       200
#define NUM_LIFOS          13
#define ACCEL_X_LIFO        0
#define ACCEL_Y_LIFO        1
#define ACCEL_Z_LIFO        2
#define GYRO_X_LIFO         3
#define GYRO_Y_LIFO         4
#define GYRO_Z_LIFO         5
#define MAG_X_LIFO          6
#define MAG_Y_LIFO          7
#define MAG_Z_LIFO          8
#define BARO_ALT_LIFO       9
#define BARO_PRESS_LIFO    10
#define BARO_TEMP_LIFO     11
#define STRATO_LIFO        12

float lifos[NUM_LIFOS][LIFO_LENGTH]; // One Lifo for each sensor
uint32_t timeStamps[NUM_LIFOS][LIFO_LENGTH];
char *dataTags[NUM_LIFOS] = {"ACCEL_X", "ACCEL_Y", "ACCEL_Z", "GYRO_X", "GYRO_Y", "GYRO_Z", "MAG_X", "MAG_Y", "MAG_Z", "ALTITUDE", "PRESSURE", "TEMPERATURE"};
int lifoTops[NUM_LIFOS];

void getMostRecentReadings(int numReadings, float *values[NUM_LIFOS], uint32_t *timeStamps[NUM_LIFOS]);

bool getLifo(int lifoNum, float* elem, uint32_t* timeStamp);
bool putLifo(int lifoNum, float newElem);

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
//#include <SoftwareSerial.h>
//#define STRATO_SERIAL_RX 0
//#define STRATO_SERIAL_TX 1
//SoftwareSerial stratoSerial(STRATO_SERIAL_RX, STRATO_SERIAL_TX); // RX, TX
//Stratologger uses the normal serial lines so don't need to do anything

// SD card Includes and Variable Defintions
#include <SD.h>
#define WRITE_BACK_THRESHOLD 900
int dataPointsSaved = 0;

// Timer Includes and Definitions
#include <avr/io.h>
#include <avr/interrupt.h>

IntervalTimer stratoTimer;
IntervalTimer accelTimer;
IntervalTimer baroTimer;

void setup() 
{
  sei();
  Serial.begin(9600);
  //stratoSerial.begin(9600);
  for(int i = 0; i < NUM_LIFOS; i++)
  {lifoTops[i] = -1;}

  SD.begin(BUILTIN_SDCARD);
  setupDatalog();// set up data log file
  
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  imu.begin();

  baro.begin();

  stratoTimer.begin(stratoHandler, 50000);
  accelTimer.begin(accelHandler, 12500);
  baroTimer.begin(baroHandler, 12500);
  
}

void loop() 
{
  updateSensorReadings();
  if(dataPointsSaved == WRITE_BACK_THRESHOLD)
  {
    cli();
    logData();
    sei();
  } 
}

void updateSensorReadings()
{
  updateAccelReadings();
  updateGyroReadings();
  updateMagReadings();
  updateBaroReadings();
  updateStratoReadings();
}

// Lifo functions
bool getLifoElements(int lifoNum, int numReadings, float elems[LIFO_LENGTH], uint32_t tStamps[LIFO_LENGTH])
{
  cli();
  if(numReadings > LIFO_LENGTH) {numReadings = LIFO_LENGTH;}
  for(int i = 0; i < numReadings; i++)
  {
    elems[i] = lifos[lifoNum][i];
    tStamps[i] = timeStamps[lifoNum][i];
  }
  sei();
  return 1;
    
} 

bool getLifo(int lifoNum, float* elem, uint32_t *timeStamp)
{
  cli();
  *elem = lifos[lifoNum][0];
  *timeStamp = timeStamps[lifoNum][0];
  sei();
  return 1;
}



/* bool getLifo(int lifoNum, float* elem, uint32_t *timeStamp)
{
  cli();
  if(lifoTops[lifoNum] == -1) {*elem = NULL; sei(); return 0;}

  *elem = lifos[lifoNum][lifoTops[lifoNum]];
  *timeStamp = timeStamps[lifoNum][lifoTops[lifoNum]];
  lifoTops[lifoNum]--;
  sei();
  return 1;
    
} */

bool putLifo(int lifoNum, float newElem)
{
  cli();
  for(int i = LIFO_LENGTH - 1; i > 0; i--)
  {
    lifos[lifoNum][i] = lifos[lifoNum][i - 1];
    timeStamps[lifoNum][i] = timeStamps[lifoNum][i - 1];
  };  lifos[lifoNum][0] = newElem;
  timeStamps[lifoNum][0] = millis();
  sei();
  return 1;
}

/*bool putLifo(int lifoNum, float newElem)
{
  cli();
  
  if(lifoTops[lifoNum] == LIFO_LENGTH)
  {
    for(int i = LIFO_LENGTH; i > 0; i--)
    {
      lifos[lifoNum][i - 1] = lifos[lifoNum][i];
      timeStamps[lifoNum][i-+ 1] = timeStamps[lifoNum][i];
    }
    lifoTops[lifoNum]--;
  }
  
  lifos[lifoNum][++lifoTops[lifoNum]] = newElem;
  timeStamps[lifoNum][lifoTops[lifoNum]] = millis();
  sei();
  return 1;
  
} */

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
    putLifo(ACCEL_X_LIFO, imu.ax);
    putLifo(ACCEL_Y_LIFO, imu.ay);
    putLifo(ACCEL_Z_LIFO, imu.az);
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
  putLifo(ACCEL_X_LIFO, ax);
  putLifo(ACCEL_Y_LIFO, ay);
  putLifo(ACCEL_Z_LIFO, az);
}

void updateGyroReadings()
{
  if (imu.gyroAvailable())
  {
    imu.readGyro();
    putLifo(GYRO_X_LIFO, imu.gx);
    putLifo(GYRO_Y_LIFO, imu.gy);
    putLifo(GYRO_Z_LIFO, imu.gz);
  }
}

void updateMagReadings()
{
  if (imu.magAvailable())
  {
    imu.readMag();
    putLifo(MAG_X_LIFO, imu.mx);
    putLifo(MAG_Y_LIFO, imu.my);
    putLifo(MAG_Z_LIFO, imu.mz);
  }
}

// Barometer functions
void baroHandler()
{
  
}

void updateBaroReadings()
{
  putLifo(BARO_ALT_LIFO, baro.getAltitude());
  putLifo(BARO_PRESS_LIFO, baro.getPressure());
  putLifo(BARO_TEMP_LIFO, baro.getTemperature());
}

void getMostRecentReadings(int numReadings, float *values[NUM_LIFOS], uint32_t *timeStamps[NUM_LIFOS])
{
  for(int i = 0; i < NUM_LIFOS; i++)
  {
    float elems[numReadings];
    uint32_t tStamps[numReadings];
    getLifoElements(i, numReadings, elems, tStamps);
    values[i] = elems;
    timeStamps[i] = tStamps;
  }
}

// Strato functions
void updateStratoReadings()
{
    String message;
    char lastChar = 'a';
    while(lastChar != '\n')
    {
      char lastChar = Serial.read();
      message += (char)lastChar;     
    }
    float value = message.toFloat();
    putLifo(STRATO_LIFO, value);
}

void stratoHandler()
{
  
}

// SD card functions
void setupDatalog()
{
  if(!SD.exists("datalog.txt"))
  {
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    dataFile.println("__New Launch__");
  }
}

void logData()
{
  cli();
  dataPointsSaved -= WRITE_BACK_THRESHOLD;
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  float *values[NUM_LIFOS];
  uint32_t *timeStamps[NUM_LIFOS];
  getMostRecentReadings(WRITE_BACK_THRESHOLD, values, timeStamps);
  for(int lifoNum = 0; lifoNum < NUM_LIFOS; lifoNum++)
  {
    float elems[LIFO_LENGTH];
    uint32_t tStamps[LIFO_LENGTH];
    getLifoElements(lifoNum, WRITE_BACK_THRESHOLD, elems, tStamps);
    int pointsToWriteBack = (WRITE_BACK_THRESHOLD / 180) * 20;
    for(int i = 0; i < pointsToWriteBack; i++)
    {
      dataFile.print(dataTags[lifoNum]);
      dataFile.print(',');   
      dataFile.print(elems[i]);
      dataFile.print(',');     
      dataFile.println(tStamps[i]); 
    }
  }
  dataFile.close();
  sei();
}

#else

int main(int argc, char* argv[]) {}

#endif
