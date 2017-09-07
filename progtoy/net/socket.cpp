/*
 * Copyright (C), 2017, progrom.
 */

#include "socket.h"

namespace progtoy {
	
	Socket::Socket(const string &host, int port)
	{
		struct hostent htbuf;
		char tmpbuf[1024];
		struct hostent *ht;
		int herrno;
		if(gethostbyname_r(host.c_str(),
				&htbuf, tmpbuf, sizeof(tmpbuf),
				&ht, &herrno) || ht==nullptr) {
			printf("host: %s\n", hstrerror(herrno));
			throw herrno; // ToDo: 异常处理
		}

		struct sockaddr_in addrin;
		addrin.sin_family = AF_INET;
		addrin.sin_addr.s_addr = *reinterpret_cast<in_addr_t *>(ht->h_addr);
		addrin.sin_port = htons(port);
		
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd < 0) {
			perror("socket");
			throw -1; // ToDo: 异常处理
		}

		if(connect(sockfd, reinterpret_cast<struct sockaddr *>(&addrin), sizeof(addrin)) < 0) {
			perror("connect");
			throw -1; // ToDo: 异常处理
		}
	}

	Socket::Socket(const InetAddress &addr, int port) : Socket(addr.getHostAddress(), port){}

	ssize_t Socket::send(const void *buf, size_t nbytes, int flags)
	{
		ssize_t ret = 0;
		if((ret=::send(sockfd, buf, nbytes, flags)) < 0) {
			perror("send");
			throw -1; // ToDo: 异常处理
		}
		return ret;
	}

	ssize_t Socket::send(const string &streambuf, int flags)
	{
		return send(streambuf.c_str(), streambuf.size(), flags);
	}

	ssize_t Socket::recv(void *buf, size_t nbytes, int flags)
	{	
		ssize_t ret = 0;
		while((ret=::recv(sockfd, buf, nbytes, flags)) < 0) {
			if(errno != EINTR) {
				perror("recv");
				throw -1; // ToDo: 异常处理
			}
		}
		return ret;
	}

	string Socket::recv()
	{
		string ret="";
		char buf[1024];
		for(;;) {
			/* 观看接收缓存前面的若干个数据 */
			ssize_t len = recv(buf, sizeof(buf), MSG_PEEK);
			for(ssize_t i=0; i<len; ++i) {
				if(buf[i] == '\n') { // 看到了回车符
					ret.append(buf, i+1);
					::recv(sockfd, buf, i+1, 0); // 从接收缓冲缓存删除看过的数据
					return ret;
				}
			}
			ret.append(buf, len);
			::recv(sockfd, buf, len, 0); // 从接收缓存删除看过的数据
		}
		return ret;
	}

}
