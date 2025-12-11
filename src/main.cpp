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

// 10パターンの動作時間（2500-3500ms）とインターバル（0-500ms）
const int patternHoldTimes[10] = {2800, 3300, 2600, 3100, 2500, 3400, 2900, 3500, 2700, 3200};  // ms (2.5~3.5秒)
const int patternIntervals[10] = {300, 100, 450, 200, 0, 350, 500, 150, 250, 50};  // ms (0-500ms)

// ========================================
// シリアル通信用ヘルパー関数
// PC側のPython GUIが期待する厳密なフォーマットで出力
// ========================================
void sendStimMessage(const char* position, const char* strength) {
  Serial.printf("%s,%s\n", position, strength);
}

void testServoAngles(Servo &servo, const char* position, int pin) {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.printf("%s\n", position);
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.printf("Pin: G%d\n", pin);
  M5.Display.println("");
  
  // Stop
  servo.write(ANGLE_STOP);
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(3);
  M5.Display.printf("Stop");
  delay(HOLD_TIME);
  
  // ========================================
  // Weak刺激開始 - PC側へメッセージ送信
  // ========================================
  sendStimMessage(position, "Weak");
  servo.write(ANGLE_WEAK);
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(3);
  M5.Display.printf("Weak");
  delay(HOLD_TIME);
  
  // ========================================
  // Weak終了 - インターバル開始
  // ========================================
  servo.write(ANGLE_STOP);
  sendStimMessage("none", "none");
  delay(500);
  
  // ========================================
  // Strong刺激開始 - PC側へメッセージ送信
  // ========================================
  sendStimMessage(position, "Strong");
  servo.write(ANGLE_STRONG);
  M5.Display.fillRect(0, 80, 128, 48, BLACK);
  M5.Display.setCursor(0, 80);
  M5.Display.setTextSize(3);
  M5.Display.printf("Strong");
  delay(HOLD_TIME);
  
  // ========================================
  // Strong終了 - Stop
  // ========================================
  servo.write(ANGLE_STOP);
  sendStimMessage("none", "none");
  delay(500);
}

void runAllTests() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting");
  M5.Display.println("Full Test");
  delay(1000);
  
  // Left (G5)
  testServoAngles(servoLeft, "Left", SERVO_LEFT_PIN);
  delay(1000);
  
  // Center (G6)
  testServoAngles(servoCenter, "Center", SERVO_CENTER_PIN);
  delay(1000);
  
  // Right (G7)
  testServoAngles(servoRight, "Right", SERVO_RIGHT_PIN);
  delay(500);
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Full Test");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 20x pattern");
  
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
  
  // 画面表示
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(1);
  M5.Display.printf("Move %d/20\n", moveNum);
  M5.Display.println("==============");
  M5.Display.println("");
  M5.Display.setTextSize(2);
  M5.Display.printf("%s\n", position);
  M5.Display.printf("%s\n", strengthName);
  M5.Display.setTextSize(1);
  M5.Display.printf("G%d: %ddeg\n", pin, angle);
  M5.Display.printf("Hold:%dms\n", holdTime);
  M5.Display.printf("Wait:%dms", intervalTime);
  
  // ========================================
  // 刺激開始 - PC側へメッセージ送信
  // ========================================
  sendStimMessage(position, strengthName);
  
  // 指定角度に移動して保持
  targetServo->write(angle);
  delay(holdTime);
  
  // Stopに戻す
  targetServo->write(ANGLE_STOP);
  
  // ========================================
  // インターバル開始 - "none,none" を送信
  // ========================================
  if (intervalTime > 0) {
    sendStimMessage("none", "none");
    
    M5.Display.clear();
    M5.Display.setCursor(0, 0);
    M5.Display.setTextSize(1);
    M5.Display.printf("Move %d/20\n", moveNum);
    M5.Display.println("==============");
    M5.Display.println("");
    M5.Display.setTextSize(2);
    M5.Display.println("None");
    M5.Display.println("");
    M5.Display.setTextSize(1);
    M5.Display.printf("Wait:%dms", intervalTime);
    
    delay(intervalTime);
  } else {
    // インターバルが0msの場合も "none,none" を送信
    sendStimMessage("none", "none");
  }
}

void run20Pattern() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting");
  M5.Display.println("20x Pattern");
  delay(1000);
  
  // 1~10回目
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
  
  // 11~20回目（配列の順番をめちゃくちゃに）
  executePattern("Center", ANGLE_WEAK, 11, patternHoldTimes[7], patternIntervals[3]);   // 2000ms / 200ms
  executePattern("Right", ANGLE_STRONG, 12, patternHoldTimes[2], patternIntervals[8]);  // 1200ms / 250ms
  executePattern("Left", ANGLE_STRONG, 13, patternHoldTimes[5], patternIntervals[1]);   // 2800ms / 100ms
  executePattern("Left", ANGLE_WEAK, 14, patternHoldTimes[9], patternIntervals[6]);     // 2500ms / 500ms
  executePattern("Center", ANGLE_STRONG, 15, patternHoldTimes[1], patternIntervals[4]); // 2600ms / 0ms
  executePattern("Right", ANGLE_WEAK, 16, patternHoldTimes[4], patternIntervals[9]);    // 1500ms / 50ms
  executePattern("Left", ANGLE_WEAK, 17, patternHoldTimes[8], patternIntervals[5]);     // 1400ms / 350ms
  executePattern("Center", ANGLE_WEAK, 18, patternHoldTimes[3], patternIntervals[0]);   // 2200ms / 300ms
  executePattern("Right", ANGLE_STRONG, 19, patternHoldTimes[6], patternIntervals[7]);  // 1000ms / 150ms
  executePattern("Center", ANGLE_STRONG, 20, patternHoldTimes[0], patternIntervals[2]); // 1800ms / 450ms
  
  // 全サーボをStopに戻す
  servoLeft.write(ANGLE_STOP);
  servoCenter.write(ANGLE_STOP);
  servoRight.write(ANGLE_STOP);
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("20x Pattern");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 20x pattern");
  
  delay(2000);
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  // シリアル通信初期化（115200bps）
  Serial.begin(115200);
  
  // サーボ初期化（デバッグ出力なし）
  servoLeft.attach(SERVO_LEFT_PIN);
  servoLeft.write(ANGLE_STOP);
  
  servoCenter.attach(SERVO_CENTER_PIN);
  servoCenter.write(ANGLE_STOP);
  
  servoRight.attach(SERVO_RIGHT_PIN);
  servoRight.write(ANGLE_STOP);
  
  // 初期画面表示
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
  M5.Display.println("Long: 20x pattern");
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
      // 長押し：20パターン実行
      run20Pattern();
    } else {
      // 短押し：フルテスト実行
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
    M5.Display.println("Long: 20x pattern");
  }
  
  delay(10);
}