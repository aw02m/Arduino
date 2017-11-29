//
// Zone 3 Beta
//

float avex = 0;
bool isTurning = false;

float turnTo(float dir) {
  float heading, diff;

  heading = direction_G;
  if (heading>180) heading -= 360;
  heading -= dir;
  if (heading>180) heading -= 360;
  if (heading<-180) heading += 360;
  diff = -heading*5;            // P-制御
  if (diff>200) diff = 200;     // 飽和
  if (diff<-200) diff = -200;   // 飽和
  return diff;
}

void zone3beta()
{
  static int countTurn = 0;
  int done;
  float speed0, diff, diff_slant=0;
  float xdiff; // 斜め登山用

  avex = 0.9*avex + 0.1*compass.a.x;

  switch ( mode_G ) {
    case 0: // setupはここ
      countTurn = 0;
      mode_G = 1;
    case 1: // 山探し(ここでは直進)
      if (avex>5000) { // 登り始めたら
        mode_G=2;
      }
      speed0 = 150;
      diff = 0; //-0.02 * compass.a.y; // P-制御
      xdiff = 0;
      //xdiff = 0.1 * compass.a.x;
      break;
    case 2: // 山登り中
      if ((avex<2000)&&(avex>-2000)) {  // 登頂したら頂上の中心部まで進む
        done = steadyState(200);        // 200ms は要調整
      }
      if (done == 1) {  // 頂上の中心部まで来たら停止
        speed0 = 0;
        diff = 0;
        xdiff = 100;
        mode_G = 3;
      } else {
        speed0 = 200;
        diff = -0.03 * (compass.a.x + 0.5 * compass.a.y);
//        diff = -0.0175 * (compass.a.y); // P-制御
//        xdiff = 0.02 * (compass.a.x);
        if (identifyColor(146, 31, 19)) {
          delay(100);
        }
//       diff = -0.02 * compass.a.y + 0.02 * compass.a.x;
//       diff_slant = 200;
      }
      break;
    case 3: // 500ms停止
      speed0 = 0;
      diff = 0;
      xdiff = 0;
      done = steadyState(500);
      if ( done == 1 )
        mode_G = 4;
      break;
    case 4:
      speed0 = 0;
      static int d = 0;
      diff = turnTo(d); // P-制御
      if (abs(diff)<=50) {
        d += 90;
        delay(50);
        if(d>360){
          mode_G = 5;
        }
      }
      break;
    case 5:
      speed0 = 0;
      diff = turnTo(180);
      if(abs(diff)<=50){
        diff = 50;
        speed0 = 100;
        if(avex<-2000){
          mode_G = 6;
        }
      }
      break;
    case 6:
      speed0 = 0;
//      diff = turnTo(45);
 //     if(abs(diff)<=50){
   //     diff = 20;
        speed0 = 100;
        if((avex<1500)&&(avex>-1500))
          mode_G = 7;
      
    case 7: // 停止
      speed0 = 100;
      diff = 0;
      if(identifyColor(0,0,0)){
        speed0 = 0;
      }
      break;
    default:
      break;
  }
  motorL_G = speed0 + diff; // - xdiff;// - xdiff;
  motorR_G = speed0 - diff; // + 1.25 * xdiff;// + xdiff;
}


