#include "creactor.h"
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAX_EVENTS 2048
creactor::creactor() :
		epoll_fd_(-1), continue_(true), evt_list_ptr_(nullptr), event_op_list_(
				-1) {
	evt_list_ptr_ = new threading_list<cevent_handle *>;
	event_op_list_ = eventfd(0, 0);
}

creactor::~creactor() {
}

void creactor::run() {
	epoll_fd_ = epoll_create(MAX_EVENTS);
	if (epoll_fd_ < 0) {
		return;
	}

	{
		struct epoll_event evt;
		evt.events = EPOLLIN;
		evt.data.ptr = this;
		int iRet = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, event_op_list_, &evt);
		if (iRet < 0) {
			perror("epoll_ctl");
			return;
		}
	}

	struct epoll_event events[MAX_EVENTS] = { };
	bool bop_list = true;
	while (continue_) {
		if (bop_list == true) {
			GET_THREAD_SAFE_LIST(evt_list, evt_list_ptr_);
			bop_list = false;
			auto ite = evt_list.begin();
			for (; ite != evt_list.end(); ++ite) {
				if ((*ite)->stat_ == cevent_handle::STAT_ADD) {
					(*ite)->stat_ = cevent_handle::STAT_STABLE;
					struct epoll_event evt;
					evt.events = EPOLLIN;
					evt.data.ptr = (*ite);
					int iRet = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD,
							(*ite)->get_fd(), &evt);
					if (iRet < 0) {
						perror("epoll_ctl");
						return;
					}
				} else if ((*ite)->stat_ == cevent_handle::STAT_DEL) {
					int iRet = epoll_ctl(epoll_fd_, EPOLL_CTL_DEL,
							(*ite)->get_fd(), nullptr);
					if (iRet < 0) {
						perror("epoll_ctl");
						return;
					}
					ite = evt_list.erase(ite);
				}
			}
		}
		int nr_events = epoll_wait(epoll_fd_, events, MAX_EVENTS, -1);
		if (nr_events < 0) {
			perror("epoll_wait");
			return;
		}
		for (int i = 0; i < nr_events; i++) {
			if (bop_list == false && events[i].data.ptr == this) {
				bop_list = true;
				uint64_t tmp;
				eventfd_read(event_op_list_, &tmp);
				continue;
			}
			cevent_handle* evt_handle_ptr =
					(cevent_handle*) (events[i].data.ptr);
			if (events[i].events & EPOLLIN) {
				evt_handle_ptr->handle_input();
			} else if (events[i].events & EPOLLOUT) {
				evt_handle_ptr->handle_output();
			}
		}
	}
	close(epoll_fd_);
	epoll_fd_ = -1;
}

//TODO 最好是同步的
bool creactor::regist_event_handler(cevent_handle* ptr) {
	if(ptr->get_fd() == -1)
	{
		return false;
	}
	{
		GET_THREAD_SAFE_LIST(evt_list, evt_list_ptr_);
		ptr->stat_ = cevent_handle::STAT_ADD;
		evt_list.push_back(ptr);
	}
	eventfd_write(event_op_list_, 1);
	return true;
}

//TODO 最好是同步的
bool creactor::unregist_event_handler(cevent_handle* ptr) {
	if(ptr->get_fd() == -1)
	{
		return false;
	}
	ptr->stat_ = cevent_handle::STAT_DEL;
	eventfd_write(event_op_list_, 1);
	return true;
}

int cchannel::send_buffer(char* buf, int count) {
}

int cchannel::read_buffer(char* buf, int& count) {
	int read_count = 0;
	while (true) {
		if (read_count) {
		}
	}
}

int cchannel::connect(const char* ip, int port){
	struct sockaddr_in dest_addr;
	fd_ = socket(AF_INET, SOCK_STREAM, 0);
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	dest_addr.sin_addr.s_addr = inet_addr(ip);
	return ::connect(fd_, (struct sockaddr *) &dest_addr,sizeof(struct sockaddr));
}

int cchannel::listen(const char* ip, int port) {
	if(fd_ == -1)
	{
		return -1;
	}
	struct sockaddr_in my_addr;
	fd_ = socket(AF_INET, SOCK_STREAM, 0);
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = inet_addr(ip);
	bind(fd_, (struct sockaddr *) &my_addr,sizeof(struct sockaddr));
	return ::listen(fd_, 5);
}

int cchannel::close() {
	if(fd_ == -1)
	{
		return -1;
	}
	::close(fd_);
}
