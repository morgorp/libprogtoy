/*
 * Copyright (C), 2017, progrom.
 */

#include "inet_address.h"

namespace progtoy {

	InetAddress::InetAddress(const string &_hostName, const string &_hostAddress)
		: hostName(_hostName), hostAddress(_hostAddress){}
	
	InetAddress::InetAddress(const char *&_hostName, const char *&_hostAddress)
		: hostName(_hostName), hostAddress(_hostAddress){}
	
	vector<InetAddress> InetAddress::getAllByName(const string &host)
	{
		struct hostent htbuf;
		char tmpbuf[1024];
		struct hostent *ht;
		int herrno;
		if(gethostbyname_r(host.c_str(),
				&htbuf, tmpbuf, sizeof(tmpbuf),
				&ht, &herrno) || ht==nullptr) {
			throw string(hstrerror(herrno)); // ToDo: 异常处理
		}

		vector<InetAddress> ret;
		for(int i=0; ht->h_addr_list[i]!=nullptr; ++i) {
			ret.push_back( InetAddress(ht->h_name, 
				inet_ntop(AF_INET, ht->h_addr_list[i], tmpbuf, sizeof(tmpbuf))) );
		}
		return ret;
	}

	InetAddress InetAddress::getByName(const string &host)
	{
		return getAllByName(host)[0];
	}

	InetAddress InetAddress::getByAddress(const string &addr)
	{
		struct in_addr nip;
		if(inet_pton(AF_INET, addr.c_str(), &nip) <= 0) {
			throw -1; // ToDo: 异常处理
		}
		struct hostent htbuf;
		char tmpbuf[1024];
		struct hostent *ht;
		int herrno;
		if(gethostbyaddr_r(&nip, sizeof(nip), AF_INET,
				&htbuf, tmpbuf, sizeof(tmpbuf),
				&ht, &herrno) || ht == nullptr) {	
			throw string(hstrerror(herrno)); // ToDo: 异常处理
		}
		return InetAddress(ht->h_name, inet_ntop(AF_INET, ht->h_addr, tmpbuf, sizeof(tmpbuf)));
	}

	InetAddress InetAddress::getByAddress(const string &host, const string &addr)
	{
		return InetAddress(host, addr);
	}

	string InetAddress::getHostAddress() const
	{
		return hostAddress;
	}

	string InetAddress::getHostName() const
	{
		return hostName;
	}
}

