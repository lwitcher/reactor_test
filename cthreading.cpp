#include "cthreading.h"
#include <functional>

using namespace std;

void cthreading::create()
{
    auto func = bind(&cthreading::run,this);
    thread_ptr_ = new std::thread(func);
}

void cthreading::join()
{
    if (thread_ptr_ == nullptr)
    {
        return;
    }
    thread_ptr_->join();
}

cthreading::cthreading() :thread_ptr_(nullptr)
{
}


cthreading::~cthreading()
{
}
