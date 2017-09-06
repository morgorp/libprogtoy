#include "inet_address.h"

namespace progtoy {

	InetAddress::InetAddress(const string &_hostName, const string &_hostAddress)
		: hostName(_hostName), hostAddress(_hostAddress){}
	
	InetAddress::InetAddress(char *&_hostName, char *&_hostAddress)
		: hostName(_hostName), hostAddress(_hostAddress){}
	
	vector<InetAddress> InetAddress::getAllByName(const string &host)
	{
		struct hostent *ht = gethostbyname(host.c_str());
		if(ht == nullptr) {
			throw -1;
		}
		vector<InetAddress> ret;
		for(int i=0; ht->h_addr_list[i]!=nullptr; ++i) {
			struct in_addr nip = *(struct in_addr *)ht->h_addr_list[i];
			ret.push_back(InetAddress(ht->h_name, inet_ntoa(nip)));
		}
		return ret;
	}

	InetAddress InetAddress::getByName(const string &host)
	{
		struct hostent *ht = gethostbyname(host.c_str());
		if(ht == nullptr) {
			throw h_errno;
		}
		struct in_addr nip = *(struct in_addr *)ht->h_addr;
		return InetAddress(ht->h_name, inet_ntoa(nip));
	}

	InetAddress InetAddress::getByAddress(const string &addr)
	{
		struct in_addr nip;
		if(inet_aton(addr.c_str(), &nip) <= 0) {
			throw -1;
		}
		struct hostent *ht = gethostbyaddr(&nip, sizeof(nip), AF_INET);
		if(ht == nullptr) {
			throw -1;
		}
		nip = *(struct in_addr *)ht->h_addr;
		return InetAddress(ht->h_name, inet_ntoa(nip));
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
