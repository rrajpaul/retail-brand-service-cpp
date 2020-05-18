#ifndef HostInfo_hpp
#define HostInfo_hpp


#include "oatpp/core/Types.hpp"
#if defined(WIN32) || defined(_WIN32)
  #include <io.h>
  #include <WinSock2.h>
  #include <WS2tcpip.h>
#else
  #include <netdb.h>
  #include <arpa/inet.h>
  #include <sys/socket.h>
  #include <netinet/tcp.h>
  #include <unistd.h>
#endif

class HostInfo {
public:
    HostInfo() {
        char hostbuffer[256];
        char *IPbuffer;
        struct hostent *host_entry;
        int hostName;

        hostName = gethostname(hostbuffer, sizeof(hostbuffer));
        if (hostName == -1)
        {
            OATPP_LOGE("[App::getHostInfo()]", "Error: Could not get the host name");
            throw std::runtime_error("[App::getHostInfo()]: Error: Could not get the host name");
        }

        m_hostName = oatpp::String(hostbuffer);

        host_entry = gethostbyname(hostbuffer);
        if (host_entry == NULL)
        {
            OATPP_LOGE("[App::getHostInfo()]", "Error: Could not get the host name");
            throw std::runtime_error("[App::getHostInfo()]: Error: Could not get the host name");
        }

        IPbuffer = inet_ntoa(*((struct in_addr*)
                                host_entry->h_addr_list[0]));
        if (NULL == IPbuffer)
        {
            OATPP_LOGE("App::getHostInfo()]", "Error: Could not get the ip address of the host");
            throw std::runtime_error("[App::getHostInfo()]: Error. Could not get the ip address of the host");
        }

        m_hostAddress = oatpp::String(IPbuffer);

        // OATPP_LOGD("[App::getHostInfo()]", "Host name is %s", m_hostName->c_str() );
        //OATPP_LOGD("[App::getHostInfo()]", "Host address is %s", m_hostAddress->c_str() );
    }
    oatpp::String getHostName() {
        return m_hostName;
    }
    oatpp::String getHostAddress() {
        return m_hostAddress;
    }
private:
  oatpp::String m_hostName;
  oatpp::String m_hostAddress;
};

#endif /* HostInfo_hpp */