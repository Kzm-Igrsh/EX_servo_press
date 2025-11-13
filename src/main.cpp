#include <M5Unified.h>
#include <ESP32Servo.h>

// 3つのサーボ
Servo servo1;
Servo servo2;
Servo servo3;

// サーボピン
const int SERVO1_PIN = 5;  // G5
const int SERVO2_PIN = 6;  // G6
const int SERVO3_PIN = 7;  // G7

// 角度設定
const int ANGLE_0 = 0;
const int ANGLE_45 = 45;
const int ANGLE_90 = 90;

// テスト設定
const int HOLD_TIME = 2000;  // 各角度で2秒保持

void testServoAngles(Servo &servo, int servoNum, int pin) {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.printf("Servo %d\n", servoNum);
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.printf("Pin: G%d\n", pin);
  M5.Display.println("");
  
  Serial.printf("=== Servo%d G%d Test ===\n", servoNum, pin);
  
  // 0°
  servo.write(ANGLE_0);
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(3);
  M5.Display.printf("0 deg");
  Serial.println("Angle: 0 deg");
  delay(HOLD_TIME);
  
  // 45°
  servo.write(ANGLE_45);
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(3);
  M5.Display.printf("45 deg");
  Serial.println("Angle: 45 deg");
  delay(HOLD_TIME);
  
  // 90°
  servo.write(ANGLE_90);
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(3);
  M5.Display.printf("90 deg");
  Serial.println("Angle: 90 deg");
  delay(HOLD_TIME);
  
  // 0°に戻す
  servo.write(ANGLE_0);
  delay(500);
}

void runAllTests() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting");
  M5.Display.println("Test...");
  delay(1000);
  
  Serial.println("=== Starting 3 Servo Angle Test ===");
  
  // Servo 1 (G5)
  testServoAngles(servo1, 1, SERVO1_PIN);
  delay(1000);
  
  // Servo 2 (G6)
  testServoAngles(servo2, 2, SERVO2_PIN);
  delay(1000);
  
  // Servo 3 (G7)
  testServoAngles(servo3, 3, SERVO3_PIN);
  delay(500);
  
  Serial.println("=== All Tests Complete ===");
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("All Test");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("");
  M5.Display.println("Press button");
  M5.Display.println("to test again");
  
  delay(2000);
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(1);
  M5.Display.println("3 Servo Angle Test");
  M5.Display.println("==================");
  M5.Display.println("");
  M5.Display.println("Pins: G5,G6,G7");
  M5.Display.println("");
  M5.Display.println("Each servo:");
  M5.Display.println(" 0 -> 45 -> 90");
  M5.Display.println(" 2sec each angle");
  M5.Display.println("");
  M5.Display.println("Press to start");
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  Serial.begin(115200);
  Serial.println("3 Servo Angle Auto Test");
  
  // サーボ初期化
  Serial.printf("Init Servo1: Pin=%d\n", SERVO1_PIN);
  servo1.attach(SERVO1_PIN);
  servo1.write(ANGLE_0);
  
  Serial.printf("Init Servo2: Pin=%d\n", SERVO2_PIN);
  servo2.attach(SERVO2_PIN);
  servo2.write(ANGLE_0);
  
  Serial.printf("Init Servo3: Pin=%d\n", SERVO3_PIN);
  servo3.attach(SERVO3_PIN);
  servo3.write(ANGLE_0);
  
  Serial.println("Servo Init Complete");
  
  M5.Display.clear();
  M5.Display.setTextSize(1);
  M5.Display.setCursor(0, 0);
  M5.Display.println("3 Servo Angle Test");
  M5.Display.println("==================");
  M5.Display.println("");
  M5.Display.println("Pins: G5,G6,G7");
  M5.Display.println("");
  M5.Display.println("Each servo:");
  M5.Display.println(" 0 -> 45 -> 90");
  M5.Display.println(" 2sec each angle");
  M5.Display.println("");
  M5.Display.println("Press to start");
}

void loop() {
  M5.update();
  
  // ボタン押下：テスト開始
  if (M5.BtnA.wasPressed()) {
    Serial.println("Button pressed - Starting test");
    runAllTests();
  }
  
  delay(10);
}