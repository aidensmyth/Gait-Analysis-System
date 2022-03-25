#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define SAMPLE_RATE_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055(55);

const byte buttonPin = 7;
int buttonNew;
int buttonOld = 1;             
const byte LEDPin = 3;                
int LEDState = 0;                   

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

void OrientationData(void) {
  sensors_event_t event;
    bno.getEvent(&event);

    Serial.print("x: ");
    Serial.print(event.orientation.x);
    Serial.print("\n");
    Serial.print("y: ");
    Serial.print(event.orientation.y);
    Serial.print("\n");
    Serial.print("z: ");
    Serial.print(event.orientation.z);
    Serial.print("\n");

    delay(SAMPLE_RATE_MS);
}

// Setup code
void setup(void) {
  // put your setup code here, to run once:
  pinMode(buttonPin,INPUT);          
  pinMode(LEDPin,OUTPUT);           
  
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

void loop(){
  
on:  
buttonNew = digitalRead(buttonPin);
if(buttonOld == 0 && buttonNew == 1){
  while (LEDState == 0) {
    digitalWrite(LEDPin, HIGH);
    OrientationData();
  
    //displayCalStatus();
    buttonNew = digitalRead(buttonPin);
    Serial.println(buttonNew);
    Serial.println(buttonOld);
   if (buttonNew == 0) {
    LEDState = 1;
    delay(5000);
    goto off;
      }
     }
    }   
off:
while( LEDState == 1) {
  digitalWrite(LEDPin, LOW);
  buttonNew = digitalRead(buttonPin);
  if (buttonNew == 0) {
    LEDState = 0;
    delay(5000);
    goto on;
  }
}
buttonOld = buttonNew;
}
   
