
// bang-bang
void zone_linetrace()
{
  float lightNow;
  static float lightMin = 19; // 70:R // 89:G // 各自で設定
  static float lightMax = 255; // 各自で設定
  static float lightAvg = (lightMin + lightMax) / 2.0;
  static float speed = 100; // パラメーター
  float speedDiff;
  float Kp = 2.0; // パラメーター
  switch ( mode_G ) {
    case 0:
      mode_G = 1;
      break;
    case 1:
      motorL_G = speed/2;
      motorR_G = speed/2;
      if(identifyColor(146, 31, 19))
        mode_G = 2;
      break;
    case 2:
      motorL_G = -speed;
      motorR_G = speed;
      if(identifyColor(255, 255, 255))
        mode_G = 3;
      break;
    case 3:
      lightNow = blue_G; //green_G //(red_G + green_G + blue_G ) / 3.0;
      speedDiff = map(lightNow, lightMin, lightMax, -Kp * speed, Kp * speed);
      /*if( speedDiff < 0 ){
        motorL_G = speed;
        motorR_G = speed + speedDiff;
      }else{
        motorL_G = speed - speedDiff;
        motorR_G = speed;
      }*/
      motorL_G = speed + speedDiff;
      motorR_G = speed - speedDiff;
      if(identifyColor(96, 36, 59))
        mode_G = 4;
      break;
    case 4:
      motorL_G = speed/2;
      motorR_G = speed/2;
      if(identifyColor(89, 185, 89))
        mode_G = 5;
      break;
    case 5:
      motorL_G = speed*2;
      motorR_G = -1 * speed*2;
      mode_G = 6;
      break;
    case 6:
      delay(1000);
      zoneNumber_G = 0;
      mode_G = 0;
    default:
      break;
  }
}



