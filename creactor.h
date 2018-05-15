#pragma once
#include "cthreading.h"
#include <memory.h>

const int BUFFER_SIZE = 2048;

class cevent_handle
{
	friend class creactor;
	enum event_status
	{
		STAT_ADD,
		STAT_STABLE,
		STAT_DEL
	};
public:
	cevent_handle():fd_(-1),stat_(STAT_ADD){}
	virtual ~cevent_handle(){}
    void handle_input(){};
    void handle_output(){};

	int get_fd() const {
		return fd_;
	}

	void set_fd(int fd) {
		fd_ = fd;
	}

private:
    int fd_;
    event_status stat_;
};
class creactor : public cthreading
{
public:
    creactor();
    ~creactor();
    virtual void run();
    //TODO
    //void exit();
    bool regist_event_handler(cevent_handle* ptr);
    bool unregist_event_handler(cevent_handle* ptr);
private:
    int epoll_fd_;
    int event_op_list_;
    threading_list<cevent_handle*>* evt_list_ptr_;
    bool continue_;
};

class cchannel
{
public:
	cchannel():fd_(-1){
		memset(&read_buffer_, BUFFER_SIZE, 0);
	}
	virtual ~cchannel(){}
	virtual int send_buffer(char* buf, int count);
	virtual int read_buffer(char* buf, int& count);

	int get_id() const {
		return fd_;
	}

	void set_id(int fd) {
		fd_ = fd;
	}

protected:
	int fd_;
	char read_buffer_[BUFFER_SIZE];
};

