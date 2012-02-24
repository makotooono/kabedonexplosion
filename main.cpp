#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    BASS_Init(-1,44100,0,0,0);
    BASS_RecordInit(-1);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
