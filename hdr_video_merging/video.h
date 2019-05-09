#ifndef VIDEO_H
#define VIDEO_H

#include "libs.h"

class Video
{
public:
    explicit Video(string &path);
    ~Video();

    string get_path();
    int get_width();
    int get_height();
    int get_frames();
    VideoCapture *get_capture();

    vector<Mat> sequence;

    void set_width(int w);
    void set_height(int h);
    void set_frames(int f);

    void saveVideo();

private:
    string path;
    VideoCapture *cap;
    int width;
    int height;
    int frames;
};


#endif // VIDEO_H
