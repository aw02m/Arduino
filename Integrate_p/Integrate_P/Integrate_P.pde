
import processing.serial.*;
Serial port; 
int zoneNumber, mode;
int green, red, blue, green_p, red_p, blue_p;
int high, low;
int[] sensors = new int[5];
int count;
int[] sensors_p = new int[5];
float direction_G;
float distance;

void setup() {
  size(1200, 400);
  background(255);
  count = 0;
  println(Serial.list());
  // String arduinoPort = Serial.list()[1];
  // port = new Serial(this, arduinoPort, 9600 );
  port = new Serial(this, "/dev/ttyUSB0", 9600 ); // シリアルポート名は各自の環境に合わせて適宜指定
  zoneNumber = 0;
  red_p = 0; 
  green_p = 0; 
  blue_p = 0;
}

void draw() {
  float y_p, y;

  stroke(200, 200, 200);
  line(0, height*0.1, width, height*0.1);
  line(0, height*0.9, width, height*0.9);

  y_p = map(red_p, 0, 255, height*0.9, height*0.1);
  y = map(red, 0, 255, height*0.9, height*0.1);
  stroke(255, 0, 0);
  line((count-1)*10, y_p, (count)*10, y );

  y_p = map(green_p, 0, 255, height*0.9, height*0.1);
  y = map(green, 0, 255, height*0.9, height*0.1);
  stroke(0, 255, 0);
  line((count-1)*10, y_p, (count)*10, y );


  y_p = map(blue_p, 0, 255, height*0.9, height*0.1);
  y = map(blue, 0, 255, height*0.9, height*0.1);
  stroke(0, 0, 255);
  line((count-1)*10, y_p, (count)*10, y );

  stroke(255);
  fill(255);
  rect(0, height-50, 300, 50);
  textSize(16);
  fill(0);
  text("zone = ", 10, height-10);
  text((int)zoneNumber, 150, height-10);
  text(", ", 225, height-10);
  text((int)mode, 200, height-10);
  text("color = ", 10, height-30);
  text((int)red, 150, height-30);
  text((int)green, 200, height-30);
  text((int)blue, 250, height-30);
  text("direction_G = ", 10, height-50);
  text((int)direction_G, 150, height-50);
  text("distance = ", 10, height-70);
  text((int)distance, 150, height-70);
  noFill();

  if ( count == 120 ) {
    count = 0;
    background(255);
  }
}

int read2byte(Serial p) {
  int x = p.read();  x <<= 8;   x |=p.read();
  if (x>32757) x -= 65536;
  return x;
}

// 通信方式1
void serialEvent(Serial p) { 

  if ( p.available() >= 8 ) { 
    if ( p.read() == 'H' ) {

      red_p = red;
      green_p = green;
      blue_p = blue;

      zoneNumber = p.read();
      mode =  p.read();
      red =  p.read();
      green = p.read();
      blue =  p.read();
      direction_G = read2byte(p);
      distance = read2byte(p);
      p.clear(); // 念のためクリア
      p.write("A");

      print("  RGB = ");
      println(red, green, blue);
      ++count;
    }
  }
}