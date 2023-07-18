import os
import cv2
import numpy as np

TARGET_INPUT_PATH = "objv3"
TARGET_OUTPUT_PATH = "sample_outputs"

dir_path = os.path.dirname(os.path.realpath(__file__))

# List of images in the test directory.
images = [
    os.path.join(TARGET_INPUT_PATH, i)
    for i in os.listdir(TARGET_INPUT_PATH)
    if i.endswith(".jpg")
]


# YOLO paths.
config_path = os.path.join(dir_path, "./yolov4-tiny-custom.cfg")
weights_path = os.path.join(dir_path, "./training/yolov4-tiny-custom_best.weights")
classes = ["Bottle", "Facemask", "Fork", "Glove", "Paper", "Spoon"]

# YOLO objects.
yolo = cv2.dnn.readNet(weights_path, config_path)
layer_names = yolo.getLayerNames()

# Prepare layers.
output_layers = [layer_names[i - 1] for i in yolo.getUnconnectedOutLayers()]


for image in images:
    frame = cv2.imread(image)

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

    for index, _ in enumerate(boxes):
        if index in indices:
            x, y, w, h = boxes[index]

            label = str(classes[class_ids[index]])
            confidence = round(confidences[index], 2)

            # Only include items passing 0.70 confidence level.
            if confidence < 0.70:
                continue

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

    image_name = os.path.basename(image)
    new_path = os.path.join(TARGET_OUTPUT_PATH, image_name)

    print(new_path)

    cv2.imwrite(new_path, frame)
