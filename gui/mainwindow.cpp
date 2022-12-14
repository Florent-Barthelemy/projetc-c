#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(ACQUISITION_HANDLER& acq, STIMULI_HANDLER& stim, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , acq(acq)
    , stim(stim)
{
    ui->setupUi(this);

    ui->waveView->setScene(&waveScene);

    updateStimulis();
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
    {
        waveScene.addLine(QLine(WAVE_LEFT_OFFSET + lengthOff, waveOff + newHeight, WAVE_LEFT_OFFSET + lineLength + lengthOff, waveOff + newHeight), *pen);
        lengthOff += lineLength;
    }

    waveOff += WAVE_MARGIN;
}

void MainWindow::clearWindow()
{
    waveScene.clear();
    waveScene.setSceneRect(ui->waveView->rect() - QMargins(0, 0, 20, 20));
    waveScene.setBackgroundBrush(bblack);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    updateStimulis();
}

void MainWindow::updateStimulis()
{
    std::map<std::string, std::vector<TIMED_SIG>> rawAcq;
    acq.saveAcquisition(rawAcq);

    long maxTimestamp = 1;

    for(auto stimIT = stim.getStimulis().begin(); stimIT != stim.getStimulis().end(); ++stimIT)
        if(maxTimestamp < stimIT->second.second.back().timestamp)
            maxTimestamp = stimIT->second.second.back().timestamp;

    for(auto acqIT = rawAcq.begin(); acqIT != rawAcq.end(); ++acqIT)
        if(maxTimestamp < acqIT->second.back().timestamp)
            maxTimestamp = acqIT->second.back().timestamp;

    QRect dims = ui->waveView->geometry();
    dims.setHeight(this->height() - 10);
    dims.setWidth(this->width() - 181);

    this->lineLength = dims.width() / (maxTimestamp + 1);

    ui->waveView->setGeometry(dims);

    ui->waveView->centerOn(this->waveScene.sceneRect().center());
    clearWindow();

    ui->waveList->clear();
    waveOff = WAVE_TOP_OFFSET;

    for(auto stimIT = stim.getStimulis().begin(); stimIT != stim.getStimulis().end(); ++stimIT)
    {
        displayWave(stimIT->second.second, QString(stimIT->first.c_str()), maxTimestamp + 1);
    }

    for(auto acqIT = rawAcq.begin(); acqIT != rawAcq.end(); ++acqIT)
    {
        displayWave(acqIT->second, QString(acqIT->first.c_str()), maxTimestamp + 1);
    }
}
