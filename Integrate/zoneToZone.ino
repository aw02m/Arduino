const float color[4][3] = { // 各ゾーン（1-4)の識別色（RGB値）（各自で設定）
  { 146, 31, 19 },
  { 89, 185, 89 },//{ 51, 131, 52 },
  { 20, 38, 111 },
  { 224, 214, 76 },
};

const float direction_exit[3] = { // 各ゾーン(1-3)の脱出方向の角度（各自で設定）
  180, 270, 270
}; 
float minDistance; // identifyZone()用のグローバル変数

unsigned long turnTime = 200;

// zoneからzoneへの移動（mode_G=0からスタート)
// startからzoneへの移動（mode_G=2からスタート）
void zoneToZone()
{
  int zoneNumber;
  float direction_target;
  static int zoneNumber_in; // ゾーンに入った時のゾーン番号を記録
  static int td = 1;
  float diff = 0;

  switch ( mode_G ) {
    case 0:  // setup（必要がなくても形式的に置いておく）
      mode_G = 1;
      break;
    case 1: // direction_targetの方向を向く
      diff = turnTo(direction_exit[ zoneNumber_in ]);
      motorL_G = diff;
      motorR_G = -1*diff;
      if (turnToDirection(direction_exit[zoneNumber_in])){
        mode_G = 2;
      }
      break;
    case 2: // 黄を検知するまで直進
      goStraight( SPEED );
      if ( identifyColor( 211, 191, 58 ) ) { // 黄を検知
        mode_G = 3;
      } else if ( identifyColor ( 0, 0, 0 ) ) {
        mode_G = 5;
      }
      break;
    case 3: // 黒を検知するまで直進
      goStraight( SPEED );
      if ( identifyColor( 0, 0, 0 ) ) { // 黒を検知
        minDistance = 9999999; // identifyZone()関数で使う変数の初期化
        mode_G = 4;
      }
      break;
    case 4: // 白を検知するまで直進（その間ゾーン番号を検知）
      goStraight( SPEED );
      zoneNumber = identifyZone();
      if ( identifyColor( 255, 255, 255 ) ) { // 白を検知
        goStraight( 0 );
        zoneNumber_G = zoneNumber; // 状態変数の更新
        mode_G = 0;                // 状態変数の更新
        zoneNumber_in = zoneNumber; // どのゾーンに入ったのか記録
      }
      break;
     case 5:
      if ( steadyState(turnTime) ) {
        turnTime += 200;
        td *= -1;
      }
      turnRight( -td*SPEED );
      if ( !identifyColor(0, 0, 0) ) {
        mode_G = 2;
        turnTime = 200;
        td = 1;
      }
    default:
      break;
  }
}

// 直進
void goStraight( int speed )
{
  motorL_G = speed;
  motorR_G = speed;
}

// 右回転（負の値は左回転）
void turnRight( int speed )
{
  motorL_G = speed;
  motorR_G = -speed;
}

// 指定した方向いたら1を返す（それ以外0)
int turnToDirection( int direction_target )
{
  if( direction_G-1 <= direction_target && direction_target <= direction_G+1) {
    return 1;
  } else {
    return 0;
  }
}


// KNNで現在最も近い番号を返す
int identifyZone()
{
  // float minDistance; グローバル変数で定義
  static int zoneNumber; // 現状で一番近い識別色の番号
  float distance; // 距離の2乗

  for ( int i = 0; i < 4; ++i ) {
    distance = (red_G - color[i][0]) * (red_G - color[i][0])
               + (green_G - color[i][1]) * (green_G - color[i][1])
               + (blue_G - color[i][2]) * (blue_G - color[i][2]);
    if ( distance < minDistance ) {
      minDistance = distance;
      zoneNumber = i;
    }
  }

  return zoneNumber + 1; // zone番号は1-4なので+1
}

int identifyColor( int red, int green, int blue )
{
  float d2;
  float d2_max = 30; // パラメーター

  d2 = pow(red - red_G, 2) + pow(green - green_G, 2) + pow(blue - blue_G, 2);
  if ( d2 < d2_max * d2_max )
    return 1;
  else
    return 0;
}


















