from random import randrange
from paho.mqtt import client as mqtt_client
from struct import *
from crc import Crc16, CrcCalculator, Configuration
import time

broker = 'broker.hivemq.com'
port = 1883
topic = 'centralHeating/roomTemperature'
client_id = f'python-mqtt-{randrange(1000)}'
roomName = [0x53, 0x61, 0x6C, 0x6F, 0x6E, 0x00]

report_station_id = 0x00
room_temp = 24.5
humidity = 56.87

name = bytearray(roomName)
empty_bytes = bytearray(14)

data_to_send = pack('<B6s14sBdd', report_station_id,
                    name, empty_bytes, 60, room_temp, humidity)

width = 16
poly = 0x1021
init_value = 0xFFFF
final_xor_value = 0x0000
reverse_input = False
reverse_output = False

configuration = Configuration(
    width, poly, init_value, final_xor_value, reverse_input, reverse_output)

use_table = True
crc_calculator = CrcCalculator(configuration, use_table)

checksum = crc_calculator.calculate_checksum(data_to_send)
data_to_send_with_checksum = pack(
    '<B6s14sBddH', report_station_id, name, empty_bytes, 60, room_temp, humidity, checksum)


def connect_mqtt():
    def on_connect_callback(client, userdata, flags, rc):
        if rc == 0:
            print('Connected to broker!')
        else:
            print('Failed to connect!')

    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect_callback
    client.connect(broker, port)
    return client


def publish(client):
    while True:
        result = client.publish(topic, data_to_send_with_checksum)
        if result[0] == 0:
            print('Message sent!')
        time.sleep(10)


if __name__ == '__main__':
    client = connect_mqtt()
    client.loop_start()
    publish(client)
