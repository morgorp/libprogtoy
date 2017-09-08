/*
 * Copyright (C), 2017, progrom.
 */

#include "server_socket.h"

namespace progtoy {

	InetAddress ServerSocket::anyAddr = InetAddress::getByAddress("", "0.0.0.0");

	ServerSocket::ServerSocket()
	{	
		closed = true;
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd < 0) {
			throw string(strerror(errno)); // ToDo: 异常处理
		}
	}

	ServerSocket::ServerSocket(int port, int backlog, const InetAddress &bindAddr) : ServerSocket()
	{
		bind(port, backlog, bindAddr);
	}

	void ServerSocket::bind(int port, int backlog, const InetAddress &bindAddr)
	{
		if(!isClosed()) {
			throw string("Not closed."); // ToDo: 异常处理
		}
		struct sockaddr_in addrin;
		if(inet_aton(bindAddr.getHostAddress().c_str(), 
				reinterpret_cast<in_addr*>(addrin.sin_addr.s_addr)) <= 0) {
			::close(sockfd);
			throw -1; // ToDo: 异常处理
		}
		addrin.sin_family = AF_INET;
		addrin.sin_port = htons(port);

		if(::bind(sockfd, reinterpret_cast<struct sockaddr *>(&addrin), sizeof(addrin)) < 0) {
			::close(sockfd);
			throw string(strerror(errno)); // ToDo: 异常处理
		}

		if(::listen(sockfd, backlog) < 0) {
			::close(sockfd);
			throw string(strerror(errno)); // ToDo: 异常处理
		}
		closed = false; // ToDo: 线程安全
	}

	Socket ServerSocket::accept()
	{
		if(isClosed()) {
			throw string("Already closed."); // ToDo: 异常处理
		}
		int fd;
		struct sockaddr_in addrin;
		socklen_t len = sizeof(addrin);
		while((fd=::accept(sockfd, reinterpret_cast<struct sockaddr *>(&addrin), &len)) < 0) {
			if(errno != EINTR) {
				throw string(strerror(errno)); // ToDo: 异常处理
			}
		}
		return Socket(fd);
	}

	void ServerSocket::close()
	{
		if(isClosed()) {
			throw string("Already closed."); // ToDo: 异常处理
		}	
		::shutdown(sockfd, 2);
		closed = true; // ToDo: 线程安全
	}

	bool ServerSocket::isClosed()
	{
		return closed; // ToDo: 线程安全
	}

	void ServerSocket::setRecvBufferSize(int size)
	{
		if(setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)) < 0) {
			throw -1; // ToDo: 异常处理
		}
	}

	int ServerSocket::getRecvBufferSize() const
	{
		int val = 0;
		socklen_t len = sizeof(val);
		if(getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &val, &len) < 0) {
			throw -1; // ToDo: 异常处理
		}
		return val;
	}

	void ServerSocket::setReuseAddress(bool on)
	{
		int val = on;
		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
			throw -1; // ToDo: 异常处理
		}
	}

	bool ServerSocket::getReuseAddress() const
	{
		int val = 0;
		socklen_t len = sizeof(val);
		if(getsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, &len) < 0) {
			throw -1; // ToDo: 异常处理
		}
		return static_cast<bool>(val);
	}

}
