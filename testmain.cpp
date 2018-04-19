#include "cthreading.h"
#include <iostream>

using namespace std;
//测试线程安全list
/*
class testin :public cthreading
{
public:
    testin(threading_list<int>* l)
    {
        l_ptr_ = l;
    }
    virtual void run()
    {
        this_thread::sleep_for(chrono::microseconds(1));
        for (int i = 0; i < 10000;i+=2)
        {
            GET_THREAD_SAFE_LIST(inlist, l_ptr_);
            inlist.push_back(i);
        }
    }
    threading_list<int>* l_ptr_;

};
class testin2 :public cthreading
{
public:
    testin2(threading_list<int>* l)
    {
        l_ptr_ = l;
    }
    virtual void run()
    {
        for (int i = 1; i < 10000; i += 2)
        {
            GET_THREAD_SAFE_LIST(inlist, l_ptr_);
            inlist.push_back(i);
            //this_thread::sleep_for(chrono::microseconds(1));
        }
    }
    threading_list<int>* l_ptr_;

};
class testout :public cthreading
{
public:
    testout(threading_list<int>* l)
    {
        l_ptr_ = l;
    }
    virtual void run()
    {
        this_thread::sleep_for(chrono::microseconds(3));
        while (true)
        {
            GET_THREAD_SAFE_LIST(outlist, l_ptr_);
            if (outlist.empty())
                continue;
            cout << outlist.front() << endl;
            outlist.pop_front();
        }
    }
    threading_list<int>* l_ptr_;

};
int main()
{
    //输出数量应该正确，若将加锁去掉后输出个数不正确
    threading_list<int> l;
    testin thd(&l);
    thd.create();
    testin2 thd2(&l);
    thd2.create();
    testout thd3(&l);
    thd3.create();
    thd.join();
    thd2.join();
    thd3.join();
    return 0;
}
*/