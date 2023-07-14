import sys
import os

from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.uic import loadUi

from object_detection import ObjectDetection

IN_RPI = False
try:
    import RPi.GPIO

    IN_RPI = True
except ImportError as e:
    print("Not in Raspberry Pi.")


class MainWindow(QtWidgets.QMainWindow):

    """
    Class for GUI related controls in mainwindow.ui.
    """

    def __init__(self):
        super(MainWindow, self).__init__()
        dir_path = os.path.dirname(os.path.realpath(__file__))
        loadUi(os.path.join(dir_path, "mainwindow.ui"), self)

        self.capture_button.clicked.connect(self.capture_button_clicked)
        self.reset_button.clicked.connect(self.reset_button_clicked)
        self.applyFilterCheckbox.stateChanged.connect(self.filter_state_changed)

        self.object_detection = ObjectDetection(
            config_path=os.path.join(dir_path, "./yolov4-tiny-custom.cfg"),
            weights_path=os.path.join(
                dir_path, "./training/yolov4-tiny-custom_best.weights"
            ),
            classes=["1 pesos", "5 pesos", "10 pesos", "20 peso"],
        )

        self.is_captured = False

        if IN_RPI:
            self.showFullScreen()

        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.update_frames)
        self.timer.start(1)

    def capture_button_clicked(self):
        """
        Stops the timer when the capture button is clicked
        then it sets to run the detection from object_detection
        object and manually update the frames with the detected objects.

        Only run detector if the current view is not canny filtered.
        """

        if not self.is_captured and not self.object_detection.is_filter_applied:
            self.is_captured = True
            self.timer.stop()
            self.update_frames()

    def reset_button_clicked(self):
        """
        Starts the timer when the reset button is clicked then
        sets the object_detection object to stop running the detection.
        """

        if self.is_captured:
            self.is_captured = False
            self.timer.start(1)

            self.one_pesos_textbox.setText("-")
            self.five_pesos_textbox.setText("-")
            self.ten_pesos_textbox.setText("-")
            self.twenty_pesos_textbox.setText("-")

    def filter_state_changed(self):
        """
        Called whenever the state of applyFilterCheckbox changed to
        enable or disable the filter in the object_detection object.
        """
        self.object_detection.is_filter_applied = (
            not self.object_detection.is_filter_applied
        )

    def update_frames(self):
        """
        Timer that continuously get frames from the camera.
        """
        image = self.object_detection.get_frames()

        if self.is_captured:
            (
                image,
                num_one_pesos,
                num_five_pesos,
                num_ten_pesos,
                num_twenty_pesos,
            ) = self.object_detection.detect(image)

            self.one_pesos_textbox.setText(str(num_one_pesos))
            self.five_pesos_textbox.setText(str(num_five_pesos))
            self.ten_pesos_textbox.setText(str(num_ten_pesos))
            self.twenty_pesos_textbox.setText(str(num_twenty_pesos))

        # If there is only 2 items in shape, it means the
        # image is one channel.
        if len(image.shape) == 2:
            image_format = QtGui.QImage.Format_Indexed8
        # Else, it may be 3 or 4.
        else:
            # Get third item which is the number of channels.
            num_channels = image.shape[2]
            if num_channels == 1:
                image_format = QtGui.QImage.Format_Indexed8
            elif num_channels == 3:
                image_format = QtGui.QImage.Format_BGR888
            elif num_channels == 4:
                image_format = QtGui.QImage.Format_RGBA8888

        out_image = QtGui.QImage(
            image,
            image.shape[1],
            image.shape[0],
            image.strides[0],
            image_format,
        )

        self.image_label.setPixmap(QtGui.QPixmap.fromImage(out_image))
        self.image_label.setScaledContents(True)


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    w = MainWindow()
    w.show()
    app.exec_()
