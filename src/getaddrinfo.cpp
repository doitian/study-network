#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <functional>
#include <utility>
#include <sstream>
#include <string>
#include <arpa/inet.h>

using namespace std;

// struct in_addr {
//   in_addr_t       s_addr;        /* IPv4 address */
// };
//
// struct sockaddr_in {
//   sa_family_t    sin_family;    /* address family */
//   in_port_t      sin_port;      /* port number */
//   struct in_addr sin_addr;      /* IPv4 address */
// };
static ostream& operator<<(ostream& os, const sockaddr_in& addr) {
  stringstream ss;
  char buffer[INET_ADDRSTRLEN];

  ss << "{\n";

  ss << "    addr: ";
  ss << inet_ntop(AF_INET, &addr.sin_addr, buffer, INET_ADDRSTRLEN);
  ss << "\n";

  ss << "    port: " << ntohs(addr.sin_port) << "\n";

  ss << "  }";

  os << ss.str();

  return os;
}

// struct in6_addr {
//   uint8_t        s6_addr[16];    /* IPv6 address */
// };
//
// struct sockaddr_in6 {
//   sa_family_t     sin6_family;    /* address family */
//   in_port_t       sin6_port;      /* port number */
//   uint32_t        sin6_flowinfo;  /* traffic class and flow info */
//   struct in6_addr sin6_addr;      /* IPv6 address */
//   uint32_t        sin6_scope_id;  /* set of interfaces for scope */
// };
static ostream& operator<<(ostream& os, const sockaddr_in6& addr) {
  stringstream ss;
  char buffer[INET6_ADDRSTRLEN];

  ss << "{\n";

  ss << "    addr: ";
  ss << inet_ntop(AF_INET6, &addr.sin6_addr, buffer, INET6_ADDRSTRLEN);
  ss << "\n";

  ss << "    port: " << ntohs(addr.sin6_port) << "\n";

  ss << "  }";

  os << ss.str();

  return os;
}

// struct addrinfo {
//     int              ai_flags;
//     int              ai_family;
//     int              ai_socktype;
//     int              ai_protocol;
//     socklen_t        ai_addrlen;
//     struct sockaddr *ai_addr;
//     char            *ai_canonname;
//     struct addrinfo *ai_next;
// };
static ostream& operator<<(ostream& os, const addrinfo& info) {
  stringstream ss;
  ss << "addrinfo {\n";

  // ai_family   This  field  specifies  the  desired address family for the returned addresses.
  //             Valid values for this field include AF_INET and AF_INET6.  The value  AF_UNSPEC
  //             indicates  that  getaddrinfo()  should  return socket addresses for any address
  //             family (either IPv4 or IPv6, for example) that can be used with node  and  ser‐
  //             vice.
  ss << "  family: ";
  if (info.ai_family == AF_INET) {
    ss << "IPv4";
  } else if (info.ai_family == AF_INET6) {
    ss << "IPv6";
  } else {
    ss << "Unknown";
  }
  ss << "\n";

  ss << "  socktype: ";
  if (info.ai_socktype == SOCK_STREAM) {
    ss << "STREAM";
  } else if (info.ai_socktype == SOCK_DGRAM) {
    ss << "DGRAM";
  } else if (info.ai_socktype == SOCK_SEQPACKET) {
    ss << "SEQPACKET";
  } else if (info.ai_socktype == SOCK_RAW) {
    ss << "RAW";
  } else {
    ss << "Unknown";
  }
  ss << "\n";

  ss << "  sockaddr: ";
  if (info.ai_family == AF_INET) {
    const sockaddr_in* addr_in = reinterpret_cast<const sockaddr_in*>(info.ai_addr);
    ss << *addr_in;
  } else if (info.ai_family == AF_INET6) {
    const sockaddr_in6* addr_in6 = reinterpret_cast<const sockaddr_in6*>(info.ai_addr);
    ss << *addr_in6;
  }
  ss << "\n";

  ss << "}\n";

  os << ss.str();

  if (info.ai_next) {
    os << *(info.ai_next);
  }

  return os;
}

const char* const DEFAULT_NODE = "localhost";
const char* const DEFAULT_SERVICE = nullptr;

int main(int argc, char* argv[]) {
  addrinfo *info = nullptr;
  const char* node = DEFAULT_NODE;
  const char* service = DEFAULT_SERVICE;

  if (argc > 1) {
    node = argv[1];
  }
  if (argc > 2) {
    service = argv[2];
  }

  cout << "getaddrinfo " << node;
  if (service) {
    cout << " " << service;
  }
  cout << endl;

  int ret = 0;

  ret = getaddrinfo(node, service, nullptr, &info);
  if (ret == -1) {
    perror("getaddrinfo");
    exit(EXIT_FAILURE);
  }

  if (info) {
    cout << *info << endl;
  } else {
    cout << "N/A" << endl;
  }
  freeaddrinfo(info);
  info = nullptr;

  return EXIT_SUCCESS;
}
