import serial
import time
import serial.tools.list_ports
import SerialCommunication
from random import randint
import pyautogui
from python_imagesearch.imagesearch import imagesearch,imagesearcharea
import webbrowser
import time
import os
from datetime import datetime
from pynput.keyboard import Listener


#define variable
work_path =  os.path.dirname(os.path.abspath(__file__))+"\\" # add path to script

PngPath = work_path+"png\\" # path to png ico # path to png ico

CandyPod_Button=PngPath+"Candy.png"

ports = list(serial.tools.list_ports.comports())
for port in ports:
    print(f"Port: {port.device}, Description: {port.description}")

portName="COM3"
tryOpen=True
openSerialPort = SerialCommunication.connectToPiranhaESP32(portName,tryOpen)


def on_press(key):
    return False  # ѕрекращает слушатель при любом нажатии

with Listener(on_press=on_press) as listener:
    
   while listener.running:

    print("Activate This Window And\n")
    print("Press Any Key On To Exit...")   
    time.sleep(6)  # 6 с
    posIco_Button = imagesearch(CandyPod_Button)

    if (posIco_Button[0] != -1):
        print(f"Image Found Koordinates: X={posIco_Button[0]}, Y={posIco_Button[1]}")
    pos = pyautogui.position()
    print(f"Koordinates Cursor: X={pos[0]}, Y={pos[1]}")

    toIntMoveX=posIco_Button[0]-pos[0];
    toIntMoveY=posIco_Button[1]-pos[1];
    toMoveX="";
    toMoveY="";

    if(toIntMoveX<0):
        toMoveX=str(toIntMoveX).zfill(5);
    if(toIntMoveY<0):
        toMoveY=str(toIntMoveY).zfill(5);

    if(toIntMoveX>=0):
        toMoveX="+"+str(toIntMoveX).zfill(4);
    if(toIntMoveY>=0):
        toMoveY="+"+str(toIntMoveY).zfill(4);

    commandText="mouse:move:"+toMoveX+":"+toMoveY+"\0"
    print(commandText)
    print("\n")
    print("Searching Image\n")
    time.sleep(1)
    SerialCommunication.sendCommand(commandText,openSerialPort)
    
openSerialPort.close()