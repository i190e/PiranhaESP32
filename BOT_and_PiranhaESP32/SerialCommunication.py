import serial
import time
import serial.tools.list_ports

def connectToPiranhaESP32(portName,tryOpen):
    # Configure serial connection
    if(tryOpen==True):
        
      try:
           openedSerialPort = serial.Serial( port=portName,baudrate=115200, timeout=1,)
           print("Serial port opened:", openedSerialPort.name)          
      except KeyboardInterrupt:
            print("Exiting...")
      except Exception as e:
            print(f"Error: {e}")
      finally:
           # openedSerialPort.close()
            print("Serial port closed.")
    else: 
          openedSerialPort.close()
          print("Serial port closed.")
    return openedSerialPort;

def sendCommand(message,openSerialPort):
        openSerialPort.write(message.encode('utf-8'))

#structure: mouse:move:+-x[4]:+-y[4] // mouse click [command]//example//mouse:move:+0100:+0150//mouse:click:DOUBLECLICK//
