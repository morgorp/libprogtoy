/*
 * Copyright (C), 2017, progrom.
 */

#include "socket.h"

namespace progtoy {

	Socket::Socket()
	{
		closed = true;
		sockfd = socket(PF_INET, SOCK_STREAM, 0);
		if(sockfd < 0) {
			throw string(strerror(errno)); // ToDo: 异常处理
		}
	}

	Socket::Socket(const string &host, int port) : Socket()
	{
		connect(host, port);
	}

	Socket::Socket(const InetAddress &addr, int port) : Socket(addr.getHostAddress(), port) {}

	Socket::Socket(int fd)
	{
		sockfd = fd;
		closed = false;
	}

	void Socket::connect(const string &host, int port)
	{
		if(!isClosed()) {
			throw string("Not closed."); // ToDo: 异常处理
		}
		struct hostent htbuf;
		char tmpbuf[1024];
		struct hostent *ht;
		int herrno;
		if(gethostbyname_r(host.c_str(),
				&htbuf, tmpbuf, sizeof(tmpbuf),
				&ht, &herrno) || ht==nullptr) {
			throw string(hstrerror(herrno)); // ToDo: 异常处理
		}

		struct sockaddr_in addrin = {0};
		addrin.sin_family = AF_INET;
		addrin.sin_addr.s_addr = *reinterpret_cast<in_addr_t *>(ht->h_addr);
		addrin.sin_port = htons(port);
		
		if(::connect(sockfd, reinterpret_cast<struct sockaddr *>(&addrin), sizeof(addrin)) < 0) {
			throw string(strerror(errno)); // ToDo: 异常处理
		}
		closed = false; // ToDo: 线程安全
	}

	void Socket::connect(const InetAddress &addr, int port)
	{
		connect(addr.getHostAddress(), port);
	}

	ssize_t Socket::write(const void *buf, size_t nbytes, int flags)
	{
		if(buf == nullptr || nbytes == 0) {
			return 0;
		}
		if(isClosed()) {
			throw string("Already closed."); // ToDo: 异常处理
		}
		ssize_t ret = 0;
		if((ret=::send(sockfd, buf, nbytes, flags)) < 0) {
			throw string(strerror(errno)); // ToDo: 异常处理
		}
		return ret;
	}

	ssize_t Socket::writeLine(string streambuf, int flags)
	{
		streambuf.append("\r\n");
		return write(streambuf.c_str(), streambuf.size(), flags);
	}

	ssize_t Socket::read(void *buf, size_t nbytes, int flags)
	{
		if(buf == nullptr || nbytes == 0) {
			return 0;
		}
		if(isClosed()) {
			throw string("Already closed."); // ToDo: 异常处理
		}
		ssize_t ret = 0;
		while((ret=::recv(sockfd, buf, nbytes, flags)) < 0) {
			if(errno != EINTR) {
				throw string(strerror(errno)); // ToDo: 异常处理
			}
		}
		return ret;
	}

	string Socket::readLine()
	{
		string ret="";
		char buf[1024];
		for(;;) {
			/* 观看接收缓冲区前面的若干个数据 */
			ssize_t len = read(buf, sizeof(buf), MSG_PEEK);
			for(ssize_t i=0; i<len; ++i) {
				if(buf[i] == '\n') { // 看到了回车符
					ret.append(buf, i+1);
					::recv(sockfd, buf, i+1, 0); // 从接收缓冲区缓存删除看过的数据
					return ret;
				}
			}
			ret.append(buf, len);
			::recv(sockfd, buf, len, 0); // 从接收缓冲区删除看过的数据
		}
		return ret;
	}

	void Socket::close()
	{
		if(isClosed()) {
			throw string("Already closed."); // ToDo: 异常处理
		}
		::shutdown(sockfd, 2);
		closed = true; // ToDo: 线程安全
	}

	bool Socket::isClosed()
	{
		return closed; // ToDo: 线程安全
	}
	
	void Socket::setDontRoute(bool on)
	{
		int val = on;
		if(setsockopt(sockfd, SOL_SOCKET, SO_DONTROUTE, &val, sizeof(val)) < 0) {
			throw -1; // ToDo: 异常处理
		}
	}

	bool Socket::getDontRoute() const
	{
		int val = 0;
		socklen_t len = sizeof(val);
		if(getsockopt(sockfd, SOL_SOCKET, SO_DONTROUTE, &val, &len) < 0) {
			throw -1; // ToDo: 异常处理
		}
		return static_cast<bool>(val);
	}

	
	void Socket::setKeepAlive(bool on)
	{
		int val = on;
		if(setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val)) < 0) {
			throw -1; // ToDo: 异常处理
		}
	}

	bool Socket::getKeepAlive() const
	{
		int val = 0;
		socklen_t len = sizeof(val);
		if(getsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &val, &len) < 0) {
			throw -1; // ToDo: 异常处理
		}
		return static_cast<bool>(val);
	}
	
	void Socket::setSoLinger(bool on, int linger)
	{
		struct linger val {on, linger};
		if(setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &val, sizeof(val)) < 0) {
			throw -1; // ToDo: 异常处理
		}
	}

	int Socket::getSoLinger() const
	{
		struct linger val {0};
		socklen_t len = sizeof(val);
		if(getsockopt(sockfd, SOL_SOCKET, SO_LINGER, &val, &len) < 0) {
			throw -1; // ToDo: 异常处理
		}
		if(val.l_onoff == 0) return -1;
		return val.l_linger;
	}

	void Socket::setOOBInline(bool on)
	{
		int val = on;
		if(setsockopt(sockfd, SOL_SOCKET, SO_OOBINLINE, &val, sizeof(val)) < 0) {
			throw -1; // ToDo: 异常处理
		}
	}

	bool Socket::getOOBInline() const
	{
		int val = 0;
		socklen_t len = sizeof(val);
		if(getsockopt(sockfd, SOL_SOCKET, SO_OOBINLINE, &val, &len) < 0) {
			throw -1; // ToDo: 异常处理
		}
		return static_cast<bool>(val);
	}

	void Socket::setRecvBufferSize(int size)
	{
		if(setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)) < 0) {
			throw -1; // ToDo: 异常处理
		}
	}

	int Socket::getRecvBufferSize() const
	{
		int val = 0;
		socklen_t len = sizeof(val);
		if(getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &val, &len) < 0) {
			throw -1; // ToDo: 异常处理
		}
		return val;
	}

	void Socket::setReuseAddress(bool on)
	{
		int val = on;
		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
			throw -1; // ToDo: 异常处理
		}
	}

	bool Socket::getReuseAddress() const
	{
		int val = 0;
		socklen_t len = sizeof(val);
		if(getsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, &len) < 0) {
			throw -1; // ToDo: 异常处理
		}
		return static_cast<bool>(val);
	}

	void Socket::setSendBufferSize(int size)
	{
		if(setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)) < 0) {
			throw -1; // ToDo: 异常处理
		}
	}

	int Socket::getSendBufferSize() const
	{
		int val = 0;
		socklen_t len = sizeof(val);
		if(getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &val, &len) < 0) {
			throw -1; // ToDo: 异常处理
		}
		return val;
	}

	void Socket::setTcpNoDelay(bool on)
	{
		int val = on;
		if(setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val)) < 0) {
			throw -1; // ToDo: 异常处理
		}
	}

	bool Socket::getTcpNoDelay() const
	{
		int val = 0;
		socklen_t len = sizeof(val);
		if(getsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &val, &len) < 0) {
			throw -1; // ToDo: 异常处理
		}
		return static_cast<bool>(val);
	}

}
