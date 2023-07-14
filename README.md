# Philippine Coins Object Detection

This project aims to detect and count the number of new Philippine coins (1, 5, 10, and 20 pesos) using computer vision techniques. A desktop application was created using Python and PyQt5, and it was ported onto a Raspberry Pi 4. The detection and counting process is based on the Darknet YOLO tiny v4 model, implemented using OpenCV.

## Languages and Tools Used
The project was developed using the following languages and tools:
- Python
- OpenCV
- Darknet YOLO tiny v4
- PyQt5
- Raspberry Pi 4

To get started with this project, please see the installation and usage instructions below.

![Sample Detection](sample_detection.png)
![Sample GUI](sample_gui.png)

## Setup

### Windows
- Python 3.8 above and pip must be installed in your computer.
- Clone this repository, open the terminal, change directory to the cloned repository and follow the commands below.
- Install virtualenv package: ```pip install virtualenv```
- Install Qt5 or QtDesigner (optional) if you want to edit the GUI.
- Create a virtual environment. Lets name it my-venv: ```virtualenv my-venv```
- Activate virtual environment: ```my-venv/Scripts/activate```
- Install the required packages: ```pip install -r requirements.txt```
- Run the main program: ```python main.py```

### Raspberry Pi/Linux
- Python 3.8 above and pip must be installed in your computer.
- Clone this repository, open the terminal, change directory to the cloned repository and follow the commands below.
- Install virtualenv package: ```pip install virtualenv```
- Install Qt5 required programs: ```sudo apt install qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools```
- Create a virtual environment. Lets name it my-venv: ```virtualenv my-venv```
- Activate virtual environment: ```source my-venv/bin/activate```
- Install the required packages: ```pip install -r requirements_raspberry_pi.txt```
- Run the main program: ```python main.py```


The application is designed to utilize the built-in webcam of your computer or Raspberry Pi, and is also capable of functioning with a Pi Camera. Note that the training images used has been captured with Raspberry Pi camera only with very little variations, so using other camera might have an effect on its detection accuracy.
