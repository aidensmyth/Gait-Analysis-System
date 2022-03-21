#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <bluefruit.h>

#define SAMPLE_RATE_MS (500)
Adafruit_BNO055 bno = Adafruit_BNO055(55);

BLEDfu  bledfu;  // OTA DFU service
BLEDis  bledis;  // device information
BLEUart bleuart; // uart over ble
BLEBas  blebas;  // battery

const byte buttonPin = 7;
int buttonNew;
int buttonOld = 1;             
const byte LEDPin = 3;                
int LEDState = 0;                   

void OrientationData(void) {  
  on:  
  buttonNew = digitalRead(buttonPin);
  if(buttonOld == 0 && buttonNew == 1){
    while (LEDState == 0) {
      digitalWrite(LEDPin, HIGH);
       
      uint8_t system, gyro, accel, mag;
      system = gyro = accel = mag = 0;
      bno.getCalibration(&system, &gyro, &accel, &mag);
        
      bleuart.print("KR: ");
      bleuart.print("Sys:");
      bleuart.print(system, DEC);
      bleuart.print(",");
      bleuart.print("G:");
      bleuart.print(gyro, DEC);
      bleuart.print(",");
      bleuart.print(" A:");
      bleuart.print(accel, DEC);
      bleuart.print(",");
      bleuart.print(" M:");
      bleuart.print(mag, DEC);
      bleuart.print("\n");
          
      if(gyro == 3 && accel == 3 && mag == 3){
        while(gyro == 3 && accel == 3 && mag == 3) {
          sensors_event_t event;
          bno.getEvent(&event);
    
          bleuart.print("KR: ");
          bleuart.print("x:");
          bleuart.print(event.orientation.x);
          bleuart.print(",");
          bleuart.print("y:");
          bleuart.print(event.orientation.y);
          bleuart.print(",");
          bleuart.print("z:");
          bleuart.print(event.orientation.z);
          bleuart.print("\n");

          delay(SAMPLE_RATE_MS);

          buttonNew = digitalRead(buttonPin);

          if (buttonNew == 0) {
          delay(5000);
          LEDState = 1;
          goto off;
        }
      }
     }
     delay(SAMPLE_RATE_MS);
    }
   }   
  
  off:
  while( LEDState == 1) {
    digitalWrite(LEDPin, LOW);
    buttonNew = digitalRead(buttonPin);
    if (buttonNew == 0) {
      delay(5000);
      LEDState = 0;
      goto on;
    }
  }
  buttonOld = buttonNew;
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
  
  Bluefruit.autoConnLed(true);

  // Config the peripheral connection with maximum bandwidth 
  // more SRAM required by SoftDevice
  // Note: All config***() function must be called before begin()
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  Bluefruit.begin();
  Bluefruit.setName("Knee Sensor R");
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  //Bluefruit.setName(getMcuUniqueID()); // useful testing with multiple central connections
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  // To be consistent OTA DFU should be added first if it exists
  bledfu.begin();

  // Configure and Start Device Information Service
  bledis.setManufacturer("Gait Analysis System");
  bledis.setModel("Knee Sensor");
  bledis.begin();

  // Configure and Start BLE Uart Service
  bleuart.begin();

  // Start BLE Battery Service
  blebas.begin();
  blebas.write(100);

  // Set up and start advertising
  startAdv();

}

void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

void loop(){ 
    OrientationData();
}
// callback invoked when central connects
void connect_callback(uint16_t conn_handle)
{
  // Get the reference to current connection
  BLEConnection* connection = Bluefruit.Connection(conn_handle);

  char central_name[32] = { 0 };
  connection->getPeerName(central_name, sizeof(central_name));

  Serial.print("Connected to ");
  Serial.println(central_name);
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle connection where this event happens
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.println();
  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
}
