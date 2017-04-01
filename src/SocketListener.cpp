#include "SocketListener.h"

#include <cstdlib>
#include <iostream>

namespace ryuuk
{
    SocketListener::SocketListener() :
        m_socketfd(0),
        m_serverInfo(nullptr)
    {

    }

    bool SocketListener::listen(int port, int n)
    {
        int status;
        addrinfo hints;
        addrinfo *serverInfo = nullptr;

        std::memset(&hints, 0, sizeof hints); // make sure the struct is empty
        hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
        hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
        hints.ai_flags = AI_PASSIVE;     // fill in my IP for me


        if ((status = getaddrinfo(NULL, std::to_string(port), &hints, &serverInfo)) != 0)
        {
            std::cerr << "getaddrinfo error:" << gai_strerror(status) << std::endl;
            return false;
        }

        auto p = serverInfo;
        for (; p != nullptr; p = p->ai_next)
        {
            m_socketfd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
            if (m_socketfd < 0)
            {
                std::cerr << "Error in creating socket file descriptor. Trying a different one." << std::endl;
                continue;
            }

            int yes = 1;
            if (setsockopt(m_socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
            {
                std::cerr << "setsockopt" << std::endl;
                return false;
            }

            if (bind(m_socketfd, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0)
            {
                std::cerr << "Error in binding socket to port, trying with different settings." << std::endl;
                continue;
            }
        }

        freeaddrinfo(serverInfo);

        if (p == nullptr)
        {
            std::cerr << "Couldn't find an appropriate server info" << std::endl;
            return false;
        }

        if (::listen(m_socketfd, n) < 0)
        {
            std::cerr << "Error in listening" << std::endl;
            return false;
        }

        return true;
    }

    StreamSocket SocketListener::accept()
    {
        int client_sockfd = ::accept(m_socketfd, foo, bar);
        return StreamSocket{client_sockfd};
    }

    SocketListener::~SocketListener()
    {
    }


}