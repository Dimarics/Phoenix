#include <Wire.h>
//#include <VL53L0X.h>

#define TRIG_PIN 2
#define ECHO_PIN 3
#define RED_PIN 9
#define GREEN_PIN 6
#define BLUE_PIN 5

enum I2C_REQUEST_ID: uint8_t {
  I2C_REQUEST_GET_DISTANCE = 1
};

uint8_t request = 0;
uint16_t sonar_distance = 0;
uint16_t dalnomer_distance = 0;

//VL53L0X dalnomer;

void setup() {
  Serial.begin(9600);

  // Сонар
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // I2C
  Wire.begin(0x08);
  Wire.onReceive([] {
    while (Wire.available()) {
      request = Wire.read();
    }
  });
  Wire.onRequest([] {
    if (request == I2C_REQUEST_GET_DISTANCE) {
      Wire.write(sonar_distance >> 8);
      Wire.write(sonar_distance);
    }
    request = 0;
  });

  // Дальномер
  // dalnomer.setTimeout(1000);
  // if (!dalnomer.init())
  // {
  //   Serial.println("Failed to detect and initialize sensor!");
  // }

  // Светодиод
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);
}

void loop() {
  static uint32_t sonar_timer = millis();
  static uint32_t dalnomer_timer = millis();
  
  // Сонар
  if (millis() - sonar_timer >= 50) {
    sonar_timer = millis();
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    //Serial.print("sonar distance: ");
    sonar_distance = float(pulseIn(ECHO_PIN, HIGH, 100000)) / 5.82;
    if (sonar_distance == 0) sonar_distance = 65535;
    
    //analogWrite(RED_PIN, map(sonar_distance, 0, 500, 0, 255));
    //analogWrite(GREEN_PIN, map(sonar_distance, 500, 1000, 0, 255));
    //analogWrite(BLUE_PIN, map(sonar_distance, 1000, 1500, 0, 255));
    float t = float(sonar_distance) / 1500.f;
    //Serial.println(sonar_distance);
    int red = 0;
    int green = 0;
    int blue = 0;
    if (t > 0 && t <= 0.5) {
      red = 255.f * (1.f - t * 2.f);
      green = 0;
      blue = 255.f * (t * 2.f);
    } else if (t > 0.5) {
      red = 0;
      green = 255.f * ((t - 0.5f) * 2.f);
      blue = 255.f * (1.f - (t - 0.5f) * 2.f);
    }
    if (red < 0) {
      red = 0;
    } else if (red > 255) {
      red = 255;
    }
    if (green < 0) {
      green = 0;
    } else if (green > 255) {
      green = 255;
    }
    if (blue < 0) {
      blue = 0;
    } else if (blue > 255) {
      blue = 255;
    }
    analogWrite(RED_PIN, red);
    analogWrite(GREEN_PIN, green);
    analogWrite(BLUE_PIN, blue);
  }
  
  // Дальномер
  // if (millis() - dalnomer_timer >= 50) {
  //   dalnomer_timer = millis();
  //   dalnomer_distance = dalnomer.readRangeSingleMillimeters();
    
  //   if (dalnomer.timeoutOccurred()) {
  //     Serial.println(" TIMEOUT");
  //     dalnomer_distance = 0;
  //   }
  //   Serial.print("dalnomer distance: ");
  //   Serial.println(dalnomer_distance);
  // }
}
