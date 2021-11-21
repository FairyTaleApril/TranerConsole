#coding:utf-8
import base64
import struct
import threading
import time
from time import sleep
import json
import datetime
import serial
import os
import serial.tools.list_ports
try:
    import paho.mqtt.client as mqtt
except ImportError:
    print("MQTT client not find. Please install as follow:")
    print("git clone http://git.eclipse.org/gitroot/paho/org.eclipse.paho.mqtt.python.git")
    print("cd org.eclipse.paho.mqtt.python")
    print("sudo python setup.py install")


# 服务器地址
#strBroker = "106.13.86.141"
strBroker = "192.168.1.5"   # 树莓派IP
# 通信端口
port = 1883

class MqttThread(threading.Thread):

    def __init__(self, mqttc):
        super(MqttThread, self).__init__()
        self.mqttc = mqttc

    def on_connect(self, mqtt, obj, rc):
        print("OnConnetc, rc: " + str(rc))

    def on_publish(self, mqtt, obj, mid):
        print("OnPublish, mid: " + str(mid))

    def on_subscribe(self, mqtt, obj, mid, granted_qos):
        print("Subscribed: " + str(mid) + " " + str(granted_qos))

    def on_log(self, mqttc, obj, level, string):
        print("Log:" + string)

    #处理控制消息
    def on_message(self, mqttc, obj, msg):
        curtime = datetime.datetime.now()
        strcurtime = curtime.strftime("%Y-%m-%d %H:%M:%S")
        print(strcurtime + ": " + msg.topic + " " + str(msg.qos) + " " + str(msg.payload))
        data = json.loads(str(msg.payload))
        print(data["command"],"#####",data["target"])
        self.on_exec(str(msg.payload))

    def on_exec(self, strcmd):
        print("Exec:", strcmd)

    def run(self):
        self.mqttc.on_message = self.on_message
        self.mqttc.on_connect = self.on_connect
        self.mqttc.on_publish = self.on_publish
        self.mqttc.on_subscribe = self.on_subscribe
        self.mqttc.on_log =self.on_log
        self.mqttc.connect(strBroker, port, 60)
        self.mqttc.subscribe("command", 0)
        self.mqttc.loop_forever()


def mqtt_pub(mqttc, data):
    """
    将串口蓝牙接收到的信息发送出去
    :return:
    """
    print("start")
    mqttc.publish("data/ble", payload=data, qos=0)  # 发送消息



if __name__=='__main__':
    port_list = list(serial.tools.list_ports.comports())
    k = 0
    for i in port_list:
        print(i,k)
        k=k+1

    if len(port_list) <= 0:
        print("not find serial")
    else:
        serial_k=input("please switch serial:")
        k = int(serial_k)
        serial_list = list(port_list[k])
        serialName = serial_list[0]
        #print(serialName)
        serial = serial.Serial(serialName, 9600, timeout=3600)
        if serial.isOpen():
            print("open succeed >", serial.name)
        else:
            print("open failed >", serial.name)

        # 选择好串口以后，开启mqtt
        # 开启mqtt线程
        mqttc = mqtt.Client("bleSerialData")
        # mqttc.username_pw_set("zcc3b0k/raspberry", "lBQZOuxNbhSUfs97")
        t1 = MqttThread(mqttc)
        t1.start()
        
        # 接受到的数据实例 55 55 08 03 01 32 00 50 ce 04
        try:
            while True:
                data1 = struct.unpack("B", serial.read(1))
                data2 = struct.unpack("B", serial.read(1))
                print(data1,data2)
                pre_data =()
                if data1[0] is 85 and data2[0] is 85:
                    buff = serial.read(8)
                    data = struct.unpack(8*"B", buff)
                    #sleep(0.1)
                    pre_data = (85, 85)
                    pre_data = pre_data + data

                elif data2[0] is 85:
                    print("85 is in 2")
                    buff = serial.read(9)
                    data = struct.unpack(9 * "B", buff)
                    # sleep(0.1)
                    pre_data = (85,)
                    pre_data = pre_data + data
                else:
                    print("error Data")
                    continue
                sendStr = ""
                for i in pre_data:
                    sendStr = sendStr + str(i) + "#"
                    # 组装以后的数据    85#85#8#3#1#50#0#80#206#37
                print(sendStr[0:len(sendStr)-1])
                mqtt_pub(mqttc, sendStr[-1::])
        except(KeyboardInterrupt, SystemExit):
            pass

