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
   # while True:
        # Read a line (ends with \n or timeout)
       # message = "mouse:move:+0100:+0100"
   #     dataVerified="OK"
        openSerialPort.write(message.encode('utf-8'))
        time.sleep(0.5)  # 0.075 ñ
    #    line = openSerialPort.readline()  #read Answer Echo Command   
    #    if line:  # If data received
            # Decode bytes to string (remove trailing whitespace/newline)
        #decoded_line = line.decode('utf-8').strip()
            #print(f"Received: {decoded_line}")
            #if(decoded_line=="DataEcho:"+message):
            #    openSerialPort.write(dataVerified.encode('utf-8'))
            #    time.sleep(1)  # 0.075 ñ       
            # Write a response line back
            #response = f"Echo: {decoded_line}\r\n"
            #ser.write(response.encode('utf-8'))
            #    print(f"Sent: {dataVerified}")

#structure: mouse:move:+-x[4]:+-y[4] // mouse click [command]//example//mouse:move:+0100:+0150//mouse:click:DOUBLECLICK//
