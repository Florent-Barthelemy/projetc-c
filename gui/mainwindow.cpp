#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(ACQUISITION_HANDLER& acq, STIMULI_HANDLER& stim, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->waveView->setScene(&waveScene);
    ui->waveView->centerOn(this->waveScene.sceneRect().center());
    clearWindow();

    std::map<std::string, std::vector<TIMED_SIG>> rawAcq;
    acq.saveAcquisition(rawAcq);

    for(auto stimIT = stim.stimulis.begin(); stimIT != stim.stimulis.end(); ++stimIT)
    {
        displayWave(stimIT->second.second, QString(stimIT->first.c_str()), 4);
    }

    for(auto acqIT = rawAcq.begin(); acqIT != rawAcq.end(); ++acqIT)
    {
        displayWave(acqIT->second, QString(acqIT->first.c_str()));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::displayWave(std::vector<TIMED_SIG> sigs, QString name, long toTimestamp)
{
    ui->waveList->addItem(name);

    long lengthOff = 0;
    long timestamp = 0;
    long prevHeight;
    long newHeight = WAVE_L_X;
    QPen* pen = &pred;

    for(auto stateIT = sigs.begin(); stateIT != sigs.end(); ++stateIT)
    {
        prevHeight = newHeight;

        while(timestamp < stateIT->timestamp)
        {
            waveScene.addLine(QLine(WAVE_LEFT_OFFSET + lengthOff, waveOff + prevHeight, WAVE_LEFT_OFFSET + lineLength + lengthOff, waveOff + prevHeight), *pen);
            lengthOff += lineLength;
            timestamp++;
        }

        switch (stateIT->state) {
        case L:
            pen = &pgreen;
            newHeight = WAVE_L_L;
            break;

        case H:
            pen = &pgreen;
            newHeight = WAVE_L_H;
            break;

        case Z:
            pen = &pblue;
            newHeight = WAVE_L_X;
            break;

        default:
            pen = &pred;
            newHeight = WAVE_L_X;
            break;
        }

        waveScene.addLine(QLine(WAVE_LEFT_OFFSET + lengthOff, waveOff + prevHeight, WAVE_LEFT_OFFSET + lengthOff, waveOff + newHeight), *pen); //interpolation
        waveScene.addLine(QLine(WAVE_LEFT_OFFSET + lengthOff, waveOff + newHeight, WAVE_LEFT_OFFSET + lineLength + lengthOff, waveOff + newHeight), *pen);
        lengthOff += lineLength;
        timestamp++;
    }

    while(timestamp++ < toTimestamp)
        waveScene.addLine(QLine(WAVE_LEFT_OFFSET + lengthOff, waveOff + newHeight, WAVE_LEFT_OFFSET + lineLength + lengthOff, waveOff + newHeight), *pen);

    waveOff += WAVE_MARGIN;
}

void MainWindow::clearWindow()
{
    waveScene.clear();
    waveScene.setSceneRect(ui->waveView->rect() - QMargins(0, 0, 20, 20));
    waveScene.setBackgroundBrush(bblack);
}
