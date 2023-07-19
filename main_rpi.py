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
ser = serial.Serial("/dev/ttyACM0", 9600)
# ser = serial.Serial("COM5", 9600)

# Allow some time for the Arduino to reset.
time.sleep(2)

isDoneTilting = True
num_images = 0
last_detection_time = time.time()
categories = []
detect_counter = 0
temp_frame = None

ser.write("start".encode())

while True:
    # Capture frame-by-frame.
    ret, frame = cap.read()

    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break

    current_time = time.time()

    
    if current_time - last_detection_time >= 5: # Check if 5 seconds have passed

        last_detection_time = current_time

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
                temp_frame = None

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
                    categories.append("plasticWaste")
                elif label in ["Facemask", "Glove"]:
                    categories.append("infectiousWaste")
                else:
                    categories.append("paperWaste")

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
                
##            temp_frame = frame.copy()

##    if temp_frame is None:
##        cv2.imshow("frame", frame)
##    else:
##        cv2.imshow("frame", temp_frame)

    cv2.imshow("frame", frame)

        
    # Remove the duplicates.
    categories = list(set(categories))

    # Send category if only one category is present.
    if len(categories) == 1 and isDoneTilting:
        detect_counter += 1
        print(detect_counter)

        if detect_counter > 2:
            detect_counter = 0
            ser.write(categories[0].encode())
            print(f"Sent command: {categories[0]}")

            isDoneTilting = False
            print("Started tilting. Please wait.")
    elif len(categories) > 1:
        print("Detected 2 or more categories.")
        ser.write("invalid".encode()) # Reset detect counter if nothing is detected:
    
    if len(categories) > 0:
        time.sleep(2)
   
    if cv2.waitKey(1) == ord("q"):
        break

    categories = []

# When everything done, release the capture
cap.release()

# Close the serial connection.
ser.close()
