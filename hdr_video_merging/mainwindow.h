#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "libs.h"
#include "video.h"
#include "hdr.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    string filename;

private slots:
    void on_convertBtn_clicked();

    void on_playPauseBtn_clicked();

    void on_nextBtn_clicked();

    void on_stopBtn_clicked();

    void on_actionOpen_triggered();

    void on_moveCheckBox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    Video* video;
    HdrCap* hdrCap;

    bool SET_WATCH_EVERY_FRAME;
    bool NEXT_BUTTON_WAS_PRESSED;
    bool STOP_BUTTON_WAS_PRESSED;
    bool SHOW_MOVEMENT_MAP;
};

#endif // MAINWINDOW_H
