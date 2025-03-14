import random
import os

from PIL import Image

from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

class Widget(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Лабораторная работа #4")

        path_ = os.path.dirname(__file__)
        filename = "_сarib_TC"
        ext = 'bmp'

        pictures = ["CAT16.BMP","CAT256.BMP","_сarib_TC.bmp"]
        self.pictures_paths = [f"{path_}//{pic}" for pic in pictures]

        self.img = QImage(self.pictures_paths[0])
        self.img_width = self.img.size().width()
        self.img_height = self.img.size().height()

        self.resize(self.img_width, self.img_height)

        self.pixel_list = [(y, x) for y in range(self.img_height) for x in range(self.img_width)]



        self.new_img = QImage(self.img_width, self.img_height, QImage.Format_RGB32)

        self.timer = QTimer()
        self.timer.setInterval(1)
        self.timer.timeout.connect(self._draw_pixel)
        self.timer.start()

    def keyPressEvent(self, event):
        if event.key() == Qt.Key_Space:
            self.pictures_paths = self.pictures_paths[1:] + self.pictures_paths[:1]
            self.img = QImage(self.pictures_paths[1])
            self.img_width = self.img.size().width()
            self.img_height = self.img.size().height()

            self.pixel_list = [(y, x) for y in range(self.img_height) for x in range(self.img_width)]



            self.new_img = QImage(self.img_width, self.img_height, QImage.Format_RGB32)

            self.resize(self.img_width, self.img_height)
            self.center()
            self.timer.start()

        event.accept()

    def center(self):
        qr = self.frameGeometry()
        cp = QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)
        self.move(qr.topLeft())

    def _draw_pixel(self):
        for _ in range(1000):
            if not self.pixel_list:
                self.timer.stop()
                return

            y, x = self.pixel_list.pop()
            pixel = self.img.pixel(x, y)

            self.new_img.setPixel(x, y, pixel)

            self.update()

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.drawImage(0, 0, self.new_img)


if __name__ == '__main__':
    app = QApplication([])

    w = Widget()
    w.show()

app.exec()