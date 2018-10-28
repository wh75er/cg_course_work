#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QtMultimedia/QCamera>
#include <QVBoxLayout>
#include <QUrl>

#include <iostream>

class QCamera;
class QCameraViewfinder;
class QCameraImageCapture;
class QVBoxLayout;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_recordBtn_clicked();

private:
    Ui::MainWindow *ui;
    QCamera *mCamera;
    QCameraViewfinder *mCameraViewfinder;
    QCameraImageCapture *mCameraImageCapture;
    QVBoxLayout *mLayout;
};

#endif // MAINWINDOW_H
