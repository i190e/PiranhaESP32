import serial
import time
import serial.tools.list_ports


ports = list(serial.tools.list_ports.comports())
for port in ports:
    print(f"Port: {port.device}, Description: {port.description}")

# Configure serial connection
ser = serial.Serial(
    port='COM3',           # Replace with your port (e.g., '/dev/ttyUSB0' on Linux)
    baudrate=115200,       # Common baud rate
    timeout=1,             # Read timeout in seconds
)
delay_ms = 700

#structure: mouse:move:+-x[4]:+-y[4] // mouse click [command]//example//mouse:move:+0100:+0150//mouse:click:DOUBLECLICK//

try:
    print("Serial port opened:", ser.name)

    while True:
        # Read a line (ends with \n or timeout)
        message = "mouse:move:+0100:+0100"
        dataVerified="OK"
        ser.write(message.encode('utf-8'))
        time.sleep(0.5)  # 0.075 ñ
        line = ser.readline()     
        if line:  # If data received
            # Decode bytes to string (remove trailing whitespace/newline)
            decoded_line = line.decode('utf-8').strip()
            print(f"Received: {decoded_line}")
            if(decoded_line=="DataEcho:"+message):
                ser.write(dataVerified.encode('utf-8'))
                time.sleep(15)  # 0.075 ñ       
            # Write a response line back
            #response = f"Echo: {decoded_line}\r\n"
            #ser.write(response.encode('utf-8'))
            print(f"Sent: {dataVerified}")
        
except KeyboardInterrupt:
    print("Exiting...")
except Exception as e:
    print(f"Error: {e}")
finally:
    ser.close()
    print("Serial port closed.")