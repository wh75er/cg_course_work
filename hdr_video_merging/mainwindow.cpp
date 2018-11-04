#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setbuf(stdout, nullptr);
    this->filename = new string("../video.avi");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->filename;
}

void MainWindow::on_convertBtn_clicked()
{
    std::cout << "hello world\n";

    Video* video = new Video(this->filename);
    std::cout << video->get_path();

    if(!video->get_capture()->isOpened())
    {
        std::cout << "Cannot open video!\n";
        return;
    }
    std::cout << "Nah, das ist gut!\n";

    Mat edges;
    for(;;)
    {
        Mat frame;
        *(video->get_capture()) >> frame;
        if(frame.empty() || waitKey(30) == 'q')
            break;
        cvtColor(frame, edges, CV_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);
        imshow("edges", edges);
    }

    video->get_capture()->release();
    cvDestroyWindow("edges");
    delete video;
}
