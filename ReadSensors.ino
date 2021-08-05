
/*
 * Read the sensors 
 * mxsy = module x sensor y
 * There are three modules m1, m2, m3
 * m1 have three sensors: s1, s2, s3
 * m2 and m3 have two sensors: s1, s2
 * Based on the example SeralCallResponse by Tom Igoe, Scott Fitgerald and Greg Shakar
 * Requires a sketch writed in Processing on the PC side https://processing.org/
 *
 */
#include <Arduino.h>
#include <Ewma.h>
#include <Wire.h>

// filter
Ewma adc0Filter(10);  // More smoothing - less prone to noise, but slower to detect changes
Ewma adc1Filter(10);
Ewma adc2Filter(10);
Ewma adc3Filter(10);   
Ewma adc4Filter(10); 
Ewma adc5Filter(10); 
Ewma adc6Filter(10); 
Ewma accx1Filter(8); 
Ewma accy1Filter(8); 
Ewma accz1Filter(8);
Ewma gyrx1Filter(8);  
Ewma gyry1Filter(8);
Ewma gyrz1Filter(8);
Ewma accx2Filter(8); 
Ewma accy2Filter(8); 
Ewma accz2Filter(8);
Ewma gyrx2Filter(8);  
Ewma gyry2Filter(8);
Ewma gyrz2Filter(8);
 //MPU variables
  const int MPU2 = 0x69, MPU1=0x68;
  int16_t accelX1 = 0, accelY1 = 0, accelZ1 = 0;
  int16_t gyroX1 = 0, gyroY1 = 0, gyroZ1 = 0;
  int16_t accelX2 = 0, accelY2 = 0 , accelZ2 = 0;
  int16_t gyroX2 = 0, gyroY2 = 0, gyroZ2 = 0;
 //Draw-wire sensors
  int16_t m1s1 = 0; //module 1, sensor 1
  int16_t m1s2 = 0; //module 1, sensor 2
  int16_t m1s3 = 0; //module 1, sensor 3
  int16_t m2s1 = 0; //module 2, sensor 1
  int16_t m2s2 = 0; //module 2, sensor 2
  int16_t m3s1 = 0; //module 3, sensor 1
  int16_t m3s2 = 0; //module 3, sensor 2
  int inByte = 0; //incoming serial byte
  
void setup() {
  //MPU sensor 1
  Wire.begin();
  Wire.beginTransmission(MPU1);
  Wire.write(0x6B);
  Wire.write(0b00000000);
  Wire.endTransmission();  
  Wire.beginTransmission(MPU1);
  Wire.write(0x1B);
  Wire.write(0x00000000);
  Wire.endTransmission(); 
  Wire.beginTransmission(MPU1);
  Wire.write(0x1C);
  Wire.write(0b00000000);
  Wire.endTransmission(); 
  //MPU Sensor 2
  Wire.begin();
  Wire.beginTransmission(MPU2);
  Wire.write(0x6B);
  Wire.write(0b00000000); 
  Wire.endTransmission();  
  Wire.beginTransmission(MPU2); 
  Wire.write(0x1B);
  Wire.write(0x00000000);
  Wire.endTransmission(); 
  Wire.beginTransmission(MPU2);
  Wire.write(0x1C);
  Wire.write(0b00000000);
  Wire.endTransmission(); 
  //start seriall port at 9600 bps
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect, Needed for native USB port only
  }
  establishContact(); //send a byte to establish contact until receiver responds
}

void loop() {
  // if we get a valid byte, read analog ins:

  if (Serial.available() >0) {
    // get incoming byte
    inByte = Serial.read();
    //Get MPU1 and MPU2
    GetMpuValue();
    
    //send the draw wire sensor values
    Serial.write(highByte(m1s1));
    Serial.write(lowByte(m1s1));
    Serial.write(highByte(m1s2));
    Serial.write(lowByte(m1s2));
    Serial.write(highByte(m1s3));
    Serial.write(lowByte(m1s3));
    Serial.write(highByte(m2s1));
    Serial.write(lowByte(m2s1));
    Serial.write(highByte(m2s2));
    Serial.write(lowByte(m2s2));
    Serial.write(highByte(m3s1));
    Serial.write(lowByte(m3s1));
    Serial.write(highByte(m3s2));
    Serial.write(lowByte(m3s2));
   //send gyro data MPU1
    Serial.write(highByte(gyroX1));
    Serial.write(lowByte(gyroX1));
    Serial.write(highByte(gyroY1));
    Serial.write(lowByte(gyroY1));
    Serial.write(highByte(gyroZ1));
    Serial.write(lowByte(gyroZ1));
   //send accelerometer data MPU1;
    Serial.write(highByte(accelX1));
    Serial.write(lowByte(accelX1));
    Serial.write(highByte(accelY1));
    Serial.write(lowByte(accelY1));
    Serial.write(highByte(accelZ1));
    Serial.write(lowByte(accelZ1));
   //send gyro data MPU2
    Serial.write(highByte(gyroX2));
    Serial.write(lowByte(gyroX2));
    Serial.write(highByte(gyroY2));
    Serial.write(lowByte(gyroY2));
    Serial.write(highByte(gyroZ2));
    Serial.write(lowByte(gyroZ2));
   //send accelerometer data MPU2;
    Serial.write(highByte(accelX2));
    Serial.write(lowByte(accelX2));
    Serial.write(highByte(accelY2));
    Serial.write(lowByte(accelY2));
    Serial.write(highByte(accelZ2));
    Serial.write(lowByte(accelZ2));

  }

}

//get mpu values
void GetMpuValue(){
//read MPU1
  Wire.beginTransmission(MPU1); 
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(MPU1,6);
  while(Wire.available() < 6);
  accelX1 = accx1Filter.filter((Wire.read()<<8|Wire.read())); 
  accelY1 = accy1Filter.filter((Wire.read()<<8|Wire.read())); 
  accelZ1 = accz1Filter.filter((Wire.read()<<8|Wire.read()));
  
  Wire.beginTransmission(MPU1);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(MPU1,6);
  
  while(Wire.available() < 6);
   //read gyro values
  gyroX1 = gyrx1Filter.filter((Wire.read()<<8|Wire.read()));
  gyroY1 = gyry1Filter.filter((Wire.read()<<8|Wire.read()));
  gyroZ1 = gyrz1Filter.filter((Wire.read()<<8|Wire.read())); 
  delay(100);
//read MPU2
  Wire.beginTransmission(MPU2); 
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(MPU2,6);
  while(Wire.available() < 6);
  accelX2 = accx2Filter.filter((Wire.read()<<8|Wire.read())); 
  accelY2 = accy2Filter.filter((Wire.read()<<8|Wire.read())); 
  accelZ2 = accz2Filter.filter((Wire.read()<<8|Wire.read()));
  
  Wire.beginTransmission(MPU2);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(MPU2,6);
  
  while(Wire.available() < 6);
   //read gyro values
  gyroX2 = gyrx2Filter.filter((Wire.read()<<8|Wire.read()));
  gyroY2 = gyry2Filter.filter((Wire.read()<<8|Wire.read()));
  gyroZ2 = gyrz2Filter.filter((Wire.read()<<8|Wire.read())); 
  delay(100);
  
//read analog inputs 
  // read first analog input, divide by 4 to make the range 0-255:
    m1s1 = adc0Filter.filter(analogRead(0));
    delay(10);
    m1s2 = adc1Filter.filter(analogRead(1));
    delay(10);
    m1s3 = adc2Filter.filter(analogRead(2));
    delay(10);
    m2s1 = adc3Filter.filter(analogRead(3));
    delay(10);
    m2s2 = adc4Filter.filter(analogRead(4));
    delay(10);
    m3s1 = adc5Filter.filter(analogRead(5));
    delay(10);
    m3s2 = adc6Filter.filter(analogRead(6));
    delay(10);
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}
