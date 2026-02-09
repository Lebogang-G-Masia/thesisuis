#include <sys/socket.h>
#include <arpa/inet.h>

#ifndef THESISUIS_CLIENT_HPP
#define THESISUIS_CLIENT_HPP

namespace Thesisuis{
    class Client {
        public:
            // create the socket
            int createSocket();
            // specify the server
            sockaddr_in getServerAddress();
            // connect to the server
            void connect(int, sockaddr_in);
            // Send data
            void send(int, const char*);
            // close socket
            void close();
    };
}

#endif // THESISUIS_CLIENT_HPP
