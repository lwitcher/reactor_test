#pragma once
#include <thread>
#include <list>
#include <mutex>
#include <chrono>
#include <functional>
class cthreading
{
public:
    cthreading();
    virtual ~cthreading();
    virtual void run(){};
    virtual void after_run(){};
    void create();
    void join();
private:
    std::thread* thread_ptr_;
};

#define GET_THREAD_SAFE_LIST(LISTNAME,LIST_OBJ_PTR) std::lock_guard<std::mutex> guard_(LIST_OBJ_PTR->mutex_);\
    auto & LISTNAME = LIST_OBJ_PTR->list_

template<typename T>
class threading_list
{
public:
    std::list<T> list_;
    std::mutex mutex_;
};
