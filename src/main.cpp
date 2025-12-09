#include <M5Unified.h>
#include <ESP32Servo.h>

// 3つのサーボ
Servo servoLeft;
Servo servoCenter;
Servo servoRight;

// サーボピン
const int SERVO_LEFT_PIN = 5;    // G5 - Left
const int SERVO_CENTER_PIN = 6;  // G6 - Center
const int SERVO_RIGHT_PIN = 7;   // G7 - Right

// 角度設定
const int ANGLE_STOP = 0;    // Stop
const int ANGLE_WEAK = 45;   // Weak
const int ANGLE_STRONG = 90; // Strong

// テスト設定
const int HOLD_TIME = 2000;  // 各角度で2秒保持

// 長押し判定時間（ミリ秒）
const int LONG_PRESS_TIME = 1000;  // 1秒以上で長押し

// 10パターンの動作時間（1000-3000ms）とインターバル（0-500ms）
const int patternHoldTimes[10] = {1800, 2600, 1200, 2200, 1500, 2800, 1000, 2000, 1400, 2500};  // ms
const int patternIntervals[10] = {300, 100, 450, 200, 0, 350, 500, 150, 250, 50};  // ms (0-500ms)

void testServoAngles(Servo &servo, const char* position, int pin) {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.printf("%s\n", position);
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.printf("Pin: G%d\n", pin);
  M5.Display.println("");
  
  Serial.printf("=== %s G%d Test ===\n", position, pin);
  
  // Stop
  servo.write(ANGLE_STOP);
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(3);
  M5.Display.printf("Stop");
  Serial.println("Angle: Stop (0 deg)");
  delay(HOLD_TIME);
  
  // Weak
  servo.write(ANGLE_WEAK);
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(3);
  M5.Display.printf("Weak");
  Serial.println("Angle: Weak (45 deg)");
  delay(HOLD_TIME);
  
  // Strong
  servo.write(ANGLE_STRONG);
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(3);
  M5.Display.printf("Strong");
  Serial.println("Angle: Strong (90 deg)");
  delay(HOLD_TIME);
  
  // Stop
  servo.write(ANGLE_STOP);
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
  
  // Left (G5)
  testServoAngles(servoLeft, "Left", SERVO_LEFT_PIN);
  delay(1000);
  
  // Center (G6)
  testServoAngles(servoCenter, "Center", SERVO_CENTER_PIN);
  delay(1000);
  
  // Right (G7)
  testServoAngles(servoRight, "Right", SERVO_RIGHT_PIN);
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

void executePattern(const char* position, int angle, int moveNum, int holdTime, int intervalTime) {
  // 全サーボをStopに戻す
  servoLeft.write(ANGLE_STOP);
  servoCenter.write(ANGLE_STOP);
  servoRight.write(ANGLE_STOP);
  delay(100);
  
  Servo* targetServo;
  int pin;
  
  if (strcmp(position, "Left") == 0) {
    targetServo = &servoLeft;
    pin = SERVO_LEFT_PIN;
  } else if (strcmp(position, "Center") == 0) {
    targetServo = &servoCenter;
    pin = SERVO_CENTER_PIN;
  } else {
    targetServo = &servoRight;
    pin = SERVO_RIGHT_PIN;
  }
  
  const char* strengthName;
  if (angle == ANGLE_WEAK) {
    strengthName = "Weak";
  } else if (angle == ANGLE_STRONG) {
    strengthName = "Strong";
  } else {
    strengthName = "Stop";
  }
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(1);
  M5.Display.printf("Move %d/10\n", moveNum);
  M5.Display.println("==============");
  M5.Display.println("");
  M5.Display.setTextSize(2);
  M5.Display.printf("%s\n", position);
  M5.Display.printf("%s\n", strengthName);
  M5.Display.setTextSize(1);
  M5.Display.printf("G%d: %ddeg\n", pin, angle);
  M5.Display.printf("Hold:%dms\n", holdTime);
  M5.Display.printf("Wait:%dms", intervalTime);
  
  Serial.printf("Move %d/10: %s G%d %s (%ddeg) Hold:%dms Wait:%dms\n", 
                moveNum, position, pin, strengthName, angle, holdTime, intervalTime);
  
  // 指定角度に移動して保持
  targetServo->write(angle);
  delay(holdTime);
  
  // Stopに戻す
  targetServo->write(ANGLE_STOP);
  
  // インターバル中は「None」を表示
  if (intervalTime > 0) {
    M5.Display.clear();
    M5.Display.setCursor(0, 0);
    M5.Display.setTextSize(1);
    M5.Display.printf("Move %d/10\n", moveNum);
    M5.Display.println("==============");
    M5.Display.println("");
    M5.Display.setTextSize(2);
    M5.Display.println("None");
    M5.Display.println("");
    M5.Display.setTextSize(1);
    M5.Display.printf("Wait:%dms", intervalTime);
    
    Serial.printf("  Interval: None (Wait:%dms)\n", intervalTime);
    delay(intervalTime);
  }
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
  executePattern("Right", ANGLE_WEAK, 1, patternHoldTimes[0], patternIntervals[0]);    // Right Weak 1800ms / 300ms
  executePattern("Left", ANGLE_STRONG, 2, patternHoldTimes[1], patternIntervals[1]);   // Left Strong 2600ms / 100ms
  executePattern("Left", ANGLE_WEAK, 3, patternHoldTimes[2], patternIntervals[2]);     // Left Weak 1200ms / 450ms
  executePattern("Center", ANGLE_STRONG, 4, patternHoldTimes[3], patternIntervals[3]); // Center Strong 2200ms / 200ms
  executePattern("Right", ANGLE_STRONG, 5, patternHoldTimes[4], patternIntervals[4]);  // Right Strong 1500ms / 0ms
  executePattern("Center", ANGLE_WEAK, 6, patternHoldTimes[5], patternIntervals[5]);   // Center Weak 2800ms / 350ms
  executePattern("Right", ANGLE_WEAK, 7, patternHoldTimes[6], patternIntervals[6]);    // Right Weak 1000ms / 500ms
  executePattern("Right", ANGLE_WEAK, 8, patternHoldTimes[7], patternIntervals[7]);    // Right Weak 2000ms / 150ms
  executePattern("Center", ANGLE_STRONG, 9, patternHoldTimes[8], patternIntervals[8]); // Center Strong 1400ms / 250ms
  executePattern("Left", ANGLE_STRONG, 10, patternHoldTimes[9], patternIntervals[9]);  // Left Strong 2500ms / 50ms
  
  // 全サーボをStopに戻す
  servoLeft.write(ANGLE_STOP);
  servoCenter.write(ANGLE_STOP);
  servoRight.write(ANGLE_STOP);
  
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
  Serial.printf("Init Left Servo: Pin=%d\n", SERVO_LEFT_PIN);
  servoLeft.attach(SERVO_LEFT_PIN);
  servoLeft.write(ANGLE_STOP);
  
  Serial.printf("Init Center Servo: Pin=%d\n", SERVO_CENTER_PIN);
  servoCenter.attach(SERVO_CENTER_PIN);
  servoCenter.write(ANGLE_STOP);
  
  Serial.printf("Init Right Servo: Pin=%d\n", SERVO_RIGHT_PIN);
  servoRight.attach(SERVO_RIGHT_PIN);
  servoRight.write(ANGLE_STOP);
  
  Serial.println("Servo Init Complete");
  
  M5.Display.clear();
  M5.Display.setTextSize(1);
  M5.Display.setCursor(0, 0);
  M5.Display.println("3 Servo Angle Test");
  M5.Display.println("==================");
  M5.Display.println("");
  M5.Display.setTextSize(2);
  M5.Display.println("None");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 10x pattern");
  
  Serial.println("\nShort press: Full test");
  Serial.println("Long press: 10x pattern\n");
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
    
    // テスト完了後、Noneを表示
    M5.Display.clear();
    M5.Display.setTextSize(1);
    M5.Display.setCursor(0, 0);
    M5.Display.println("3 Servo Angle Test");
    M5.Display.println("==================");
    M5.Display.println("");
    M5.Display.setTextSize(2);
    M5.Display.println("None");
    M5.Display.println("");
    M5.Display.setTextSize(1);
    M5.Display.println("Short: Full test");
    M5.Display.println("Long: 10x pattern");
  }
  
  delay(10);
}