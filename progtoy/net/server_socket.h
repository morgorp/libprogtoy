/*
 * Copyright (C), 2017, progrom.
 */

#ifndef PROGTOY_NET_SERVER_SOCKET
#define PROGTOY_NET_SERVER_SOCKET

#include "inet_address.h"
#include "socket.h"
#include <unistd.h>

extern int errno;

namespace progtoy {

	/**
	 * ServerSocket类
	 */
	class ServerSocket {
	public:

		/* 创建一个未绑定监听套接字，随后可以设置套接字选项然后调用bind()绑定监听端口 */
		ServerSocket();

		/* 创建一个已绑定监听的套接字 */
		ServerSocket(int port, int backlog = 50, const InetAddress &bindAddr = anyAddr);

		/* 绑定并监听给定端口号 */
		void bind(int port, int backlog = 50, const InetAddress &bindAddr = anyAddr);

		/* 从accept队列中获取一个已建立连接与客户端通信的Socket */
		Socket accept();

		/* 完全关闭套接字 */
		void close();

		bool isClosed();

		/* 套接字选项 */	
		void setRecvBufferSize(int size);
		int getRecvBufferSize() const;

		void setReuseAddress(bool on);
		bool getReuseAddress() const;

	private:
		static InetAddress anyAddr; /* 任意地址(0.0.0.0) */
		
		int sockfd; /* 套接字文件描述符 */
		bool closed;
	};
}

#endif
