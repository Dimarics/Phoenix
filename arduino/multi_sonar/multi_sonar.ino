#include <Wire.h>

enum I2C_REQUEST_ID: uint8_t {
  I2C_REQUEST_FORWARD = 1,
  I2C_REQUEST_BACKWARD,
  I2C_REQUEST_LEFT,
  I2C_REQUEST_RIGHT,
  I2C_REQUEST_STOP
};

uint8_t request = 0;

void setup() {
  Serial.begin(9600);

  // I2C
  Wire.begin(0x08);
  Wire.onReceive([] {
    while (Wire.available()) {
      request = Wire.read();
    }
  });
  Wire.onRequest([] {
    switch (request) {
    case I2C_REQUEST_FORWARD:
      Serial.println("forward");
      break;
    case I2C_REQUEST_BACKWARD:
      Serial.println("backward");
      break;
    case I2C_REQUEST_LEFT:
      Serial.println("left");
      break;
    case I2C_REQUEST_RIGHT:
      Serial.println("right");
      break;
    case I2C_REQUEST_STOP:
      Serial.println("stop");
      break;
    }
    request = 0;
  });
}

void loop() {
  static uint32_t sonar_timer = millis();

  // Сонар
  if (millis() - sonar_timer >= 100) {
    sonar_timer = millis();
    // front
    digitalWrite(front_sonar_trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(front_sonar_trig_pin, LOW);
    front_sonar_distance = float(pulseIn(front_sonar_echo_pin, HIGH, 100000)) / 5.82;
    if (front_sonar_distance == 0) front_sonar_distance = 65535;

    // back
    digitalWrite(back_sonar_trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(back_sonar_trig_pin, LOW);
    back_sonar_distance = float(pulseIn(back_sonar_echo_pin, HIGH, 100000)) / 5.82;
    if (back_sonar_distance == 0) back_sonar_distance = 65535;

    // left
    digitalWrite(left_sonar_trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(left_sonar_trig_pin, LOW);
    left_sonar_distance = float(pulseIn(left_sonar_echo_pin, HIGH, 100000)) / 5.82;
    if (left_sonar_distance == 0) left_sonar_distance = 65535;

    // right
    digitalWrite(right_sonar_trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(right_sonar_trig_pin, LOW);
    right_sonar_distance = float(pulseIn(right_sonar_echo_pin, HIGH, 100000)) / 5.82;
    if (right_sonar_distance == 0) right_sonar_distance = 65535;

    Serial.print("front sonar distance: ");
    Serial.println(front_sonar_distance);
    Serial.print("back sonar distance: ");
    Serial.println(back_sonar_distance);
    Serial.print("left sonar distance: ");
    Serial.println(left_sonar_distance);
    Serial.print("right sonar distance: ");
    Serial.println(right_sonar_distance);
    
    /*
    float t = float(sonar_distance) / 1500.f;
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
    */
  }
}
