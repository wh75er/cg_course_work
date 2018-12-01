#ifndef HDR_H
#define HDR_H

#include "libs.h"

class Hdr_cap
{
public:
    Hdr_cap();
    ~Hdr_cap();

    bool align_frames();
    bool deghost_frames();
    bool merge_frames();
    bool tonemap_frames();
};

#endif // HDR_H
