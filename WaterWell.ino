#include "arduino_secrets.h"
#include "thingProperties.h"
#include <SoftwareSerial.h>

// Defined pins used by A02YYUW ultrasonic sensor
#define rxPin D1 //green 
#define txPin D2 //blue

SoftwareSerial Sensor (rxPin, txPin);

// Defined values for A02YYUW ultrasonic Sensor
unsigned char sensorData[4] = {};
byte dataIndex = 0;
bool headerReceived = false;
bool newData = false;
float calculatedDistance;
long calculatedVolume;

void setup() {
  Serial.begin(57600);
  delay(1500);

  Sensor.begin(9600);
  delay(1500);

  pinMode(LED_BUILTIN, OUTPUT);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  A02YYUW_Read();
  calculateDistance();
  ArduinoCloud.update();
}

void onLedChange()  {
  if (led == 0)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void A02YYUW_Read() {

  if (Sensor.available() > 0 && newData == false)
  {
    if (headerReceived == false)
    {
      if (Sensor.read() == 0xFF)
      {
        headerReceived = true;
        sensorData[0] = 0xFF;
        dataIndex++;
      }
    }

    else //if (headerReceived == true)
    {
      sensorData[dataIndex] = Sensor.read();
      dataIndex++;
      if (dataIndex == 4) //three bytes after header
      {
        dataIndex = 0;//reset for next reading
        newData = true;
      }
    }
  }
}

void calculateDistance() {
  if (newData)
  {
    int sum;
    sum = (sensorData[0] + sensorData[1] + sensorData[2]) & 0x00FF;
    if (sum == sensorData[3])
    {
      calculatedDistance = ((sensorData[1] << 8) + sensorData[2]) / 10;
      if (calculatedDistance > 30)
      {
        distance = calculatedDistance; //CloudLength distance
        calculateVolume(calculatedDistance, 400, 40); // depth=400;radius=40;
      }
      else
      {
        Serial.println("Below the lower limit");
      }
    }
    else Serial.println("ERROR");
    //enable new reading
    newData = false;
    headerReceived = false;
  }
}

void calculateVolume(int distance, int depth, int radius) {
  if (distance > 3) {
    waterLevel = depth - distance; //cm
    calculatedVolume = (3.14 * sq(radius) * waterLevel) / 1000; //l
    waterVolume = calculatedVolume;
  }
}