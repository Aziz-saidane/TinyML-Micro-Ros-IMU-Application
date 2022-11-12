

#include "MPU9250.h"

#include <Wire.h>

MPU9250 mpu;

const float accelerationThreshold = 3.5; // threshold of significant in G's
const int numSamples = 119;

int samplesRead = numSamples;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2000);
  
  while (!Serial);

   if (!mpu.setup(0x68)) {  // change to your own address
        while (1) {
           // Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
  }

  // calibrate IMU///////////////////

    Serial.println("Please leave the device still on the flat plane.");
    mpu.verbose(true);
    delay(5000);
    mpu.calibrateAccelGyro();
    mpu.verbose(false);

    // END calibrate IMU///////////////

  // print the header
  Serial.println("aX,aY,aZ,gX,gY,gZ");
}

void loop() {
  float aX, aY, aZ, gX, gY, gZ;

  // wait for significant motion
  while (samplesRead == numSamples) {
    if (mpu.update()) { 
    // read the acceleration data
       aX = mpu.getAccX() ;
       aY = mpu.getAccY() ;
       aZ = mpu.getAccZ() ;
      
    // sum up the absolutes
       float aSum = fabs(aX) + fabs(aY) + fabs(aZ);
             
    // check if it's above the threshold
       if (aSum >= accelerationThreshold) {
    // reset the sample read count
       samplesRead = 0;
       break;
         }
     }
  }

  // check if the all the required samples have been read since
  // the last time the significant motion was detected
  while (samplesRead < numSamples) {
    // check if both new acceleration and gyroscope data is
    // available
      mpu.update() ;
      // read the acceleration and gyroscope data
      aX = mpu.getAccX() ;
      aY = mpu.getAccY() ;
      aZ = mpu.getAccZ() ;
      gX =  mpu.getGyroX() ;
      gY =  mpu.getGyroY() ;
      gZ =  mpu.getGyroZ() ;

      samplesRead++;

      // print the data in CSV format
      Serial.print(aX, 3);
      Serial.print(',');
      Serial.print(aY, 3);
      Serial.print(',');
      Serial.print(aZ, 3);
      Serial.print(',');
      Serial.print(gX, 3);
      Serial.print(',');
      Serial.print(gY, 3);
      Serial.print(',');
      Serial.print(gZ, 3);
      Serial.println();
      delay(5);
      
      

      if (samplesRead == numSamples) {
        // add an empty line if it's the last sample
        Serial.println();
      }
  }
}
