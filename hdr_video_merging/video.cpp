#include "video.h"

Video::Video(string &path)
{
    this->path = path;
    this->cap = new VideoCapture(this->path);
}

Video::~Video()
{
    delete this->cap;
}


void Video::set_width(int w)
{
    this->width = w;
}

void Video::set_height(int h)
{
    this->height = h;
}

void Video::set_frames(int f)
{
    this->frames = f;
}

int Video::get_width()
{
    return this->width;
}

int Video::get_height()
{
    return this->height;
}

int Video::get_frames()
{
    return this->frames;
}

string Video::get_path()
{
    return this->path;
}

VideoCapture* Video::get_capture()
{
    return this->cap;
}

void Video::saveVideo()
{
    VideoWriter vid("../hdr_saved.avi", VideoWriter::fourcc('M','J','P','G'), this->frames, Size(this->width, this->height));

    for(size_t i = 0; i < this->sequence.size(); i++){
        vid.write(sequence[i]);
    }

    vid.release();
}
