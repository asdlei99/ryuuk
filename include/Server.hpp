#ifndef SERVER_HPP
#define SERVER_HPP

#include "Log.hpp"
#include "SocketStream.hpp"
#include "SocketListener.hpp"
#include "ResponseCreator.hpp"

#include <map>
#include <list>
#include <memory>
#include <thread>

namespace ryuuk
{

    class Server
    {
    public:

        Server();

        ~Server();

        void init();

        void parseConfigFile();

        void run();

        void shutdown();

        void setConfigFile(const std::string& file);

    public:
        const std::string SERVER_CONFIG_FILE = "ryuuk.conf";
        std::string m_configPath;

        struct manifest
        {
            std::string ip;
            unsigned    port;
            unsigned    backlog;
        } server_manifest;

    private:
        SocketListener m_listener;
        std::mutex m_queueMutex;
        std::list<int> m_cleanupQueue;
        std::map<int, std::thread> m_connections;
        bool m_running;
    };

}

#endif // SERVER_HPP
