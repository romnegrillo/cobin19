import cv2
import os
import re


def extract_number(filename):
    match = re.search(r"\((\d+)\)", filename)
    return int(match.group(1)) if match else 0


def get_next_filename(directory=".", prefix="Name", extension=".jpg"):
    files = os.listdir(directory)
    files = [f for f in files if f.startswith(prefix) and f.endswith(extension)]
    files.sort(key=extract_number)

    if not files:
        return os.path.join(directory, f"{prefix} (1){extension}")
    else:
        last_file = files[-1]
        number = extract_number(last_file) + 1
        return os.path.join(directory, f"{prefix} ({number}){extension}")


cap = cv2.VideoCapture(1)

while True:
    ret, frame = cap.read()

    cv2.imshow("Capture", frame)

    if cv2.waitKey(1) & 0xFF == ord("\r"):  # Enter key
        filename = get_next_filename("./captured_images", prefix="Paper")
        print(filename)
        cv2.imwrite(filename, frame)
        print(f"Saved image: {filename}")

    if cv2.waitKey(1) & 0xFF == ord("q"):  # Quit with 'q' key
        break

cap.release()
cv2.destroyAllWindows()
