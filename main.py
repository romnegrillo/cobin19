import os
import sys
import time
import serial
import cv2
import numpy as np
from object_detection import ObjectDetection

dir_path = os.path.dirname(os.path.realpath(__file__))

# YOLO paths.
config_path = os.path.join(dir_path, "./yolov4-tiny-custom.cfg")
weights_path = os.path.join(dir_path, "./training/yolov4-tiny-custom_best.weights")
classes = ["Bottle", "Facemask", "Fork", "Glove", "Paper", "Spoon"]

# YOLO objects.
yolo = cv2.dnn.readNet(weights_path, config_path)
layer_names = yolo.getLayerNames()

# Prepare layers.
output_layers = [layer_names[i - 1] for i in yolo.getUnconnectedOutLayers()]

# OpenCV camera loop.
cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("Cannot open camera")
    sys.exit(1)


# Create a serial object.
# ser = serial.Serial("/dev/ttyAMA0", 9600)
ser = serial.Serial("COM8", 9600)

# Allow some time for the Arduino to reset.
time.sleep(2)

isDoneTilting = True

while True:
    # Capture frame-by-frame.
    ret, frame = cap.read()

    # Replace by an image for testing.
    frame = cv2.imread("./objv3/Facemask (1).jpg")

    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break

    # Display the resulting frame.
    height, width, _ = frame.shape
    blob = cv2.dnn.blobFromImage(frame, 1 / 255.0, (416, 416), swapRB=True, crop=False)
    yolo.setInput(blob)
    outputs = yolo.forward(output_layers)

    class_ids = []
    confidences = []
    boxes = []

    for output in outputs:
        for detection in output:
            scores = detection[5:]
            class_id = np.argmax(scores)
            confidence = scores[class_id]

            center_x = int(detection[0] * width)
            center_y = int(detection[1] * height)
            w = int(detection[2] * width)
            h = int(detection[3] * height)
            x = int(center_x - w / 2)
            y = int(center_y - h / 2)

            boxes.append([x, y, w, h])
            confidences.append(float(confidence))
            class_ids.append(class_id)

    indices = cv2.dnn.NMSBoxes(boxes, confidences, 0.1, 0.1)
    num_images = 0
    category = None

    # Wait for the Arduino to finish and send a response
    # only if it is currenly in tilt.
    if ser.inWaiting() and not isDoneTilting:
        response = ser.readline().decode().strip()
        print(f"Received response: {response}")

        if response == "tiltDone":
            print("Done tilting.")
            isDoneTilting = True

    # Only try to detect if it is done tilting.
    if not isDoneTilting:
        continue

    for index, _ in enumerate(boxes):
        if index in indices:
            x, y, w, h = boxes[index]

            label = str(classes[class_ids[index]])
            confidence = round(confidences[index], 2)

            # # Only include items passing 0.70 confidence level.
            # if confidence < 0.70:
            #     continue

            if label == classes[0]:  # Bottle
                mask_color = (0, 0, 255)
            elif label == classes[1]:  # Facemask
                mask_color = (0, 255, 0)
            elif label == classes[2]:  # Fork
                mask_color = (255, 0, 0)
            elif label == classes[3]:  # Gloves
                mask_color = (255, 255, 0)
            elif label == classes[3]:  # Paper
                mask_color = (0, 255, 255)
            else:  # Spoon
                mask_color = (255, 0, 255)

            if label in ["Bottle", "Spoon", "Fork"]:
                category = "plasticWaste"
            elif label in ["Facemask", "Glove"]:
                category = "infectiousWaste"
            else:
                category = "paperWaste"

            cv2.rectangle(frame, (x, y), (x + w, y + h), mask_color, 1)
            cv2.rectangle(frame, (x, y - 30), (x + w, y), mask_color, -1)
            cv2.putText(
                frame,
                f"{label} - {confidence}",
                (x, y - 10),
                cv2.FONT_HERSHEY_PLAIN,
                1,
                (0, 0, 0),
                1,
            )

            num_images += 1

            # print(f"Category: {category}")
            # print(f"Confidence level: {confidence}")
            # print(f"Number of images: {num_images}")

    cv2.imshow("frame", frame)

    # Send category if only one image is present
    # and it is done tilting.
    if num_images == 1 and isDoneTilting:
        ser.write(category.encode())
        print(f"Sent command: {category}")

        isDoneTilting = False
        print("Started tilting. Please wait.")

    if cv2.waitKey(1) == ord("q"):
        break

# When everything done, release the capture
cap.release()

# Close the serial connection.
# ser.close()
