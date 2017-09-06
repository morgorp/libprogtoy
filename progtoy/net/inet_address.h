/*
 * Copyright (C), 2017, progrom.
 */

#ifndef PROGTOY_NET_INET_ADDRESS
#define PROGTOY_NET_INET_ADDRESS

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <string>
#include <vector>

extern int h_errno;

namespace progtoy {
	using std::string;
	using std::vector;

	/**
	 * InetAddress类
	 */
	class InetAddress {
	public:

		/* 给定主机名返回从域名服务中获得的多个网络地址信息 */
		static vector<InetAddress> getAllByName(const string &);

		/* 给定主机名返回确定主机的网络地址信息 */
		static InetAddress getByName(const string &);

		/* 给定IP地址返回对应主机的网络地址信息 */
		static InetAddress getByAddress(const string &);

		/* 给定主机名和IP地址，创建InetAddress对象 */
		static InetAddress getByAddress(const string &, const string &);

		string getHostAddress() const;
		string getHostName() const;

	private:
		InetAddress() = default;
		InetAddress(const string &, const string &);
		InetAddress(char *&, char *&);
		
		string hostName; /* 主机名 */
		string hostAddress; /* 主机地址(IPv4) */
	};
}

#endif

