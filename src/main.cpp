#include <M5Unified.h>
#include <ESP32Servo.h>

Servo servo;

// サーボピン（AtomS3のGrove互換ピン）
const int SERVO_PIN = 5;  // G5 (Grove Yellow)

// 角度設定
const int ANGLE_0 = 0;
const int ANGLE_45 = 45;
const int ANGLE_90 = 90;

int angleMode = 0; // 0=0度, 1=45度, 2=90度
int currentAngle = ANGLE_0;

void updateDisplay() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Servo Angle");
  M5.Display.println("");
  
  M5.Display.setTextSize(3);
  M5.Display.printf("%d deg\n", currentAngle);
  
  M5.Display.setTextSize(1);
  M5.Display.println("");
  M5.Display.println("Press button");
  M5.Display.println("to change angle");
  M5.Display.println("");
  M5.Display.println("0 -> 45 -> 90");
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  // サーボ初期化
  servo.attach(SERVO_PIN);
  servo.write(ANGLE_0);
  
  M5.Display.clear();
  M5.Display.setTextSize(1);
  M5.Display.setCursor(0, 0);
  M5.Display.println("Servo Control");
  M5.Display.println("=============");
  M5.Display.println("");
  M5.Display.println("Press button");
  M5.Display.println("to change angle");
  M5.Display.println("");
  M5.Display.println("0 -> 45 -> 90");
  M5.Display.println("-> 0 -> ...");
  M5.Display.println("");
  M5.Display.println("Current: 0 deg");
  
  delay(2000);
  updateDisplay();
}

void loop() {
  M5.update();
  
  // ボタン押下：角度切り替え
  if (M5.BtnA.wasPressed()) {
    angleMode++;
    if (angleMode > 2) {
      angleMode = 0;
    }
    
    switch(angleMode) {
      case 0:
        currentAngle = ANGLE_0;
        break;
      case 1:
        currentAngle = ANGLE_45;
        break;
      case 2:
        currentAngle = ANGLE_90;
        break;
    }
    
    servo.write(currentAngle);
    updateDisplay();
  }
  
  delay(10);
}