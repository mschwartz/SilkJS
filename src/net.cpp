/**
 * @module builtin/net
 * 
 * ### Synopsis
 * SilkJS builtin net object.
 * 
 * ### Description
 * The builtin/net object provides low-level access to the OS networking functions.
 * 
 * ### Usage
 * var net = require('builtin/net');
 * 
 * ### See Also:
 * Operating system man pages
 */
#include "SilkJS.h"

#define USE_CORK
#ifdef __APPLE__
//#define TCP_CORK TCP_NOPUSH
#define TCP_CORK TCP_NODELAY
#endif

//#undef USE_CORK

// net.nonblock(sock)
// net.cork(flag)
// net.select(fd_array)

static char remote_addr[16];

/**
 * @function net.connect
 * 
 * ### Synopsis
 * var sock = net.connect(host, port);
 * 
 * This function creates a socket and connects to the specified host and port.
 * 
 * @param {string} host - name of host to connect to
 * @param {int} port - port number to connect to
 * @return {int} sock - file descriptor or false if error occurred.
 */
static JSVAL net_connect (JSARGS args) {
    String::Utf8Value host(args[0]);
    int port = args[1]->IntegerValue();
    struct hostent *h = gethostbyname(*host);
    if (h == NULL) {
        /* gethostbyname returns NULL on error */
        herror("gethostbyname failed");
        return False();
    }

    struct sockaddr_in sock_addr;

    /* memcpy(dest, src, length) */
    memcpy(&sock_addr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    /* copy the address to the sockaddr_in struct. */

    /* set the family type (PF_INET) */
    sock_addr.sin_family = h->h_addrtype;

    /*
     * addr->sin_port = port won't work because they are different byte
     * orders
     */
    sock_addr.sin_port = htons(port);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return False();
    }
    if (connect(fd, (struct sockaddr *) &sock_addr, sizeof (struct sockaddr_in)) < 0) {
        /* connect returns -1 on error */
        perror("connect(...) error");
        close(fd);
        return False();
    }
    return Integer::New(fd);
}

/**
 * @function net.listen
 * 
 * ### Synopsis
 * 
 * var sock = net.listen(port);
 * var sock = net.listen(port, backlog);
 * var sock = net.listen(port, backlog, ip);
 * 
 * This function creates a TCP SOCK_STREAM socket, binds it to the specified port, and does a listen(2) on the socket.
 * 
 * Connections to the socket from the outside world can be accepted via net.accept().
 * 
 * The backlog argument specifies the maximum length for the queue of pending connections.  If the queue fills and another connection attempt is made, the client will likely receive a "connection refused" error.
 * 
 * The ip argument specifies what IP address to listen on.  By default, it will be 0.0.0.0 for "listen on any IP."  If you set this to a different value, only that IP will be listened on, and the socket will not be reachable via localhost (for example).
 * 
 * @param {int} port - port number to listen on
 * @param {int} backlog - length of pending connection queue
 * @param {string} ip - ip address to listen on
 * @return {int} sock - file descriptor of socket in listen mode
 * 
 * ### Exceptions
 * This function throws an exception of the socket(), bind(), or listen() OS calls fail.
 */
static JSVAL net_listen (JSARGS args) {
    int port = args[0]->IntegerValue();
    int backlog = 30;
    if (args.Length() > 1) {
        backlog = args[1]->IntegerValue();
    }
    int listenAddress = INADDR_ANY;
//    char *listenAddressString = (char *)"0.0.0.0";
    if (args.Length() > 2) {
        String::AsciiValue addr(args[2]);
//        listenAddressString = *addr;
        listenAddress = inet_addr(*addr);
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return ThrowException(String::Concat(String::New("socket() Error: "), String::New(strerror(errno))));
    }
    {
        int on = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof (on));
    }
    struct sockaddr_in my_addr;
    bzero(&my_addr, sizeof (my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
//    printf("listenAddress: '%s' %08x\n", listenAddressString, listenAddress);
    my_addr.sin_addr.s_addr = listenAddress; // htonl(listenAddress);
    if (bind(sock, (struct sockaddr *) &my_addr, sizeof (my_addr))) {
        return ThrowException(String::Concat(String::New("bind()Error: "), String::New(strerror(errno))));
    }
    if (listen(sock, backlog)) {
        return ThrowException(String::Concat(String::New("listen() Error: "), String::New(strerror(errno))));
    }
    return Integer::New(sock);
}

/**
 * @function net.accept
 * 
 * ### Synopsis
 * 
 * var sock = net.accept(listen_socket);
 * 
 * This function waits until there is an incoming connection to the listen_socket and returns a new socket directly connected to the client.
 * 
 * The IP address of the connecting client is stored by this function.  It may be retrieved by calling net.remote_addr().
 * 
 * @param {int} listen_socket - socket already in listen mode
 * @return {int} client_socket - socket connected to a client
 * 
 * ### Notes
 * There is an old and well-known issue known as the "thundering herd problem" involving the OS accept() call.  The problem may occur if there are a large number of processes calling accept() on the same listen_socket. 
 * 
 * The solution is to use some sort of semaphore such as fs.flock() or fs.lockf() around net.accept().
 * 
 * See http://en.wikipedia.org/wiki/Thundering_herd_problem for a brief description of the problem.
 */
static JSVAL net_accept (JSARGS args) {
    struct sockaddr_in their_addr;

    int sock = args[0]->IntegerValue();

    socklen_t sock_size = sizeof (struct sockaddr_in);
    bzero(&their_addr, sizeof (their_addr));

#if 0
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(sock, &fds);
    //	struct timeval timeout;
    //	timeout.tv_sec = 5;
    //	timeout.tv_usec = 0;
    switch (select(sock + 1, &fds, NULL, NULL, NULL)) {
        case -1:
            perror("select");
            return ThrowException(String::Concat(String::New("Read Error: "), String::New(strerror(errno))));
        case 0:
            printf("select timed out\n");
            return Null();
    }

    while (1) {
        sock = accept(sock, (struct sockaddr *) &their_addr, &sock_size);
        if (sock > 0) {
            break;
        }
        else {
            perror("accept");
        }
    }
#else
    sock = accept(sock, (struct sockaddr *) &their_addr, &sock_size);
#endif
    //	int yes = 1;
    //#ifdef USE_CORK
    //	setsockopt( sock, IPPROTO_TCP, TCP_CORK, (char *)&yes, sizeof(yes) );
    //#else
    //	setsockopt( sock, IPPROTO_TCP, TCP_NODELAY, (char *)&yes, sizeof(yes) );
    //#endif
//    	{
 //   		int x;
  //  		x = fcntl(sock, F_GETFL, 0);
   // 		fcntl(sock, F_SETFL, x | O_NONBLOCK);
    //	}
    strcpy(remote_addr, inet_ntoa(their_addr.sin_addr));

    return Integer::New(sock);
}

static JSVAL net_readReady(JSARGS args) {
    int sock = args[0]->IntegerValue();
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(sock, &fds);
     struct timeval timeout;
     timeout.tv_sec = 0;
     timeout.tv_usec = 0;
     switch (select(sock+1, &fds, NULL, NULL, &timeout)) {
        case -1:
            perror("select");
            return ThrowException(String::Concat(String::New("Read Error: "), String::New(strerror(errno))));
        case 0:
            return False();
     }
     return True();
}

/**
 * @function net.remote_addr
 * 
 * ### Synopsis
 * 
 * var remote_ip = net.remote_addr();
 * 
 * This function returns the IP address of the last client to connect via net.accept().
 * 
 * @return {string} remote_ip - ip address of client
 */
static JSVAL net_remote_addr (JSARGS args) {
    return String::New(remote_addr);
}

/**
 * @function net.cork
 * 
 * ### Synopsis
 * 
 * net.cork(sock, flag);
 * 
 * This function sets or clears the Linux TCP_CORK flag on the specified socket, based upon the value of flag.
 * 
 * ### Description
 * 
 * There are two issues to consider when writing to a TCP socket.
 * 
 * TCP implements the Nagle Algorithm which is on by default for sockets.  The idea is that for connections like telnet, sending a packet per keystroke is wasteful.  The Nagle Algorithm causes a 250ms delay before sending a packet after write, in case additional writes (keystrokes) might occur.  A 250ms delay from net.write() to actual data going out to the network is a performance killer for high transaction applications (e.g. HTTP).
 * 
 * Turning off Nagle eliminates the 250ms delay, but in an HTTP type application, the headers will get sent in the first (and maybe additional) packets, then the data in succeeding packets.  It is ideal to pack all the headers and data into full packets before teh data is written.
 * 
 * Linux implements TCP_CORK to solve this issue.  If set, don't send out partial frames. All queued partial frames are sent when the option is cleared again. This is useful for prepending headers before calling sendfile(2), or for throughput optimization. As currently implemented, there is a 200 millisecond ceiling on the time for which output is corked by TCP_CORK. If this ceiling is reached, then queued data is automatically transmitted. This option can be combined with TCP_NODELAY only since Linux 2.5.71. This option should not be used in code intended to be portable.
 * 
 * OSX does not implement TCP_CORK, so we do our best with TCP_NODELAY (disable Nagle).
 * 
 * @param {int} sock - socket to perform TCP_CORK on
 * @param {boolean} flag - true to turn on TCP_CORK, false to turn it off
 */
static JSVAL net_cork (JSARGS args) {
    int fd = args[0]->IntegerValue();
    int flag = args[1]->IntegerValue();
    setsockopt(fd, IPPROTO_TCP, TCP_CORK, &flag, sizeof (flag));
    return Undefined();
}

/**
 * @function net.close
 * 
 * ### Synopsis
 * 
 * net.close(sock);
 * 
 * This function closes a network socket and frees any memory it uses.  You should call this when done with your sockets, or you may suffer a memory leak.
 * @param {int} socket to close
 */
static JSVAL net_close (JSARGS args) {
    int fd = args[0]->IntegerValue();
    close(fd);
    return Undefined();
}

/**
 * @function net.read
 * 
 * ### Synopsis
 * 
 * var s = net.read(sock, length);
 * 
 * This function reads a string of the specified maximum length from the specified socket.  The actual length of the string returned may be less than length characters.
 * 
 * If no data can be read for 5 seconds, the function returns null.
 * 
 * @param {int} sock - socket file descriptor to read from.
 * @param {int} length - maximum length of string to read.
 * @return {string} s - string that was read from the socket, or null if the string could not be read.
 * 
 * ### Exceptions
 * This function throws an exception if there is a read error with the error message.
 */
static JSVAL net_read (JSARGS args) {
    int fd = args[0]->IntegerValue();
    long size = args[1]->IntegerValue();

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    switch (select(fd + 1, &fds, NULL, NULL, &timeout)) {
        case -1:
            perror("select");
            return ThrowException(String::Concat(String::New("Read Error: "), String::New(strerror(errno))));
        case 0:
            printf("Read timed out\n");
            return Null();
    }

    char buf[size];
    long count = read(fd, buf, size);
    if (count < 0) {
        return ThrowException(String::Concat(String::New("Read Error: "), String::New(strerror(errno))));
    }
    else if (count == 0) {
        return Null();
    }
    Handle<String>s = String::New(buf, count);
    return s;
}

/**
 * @function net.write
 * 
 * ### Synopsis
 * 
 * var written = net.write(sock, s, size);
 * 
 * This function writes size characters from string s to the specified socket.
 * 
 * @param {int} sock - file descriptor of socket to write to.
 * @param {string} s - the string to write.
 * @param {int} length - number of bytes to write.
 * @return {int} written - number of bytes actually written.
 * 
 * ### Exceptions
 * This function throws an exception with a string describing any write errors.
 */
static JSVAL net_write (JSARGS args) {
    int fd = args[0]->IntegerValue();
    String::Utf8Value buf(args[1]);
    long size = args[2]->IntegerValue();
    long written = 0;
    char *s = *buf;
    while (size > 0) {
        long count = write(fd, s, size);
        if (count <= 0) {
            return ThrowException(String::Concat(String::New("Write Error: "), String::New(strerror(errno))));
        }
        size -= count;
        s += count;
        written += count;
    }
    return Integer::New(written);
}

/**
 * @function net.writeBuffer
 * 
 * ### Synopsis
 * 
 * var written = net.writeBuffer(sock, buffer);
 * 
 * This function attempts to write the given buffer to the specified socket.
 * 
 * A buffer is an object to JavaScript that provides growable string functionality.  While it's true that JavaScript strings are growable in their own right, sometimes buffering data in C++ space is more optimal if you can avoid an extra string copy.
 * 
 * Buffers also have the flexibility to write a base64 encoded string to its memory as binary data.
 * 
 * @param {int} sock - file descriptor of socket to write to.
 * @param {object} buffer - buffer object to write to the socket.
 * @return {int} written - number of bytes written.
 * 
 * ### Exceptions
 * 
 * This function throws an exception if there is a write error.
 * 
 * ### Notes
 * 
 * After the buffer is written, TCP_CORK is toggled off then on to force the data to be written to the network.
 * 
 * ### See also
 * builtin/buffer
 * 
 */
static JSVAL net_writebuffer (JSARGS args) {
    int fd = args[0]->IntegerValue();
    Buffer *buf = (Buffer *)JSOPAQUE(args[1]);

    long size = buf->length();
    long written = 0;
    unsigned char *s = buf->data();
    while (size > 0) {
        long count = write(fd, s, size);
        if (count < 0) {
            return ThrowException(String::Concat(String::New("Write Error: "), String::New(strerror(errno))));
        }
		else if (count == 0) {
			return ThrowException(String::New("Write Error: End of File"));
		}
        size -= count;
        s += count;
        written += count;
    }
    int flag = 0;
    setsockopt(fd, IPPROTO_TCP, TCP_CORK, &flag, sizeof (flag));
    flag = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_CORK, &flag, sizeof (flag));
    return Integer::New(written);
}

/**
 * @function net.sendfile
 * 
 * ### Synopsis
 * 
 * net.sendFile(sock, path);
 * net.sendFile(sock, path, offset);
 * net.sendFile(sock, path, offset, size);
 * 
 * This function calls the OS sendfile() function to send a complete or partial file to the network entirely within kernel space.  It is a HUGE speed win for HTTP and FTP type servers.
 * 
 * @param {int} sock - file descriptor of socket to send the file to.
 * @param {string} path - file system path to file to send.
 * @param {int} offset - offset from beginning of file to send (for partial).  If omitted, the entire file is sent.
 * @param {int} size - number of bytes of the file to send.  If omitted, the remainder of the file is sent (or all of it).
 * 
 * ### Exceptions
 * An exception is thrown if the file cannot be opened or if there is a sendfile(2) OS call error.
 */
static JSVAL net_sendfile (JSARGS args) {
    HandleScope handle_scope;
    int sock = args[0]->IntegerValue();
    String::AsciiValue filename(args[1]);
    off_t offset = 0;
    if (args.Length() > 2) {
        offset = args[2]->IntegerValue();
    }
    size_t size;
    if (args.Length() > 3) {
        size = args[3]->IntegerValue();
    }
    else {
        struct stat buf;
        if (stat(*filename, &buf)) {
            printf("%s\n", *filename);
            perror("SendFile stat");
            return False();
        }
        size = buf.st_size - offset;
    }
    int fd = open(*filename, O_RDONLY);
    if (fd < 0) {
        return ThrowException(String::Concat(String::New("sendFile open Error: "), String::New(strerror(errno))));
    }

    while (size > 0) {
#ifdef __APPLE__
        off_t count = size;
        if (sendfile(fd, sock, offset, &count, NULL, 0) == -1) {
            close(fd);
            return ThrowException(String::Concat(String::New("sendFile Error: "), String::New(strerror(errno))));
        }
#else
        ssize_t count = sendfile(sock, fd, &offset, size);
        if (count == -1) {
            close(fd);
            return ThrowException(String::Concat(String::New("sendFile Error: "), String::New(strerror(errno))));
        }
#endif
        size -= count;
        offset += count;
    }
    close(fd);
    int flag = 0;
    setsockopt(fd, IPPROTO_TCP, TCP_CORK, &flag, sizeof (flag));
    flag = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_CORK, &flag, sizeof (flag));

    return Undefined();
}

static JSVAL net_socketpair(JSARGS args) {
    int sv[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
        return False();
    }
    JSARRAY a = Array::New();
    a->Set(0, Integer::New(sv[0]));
    a->Set(1, Integer::New(sv[1]));
    return a;
}

void init_net_object () {
    Handle<ObjectTemplate>net = ObjectTemplate::New();
    net->Set(String::New("connect"), FunctionTemplate::New(net_connect));
    net->Set(String::New("listen"), FunctionTemplate::New(net_listen));
    net->Set(String::New("accept"), FunctionTemplate::New(net_accept));
    net->Set(String::New("remote_addr"), FunctionTemplate::New(net_remote_addr));
    net->Set(String::New("cork"), FunctionTemplate::New(net_cork));
    net->Set(String::New("close"), FunctionTemplate::New(net_close));
    net->Set(String::New("read"), FunctionTemplate::New(net_read));
    net->Set(String::New("write"), FunctionTemplate::New(net_write));
    net->Set(String::New("writeBuffer"), FunctionTemplate::New(net_writebuffer));
    net->Set(String::New("sendFile"), FunctionTemplate::New(net_sendfile));
    net->Set(String::New("socketpair"), FunctionTemplate::New(net_socketpair));
    net->Set(String::New("readReady"), FunctionTemplate::New(net_readReady));
    builtinObject->Set(String::New("net"), net);
}
