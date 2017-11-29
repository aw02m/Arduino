#include <Wire.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <LSM303.h>

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
LSM303 compass;

#define SPEED 170 // Zumoのモータに与える回転力の基準値 

float red_G, green_G, blue_G; // カラーセンサーで読み取ったRGB値（0-255）
int zoneNumber_G; // ゾーン番号を表す状態変数
int mode_G; // 各ゾーンでのモードを表す状態変数
unsigned long timeInit_G, timeNow_G; //  スタート時間，経過時間
int motorR_G, motorL_G;  // 左右のZumoのモータに与える回転力
float direction_G;

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  button.waitForButton();
  setupColorSensor(); // カラーセンサーのsetup
  calibrationColorSensorWhite(); // カラーセンサーのキャリブレーション
  button.waitForButton();
  calibrationColorSensorBlack(); // カラーセンサーのキャリブレーション
  // CalibrationColorSensorManual(); // カラーセンサーのキャリブレーション（手動設定）

  setupCompass(); // 地磁気センサーのsetup
  calibrationCompass(); // 地磁気センサーのキャリブレーション
  //calibrationCompassManual(); // 地磁気センサーのキャリブレーション（手動設定）

  zoneNumber_G = 0;
  mode_G = 2; // スタート時はzoneToZoneの途中から
  button.waitForButton();
  timeInit_G = millis();
}

void loop()
{
  readRGB(); // カラーセンサでRGB値を取得(0-255)
  timeNow_G = millis() - timeInit_G; // 経過時間
  motors.setSpeeds(motorL_G, motorR_G); // 左右モーターへの回転力入力
  direction_G = averageHeadingLP();
  sendData(); // データ送信

  if ( button.isPressed() ) { // Zumo button が押されていればtrue，そうでなければ false
    zoneNumber_G = 3; // zoneToZone に移行
    mode_G = 1; 
    delay(100);
  }

  switch ( zoneNumber_G ) {
    case 0:
      zoneToZone(); // zone to zone (start to zone)
      break;
    case 1:
      zone_linetrace(); // zone 1
      break;
    case 2:
      zone_linetrace(); // zone 2
      break;
    case 3:
      zone3beta(); // zone 3
      break;
    case 4:
      zone_linetrace(); // winning action
      break;
    default:
      break;
  }
}

// 通信方式２
void sendData()
{
  static unsigned long timePrev = 0;
  static int inByte = 0;
  
  // 50msごとにデータ送信（通信方式２），500msデータ送信要求なし-->データ送信．
  if ( inByte == 0 || timeNow_G - timePrev > 500 || (Serial.available() > 0 && timeNow_G - timePrev > 50)) { // 50msごとにデータ送信
    inByte = Serial.read();
    inByte = 1;

    Serial.write('H');
    Serial.write(zoneNumber_G);
    Serial.write(mode_G);
    Serial.write((int)red_G );
    Serial.write((int)green_G );
    Serial.write((int)blue_G );
    Serial.write((int)(100*direction_G) >> 8); // Send direction_G with 2byte
    Serial.write((int)(100*direction_G) & 255);

    timePrev = timeNow_G;
  }
}
