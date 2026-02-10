#include <netinet/in.h>

#ifndef THESISUIS_SERVER_HPP
#define THESISUIS_SERVER_HPP

namespace Thesisuis {
    class Server  {
        public:
            // Create the socket
            int createSocket();
            // Bind the socket to an IP and port
            sockaddr_in bindSocket(int);
            // Listen for a connection
            void listenForConnection(int);
            // Accept connection
            int acceptConnection(int);
            // Receive data
            char* receiveData(int);
            // Close sockets
            void closeSockets(int, int);
    };
}

#endif // THESISUIS_SERVER_HPP
