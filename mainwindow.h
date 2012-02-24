#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <bass.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void refreshWidget(float level);
    HRECORD record;
        QProgressBar bar;
private slots:
    void setFileName() {
        QString name = QFileDialog::getOpenFileName(0,"select sound file","","Sound files (*.mp3 *.wav)");
        if(!name.isNull())
        {
            filepath = name;
            fileLabel.setText(name.split("/").last());
        }
    }


private:
    QSlider borderSlider;
    QString filepath;
    QLabel fileLabel;
    QPushButton openButton;
    QFileDialog dialog;
    int oldval;

    void setupUI();
};

#endif // MAINWINDOW_H
