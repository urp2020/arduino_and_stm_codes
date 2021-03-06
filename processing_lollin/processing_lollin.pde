import controlP5.*; //import ControlP5 library
import processing.serial.*;

static int STEP=5;
Serial port;

ControlP5 cp5; //create ControlP5 object
PFont font;
int MOTOR_SPEED1=0,MOTOR_DIR1=1;
int MOTOR_SPEED2=0,MOTOR_DIR2=1;

String max_angle_difference = "2000000";

int stored_speed1,stored_speed2;
void setup(){ //same as arduino program

  size(300, 450);    //window size, (width, height)
  printArray(Serial.list());   //prints all available serial ports
 
  port = new Serial(this, "/dev/ttyUSB0", 9600);  //i have connected arduino to com3, it would be different in linux and mac os
  port.write(max_angle_difference+":"+STEP );
  //lets add buton to empty window



}

void draw(){  //same as loop in arduino

  background(150, 150 , 150); // background color of window (r, g, b) or (0 to 255)
 
  //lets give title to our window
  fill(255, 255, 0);               //text color (r, g, b)
  text("MOTOR_SPPED1  :  "+Integer.valueOf(MOTOR_SPEED1)*Integer.valueOf(MOTOR_DIR1), 80, 30);  // ("text", x coordinate, y coordinat)
  text("MOTOR_SPPED2  :  "+Integer.valueOf(MOTOR_SPEED2)*Integer.valueOf(MOTOR_DIR2), 80, 60);
}

//lets add some functions to our buttons
//so whe you press any button, it sends perticular char over serial port

void keyPressed() {

  if (keyCode == UP) {
    MOTOR_SPEED1 =MOTOR_SPEED1+STEP;
    port.write('u');
  }
  if (keyCode ==DOWN) {
    MOTOR_SPEED1 =MOTOR_SPEED1-STEP;
    port.write('d');
  }
  if (keyCode ==LEFT) {
    MOTOR_DIR1 =-MOTOR_DIR1;
    port.write('f');
  }
  if (key == ' ' ) {
    if(MOTOR_SPEED1 ==0 && MOTOR_SPEED2 ==0){
      MOTOR_SPEED1=stored_speed1;
      MOTOR_SPEED2=stored_speed2;
    }
    else{
      stored_speed1= MOTOR_SPEED1;
      stored_speed2= MOTOR_SPEED2;
      MOTOR_SPEED1 =0;
      MOTOR_SPEED2 =0;
    }
    port.write('0');
  }
 
  if (key == 'z'||key == 'Z' ) {
    if(MOTOR_SPEED1 ==0 ){
      MOTOR_SPEED1=stored_speed1;
    }
    else{
      stored_speed1= MOTOR_SPEED1;
      MOTOR_SPEED1 =0;
    }
    port.write('z');
  }
  if (key == 'x'||key == 'X' ) {
    if(MOTOR_SPEED2 ==0 ){
      MOTOR_SPEED2=stored_speed2;
    }
    else{
      stored_speed2= MOTOR_SPEED2;
      MOTOR_SPEED2 =0;
    }
    port.write('x');
  }
  
  if (key == 'w' || key == 'W') {
    MOTOR_SPEED2 =MOTOR_SPEED2+STEP;
    port.write('w');
  }
  if (key =='s' || key =='S') {
    MOTOR_SPEED2 =MOTOR_SPEED2-STEP;
    port.write('s');
  }
  if (key =='a' || key =='A') {
    MOTOR_DIR2 =-MOTOR_DIR2;
    port.write('a');
  }
}
