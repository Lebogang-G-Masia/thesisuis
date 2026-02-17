#include <netinet/in.h>
#include <string>
#include <vector>

#ifndef THESISUIS_SERVER_HPP
#define THESISUIS_SERVER_HPP

enum Commands {

};

namespace Thesisuis {
    class Server  {
        private:
            std::vector<std::string> commands {};
            // Authenticate user
        public:
            Server();
            bool authenticate(int);
            std::string hash_password(const std::string&);
            void addUser(std::string, std::string, bool=false);
            // Create the socket
            int createSocket();
            // Bind the socket to an IP and port
            sockaddr_in bindSocket(int);
            // Listen for a connection
            void listenForConnection(int);
            // Accept connection
            int acceptConnection(int);
            // Close sockets
            void closeSockets(int, int);
            // Handle multiple clients
            void handleClients(int, Server&);
    };
}

#endif // THESISUIS_SERVER_HPP
