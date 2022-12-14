#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <vector>
#include "../TOP/TOP.h"

#define WAVE_TOP_OFFSET 0
#define WAVE_LEFT_OFFSET 0

#define WAVE_MARGIN 30

#define WAVE_L_L 10
#define WAVE_L_H 0
#define WAVE_L_X 5

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(ACQUISITION_HANDLER& acq, STIMULI_HANDLER& stim, QWidget *parent = nullptr);
    ~MainWindow();
    void displayWave(std::vector<TIMED_SIG> sigs, QString name, long toTimestamp = 0);
    void clearWindow();
    void resizeEvent(QResizeEvent* event);
    void updateStimulis();

private:
    Ui::MainWindow *ui;
    QGraphicsScene waveScene;
    ACQUISITION_HANDLER& acq;
    STIMULI_HANDLER& stim;
    long lineLength = 100;
    long waveOff = WAVE_TOP_OFFSET;

    QBrush bred = QBrush({255, 0, 0}, Qt::SolidPattern);
    QBrush bgreen = QBrush({0, 255, 0}, Qt::SolidPattern);
    QBrush bblue = QBrush({0, 0, 255}, Qt::SolidPattern);
    QBrush bblack = QBrush({0, 0, 0}, Qt::SolidPattern);

    QPen pred = QPen(bred, 1);
    QPen pgreen = QPen(bgreen, 1);
    QPen pblue = QPen(bblue, 1);
};
#endif // MAINWINDOW_H
