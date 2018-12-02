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

    HdrCap* hdrCap = new HdrCap();

    bool SET_WATCH_EVERY_FRAME = true;
    int pressed_key;
    Mat frame, prev_frame;
    for(;;)
    {
        *(video->get_capture()) >> frame;
        if(frame.empty() || (pressed_key = waitKey(30)) == 'q')
            break;

        if(pressed_key == 'm')
            SET_WATCH_EVERY_FRAME = true;


        imshow("frame", frame);
        if (!(prev_frame.empty())) {
            imshow("prev_frame", prev_frame);
            vector<Mat> images = hdrCap->align_frames(prev_frame, frame);
            imshow("prev_frame_aligned", images[0]);
            imshow("frame_aligned", images[1]);
        }


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

        frame.copyTo(prev_frame);
    }

    video->get_capture()->release();
    //cvDestroyWindow("frame");
    //cvDestroyWindow("prev_frame");
    cvDestroyAllWindows();
    delete video;
    delete hdrCap;
}
