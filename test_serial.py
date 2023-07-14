import serial
import time

# Create a serial object.
ser = serial.Serial("/dev/ttyACM0", 9600)

# Allow some time for the Arduino to reset.
time.sleep(2)

# List of waste types you want to send.
waste_types = ["infectiousWaste", "plasticWaste", "paperWaste"]

for waste in waste_types:
    # Send the command to the Arduino.
    ser.write(waste.encode())
    print(f"Sent command: {waste}")

    # Wait for the Arduino to finish and send a response.
    while True:
        if ser.inWaiting():
            response = ser.readline().decode().strip()
            print(f"Received response: {response}")
            break

    # Wait for a bit before sending the next command.
    time.sleep(2)

# Close the serial connection.
ser.close()
