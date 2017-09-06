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

	class InetAddress {
	public:

		static vector<InetAddress> getAllByName(const string &);
		static InetAddress getByName(const string &);
		static InetAddress getByAddress(const string &);
		static InetAddress getByAddress(const string &, const string &);
		
		string getHostAddress() const;
		string getHostName() const;

	private:
		InetAddress() = default;
		InetAddress(const string &, const string &);
		InetAddress(char *&, char *&);
		
		string hostName;
		string hostAddress;
	};
}

#endif
