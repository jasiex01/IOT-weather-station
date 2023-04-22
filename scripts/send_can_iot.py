#!/usr/bin/env python

import can

def send_one():

    bus = can.Bus(interface='socketcan', channel='can0', bitrate=500000,receive_own_messages=True) #receive own messages set to true for debugging
    msg = can.Message(arbitration_id=0xC0FFEE, data=[0, 25, 0, 1, 3, 1, 4, 1], is_extended_id=True)
    try:
        bus.send(msg)
        print(f"Message sent on {bus.channel_info}")
    except can.CanError:
        print("Message NOT sent")

if __name__ == "__main__":
    send_one()
