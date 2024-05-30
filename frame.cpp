#include "frame.h"
#include <chrono>
#include <thread>

Frame::Frame(long fps) 
{
    milliseconds_per_frame = (60 / fps) * 1000;
}

void Frame::limit()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds_per_frame));
}