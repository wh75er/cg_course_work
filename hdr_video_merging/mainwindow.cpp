#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setbuf(stdout, nullptr);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_recordBtn_clicked()
{
    std::cout << "hello world\n";
}
