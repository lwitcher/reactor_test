#include "cthreading.h"
#include "creactor.h"
#include <iostream>

using namespace std;
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


/*测试eventfd
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <iostream>
#include <unistd.h>
using namespace std;
class testwrite :public cthreading
{
public:

    virtual void run()
    {
        this_thread::sleep_for(chrono::microseconds(20));
        eventfd_write(fd_, 1);
        this_thread::sleep_for(chrono::microseconds(20));
        eventfd_write(fd_, 1);
        this_thread::sleep_for(chrono::microseconds(20));
        eventfd_write(fd_, 1);
        eventfd_write(fd_, 1);
    }
    int fd_;
};

class testread :public cthreading
{
public:

    virtual void run()
    {
        //this_thread::sleep_for(chrono::microseconds(1));
        int efd = epoll_create(1);
        struct epoll_event evt;
		evt.events = EPOLLIN;
		int iRet = epoll_ctl(efd, EPOLL_CTL_ADD, fd_, &evt);
		while(true)
		{
			struct epoll_event events[8] = {};
			int nr_events = epoll_wait(efd, events, 8, -1);
			if (nr_events < 0)
			{
				perror("epoll_wait");
				return;
			}
			for (int i = 0; i < nr_events; i++)
			{
				uint64_t tmp;
				eventfd_read(fd_, &tmp);
				cout << "read:  " << tmp<< endl;
			}
		}
    }
    int fd_;
};

int main()
{
	int fd = eventfd(0,0);
	testread reader;
	reader.fd_ = fd;
	testwrite w1;
	w1.fd_ = fd;
	reader.create();
	w1.create();
	reader.join();
    return 0;
}
*/


#include <unistd.h>
class input_handler:public cevent_handle
{
public:
	void handle_input()
	{
		cout << "handle_input: ";
		char s[1024] = {0};
		read(fd_, s, 1024);
		cout << s << endl;
	}
};
int main()
{
	creactor reactor;
	input_handler handler;
	handler.set_fd(0);
	reactor.regist_event_handler(&handler);
	reactor.create();
	reactor.join();
    return 0;
}
