/*
 * Copyright (C), 2017, progrom.
 */

#ifndef PROGTOY_NET_SOCKET
#define PROGTOY_NET_SOCKET

#include "inet_address.h"
#include <netinet/tcp.h>
#include <string.h>

extern int errno;

namespace progtoy {

	/**
	 * Socket类
	 */
	class Socket {
		friend class ServerSocket;
	public:

		/* 创建一个未连接套接字，随后可以设置套接字选项然后调用connect()函数创建连接 */
		Socket();

		/* 给定目的主机和端口，创建一个套接字对象 */
		Socket(const string &host, int port);

		/* 给定目的主机地址信息和端口，创建一个套接字 */
		Socket(const InetAddress &addr, int port);

		/* 创建连接 */
		void connect(const string &host, int port);
		void connect(const InetAddress &addr, int port);

		/* 发送数据，返回发送的字节数 */
		ssize_t write(const void *buf, size_t nbytes, int flags = 0);

		/* 发送一行字符流，返回发送的字节数 */
		ssize_t writeLine(string streambuf, int flags = 0);

		/* 接收数据，返回接收的实际字节数 */
		ssize_t read(void *buf, size_t nbytes, int flags = 0);

		/* 接收一整行数据，返回string */
		string readLine();

		/* 完全关闭连接 */
		void close();

		bool isClosed();

		/* 套接字选项 */
		void setDontRoute(bool on);
		bool getDontRoute() const;

		void setKeepAlive(bool on);
		bool getKeepAlive() const;

		void setSoLinger(bool on, int linger);
		int getSoLinger() const;

		void setOOBInline(bool on);
		bool getOOBInline() const;

		void setRecvBufferSize(int size);
		int getRecvBufferSize() const;

		void setReuseAddress(bool on);
		bool getReuseAddress() const;

		void setSendBufferSize(int size);
		int getSendBufferSize() const;

		/* TCP选项 */
		void setTcpNoDelay(bool on);
		bool getTcpNoDelay() const;

	private:
		Socket(int fd);

		int sockfd; /* 套接字文件描述符 */
		bool closed;
	};
}


#endif
