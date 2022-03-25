#include <bluefruit.h>
#include "NRF52TimerInterrupt.h"
#include "NRF52_ISR_Timer.h"
#define NRF52_TIMER_INTERRUPT_DEBUG      1
#define HW_TIMER_INTERVAL_MS      1

NRF52Timer ITimer(NRF_TIMER_1);

// Init NRF52_ISR_Timer
// Each NRF52_ISR_Timer can service 16 different ISR-based timers
NRF52_ISR_Timer ISR_Timer;

#define TIMER_INTERVAL_1S             1000L
#define TIMER_INTERVAL_50ms           50L
#define TIMER_INTERVAL_5ms           5L

BLEDis  bledis;  // device information
BLEUart bleuart; // uart over ble

int taking_data = false;
int data_ready = false;
unsigned short sensorValue = 0;

int i, factor;
int counter = 0;
uint8_t data[20];
uint8_t data_tbs[20] = {0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40};
char* dataPointer;
char* valuePointer;

const char* BLEDeviceName = "Gait Knee Sensor 1";
const uint8_t GAIT_SYSTEM_SERV_UUID[]  = { 0x10, 0xb4, 0xc6, 0xc6, 0xe0, 0x44, 0xaa, 0xb4, 0x67, 0x44, 0x3c, 0x1c, 0x05, 0xc6, 0x3b, 0x57 };
const uint8_t ORIENTATION_CHAR_UUID[]  = { 0xe2, 0x3b, 0xc6, 0x05, 0x1c, 0x3c, 0x44, 0x67, 0xb4, 0xaa, 0x44, 0xe0, 0xc6, 0xc6, 0xb4, 0x10 };// create remote services

BLEService gaitService = BLEService(GAIT_SYSTEM_SERV_UUID);

//Create remote sensor characteristics
BLECharacteristic OrientationCharacteristic = BLECharacteristic(0x6ce2);

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

void setup(void) {
  pinMode(buttonPin,INPUT);          
  pinMode(LEDPin,OUTPUT);           

  valuePointer = (char*)&sensorValue;
  dataPointer = (char*)&data;
  
  Serial.begin(115200);
  
  if (!bno.begin())
  {
    Serial.print("No BNO055");
    while (1);
  }
  delay(1000);

   // Orientation calibration
  bno.setExtCrystalUse(true);

   // Initialize the Bluetooth function of board
  setupBLE();
  // Set up the Service and Characteristics
  setupGaitServChar();
  // Advertising
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(gaitService); //Include all service uuids
  Bluefruit.Advertising.addName(); // Include Name.
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
  
  // Set up Timers
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler))
  {
    Serial.println("Starting  ITimer OK, millis() = " + String(millis()));
  }
  else {
    Serial.println("Can't set ITimer correctly. Select another freq. or interval");

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer

  Serial.println("Setup completed. Ready for connection.");
  }
}

// Main loop
//void loop(){ 
//  on:  
//  buttonNew = digitalRead(buttonPin);
//  if(buttonOld == 0 && buttonNew == 1){
//    while (LEDState == 0) {
//      digitalWrite(LEDPin, HIGH);
//      OrientationData();
//    
//      //displayCalStatus();
//      buttonNew = digitalRead(buttonPin);
//      Serial.println(buttonNew);
//      Serial.println(buttonOld);
//     if (buttonNew == 0) {
//      LEDState = 1;
//      delay(1000);
//      goto off;
//        }
//       }
//      }   
//  off:
//  while( LEDState == 1) {
//    digitalWrite(LEDPin, LOW);
//    buttonNew = digitalRead(buttonPin);
//    if (buttonNew == 0) {
//      LEDState = 0;
//      delay(1000);
//      goto on;
//    }
//  }
//  buttonOld = buttonNew;
//}
void loop() {
    if (data_ready) {
      OrientationCharacteristic.notify(data_tbs,20);
      data_ready = false;
      Serial.println("sent data package...");
    }
}

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

void TimerHandler(void)
{    
  ISR_Timer.run();
}

void setupBLE() {
  delay(500); // Delay a bit so the timestamp shows up.
  //Initialize Bluefruit module
  Serial.println("Initialize the Bluefruit nRF52 module");
  // Set up and initialize Bluetooth
  Bluefruit.begin();
  //Configure and start device information service
  bledis.setManufacturer("Stryde");
  bledis.setModel("Gait sensor alpha");
  bledis.begin();
  // Set max power to one of the following accepted values:
  //  -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(4);
  Bluefruit.setName(BLEDeviceName);

  // Set up the connect/disconnect callback handlers.
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);
}

void setupGaitServChar() {
  gaitService.begin(); //BLECharacteristics cannot be added until BLEService.begin()
  Serial.println("gait service has begun.");
  // Configure the characteristics.
  OrientationCharacteristic.setProperties(CHR_PROPS_READ | CHR_PROPS_NOTIFY);
  OrientationCharacteristic.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  OrientationCharacteristic.setFixedLen(20);
  OrientationCharacteristic.setCccdWriteCallback(OrientationService_cccd_callback); // Optionally capture CCCD updates
  OrientationCharacteristic.begin();
  Serial.println("OrientationCharacteristic has begun.");
}

void connect_callback(uint16_t conn_handle) {
  // Get the reference to the current connection.
  BLEConnection* connection = Bluefruit.Connection(conn_handle);

  char central_name[16] = {0};
  connection->getPeerName(central_name, sizeof(central_name));

  Serial.print("Connected to ");
  Serial.println(central_name);
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason) {
  //BLEConnection* connection = Bluefruit.Connection(conn_handle);
  
  (void) conn_handle;
  (void) reason;
  taking_data = false;
  ISR_Timer.disableAll();
  //  also reset the data taking parameters
  counter = 0;
  dataPointer = (char*)&data;
  Serial.print("Disconnected, reason = 0x");
  Serial.println(reason, HEX);
  Serial.println("Ready for connection");
}

void OrientationService_cccd_callback(uint16_t conn_hdl, BLECharacteristic* Orient1, uint16_t cccd_value) {
  if (Orient1->uuid == OrientationCharacteristic.uuid) {
    if (Orient1->notifyEnabled(conn_hdl)) {
      taking_data = true;
      ISR_Timer.setInterval(TIMER_INTERVAL_50ms,  Timer_callback);
      Serial.println("notify...");
    } else {
      Serial.println("stopped notify...");
      taking_data = false;
      ISR_Timer.disableAll();
      //  also reset the data taking parameters
      counter = 0;
      dataPointer = (char*)&data;
    }
  }
}
void Timer_callback() {
    if (taking_data) {
      sensors_event_t event;
      bno.getEvent(&event);
      sensorValue = event.orientation.x;
      Serial.print(sensorValue);
      // copy value into char buffer
      for(int i = 0; i < 2; i++) {
        dataPointer[i] = valuePointer[i];
      }
      dataPointer +=2;
      counter +=1;
      if (counter>9) {
        memcpy(data_tbs,data,sizeof(data));
        data_ready = true;
        counter = 0;
        dataPointer = (char*)&data;
      }
   }
}
