#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setbuf(stdout, nullptr);
    this->filename = string("../video_street.avi");
    ui->TextStatus->setStyleSheet("QLabel { color : green; }");
    std::string info = "File exists\nPATH: " + this->filename;
    ui->TextStatus->setText(QString::fromStdString(info));
    ui->playPauseBtn->setEnabled(false);
    ui->saveBtn->setEnabled(false);
    ui->nextBtn->setEnabled(false);
    ui->stopBtn->setEnabled(false);

    this->SET_WATCH_EVERY_FRAME = true;
    this->STOP_BUTTON_WAS_PRESSED = false;
    this->NEXT_BUTTON_WAS_PRESSED = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_convertBtn_clicked()
{
    std::cout << "hello world\n";

    this->video = new Video(this->filename);
    std::cout << this->video->get_path();

    if(!this->video->get_capture()->isOpened())
    {
        std::cout << "Cannot open video!\n";
        return;
    }
    std::cout << "Nah, das ist gut!\n";

    this->hdrCap = new HdrCap();

    ui->convertBtn->setEnabled(false);
    ui->playPauseBtn->setEnabled(true);
    ui->nextBtn->setEnabled(true);
    ui->stopBtn->setEnabled(true);

    int pressed_key;
    Mat frame, prev_frame;
    for(;;)
    {
        *(this->video->get_capture()) >> frame;
        if(frame.empty() || (pressed_key = waitKey(30)) == 'q' || this->STOP_BUTTON_WAS_PRESSED) {
            this->STOP_BUTTON_WAS_PRESSED = false;
            break;
        }

        if(pressed_key == 'm') {
            this->SET_WATCH_EVERY_FRAME = true;
            ui->playPauseBtn->setText("Play");
        }


        imshow("frame", frame);
        if (!(prev_frame.empty())) {
            imshow("prev_frame", prev_frame);
            //auto start = chrono::steady_clock::now();
            vector<Mat> images = this->hdrCap->align_frames(prev_frame, frame);
            //imshow("prev_frame_aligned", images[0]);
            //imshow("frame_aligned", images[1]);

            std::vector<Mat> labeledMap;
            int nLabels = this->hdrCap->deghost_frames(images[0], images[1], labeledMap);
            //imshow("Bitmap threshold", labeledMap[1]);

            //Mat hdr_frame = hdrCap->merge_frames(images[0], images[1]);
            //imshow("hdr_frame", hdr_frame);

            Mat exposure_fusion = this->hdrCap->exposure_fusion(images[0], images[1], nLabels, labeledMap[0]);
            //auto end = chrono::steady_clock::now();
            imshow("hdr_exposure_fusion", exposure_fusion);
            //cout << "Time of exposure fusion: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms"
            //     << endl;
        }


        if(this->SET_WATCH_EVERY_FRAME)
            for(;;) {
                int key = waitKey(30);
                if(key == 'm' || this->SET_WATCH_EVERY_FRAME == false) {
                    this->SET_WATCH_EVERY_FRAME = false;
                    ui->playPauseBtn->setText("Pause");
                    break;
                }
                if(key == 'n' || this->NEXT_BUTTON_WAS_PRESSED) {
                    this->NEXT_BUTTON_WAS_PRESSED = false;
                    break;
                }
                if(key == 'q' || this->STOP_BUTTON_WAS_PRESSED)
                    break;
            }

        frame.copyTo(prev_frame);
    }

    this->video->get_capture()->release();
    cvDestroyAllWindows();
    delete this->video;
    delete this->hdrCap;

    ui->convertBtn->setEnabled(true);
    ui->playPauseBtn->setEnabled(false);
    ui->nextBtn->setEnabled(false);
    ui->saveBtn->setEnabled(false);
    ui->stopBtn->setEnabled(false);
}

void MainWindow::on_playPauseBtn_clicked()
{
    if(this->SET_WATCH_EVERY_FRAME) {
        this->SET_WATCH_EVERY_FRAME = false;
        ui->playPauseBtn->setText("Pause");
    } else {
        this->SET_WATCH_EVERY_FRAME = true;
        ui->playPauseBtn->setText("Play");
    }
}

void MainWindow::on_nextBtn_clicked()
{
    this->NEXT_BUTTON_WAS_PRESSED = true;
}

void MainWindow::on_stopBtn_clicked()
{
    this->STOP_BUTTON_WAS_PRESSED = true;
    this->SET_WATCH_EVERY_FRAME = true;
    ui->playPauseBtn->setText("Play");
}
