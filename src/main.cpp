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

// 長押し判定時間（ミリ秒）
const int LONG_PRESS_TIME = 1000;  // 1秒以上で長押し

// 10パターンの動作時間（1000-3000ms）とインターバル（0-500ms）
const int patternHoldTimes[10] = {1800, 2600, 1200, 2200, 1500, 2800, 1000, 2000, 1400, 2500};  // ms
const int patternIntervals[10] = {300, 100, 450, 200, 0, 350, 500, 150, 250, 50};  // ms (0-500ms)

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

void executePattern(int servoNum, int angle, int moveNum, int holdTime, int intervalTime) {
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
  M5.Display.printf("G%d\n", pin);
  M5.Display.printf("Hold:%dms\n", holdTime);
  M5.Display.printf("Wait:%dms", intervalTime);
  
  Serial.printf("Move %d/10: Servo%d G%d %s Hold:%dms Wait:%dms\n", 
                moveNum, servoNum, pin, angleName, holdTime, intervalTime);
  
  // 指定角度に移動して保持
  targetServo->write(angle);
  delay(holdTime);
  
  // 0°に戻す
  targetServo->write(ANGLE_0);
  
  delay(intervalTime);
}

void run10Pattern() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting");
  M5.Display.println("10x Pattern");
  delay(1000);
  
  Serial.println("\n=== 10 Pattern Fixed Sequence ===");
  
  // 固定の10パターン（順番と角度は固定、時間だけバラバラ）
  executePattern(3, ANGLE_45, 1, patternHoldTimes[0], patternIntervals[0]);   // Servo3 45° 1800ms / 300ms
  executePattern(1, ANGLE_90, 2, patternHoldTimes[1], patternIntervals[1]);   // Servo1 90° 2600ms / 100ms
  executePattern(1, ANGLE_45, 3, patternHoldTimes[2], patternIntervals[2]);   // Servo1 45° 1200ms / 450ms
  executePattern(2, ANGLE_90, 4, patternHoldTimes[3], patternIntervals[3]);   // Servo2 90° 2200ms / 200ms
  executePattern(3, ANGLE_90, 5, patternHoldTimes[4], patternIntervals[4]);   // Servo3 90° 1500ms / 0ms
  executePattern(2, ANGLE_45, 6, patternHoldTimes[5], patternIntervals[5]);   // Servo2 45° 2800ms / 350ms
  executePattern(3, ANGLE_45, 7, patternHoldTimes[6], patternIntervals[6]);   // Servo3 45° 1000ms / 500ms
  executePattern(3, ANGLE_45, 8, patternHoldTimes[7], patternIntervals[7]);   // Servo3 45° 2000ms / 150ms
  executePattern(2, ANGLE_90, 9, patternHoldTimes[8], patternIntervals[8]);   // Servo2 90° 1400ms / 250ms
  executePattern(1, ANGLE_90, 10, patternHoldTimes[9], patternIntervals[9]);  // Servo1 90° 2500ms / 50ms
  
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
      // 長押し：10パターン実行
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