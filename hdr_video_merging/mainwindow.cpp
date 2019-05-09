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

    this->hdrVideo = new Video(this->filename);
    hdrVideo->set_frames(30);
    hdrVideo->set_width(1280);
    hdrVideo->set_height(720);

    this->SET_WATCH_EVERY_FRAME = true;
    this->STOP_BUTTON_WAS_PRESSED = false;
    this->NEXT_BUTTON_WAS_PRESSED = false;
    this->SHOW_MOVEMENT_MAP = false;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete hdrVideo;
}

void MainWindow::on_convertBtn_clicked()
{
    std::cout << "hello world\n";

    ui->saveBtn->setEnabled(false);
    this->hdrVideo->sequence.clear();

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
    ui->actionOpen->setEnabled(false);

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
            if(this->SHOW_MOVEMENT_MAP)
                imshow("labeled movement map", labeledMap[1]);

            //Mat hdr_frame = hdrCap->merge_frames(images[0], images[1]);
            //imshow("hdr_frame", hdr_frame);

            Mat exposure_fusion = this->hdrCap->exposure_fusion(images[0], images[1], nLabels, labeledMap[0]);
            //auto end = chrono::steady_clock::now();
            imshow("hdr_exposure_fusion", exposure_fusion);
            if(exposure_fusion.cols < hdrVideo->get_width())
                hdrVideo->set_width(exposure_fusion.cols);
            if(exposure_fusion.rows < hdrVideo->get_height())
                hdrVideo->set_height(exposure_fusion.rows);
            hdrVideo->sequence.push_back(exposure_fusion);
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

    ui->saveBtn->setEnabled(true);
    ui->convertBtn->setEnabled(true);
    ui->actionOpen->setEnabled(true);
    ui->playPauseBtn->setEnabled(false);
    ui->nextBtn->setEnabled(false);
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

void MainWindow::on_actionOpen_triggered()
{
    ui->saveBtn->setEnabled(false);
    QString fName = QFileDialog::getOpenFileName(this,
            tr("Open video to convert"), "",
            tr("Video file (*.avi);;All Files (*)"));
    cout << "File name is: " << fName.toStdString() << endl;
    if (fName.isEmpty()){
        cout << "File is Empty!" << endl;

        ui->TextStatus->setStyleSheet("QLabel { color : red; }");
        std::string info = "File doesn't exist";
        ui->TextStatus->setText(QString::fromStdString(info));

        ui->convertBtn->setEnabled(false);

        return;
    }

    VideoCapture* temp = new VideoCapture(fName.toStdString());
    if(!temp->isOpened()) {
        ui->TextStatus->setStyleSheet("QLabel { color : red; }");
        std::string info = "File doesn't exist";
        ui->TextStatus->setText(QString::fromStdString(info));

        ui->convertBtn->setEnabled(false);
    } else {
        this->filename = fName.toStdString();
        ui->TextStatus->setStyleSheet("QLabel { color : green; }");
        std::string info = "File exists\nPATH: " + this->filename;
        ui->TextStatus->setText(QString::fromStdString(info));

        ui->convertBtn->setEnabled(true);
    }
    delete temp;
}

void MainWindow::on_moveCheckBox_stateChanged(int arg1)
{
    ui->saveBtn->setEnabled(false);
    this->hdrVideo->sequence.clear();

    if(this->SHOW_MOVEMENT_MAP)
        this->SHOW_MOVEMENT_MAP = false;
    else
        this->SHOW_MOVEMENT_MAP = true;
}

void MainWindow::on_saveBtn_clicked()
{
    this->hdrVideo->saveVideo();
    QMessageBox::information(this, QString("Information"), QString("Video has been saved to ../hdr_video.avi."));
}
