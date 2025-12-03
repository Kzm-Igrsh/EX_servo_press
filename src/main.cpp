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
const int PATTERN_INTERVAL = 3000;  // 10パターンのインターバル3秒

// 長押し判定時間（ミリ秒）
const int LONG_PRESS_TIME = 1000;  // 1秒以上で長押し

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
  M5.Display.println("Full Test");
  delay(1000);
  
  Serial.println("=== Starting Full Servo Test ===");
  
  // Servo 1 (G5)
  testServoAngles(servo1, 1, SERVO1_PIN);
  delay(1000);
  
  // Servo 2 (G6)
  testServoAngles(servo2, 2, SERVO2_PIN);
  delay(1000);
  
  // Servo 3 (G7)
  testServoAngles(servo3, 3, SERVO3_PIN);
  delay(500);
  
  Serial.println("=== Full Test Complete ===");
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Full Test");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 10x pattern");
  
  delay(2000);
}

void executePattern(int servoNum, int angle, int moveNum) {
  // 全サーボを0°に戻す
  servo1.write(ANGLE_0);
  servo2.write(ANGLE_0);
  servo3.write(ANGLE_0);
  delay(100);
  
  Servo* targetServo;
  int pin;
  
  switch(servoNum) {
    case 1:
      targetServo = &servo1;
      pin = SERVO1_PIN;
      break;
    case 2:
      targetServo = &servo2;
      pin = SERVO2_PIN;
      break;
    case 3:
      targetServo = &servo3;
      pin = SERVO3_PIN;
      break;
  }
  
  const char* angleName;
  if (angle == ANGLE_45) {
    angleName = "45deg";
  } else if (angle == ANGLE_90) {
    angleName = "90deg";
  } else {
    angleName = "0deg";
  }
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(1);
  M5.Display.printf("Move %d/10\n", moveNum);
  M5.Display.println("==============");
  M5.Display.println("");
  M5.Display.setTextSize(2);
  M5.Display.printf("Servo %d\n", servoNum);
  M5.Display.println(angleName);
  M5.Display.setTextSize(1);
  M5.Display.printf("G%d", pin);
  
  Serial.printf("Move %d/10: Servo%d G%d %s\n", moveNum, servoNum, pin, angleName);
  
  // 指定角度に移動して保持
  targetServo->write(angle);
  delay(HOLD_TIME);
  
  // 0°に戻す
  targetServo->write(ANGLE_0);
  
  delay(PATTERN_INTERVAL);  // 3秒インターバル
}

void run10Pattern() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting");
  M5.Display.println("10x Pattern");
  delay(1000);
  
  Serial.println("\n=== 10 Pattern Fixed Sequence ===");
  
  // よりバラバラで同じ箇所連続も含むパターン（全箇所×全角度を網羅）
  executePattern(3, ANGLE_45, 1);   // Servo3 45°
  executePattern(1, ANGLE_90, 2);   // Servo1 90°
  executePattern(1, ANGLE_45, 3);   // Servo1 45°（連続＆角度変更）
  executePattern(2, ANGLE_90, 4);   // Servo2 90°
  executePattern(3, ANGLE_90, 5);   // Servo3 90°（角度変更）
  executePattern(2, ANGLE_45, 6);   // Servo2 45°（角度変更）
  executePattern(3, ANGLE_45, 7);   // Servo3 45°
  executePattern(3, ANGLE_45, 8);   // Servo3 45°
  executePattern(2, ANGLE_90, 9);   // Servo2 90°
  executePattern(1, ANGLE_90, 10);  // Servo1 90°
  
  // 全サーボを0°に戻す
  servo1.write(ANGLE_0);
  servo2.write(ANGLE_0);
  servo3.write(ANGLE_0);
  
  Serial.println("=== 10 Pattern Complete ===\n");
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("10x Pattern");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 10x pattern");
  
  delay(2000);
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
  M5.Display.println("Short press:");
  M5.Display.println(" Full test");
  M5.Display.println("");
  M5.Display.println("Long press:");
  M5.Display.println(" 10x pattern");
  M5.Display.println("");
  M5.Display.println("Press to start");
}

void loop() {
  M5.update();
  
  // ボタンが押された瞬間の時刻を記録
  if (M5.BtnA.wasPressed()) {
    unsigned long pressStartTime = millis();
    
    // ボタンが離されるまで待つ
    while (M5.BtnA.isPressed()) {
      M5.update();
      delay(10);
    }
    
    // 押していた時間を計算
    unsigned long pressDuration = millis() - pressStartTime;
    
    if (pressDuration >= LONG_PRESS_TIME) {
      // 長押し：10パターン実行（不規則順）
      Serial.printf("Long press detected (%lums)\n", pressDuration);
      run10Pattern();
    } else {
      // 短押し：フルテスト実行
      Serial.printf("Short press detected (%lums)\n", pressDuration);
      runAllTests();
    }
  }
  
  delay(10);
}