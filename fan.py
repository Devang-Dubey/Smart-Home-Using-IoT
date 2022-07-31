'''
This file emulate FAN appliance
'''
import time
import threading

from tkinter import *
from paho.mqtt import client
from PIL import Image, ImageTk

from awscrt import io, mqtt, auth, http
from awsiot import mqtt_connection_builder

ENDPOINT = "a30xom1njimvh2-ats.iot.ap-south-1.amazonaws.com"
CLIENT_ID = "fan"
PATH_TO_CERTIFICATE = "Certificates/fan_device.crt"
PATH_TO_PRIVATE_KEY = "Certificates/fan_private.key"
PATH_TO_AMAZON_ROOT_CA_1 = "Certificates/AmazonRootCA1.pem"

def on_message_received(topic, payload, **kwargs):

    data = payload.decode('utf-8')
    ListTemp  = data.split('-')
    Device = str(ListTemp[0])
    Action = str(ListTemp[1])
    if(str(Device).upper() == "FAN"):

        status['text'] = "Current Status :" + str(Action)

        if(str(Action).upper() == "ON"):
            status['fg'] = 'green'
        else:
            status['fg'] = 'red'


event_loop_group = io.EventLoopGroup(1)
host_resolver = io.DefaultHostResolver(event_loop_group)
client_bootstrap = io.ClientBootstrap(event_loop_group, host_resolver)
mqtt_connection = mqtt_connection_builder.mtls_from_path(
            endpoint=ENDPOINT,
            cert_filepath=PATH_TO_CERTIFICATE,
            pri_key_filepath=PATH_TO_PRIVATE_KEY,
            client_bootstrap=client_bootstrap,
            ca_filepath=PATH_TO_AMAZON_ROOT_CA_1,
            client_id=CLIENT_ID,
            clean_session=False,
            keep_alive_secs=6
            )

print("Connecting to {} with client ID '{}'...".format(ENDPOINT, CLIENT_ID))
connect_future = mqtt_connection.connect()
connect_future.result()
print("FAN Connected!")


def mqttProcess():
    
    subscribe_future, packet_id = mqtt_connection.subscribe(
    topic="smart_home/room/Device-Actions",
    qos=mqtt.QoS.AT_LEAST_ONCE,
    callback=on_message_received
    )
    subscribe_result = subscribe_future.result()


tk = Tk()  

tk.geometry("500x550")
tk.configure(bg='black')
title = Label(tk,text = "FAN Appliance",pady = 5,bg='black',fg='white')
title.pack()
fan_image = ImageTk.PhotoImage(Image.open("Images/fan.png").resize((500,400)))

bg = Label(tk,image = fan_image,bd=0)
bg.pack()

display_pane = Frame(tk,bd=1,bg='white',relief = "raised")
display_pane.pack()

status = Label(display_pane,text ="Status : OFF",fg="red",bg="white",padx=5,pady=5)
status.pack()

threading.Thread(target=mqttProcess).start()

tk.mainloop()

while True:
    time.sleep(0.0001)
