import cv2
import numpy as np

cap = cv2.VideoCapture(0)

# Get the base frame
_, base_frame = cap.read()
base_gray = cv2.cvtColor(base_frame, cv2.COLOR_BGR2GRAY)

# Calculate the histogram for the base frame
base_hist = cv2.calcHist([base_gray], [0], None, [256], [0, 256])

current_diff = None

while True:
    # Capture frame-by-frame
    _, frame = cap.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Calculate the histogram for the current frame
    hist = cv2.calcHist([gray], [0], None, [256], [0, 256])

    # Calculate the absolute difference between the base histogram and the current histogram
    hist_diff = cv2.absdiff(base_hist, hist)

    # Sum up all the values in the histogram difference
    total_diff = np.sum(hist_diff)

    if current_diff is None:
        current_diff = total_diff

    percent_diff = abs((total_diff - current_diff) / total_diff * 100)

    current_diff = total_diff

    # Display the resulting frame
    cv2.imshow("Frame", frame)

    # Quit the program when 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
