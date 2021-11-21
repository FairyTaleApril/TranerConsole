#include <Servo.h>


#define  relayPin1_battery    14
#define  relayPin2_battery    15

#define  relayPin1_pump       16
#define  relayPin2_pump       17  // 地线，不能动

#define  relayPin1_ABputter   18
#define  relayPin2_ABputter   19
#define  relayPin1_UDputter   20
#define  relayPin2_UDputter   21


Servo myservo[11];
Servo up, down, saw;


String msg = "";
int tool = 0;
int degree1 = 90, degree2 = 90;
int servo_pin[11] = {};
int servo_pos[11] = {1500, 800, 850, 1450, 800, 1500, 1500, 800, 1600, 1500, 1500};
/* 
leftarm[6] = {1500, 1500, 1500, 2200, 800, 1500};
  0号舵机(orange)：1500(水平) ~ 2100(垂直)
  1号舵机(red)：600(伸出) ~ 1400(垂直) ~ 2150(收回)
  2号舵机(blue)：850(直着) ~ 1900(内侧)
  3号舵机(green)：500(最内侧) ~ 1450(垂直) ~ 2500(最外侧)
  4号舵机(purple)：800(垂直) ~ 1500(水平)
  5号舵机(yellow)：1000(张开) ~ 2300(加紧)
  
rightarm[5] = {1500, 1500, 1500, 2200, 800}
  6号舵机(yellow)：900(垂直) ~ 1500(水平)
  7号舵机(orange)：800(最外侧) ~ 1500(垂直) ~ 2100(最内侧)
  8号舵机(red)：500(内侧) ~ 1600(垂直)
  9号舵机(green)：500(外侧) ~ 1500(垂直) ~ 2300(内侧)
  10号舵机(blue)：500 ~ 2500
*/


void servopos(int id, int pos){
  if(pos < 500){
    pos = 500;
  }
  else if(pos > 2500){
    pos = 2500;
  }
  
  if(pos > servo_pos[id]){
    for(; servo_pos[id] < pos; servo_pos[id] += 5){
        myservo[id].writeMicroseconds(servo_pos[id]);
        delay(10);
    }
  }
  else{
    for(; servo_pos[id] > pos; servo_pos[id] -= 5){
        myservo[id].writeMicroseconds(servo_pos[id]);
        delay(10);
    }
  }
  
  servo_pos[id] = pos;
}


void setup(){
  Serial.begin(9600);
	
  up.attach(Pin1);
  up.write(degree1);
  down.attach(Pin2);
  down.write(degree2);
  
  saw.attach(Pin2);
  saw.writeMicroseconds(1500);
  delay(100);

  pinMode(relayPin1_battery,  OUTPUT);
  pinMode(relayPin2_battery,  OUTPUT);
  pinMode(relayPin1_pump,     OUTPUT);
  pinMode(relayPin1_ABputter, OUTPUT);
  pinMode(relayPin2_ABputter, OUTPUT);
  pinMode(relayPin1_UDputter, OUTPUT);
  pinMode(relayPin2_UDputter, OUTPUT);

  digitalWrite(relayPin1_battery,  HIGH);
  digitalWrite(relayPin2_battery,  HIGH);
  digitalWrite(relayPin1_pump,     LOW);
  digitalWrite(relayPin1_ABputter, LOW);
  digitalWrite(relayPin2_ABputter, LOW);
  digitalWrite(relayPin1_UDputter, LOW);
  digitalWrite(relayPin2_UDputter, LOW);
  
  for(int i = 0, j = 2; i < 6; i ++, j = j + 2 * i){
    myservo[servo_pin[i]].attach(j);
    servopos(servo_pin[i], servo_pos[servo_pin[i]]);
  }

  for(int i = 6, j = 3; i < 11; i ++, j = j + 2 * (i - 6)){
    myservo[servo_pin[i]].attach(j);
    servopos(servo_pin[i], servo_pos[servo_pin[i]]);
  }
}


void loop(){
  if (Serial.available()){
    delay(10);
    msg = Serial.readStringUntil(';');
    
    if(msg.equals("stretch")){  // 伸出
      // 左臂伸出
      servopos(servo_pin[0], 1500);
      delay(200);
      servopos(servo_pin[3], 1200);
      delay(200);
      servopos(servo_pin[2], 1200);
      delay(200);
      servopos(servo_pin[1], 1800);
      delay(200);
      servopos(servo_pin[4], 800);
      delay(200);
      servopos(servo_pin[5], 2300);
      delay(200);
      
      // 右臂伸出
      servopos(servo_pin[6], 1500);
      delay(200);
      servopos(servo_pin[9], 1200);
      delay(200);
      servopos(servo_pin[8], 1200);
      delay(200);
      servopos(servo_pin[7], 1900);
      delay(200);
      servopos(servo_pin[10], 1500);
      delay(200);
      Serial.println("stretch");
    }
    else if(msg.equals("retrieve")){  // 收回
      // 左臂收回
      servopos(servo_pin[0], 1500);
      delay(200);
      servopos(servo_pin[0], 800);
      delay(200);
      servopos(servo_pin[2], 850);
      delay(200);
      servopos(servo_pin[3], 1450);
      delay(200);
      servopos(servo_pin[4], 800);
      delay(200);
      servopos(servo_pin[5], 1500);
      delay(200);

      // 右臂收回
      servopos(servo_pin[6], 1500);
      delay(200);
      servopos(servo_pin[7], 800);
      delay(200);
      servopos(servo_pin[8], 1600);
      delay(200);
      servopos(servo_pin[9], 1500);
      delay(200);
      servopos(servo_pin[10], 1500);
      delay(200);
      Serial.println("retrieve");
    }
    else if(msg.equals("grab")){  // 抓取
      servopos(servo_pin[5], 1000);
      delay(3000);
      servopos(servo_pin[5], 2300);
    }
    else if(msg.equals("offtool")){  // 摘工具
      // 左臂伸出
      servopos(servo_pin[0], 1500);
      delay(200);
      servopos(servo_pin[3], 1200);
      delay(200);
      servopos(servo_pin[2], 1200);
      delay(200);
      servopos(servo_pin[1], 2100);
      delay(200);
      servopos(servo_pin[4], 800);
      delay(200);
      servopos(servo_pin[5], 2300);
      delay(200);

      servopos(servo_pin[6], 1580);
      delay(200);
      servopos(servo_pin[9], 1300);
      delay(200);
      servopos(servo_pin[8], 1250);
      delay(200);
      servopos(servo_pin[7], 2000);
      delay(200);
      servopos(servo_pin[10], 1500);
      delay(200);

      // 左手拔工具
      servopos(servo_pin[5], 1000);
      delay(200);
      servopos(servo_pin[3], 1600);
      delay(200);

      // 左手装工具
      servopos(servo_pin[0], 800);
      delay(200);
      servopos(servo_pin[1], 1600);
      delay(200);
      servopos(servo_pin[2], 1700);
      delay(200);
      servopos(servo_pin3], 850);
      delay(200);

      // 左臂归位
      servopos(servo_pin[5], 2300);
      delay(200);
      servopos(servo_pin[1], 1400);
      delay(200);
      servopos(servo_pin[2], 1200);
      delay(200);
      servopos(servo_pin[1], 1800);
      delay(200);
      servopos(servo_pin[1], 1200);
      delay(200);
      servopos(servo_pin[0], 1500);
      delay(200);

      // 右臂归位
      servopos(servo_pin[9], 1500);
      delay(200);
      servopos(servo_pin[8], 1200);
      delay(200);
      servopos(servo_pin[7], 1900);
      delay(200);
      
    }
    else if(msg.equals("ontool1")){  // 装工具
      // 右臂伸出
      servopos(servo_pin[6], 1550);
      delay(200);
      servopos(servo_pin[9], 1200);
      delay(200);
      servopos(servo_pin[8], 1300);
      delay(200);
      servopos(servo_pin[7], 2000);
      delay(200);
      servopos(servo_pin[10], 1500);
      delay(200);
      
      // 左臂伸出
      servopos(servo_pin[0], 1500);
      delay(200);
      servopos(servo_pin[3], 1200);
      delay(200);
      servopos(servo_pin[2], 1200);
      delay(200);
      servopos(servo_pin[1], 1800);
      delay(200);
      servopos(servo_pin[4], 800);
      delay(200);
      servopos(servo_pin[5], 2300);
      delay(200);
      
      // 左手就位
      servopos(servo_pin[0], 800);
      delay(200);
      servopos(servo_pin[1], 1200);
      delay(200);
      servopos(servo_pin[2], 1600);
      delay(200);
      servopos(servo_pin[1], 1600);
      delay(200);
      servopos(servo_pin[3], 850);
      delay(200);

      // 左手拔工具
      servopos(servo_pin[5], 1000);
      delay(200);
      servopos(servo_pin[3], 1500);
      delay(200);
      
      // 左手装工具
      servopos(servo_pin[0], 1400);
      delay(200);
      servopos(servo_pin[2], 1200);
      delay(200);
      servopos(servo_pin[3], 1400);
      delay(200);
      servopos(servo_pin[6], 1700);
      delay(200);
      servopos(servo_pin[9], 1300);
      delay(200);
      servopos(1servo_pin[1], 2400);
      delay(200);

      // 左臂归位
      servopos(servo_pin[5], 2300);
      delay(200);
      servopos(servo_pin[9], 1600);
      delay(200);
      servopos(servo_pin[2], 1200);
      delay(200);
      servopos(servo_pin[1], 1800);
      delay(200);
      servopos(servo_pin[3], 1200);
      delay(200);
      servopos(servo_pin[0], 1500);
      delay(200);
      
      // 右臂归位
      servopos(servo_pin[6], 1500);
      delay(200);
      servopos(servo_pin[9], 1400);
      delay(200);
      servopos(servo_pin[8], 1100);
      delay(200);
      servopos(servo_pin[7], 1850);
      delay(200);
    }
    else if(msg[0] == 'c'){
      int iD = (msg[1] - 48) * 10 + (msg[2] - 48);
      int post = (msg[3] - 48) * 1000 + (msg[4] - 48) * 100 + (msg[5] - 48) * 10 + (msg[6] - 48);
      Serial.println(iD);
      Serial.println(post);
      servopos(servo_pin[iD], post);
    }

  	else if(msg.equals("Down")){
      digitalWrite(relayPin1_UDputter, HIGH);
      digitalWrite(relayPin2_UDputter, LOW);
      
      delay(6000);

      digitalWrite(relayPin1_UDputter, HIGH);
      digitalWrite(relayPin2_UDputter, HIGH);
  	}
  	else if(msg.equals("Up")){
      digitalWrite(relayPin2_UDputter, HIGH);
      digitalWrite(relayPin1_UDputter, LOW);
      
      delay(6000);

      digitalWrite(relayPin1_UDputter, HIGH);
      digitalWrite(relayPin2_UDputter, HIGH);
  	}
   
    else if(msg.equals("gAhead")){
      digitalWrite(relayPin1_ABputter, HIGH);
      digitalWrite(relayPin2_ABputter, LOW);
      
      delay(6000);

      digitalWrite(relayPin1_ABputter, HIGH);
      digitalWrite(relayPin2_ABputter, HIGH);
    }
    else if(msg.equals("gBack")){
      digitalWrite(relayPin2_ABputter, HIGH);
      digitalWrite(relayPin1_ABputter, LOW);
      
      delay(6000);

      digitalWrite(relayPin1_ABputter, HIGH);
      digitalWrite(relayPin2_ABputter, HIGH);
    }
    
    else if(msg.equals("pumpOn")){
      digitalWrite(relayPin1_pump, HIGH);
    }
    else if(msg.equals("pumpOff")){
      digitalWrite(relayPin1_pump, LOW);
    }

    else if(msg.equals("sawOn")){
      saw.writeMicroseconds(2000);
    }
    else if(msg.equals("sawOff")){
      saw.writeMicroseconds(1500);
    }
    
    else if(msg.equals("8v1On")){
      digitalWrite(relayPin1_pump, HIGH);
    }
    else if(msg.equals("8v1Off")){
      digitalWrite(relayPin1_pump, LOW);
    }
    else if(msg.equals("8v2On")){
      digitalWrite(relayPin1_pump, HIGH);
    }
    else if(msg.equals("8v2Off")){
      digitalWrite(relayPin1_pump, LOW);
    }
  	
  	else if(msg.equals("Reset")){
      if(degree1 != 180){
        degree1 = degree1 + 10;
        up.write(degree1);
  	}
  	else if(msg.equals("P1(10)")){
      if(degree1 != 0){
        degree1 = degree1 - 10;
        up.write(degree1);
  	}
  	else if(msg.equals("P1(-10)")){
      if(degree2 != 180){
        degree2 = degree2 + 10;
        down.write(degree2);
  	}
  	else if(msg.equals("P2(10)")){
      if(degree2 != 0){
        degree2 = degree2 - 10;
        down.write(degree2);
  	}
  	else if(msg.equals("P2(-10)")){
      degree1 = 90;
      degree2 = 90;
      up.write(degree1);
      down.write(degree2);
  	}
  	
    else{
      Serial.println("error");
    }
  }
}
