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

    bool SET_WATCH_EVERY_FRAME = true;
    int pressed_key;
    for(;;)
    {
        Mat frame;
        *(video->get_capture()) >> frame;
        if(frame.empty() || (pressed_key = waitKey(30)) == 'q')
            break;
        if(pressed_key == 'm')
            SET_WATCH_EVERY_FRAME = true;
        imshow("frame", frame);
        if(SET_WATCH_EVERY_FRAME)
            for(;;) {
                int key = waitKey(30);
                if(key == 'm') {
                    SET_WATCH_EVERY_FRAME = false;
                    break;
                }
                if(key == 'n')
                    break;
            }
    }

    video->get_capture()->release();
    cvDestroyWindow("frame");
    delete video;
}
