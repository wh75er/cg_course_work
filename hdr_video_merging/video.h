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

private:
    string path;
    VideoCapture *cap;
    int width;
    int height;
    int frames;
};


#endif // VIDEO_H
