#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    if(argc < 2)
        return -1;

    STIMULI_HANDLER stimulis;
    WAVEDROM_JSON(jsonWavedrom);

    for(int i = 1; i < argc; i++)
    {
        fstream stimuliFile(argv[i], ios_base::in);
        parseJSON(stimuliFile, jsonWavedrom);
        generateStimuliData(jsonWavedrom, stimulis);
    }

    QApplication a(argc, argv);
    MainWindow w(*new ACQUISITION_HANDLER(), stimulis);
    w.show();
    return a.exec();
}
