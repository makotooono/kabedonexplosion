#include "mainwindow.h"
#include <QDebug>

BOOL CALLBACK myRecordingCallBack(HRECORD handle, const void *buffer,DWORD length,void *user)
{
    MainWindow *w = (MainWindow *)user;
    QVector<float> wave(100);
    BASS_ChannelGetData(w->record, wave.data(), wave.size() * 4 | BASS_DATA_FLOAT);

    float maxv = 0;
    foreach(float d, wave)
    {
        if(qAbs(d) > maxv)
            maxv = qAbs(d);
    }
    w->bar.setValue(maxv*w->bar.maximum());
    qDebug() << maxv;
    return true;

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      oldval(0),
      filepath("/Users/oonomakoto/coin.wav")
{

    record = BASS_RecordStart(16000, 1, MAKELONG(BASS_SAMPLE_FLOAT,10), &myRecordingCallBack, this);
    if(!record) qDebug() << BASS_ErrorGetCode();


    setupUI();

}

MainWindow::~MainWindow()
{
    BASS_RecordFree();
}



void MainWindow::refreshWidget(float level)
{
    int val = level*bar.maximum();
    bar.setValue(val);
    if(val > borderSlider.value() && val > oldval)
    {
        HSTREAM soundeffect = BASS_StreamCreateFile(false,filepath.toLocal8Bit().data(),0,0,BASS_STREAM_AUTOFREE);
        BASS_ChannelPlay(soundeffect,true);
    }
    oldval = val-1000;
}


void MainWindow::setupUI()
{
    borderSlider.setRange(0,100000);
    borderSlider.setValue(100000);
    borderSlider.setOrientation(Qt::Horizontal);
    bar.setRange(0,100000);
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(&bar);
    vlay->addWidget(&borderSlider);
    vlay->addWidget(&openButton);
    vlay->addWidget(&fileLabel);
    fileLabel.setText(filepath.split("/").last());
    openButton.setText("select");
    connect(&openButton, SIGNAL(clicked()),SLOT(setFileName()));

    QWidget *w = new QWidget;
    w->setLayout(vlay);
    this->setCentralWidget(w);
}
