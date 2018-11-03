#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "libs.h"
#include "video.h"

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
    Video* video;
    string* filename;

private slots:
    void on_convertBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
