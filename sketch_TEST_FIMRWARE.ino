#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <HX711.h>
#include <MAX6675.h>
#include <Servo.h>
#include <MPU6050.h>
#include <HardwareSerial.h>


#define I2C_SDA_PIN    8
#define I2C_SCL_PIN    9
#define HX711_DT       10
#define HX711_SCK      11
#define LED_STATUS_PIN 21
#define SERVO_PIN      13
#define PYRO1_PIN      14
#define PYRO2_PIN      15
#define PYRO3_PIN      16
#define SD_CS          26
#define SD_CD          33
#define SPI_MOSI       35
#define SPI_MISO       37
#define SPI_SCK        36
#define TEMP_CS1       42
#define TEMP_CS2       40
#define TEMP_CS3       38
#define HC12_TX        43
#define HC12_RX        44
#define ARM_SWITCH_PIN 45
#define ACK_BUTTON_PIN 46

HX711 loadCell;
MPU6050 mpu;
Servo armingServo;
MAX6675 tc1(SPI, TEMP_CS1), tc2(SPI, TEMP_CS2), tc3(SPI, TEMP_CS3);
HardwareSerial HC12(1);

enum State { SAFE, ARMING, ARMED, IGNITED, FAIL };
State currentState = SAFE;

bool sdCardReady = false;
unsigned long armStart = 0;
bool sensorsOK = true;

void blinkLED(int times, int d) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_STATUS_PIN, HIGH);
    delay(d);
    digitalWrite(LED_STATUS_PIN, LOW);
    delay(d);
  }
}

void setState(State s) {
  currentState = s;
  switch (s) {
    case SAFE:
      armingServo.write(0);
      digitalWrite(PYRO1_PIN, LOW);
      digitalWrite(PYRO2_PIN, LOW);
      digitalWrite(PYRO3_PIN, LOW);
      digitalWrite(LED_STATUS_PIN, LOW);
      break;

    case ARMING:
      blinkLED(3, 200);
      break;

    case ARMED:
      armingServo.write(90);
      digitalWrite(LED_STATUS_PIN, HIGH);
      break;

    case IGNITED:
      digitalWrite(PYRO1_PIN, HIGH);
      digitalWrite(PYRO2_PIN, HIGH);
      digitalWrite(PYRO3_PIN, HIGH);
      armingServo.write(90);
      digitalWrite(LED_STATUS_PIN, HIGH);
      break;

    case FAIL:
      armingServo.write(0);
      digitalWrite(PYRO1_PIN, LOW);
      digitalWrite(PYRO2_PIN, LOW);
      digitalWrite(PYRO3_PIN, LOW);
      while (true) {   // lockout
        blinkLED(2, 100);
      }
  }
}

bool selfTest() {
  bool ok = true;

  if (!mpu.testConnection()) {
    Serial.println("FAIL: MPU6050");
    ok = false;
  }
  if (loadCell.is_ready() == false) {
    Serial.println("FAIL: HX711");
    ok = false;
  }
  if (digitalRead(SD_CD) == LOW) {
    if (!SD.begin(SD_CS)) {
      Serial.println("FAIL: SD Card");
      ok = false;
    } else sdCardReady = true;
  } else {
    Serial.println("FAIL: No SD Card");
    ok = false;
  }

  float t1 = tc1.readCelsius();
  if (isnan(t1)) { Serial.println("FAIL: TC1"); ok = false; }
  float t2 = tc2.readCelsius();
  if (isnan(t2)) { Serial.println("FAIL: TC2"); ok = false; }
  float t3 = tc3.readCelsius();
  if (isnan(t3)) { Serial.println("FAIL: TC3"); ok = false; }

  return ok;
}

void setup() {
  Serial.begin(115200);
  HC12.begin(9600, SERIAL_8N1, HC12_RX, HC12_TX);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  mpu.initialize();
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SD_CS);

  armingServo.attach(SERVO_PIN);
  pinMode(PYRO1_PIN, OUTPUT);
  pinMode(PYRO2_PIN, OUTPUT);
  pinMode(PYRO3_PIN, OUTPUT);
  pinMode(ARM_SWITCH_PIN, INPUT_PULLUP);
  pinMode(ACK_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_STATUS_PIN, OUTPUT);

  sensorsOK = selfTest();
  if (!sensorsOK) setState(FAIL);
}

void loop() {
  if (!sensorsOK) return;

  bool armSwitch  = (digitalRead(ARM_SWITCH_PIN) == LOW);
  bool fireButton = (digitalRead(ACK_BUTTON_PIN) == LOW);

  if (currentState == SAFE && armSwitch) {
    armStart = millis();
    setState(ARMING);
  }
  else if (currentState == ARMING) {
    if (!armSwitch) setState(SAFE);
    else if (millis() - armStart > 3000) setState(ARMED);
  }
  else if (currentState == ARMED && fireButton) {
    setState(IGNITED);
  }

  float thrust = loadCell.get_units();
  int16_t ax, ay, az; mpu.getAcceleration(&ax, &ay, &az);
  float t1 = tc1.readCelsius(), t2 = tc2.readCelsius(), t3 = tc3.readCelsius();

  if (isnan(thrust) || isnan(t1) || isnan(t2) || isnan(t3)) {
    setState(FAIL);
  }

  if (sdCardReady) {
    File f = SD.open("/log.csv", FILE_APPEND);
    if (f) {
      f.printf("%lu,%d,%d,%d,%d,%.2f,%.2f,%.2f,%.2f\n",
               millis(), currentState, ax, ay, az, thrust, t1, t2, t3);
      f.close();
    } else setState(FAIL);
  }

  HC12.printf("STATE:%d THRUST:%.2f AX:%d AY:%d AZ:%d T:%.1f,%.1f,%.1f\n",
              currentState, thrust, ax, ay, az, t1, t2, t3);

  delay(100);
}
