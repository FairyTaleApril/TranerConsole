#coding:utf-8
import time
import os
import paho.mqtt.client as mqtt

import serial
import serial.tools.list_ports


strBroker = "127.0.0.1" # 服务器地址
port      = 1883        # 通信端口
topic     = ["Mega0/camera_trans", "Mega1/propeller", "Mega1/gyroscope", "Mega2/arm_tool"]


# ser = serial.Serial("com4", 9600)
ser0 = serial.Serial('/dev/ttyUSB0', 9600)
time.sleep(1)
if ser0.is_open:
    print('/dev/ttyUSB0 is open')
    
ser1 = serial.Serial('/dev/ttyUSB1', 9600)
time.sleep(1)
if ser1.is_open:
    print('/dev/ttyUSB1 is open')


def on_connect(client, userdata, flags, rc):
    print("Connected with result code: " + str(rc))
    
def on_subscribe(client, userdata, mid, granted_qos = 0):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))

def on_message(client, userdata, msg):
    msgPayload = msg.payload.decode()
    msgTopic = msg.topic
    print(msgTopic + ": " + msgPayload)
    
    if msgTopic == 'Mega1/propeller':
        if msgPayload[0] == 'A':
            ser0Write("A" + str(int(msgPayload[1]) * int(msgPayload[2])) + ";")
        elif msgPayload[0] == 'B':
            ser0Write("B" + str(int(msgPayload[1]) * int(msgPayload[2])) + ";")
        elif msgPayload[0] == 'L':
            ser0Write("L" + str(int(msgPayload[1]) * int(msgPayload[2])) + ";")
        elif msgPayload[0] == 'R':
            ser0Write("R" + str(int(msgPayload[1]) * int(msgPayload[2])) + ";")
            
        elif msgPayload[0] == 'p':  # pitchUp
            ser0Write("p" + str(int(msgPayload[1]) * int(msgPayload[2])) + ";")
        elif msgPayload[0] == 'P':  # pitchDown
            ser0Write("P" + str(int(msgPayload[1]) * int(msgPayload[2])) + ";")
        elif msgPayload[0] == 'o':  # rollLeft
            ser0Write("o" + str(int(msgPayload[1]) * int(msgPayload[2])) + ";")
        elif msgPayload[0] == 'O':  # rollRight
            ser0Write("O" + str(int(msgPayload[1]) * int(msgPayload[2])) + ";")
            
        elif msgPayload[0] == 'd':
            ser0Write("d" + str(int(msgPayload[1])) + ";")
        elif msgPayload[0] == 'v':
            pass
        elif msgPayload[0] == 's':
            ser0Write("s;")
        else:
            print("Mega1/propeller's msg is error")
        
    elif msgTopic == 'Mega2/arm_tool':
        if msgPayload[0] == '0':    # Arm伸出
            ser1Write('gAhead;')
            ser1Write('stretch;')
        elif msgPayload[0] == '1':  # Arm收回
            ser1Write('retrieve;')
            ser1Write('gBack;')
        elif msgPayload[0] == '2':  # Arm抓取
            ser1Write('grab;')
        elif msgPayload[0] == '3':  # 卸工具
            ser1Write('offtool')
        elif msgPayload[0] == '4':  # 装Tool1
            ser1Write('ontool1;')
        elif msgPayload[0] == '5':  # 装Tool2
            ser1Write('ontool2')
        elif msgPayload[0] == 's':  # saw
            ser1Write(msgPayload)
        elif msgPayload[0] == 'p':  # pump
            ser1Write(msgPayload)
        elif msgPayload[0] == 'c':  # 体感手套
            ser1Write(msgPayload)
        else:
            print("Mega2/arm_tool's msg is error")
            
    elif msgTopic == 'Mega1/gyroscope':
        pass
    
    elif msgTopic == 'Mega0/camera_trans':
        if msgPayload[0] == 'U':
            ser0Write("Up;")
        elif msgPayload[0] == 'D':
            ser0Write("Down;")
        elif msgPayload[0] == 'Reset':  # REST
            ser0Write("Reset;")
        elif msgPayload == "P1(10)":
            ser0Write("P1(10);")
        elif msgPayload == "P1(-10)":
            ser0Write("P1(-10);")
        elif msgPayload == "P2(10)":
            ser0Write("P2(10);")
        elif msgPayload == "P2(-10)":
            ser0Write("P2(-10);")
        elif msgPayload[0] == '#':
            rewrite_pos(msgPayload)
        else:
            print("Mega0/camera_trans's msg is error:" + msgPayload)
            
    else:
        print("invalid msgTopic");
    

client = mqtt.Client('Mega')
client.on_connect = on_connect
client.on_subscribe = on_subscribe
# client.on_publish = on_publish
client.on_message = on_message
client.connect(strBroker, port, 60)
for th in range(0, len(topic)):
    client.subscribe(topic[th], qos = 0)
client.loop_start()


def ser0Write(msg0):
    msg0 = msg0.encode('utf-8')
    ser0.write(msg0)
    
def ser1Write(msg1):
    msg1 = msg1.encode('utf-8')
    ser1.write(msg1)
    
    
def ser0Read():
    ser0Msg = ''
    while ser0.inWaiting() > 0:
        ser0Msg += ser0.read(1).decode()
        time.sleep(0.001)
    if ser0Msg != '':
        print("Mega0's msg is: " + ser0Msg)
        client.publish(topic[2], payload = ser0Msg, qos = 0)  


def rewrite_pos(msgpos):
    file_data = "1" + msgpos
    with open('pos.txt', "w", encoding = "utf-8") as f:
        f.truncate(0)
    with open('pos.txt', "w", encoding = "utf-8") as f:
        f.write(file_data)
        

while 1:
    ser0Read()
