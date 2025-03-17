| **Function**      | **Description** |
|------------------|---------------|
| `socket`        | Creates a new socket for communication. |
| `close`         | Closes an open file descriptor (including sockets). |
| `setsockopt`    | Sets options on a socket (e.g., timeout, buffer size). |
| `getsockname`   | Retrieves the local address and port of a socket. |
| `getprotobyname` | Gets the protocol number for a given protocol name (e.g., "tcp"). |
| `gethostbyname` | Resolves a hostname to an IP address (deprecated, use `getaddrinfo`). |
| `getaddrinfo`   | Resolves a hostname and service to address structures for use with sockets. |
| `freeaddrinfo`  | Frees memory allocated by `getaddrinfo`. |
| `bind`          | Assigns a local address to a socket. |
| `connect`       | Establishes a connection to a remote socket. |
| `listen`        | Marks a socket as a passive socket to accept incoming connections. |
| `accept`        | Accepts an incoming connection on a listening socket. |
| `htons`         | Converts a short integer (16-bit) from host byte order to network byte order. |
| `htonl`         | Converts a long integer (32-bit) from host byte order to network byte order. |
| `ntohs`         | Converts a short integer from network byte order to host byte order. |
| `ntohl`         | Converts a long integer from network byte order to host byte order. |
| `inet_addr`     | Converts an IPv4 address in dotted-decimal format to a network byte order integer. |
| `inet_ntoa`     | Converts an IPv4 address from network byte order to a dotted-decimal string. |
| `send`         | Sends data over a socket. |
| `recv`         | Receives data from a socket. |
| `signal`       | Sets up a simple signal handler. |
| `sigaction`    | Sets up an advanced signal handler with more options. |
| `lseek`        | Moves the read/write file offset. |
| `fstat`        | Gets file status information (e.g., size, type). |
| `fcntl`        | Modifies file descriptor properties (e.g., non-blocking mode). |
| `poll`         | Monitors multiple file descriptors for readiness to read/write. |
