#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraInfo>
#include <QMediaRecorder>
#include <QMediaPlayer>
#include <QCameraImageCapture>

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setbuf(stdout, nullptr);

    mCamera = new QCamera(this);
    mCameraViewfinder = new QCameraViewfinder(this);
    mLayout = new QVBoxLayout;

    mLayout->addWidget(mCameraViewfinder);
    mCamera->setViewfinder(mCameraViewfinder);
    ui->scrollArea->setLayout(mLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_recordBtn_clicked()
{
    std::cout << "hello world\n";
    if(ui->recordBtn->text() != "stop") {
        ui->recordBtn->setText("stop");
        mCamera->start();
    }
    else {
        ui->recordBtn->setText("record");
        mCamera->stop();
    }
}
