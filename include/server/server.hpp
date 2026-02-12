#include <netinet/in.h>
#include <string>

#ifndef THESISUIS_SERVER_HPP
#define THESISUIS_SERVER_HPP

namespace Thesisuis {
    class Server  {
        public:
            // Authenticate user
            bool authenticate(int);
            void addUser(std::string, std::string, bool=false);
            // Create the socket
            int createSocket();
            // Bind the socket to an IP and port
            sockaddr_in bindSocket(int);
            // Listen for a connection
            void listenForConnection(int);
            // Accept connection
            int acceptConnection(int);
            // Receive data
            std::string receiveData(int);
            // Close sockets
            void closeSockets(int, int);
    };
}

#endif // THESISUIS_SERVER_HPP
