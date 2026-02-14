#include <unistd.h>
#include <utility>

#ifndef THESISUIS_SOCKET_GUARD
#define THESISUIS_SOCKET_GUARD

namespace Thesisuis {
    class SocketGuard {
        private:
            int sock_fd;
        public:
            explicit SocketGuard(int fd=-1) : sock_fd(fd) {}
            ~SocketGuard();

            // Disable copying
            SocketGuard(const SocketGuard&) = delete;
            SocketGuard& operator=(const SocketGuard&) = delete;
            // Enable moving
            SocketGuard(SocketGuard&& other) noexcept: sock_fd(std::exchange(other.sock_fd, -1)) {}
            SocketGuard& operator=(SocketGuard&& other) noexcept;
            
            int get() const;

            operator bool() const;
    };
}

#endif // THESISUIS_SOCKET_GUARD
