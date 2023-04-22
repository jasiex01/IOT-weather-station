#!/usr/bin/env python

import threading
import can
import paho.mqtt.client as mqtt
from new import Screen
import time

scr1 = Screen(512, "pix.png", True)
scr2 = Screen(768, "pix.png", True)
bitcoin_scr1 = Screen(512, "plot.png", False)
pic_scr1 = Screen(512, "plot2.png", False)
weather_scr1 = Screen(512, "pix.png", True)
bitcoin_scr2 = Screen(768, "plot.png", False)
pic_scr2 = Screen(768, "plot2.png", False)
weather_scr2 = Screen(768, "pix.png", True)
mqtt_client = mqtt.Client()

def can_reader():
    bus = can.interface.Bus(channel='can0', bustype='socketcan')
    bus.set_filters([{"can_id": 0x02, "can_mask":0x7FC, "extended": False}])

    global scr1
    global scr2
    
    while True:
        message = bus.recv()
        if message.arbitration_id == 1:
            if message.data == bytearray(b'\x01'):
                scr1 = bitcoin_scr1
            elif message.data == bytearray(b'\x00'):
                scr1 = weather_scr1
            else:
                scr1 = pic_scr1
            scr1.update_screen(message_decoded)
        elif message.arbitration_id == 2:
            if message.data == bytearray(b'\x01'):
                scr2 = bitcoin_scr2
            elif message.data == bytearray(b'\x00'):
                scr2 = weather_scr2
            else:
                scr2 = pic_scr2
            scr2.update_screen(message_decoded)
        print("Received message: ", message.data)
        time.sleep(0.5)


def process_message(client, userdata, message):
    global message_decoded 
    message_decoded = str(message.payload.decode("utf-8"))

    delimiter = ';'
    message_decoded = message_decoded.split(delimiter)
    print(message_decoded)
    scr1.update_screen(message_decoded)
    scr2.update_screen(message_decoded)

def on_connect(client, userdata, flags, rc):
    print("Połączono z brokerem MQTT, kod odpowiedzi: " + str(rc))

def connect_to_broker():
    mqtt_host = "tajne"
    mqtt_port = 1883
    mqtt_user = "test"
    mqtt_password = "tajne"
    topic = "weather/all"

    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = process_message
    mqtt_client.username_pw_set(mqtt_user, mqtt_password)
    mqtt_client.connect(mqtt_host, mqtt_port)

    mqtt_client.loop_start()
    mqtt_client.subscribe(topic)

def disconnect_from_broker():
    mqtt_client.loop_stop()
    mqtt_client.disconnect()

if __name__ == "__main__":
    can_thread = threading.Thread(target=can_reader)
    can_thread.start()
    connect_to_broker()
    while True:
        pass
    disconnect_from_broker()
