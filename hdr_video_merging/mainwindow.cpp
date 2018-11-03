#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setbuf(stdout, nullptr);
    string* filename = new string("video.avi");
    this->video = new Video(filename);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete video;
    delete filename;
}

void MainWindow::on_convertBtn_clicked()
{
    std::cout << "hello world\n";
    std::cout << this->video->get_path();
}
