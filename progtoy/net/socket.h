/*
 * Copyright (C), 2017, progrom.
 */

#ifndef PROGTOY_NET_SOCKET
#define PROGTOY_NET_SOCKET

#include "inet_address.h"

namespace progtoy {

	/**
	 * Socket类
	 */
	class Socket {
	public:

		/* 给定目的主机和端口，创建一个套接字对象 */
		Socket(const string &host, int port);

		/* 给定目的主机地址信息和端口，创建一个套接字 */
		Socket(const InetAddress &addr, int port);

		/* 发送数据，返回发送的字节数 */
		ssize_t send(const void *buf, size_t nbytes, int flags = 0);

		/* 发送字符流，返回发送的字节数 */
		ssize_t send(const string &streambuf, int flags = 0);

		/* 接收数据，返回接收的实际字节数 */
		ssize_t recv(void *buf, size_t nbytes, int flags = 0);

		/* 接收一整行数据，返回string */
		string recv();

	private:
		int sockfd; /* 套接字文件描述符 */
	};
}


#endif
