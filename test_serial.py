import serial
import time

# Create a serial object.
ser = serial.Serial("COM4", 9600)

# Allow some time for the Arduino to reset.
time.sleep(2)

command = "plasticWaste"

ser.write(command.encode())
print(f"Sent command: {command}")

while True:
    if ser.inWaiting():
        response = ser.readline().decode().strip()
        print(f"Received response: {response}")

        if response == "tiltDone":
            break

    time.sleep(1)

# Wait for a bit before sending the next command.
time.sleep(2)

# Close the serial connection.
ser.close()
