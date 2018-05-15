#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
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
	//nc 127.0.0.1 1998
	//123
	//read_count=4,buffer=123
	cout << "read" << endl;
	int read_count = read(connfd, buffer, 1024);
	cout << buffer << endl;
	cout << read_count << endl;

	//MSG_DONTWAIT,nonblock
	cout << "recv" << endl;
	read_count = recv(connfd, buffer, 1024, MSG_DONTWAIT);
	cout << "buffer " << buffer << "read_count " << read_count << endl;
	cout << (errno == EWOULDBLOCK) << endl;

	//MSG_WAITALL,read,until read 8 char data
	cout << "recv2" << endl;
	read_count = recv(connfd, buffer, 8, MSG_WAITALL);
	cout << "buffer " << buffer << "read_count " << read_count << endl;
	cout << errno << endl;

	close(sockfd);
	return 0;
}
