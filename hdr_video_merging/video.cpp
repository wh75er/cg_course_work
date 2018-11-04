#include "video.h"

Video::Video(string* path)
{
    this->path = path;
    String temp(path->c_str());
    this->cap = new VideoCapture(temp);
}

Video::~Video()
{
    delete this->cap;
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
    return *this->path;
}

VideoCapture* Video::get_capture()
{
    return this->cap;
}
