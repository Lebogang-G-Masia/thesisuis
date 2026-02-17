#include <arpa/inet.h>

#ifndef THESISUIS_CLIENT_HPP
#define THESISUIS_CLIENT_HPP

namespace Thesisuis{
    class Client {
        public:
            void signin(int clientSocket);
            // create the socket
            int createSocket();
            // specify the server
            sockaddr_in getServerAddress();
            // connect to the server
            void connectToServer(int, sockaddr_in);
            // close socket
            void closeSocket(int);
    };
}

#endif // THESISUIS_CLIENT_HPP
