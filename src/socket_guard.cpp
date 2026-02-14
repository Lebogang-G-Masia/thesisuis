#include "../include/socket_guard.hpp"

using namespace Thesisuis;

SocketGuard::~SocketGuard() {
    if (sock_fd != -1) {
        close(sock_fd);
    }
}

SocketGuard& SocketGuard::operator=(SocketGuard&& other) noexcept {
    if (this != &other) {
        if (sock_fd == -1) close(sock_fd);
        sock_fd = std::exchange(other.sock_fd, -1);
    }
    return *this;
}

int SocketGuard::get() const { return sock_fd; }

SocketGuard::operator bool() const { return sock_fd != -1; };
