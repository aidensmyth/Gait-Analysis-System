#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define SAMPLE_RATE_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055(55);

int starttime = millis();
int endtime = starttime;
int loopcount = 0;

int xArray[600]; // Creates array 600 long
int yArray[600];
int zArray[600];
int ind = 0; // Defines the index value

int max_x = xArray[0]; // Define max x value
int min_x = xArray[0]; // Define min x value
int max_y = yArray[0];
int min_y = yArray[0];
int max_z = zArray[0];
int min_z = zArray[0];

// Sensor calibration

void displayCalStatus(void)
{
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  /* The data should be ignored until the system calibration is > 0 */
  Serial.print("\t");
  if (!system)
  { 
    Serial.print("! ");
  }

  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.print(mag, DEC);

}

// Setup code
void setup(void) {
  // put your setup code here, to run once:
  Serial.begin(115200);

  if (!bno.begin())
  {
    Serial.print("No BNO055");
    while (1);
  }
  delay(1000);

   // Orientation calibration
  bno.setExtCrystalUse(true);
}

// Main loop, gets cords for 60 seconds, stores in array, finds the min and max values for angle calculation
void loop(void)
{  
  while ((endtime - starttime) <= 60000)
  {
    
    sensors_event_t event;
    bno.getEvent(&event);
    
    float x = event.orientation.x; //Gives angle from north
    xArray[ind] = x; // Stores in array
    float y = event.orientation.y; //Gives angle from horizontal, +90 up, -90 down
    yArray[ind] = y; // Stores in array
    float z = event.orientation.z; //Gives angle of rotation along device, +180 CCW, -180 CW
    zArray[ind] = z; // Stores in array
    ind++; // iterates index

    Serial.print("X: ");
    Serial.print(x, 4);
    Serial.print("\tY: ");
    Serial.print(y, 4);
    Serial.print("\tZ: ");
    Serial.print(z, 4);

    displayCalStatus();
    Serial.println("");
    
    loopcount = loopcount+1;
    endtime = millis();
    
    delay(SAMPLE_RATE_MS);
  }
  for (int j = 0; j < 600; j++) 
  { 
    max_x = max(xArray[j],max_x);
    min_x = min(xArray[j],min_x);

    max_y = max(yArray[j],max_y);
    min_y = min(yArray[j],min_y);

    max_z = max(zArray[j],max_z);
    min_z = min(zArray[j],min_z);
  }
  Serial.print("Max X: ");
  Serial.println(max_x, 4);
  Serial.print("\tMax Y: ");
  Serial.println(max_y, 4);
  Serial.print("\tMax Z: ");
  Serial.println(max_z, 4);
  
  Serial.print("Min X: ");
  Serial.println(min_x, 4);
  Serial.print("\tMin Y: ");
  Serial.println(min_y, 4);
  Serial.print("\tMin Z: ");
  Serial.println(min_z, 4);
}

//  if(int i = 100) { 
//    
//    Serial.println("60 seconds has passed ");
//    delay(100);
//    
////    xArray[12] = -9; Dummy cords for testing, you can simply delete them if you want
////    xArray[2] = 20;
////    yArray[12] = -2;
////    yArray[2] = 10;
////    zArray[12] = -1;
////    zArray[2] = 100;
//    
//    for (int j = 0; j < 360; j++) { // for statement finds the min and max of x, y, z array
//      max_x = max(xArray[j],max_x);
//      min_x = min(xArray[j],min_x);
//
//      max_y = max(yArray[j],max_y);
//      min_y = min(yArray[j],min_y);
//
//      max_z = max(zArray[j],max_z);
//      min_z = min(zArray[j],min_z);
//    }
//    // print out the desired output
//    Serial.print("This is the max x cord: ");
//    Serial.println(max_x);
//    Serial.print("This is the min x cord: ");
//    Serial.println(min_x);
//    Serial.print("This is the max y cord: ");
//    Serial.println(max_y);
//    Serial.print("This is the min y cord: ");
//    Serial.println(min_y);
//    Serial.print("This is the max z cord: ");
//    Serial.println(max_z);
//    Serial.print("This is the min z cord: ");
//    Serial.println(min_z);
//    
//    delay(100);
//    
//    exit(0);
//  }
//}
