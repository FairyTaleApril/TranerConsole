/*******************************************************************************
              1 left     2  right
              3 left     4  right
              5 left1    6  right1
  推进器布局如上所示1,2,3,4是垂直推进器，5,6是水平推进器
*******************************************************************************/
#include <EEPROM.h>
#include <Servo.h>
#include <Wire.h>
#include <JY901.h>
#include "I2Cdev.h"
#include "MS5837.h"   // 水深度


/*
#define p1  7   //blue
#define p2  9   //orange
#define p3  11  //green
#define p4  13  //red
#define p5  17  //black
#define p6  15  //orange
*/

int addr = 0, interval = 0, speed = 0, armout = 0;
String msg = "", parameter = "";

int pin[6] = {9, 11 , 13, 15, 17, 19};
int pro_power[6], low[6] = {1483, 1483, 1488, 1488, 1480, 1483};
int flag[4] = {0, 0, 0, 0}; // r、p、y、d是否调节成功
float roll_0 = 0, pitch_0 = 0, yaw_0 = 0;
float pid[4][3] = {{200, 5, 10}, {100, 5, 10}, {200, 5, 10}, {1000, 5, 100}}; // 全部乘100进行存储，e2prom没法存储浮点数
/* pid[r、p、y、d][p、i、d] */

float errr = 0, integral_errr = 0, errr_last = 0, outputr = 0;
float targetr, currentr; // 滚筒pid调节

float errp = 0, integral_errp = 0, errp_last = 0, outputp = 0;
float targetp, currentp; // 俯仰pid调节

float erry = 0, integral_erry = 0, erry_last = 0, outputy = 0;
float targety, currenty; // 偏航pid调节

float errd = 0, integral_errd = 0, errd_last = 0, outputd = 0;
float targetd = 0, currentd; // 深度pid调节

int v_X = 0, v_Z = 0;
int manr = 0, manp = 0, many = 0;
int outr = 0, outp = 0, outy = 0, outd = 0;

bool flag_rollhold = false, flag_pitchhold = false, flag_yawhold = false, flag_depthhold = false;

Servo pro[6];
MS5837 sensor;    // 深度传感器,此传感器用I2C通信

void setup(void){
  Serial.begin(9600);
  
  JY901.StartIIC();
  Wire.begin();   //I2C通信初始化为主机
  
  for(int i = 0; i < 6; i++){
    pro[i].attach(pin[i]);
  }

  for(int i = 0; i < 6; i++){
    pro[i].writeMicroseconds(1500);
    delay(100);
  }

  for (int i = 0; i < 4; i++){
    for (int j = 0; j < 3; addr++, j++){
      EEPROM.write(addr, pid[i][j] * 100);
    }
  }

  /* 深度传感器初始化 */
  while (!sensor.init()) {
    Serial.println("Init failed!");
    delay(5000);
  }
  Serial.println("Init Success!");
  
  sensor.setModel(MS5837::MS5837_30BA);//设置深度传感器型号
  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
  JY901.GetAngle();
  roll_0  = (float)JY901.stcAngle.Angle[0] / 32768 * 180;
  pitch_0 = (float)JY901.stcAngle.Angle[1] / 32768 * 180;
  yaw_0   = (float)JY901.stcAngle.Angle[2] / 32768 * 180;
  Serial.println("success!");
  

  for(int i = 0; i < 4; i++){
  flag[i] = 0;
  }
}

void loop(void){  
  JY901.GetAngle();
  currentr = (float)JY901.stcAngle.Angle[0] / 32768 * 180 - roll_0;
  currentp = (float)JY901.stcAngle.Angle[1] / 32768 * 180 - pitch_0;
  currenty = (float)JY901.stcAngle.Angle[2] / 32768 * 180;
  
  sensor.read();
  currentd = sensor.depth() * 100; //厘米
  
  interval += 1;
  if(interval == 3){
    Serial.println(String(currentr) + "#" + String(currentp) + "#" + String(currenty) + "#" + String(currentd) + "#" + \
                   String(flag[0]) + String(flag[1]) + String(flag[2]) + String(flag[3]) + "#" + \
                   String(pro_power[0]) + String(pro_power[1]) + String(pro_power[2]) + String(pro_power[3]) + String(pro_power[4]) + String(pro_power[5]));
    interval = 0;
  }

  if(Serial.available()){
    delay(10);
    msg = Serial.readStringUntil(';');

    switch(msg[0]){
        case 'A':
          speed = msg[1] - 48;
          if(!speed){
            flag_rollhold  = true; outputr = 0;
            flag_pitchhold = true; outputp = 0;
            flag_yawhold   = true; outputy = 0;
            flag_depthhold = true; outputd = 0;
            targety = currenty;
          }
          v_X = speed * 100;
          run_PID();
          break;
        case 'B':
          speed = msg[1] - 48;
          if(!speed){
            flag_rollhold  = true; outputr = 0;
            flag_pitchhold = true; outputp = 0;
            flag_yawhold   = true; outputy = 0;
            flag_depthhold = true; outputd = 0;
            targety = currenty;
          }
          v_X = -(speed * 100);
          run_PID();
          break;
        case 'L':
          speed = msg[1] - 48;
          if(!speed){
            flag_rollhold  = true;  outputr = 0;
            flag_pitchhold = true;  outputp = 0;
            flag_yawhold   = false; outputy = 0;
            flag_depthhold = true;  outputd = 0;
            v_X=0;
          }
          many = -(speed * 100);
          run_PID();
          break;
        case 'R':
          speed = msg[1] - 48;
          if(!speed){
            flag_rollhold  = true;  outputr = 0;
            flag_pitchhold = true;  outputp = 0;
            flag_yawhold   = false; outputy = 0;
            flag_depthhold = true;  outputd = 0;
            v_X=0;
          }
          many = speed * 100;
          break;
  
        case 'd':
          speed = msg[1] - 48;
          if(speed > 4){
            speed = speed - 9;
          }
          flag_rollhold  = false; outputr = 0;
          flag_pitchhold = false; outputp = 0;
          flag_yawhold   = false; outputy = 0;
          flag_depthhold = false; outputd = 0;
          v_X=0;
          v_Z = speed * 100;
          break;
        case 's':
          v_X  = 0; v_Z=0;
          manr = 0; manp = 0; many = 0;
          flag_rollhold  = true; outputr = 0;
          flag_pitchhold = true; outputp = 0;
          flag_yawhold   = false; outputy = 0;
          flag_depthhold = true; outputd = 0;
          targety = currenty;
          targetd = currentd;
          break;
  
        case 'p': // pitchUp
          speed = msg[1] - 48;
          if(!speed){
            flag_rollhold  = false; outputr = 0;
            flag_pitchhold = false; outputp = 0;
            flag_yawhold   = true;  outputy = 0;
            flag_depthhold = false; outputd = 0;
            targety = currenty;
          }
          v_X  = 0;
          manp = speed * 100;
          break;
        case 'P': // pitchDown
          speed = msg[1] - 48;
          if(!speed){
            flag_rollhold  = false; outputr = 0;
            flag_pitchhold = false; outputp = 0;
            flag_yawhold   = true;  outputy = 0;
            flag_depthhold = false; outputd = 0;
            targety = currenty;
          }
          v_X  = 0;
          manp = -(speed * 100);
          break;
        case 'o': // rollLeft
          speed = msg[1] - 48;
          if(!speed){
            flag_rollhold  = false; outputr = 0;
            flag_pitchhold = false; outputp = 0;
            flag_yawhold   = false;  outputy = 0;
            flag_depthhold = false; outputd = 0;
            targety = currenty;
          }
          v_X  = 0;
          manr = speed * 100;
          break;
        case 'O': // rollRight
          speed = msg[1] - 48;
          if(!speed){
            flag_rollhold  = false; outputr = 0;
            flag_pitchhold = false; outputp = 0;
            flag_yawhold   = false;  outputy = 0;
            flag_depthhold = false; outputd = 0;
            targety = currenty;
          }
          v_X  = 0;
          manr = -(speed * 100);
          break;
    }
  }
	
  if(flag_rollhold){
    targetr = 0;
    PID_roll();
  }
  if(flag_pitchhold){
    targetp = 0;
    PID_pitch();
  }
  if(flag_yawhold){
    PID_yaw();
  }
  if(flag_depthhold){
    DepthLoop();
  }
  
  pid_cal_output();
  run();
  delay(200);
}


void PID_roll(){  // PID执行函数，此处为在loop执行，后期要改成定时执行，用定时器或者多任务处理
  errr = targetr - currentr;
  if(abs(errr) < 1){
    outputr = 0;
    integral_errr = 0;
    errr_last = 0;
    flag[0] = 1;
    return;
  }
  integral_errr += errr;
  outputr = (pid[0][0]/100)* errr + (pid[0][1]/100)  * integral_errr + (pid[0][2]/100)  * (errr - errr_last);
  outr = outputr;
  int(outputr) = constrain(outr, -200, 200);
  errr_last = errr;
}

void PID_pitch(){
  errp = targetp - currentp;
  if(abs(errp) < 1){
    outputp = 0;
    integral_errp = 0;
    errp_last = 0;
    flag[1] = 1;
    return;
  }
  integral_errp += errp;
  outputp = (pid[1][0]/100)  * errp + (pid[1][1]/100)  * integral_errp + (pid[1][2]/100)  * (errp - errp_last);
  outp = outputp;
  int(outputp) = constrain(outp, -200, 200);
  errp_last = errp;
}

void PID_yaw(){
  erry = targety - currenty;
  if(abs(erry) < 1){
    outputy = 0;
    integral_erry = 0;
    erry_last = 0;
    flag[2] = 1;
    return;
  }
  integral_erry += erry;
  outputy = (pid[2][0]/100)  * erry + (pid[2][1]/100) * integral_erry + (pid[2][2]/100)  * (erry - erry_last);
  outy = outputy;
  int(outputy) = constrain(outy, -200, 200);
  erry_last = erry;
}

void DepthLoop(){
  errd = targetd - currentd;
  if(abs(errd) < 0.5){
    outputd = 0;
    integral_errd = 0;
    errd_last = 0;
    flag[3] = 1;
    return;
  }
  integral_errd += errd;
  outputd = (pid[3][0]/100)  * errd + (pid[3][1]/100) * integral_errd + (pid[3][2]/100)  * (errd - errd_last);
  outd = outputd;
  int(outputd) = constrain(outd, -400, 400);
  errd_last = errd;
  Serial.println(outputd);
}

void pid_cal_output() { //六个推进器的动力分配,归一化
  pro_power[0] = -( v_Z + outputr - outputp - outputd - manr - manp);
  pro_power[1] =  ((v_Z - outputr - outputp - outputd) + manr - manp);
  pro_power[2] = -( v_Z + outputr + outputp - outputd - manr + manp);
  pro_power[3] =  ((v_Z - outputr + outputp - outputd) + manr + manp);
  pro_power[4] = -( v_X + outputy - many);
  pro_power[5] =  ( v_X - outputy + many);
  
  for(int i = 0; i < 6; i++){
    pro_power[i] = constrain(pro_power[i], -450, 450);
  }
}


void run(){
  for(int i = 0; i < 6; i++){
    pro[i].writeMicroseconds(low[i] + pro_power[i]);
  }
}
