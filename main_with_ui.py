import sys
import os
from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.uic import loadUi
from object_detection import ObjectDetection


class MainWindow(QtWidgets.QMainWindow):

    """
    Class for GUI related controls in mainwindow.ui.
    """

    def __init__(self):
        super(MainWindow, self).__init__()
        dir_path = os.path.dirname(os.path.realpath(__file__))
        loadUi(os.path.join(dir_path, "mainwindow.ui"), self)

        self.object_detection = ObjectDetection(
            config_path=os.path.join(dir_path, "./yolov4-tiny-custom.cfg"),
            weights_path=os.path.join(
                dir_path, "./training/yolov4-tiny-custom_best.weights"
            ),
            classes=["Bottle", "Facemask", "Fork", "Gloves", "Paper", "Spoon"],
        )

        self.is_captured = True

        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.update_frames)
        self.timer.start(1)
        print("DEBUG")

    def update_frames(self):
        """
        Timer that continuously get frames from the camera.
        """
        image = self.object_detection.get_frames()

        if self.is_captured:
            pass
            # (
            #     image,
            #     num_one_pesos,
            #     num_five_pesos,
            #     num_ten_pesos,
            #     num_twenty_pesos,
            # ) = self.object_detection.detect(image)

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
