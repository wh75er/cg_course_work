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
    string* filename;

private slots:
    void on_convertBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
