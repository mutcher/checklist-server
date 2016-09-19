#include <thread>
#include <csignal>
#include <sstream>
#include "inc/server_socket.h"
#include "inc/client.h"
#include "inc/list_processor.h"
#include "inc/log_manager.h"

std::atomic_bool g_isRunning = {true};
server_socket server(address(0, 51789));
log_manager log_mgr;

void clientThread(socket clientSocket, list_processor& processor)
{
    client client(clientSocket, processor, &log_mgr);
    if (client.init())
    {
        {
            std::stringstream ss;
            ss << "[" << clientSocket.getID() << "]: " << "Received connection from " << clientSocket.getAddress().toString();
            log_mgr.log(ss.str());
        }
        client.processClientRequests();
        {
            std::stringstream ss;
            ss << "[" << clientSocket.getID() << "]: " << "Disconnected";
            log_mgr.log(ss.str());
        }
    }
}

void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        g_isRunning.store(false);
        server.shutdown();
    }
}

int main(int argc, char** argv)
{
    log_mgr.init();
    std::signal(SIGINT, signalHandler);
    log_mgr.log("Server startup");
    if (socket_system::init())
    {
        list_processor processor;
        processor.init();
        if (server.open())
        {
            {
                std::stringstream ss;
                ss << "Listening on " << server.getAddress().getPort() << " port";
                log_mgr.log(ss.str());
            }
            std::vector<std::thread> thread_list;
            while (g_isRunning)
            {
                socket clientSocket;
                if (server.accept(clientSocket))
                {
                    thread_list.push_back(std::thread(clientThread, std::move(clientSocket), std::ref(processor)));
                }
            }
            log_mgr.log("Server shutdown");
            for(auto& thread : thread_list)
            {
                thread.join();
            }
        }
        else
        {
            std::stringstream ss;
            ss << "Cannot open socket on " << server.getAddress().getPort() << " port";
            log_mgr.log(ss.str());
        }
        processor.shutdown();
        socket_system::shutdown();
    }
    else
    {
        log_mgr.log("Cannot startup socket system");
    }
    log_mgr.shutdown();
    return 0;
}
