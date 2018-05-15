#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <memory.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int main() {
	struct sockaddr_in server_addr;

	int sockfd, reuse = 1;

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(1998);

	if (inet_pton(PF_INET, "127.0.0.1", &server_addr.sin_addr) == -1) {
		cout << "error inet_pton" << endl;
		return -1;
	}

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		cout << "error socket" << endl;
		return -1;
	}

	//关闭服务端后再重启，可以复用TIME_WAIT状态的端口
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse))
			== -1) {
		perror("setsockopt");
		return -1;
	}

	if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr))
			== -1) {
		cout << "error bind" << endl;
		return -1;
	}

	if (listen(sockfd, 5) == -1) {
		cout << "error listen" << endl;
		return -1;
	}

	struct sockaddr_in client_addr;
	socklen_t client_addrlen;
	int32_t connfd = accept(sockfd, (struct sockaddr *) &client_addr,
			&client_addrlen);
	cout << "accept!" << endl;

	char buffer[1024] = { 0 };

	int epollfd = epoll_create(8);
	struct epoll_event epv;
	struct epoll_event revs[2];
	//ET
	epv.events = EPOLLIN|EPOLLET;
	//epv.events = EPOLLIN;
	epv.data.fd = connfd;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &epv);

	while (true) {
		int count = epoll_wait(epollfd, revs, 2, -1);
		if (count <= 0) {
			cout << "count is " << count << endl;
			perror("epoll_wait");
			return -1;
		}
		sleep(10);
		cout << "end wait!" << endl;
		for (int i = 0; i < count; ++i) {
			cout << "doing..." << endl;
			memset(buffer, 0, sizeof(buffer));
			int recv_count = recv(revs[i].data.fd, buffer, 10, MSG_DONTWAIT);
			cout << recv_count << "   " << buffer << endl;
		}
	}

	close(sockfd);
	return 0;
}
