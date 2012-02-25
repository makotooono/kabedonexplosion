import sys
import struct
from PySide.QtMultimedia import *
from PySide.QtGui import *
from PySide.QtCore import *


class MainWindow(QMainWindow):
    _old_values = [0,0,0]
    _filename = "coin.wav"
    _range = (0,100000)
    _volumeChanged = Signal(int)
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setWindowTitle("kabedon")
        self._sound = QSound(self._filename)
        self.__setup_ui()
        self._input = self.__create_audio_input()
        self._input_buf = self._input.start()
        self._input.stateChanged.connect(self.__notify)
        self._input_buf.readyRead.connect(self.__process)
        self._volumeChanged.connect(self._bar.setValue)

    @Slot()
    def __notify(self):
        print self._input.state()
        if self._input.state() is QAudio.IdleState:
            self._input.reset()
            self._input.start()

    @Slot()
    def __process(self):
        byte = self._input.bytesReady()
        if not byte:
            return
        wave_form = struct.unpack(
                str(byte / 2) + "h",
                self._input_buf.read(byte))
        abs_form = [abs(v) for v in wave_form]
        max_value = max(abs_form) / 32767.0 * self._slider.maximum()
        self._volumeChanged.emit(max_value)
        check_peak = lambda x, y, z: max(x, y, z) == y
        self._old_values.pop(0)
        self._old_values.append(max_value)
        if (self._old_values[1] > self._slider.value() and
            check_peak(*self._old_values)):
            self._sound.play(self._filename)

    @Slot()
    def __show_file_dialog(self):
        filename = QFileDialog.getOpenFileName(
                filter="audio files (*.wav *.mp3)")[0]
        if filename:
            self._label.setText(filename.split('/')[-1])
            self._filename = filename
            self._sound = QSound(self._filename)

    def __create_audio_input(self):
        format = QAudioFormat()
        format.setSampleSize(16)
        format.setChannels(1)
        format.setSampleRate(44100)
        format.setSampleType(QAudioFormat.SignedInt)
        format.setByteOrder(QAudioFormat.LittleEndian)
        format.setCodec("audio/pcm")
        return QAudioInput(
                QAudioDeviceInfo.defaultInputDevice(),
                format)

    def __setup_ui(self):
        self._bar = QProgressBar()
        self._bar.setRange(*self._range)
        self._slider = QSlider(Qt.Horizontal)
        self._slider.setRange(*self._range)
        self._slider.setValue(self._slider.maximum())
        self._label = QLabel("please select sound file")
        self._button = QPushButton("select")
        self._button.clicked.connect(self.__show_file_dialog)

        lay = QVBoxLayout()
        lay.addWidget(self._bar)
        lay.addWidget(self._slider)
        lay.addWidget(self._label)
        lay.addWidget(self._button)

        w = QWidget()
        w.setLayout(lay)
        self.setCentralWidget(w)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    main = MainWindow()
    main.show()
    app.exec_()
