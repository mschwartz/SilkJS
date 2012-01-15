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

static JSVAL net_connect(JSARGS args) {
	HandleScope scope;
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
        return True();
    }
	return scope.Close(Integer::New(fd));
}

static JSVAL net_listen(JSARGS args) {
    HandleScope scope;
	int port = args[0]->IntegerValue();
	int backlog = 30;
	if (args.Length() > 1) {
		backlog = args[1]->IntegerValue();
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
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (struct sockaddr *) &my_addr, sizeof (my_addr))) {
		return ThrowException(String::Concat(String::New("bind()Error: "), String::New(strerror(errno))));
	}
	if (listen(sock, backlog)) {
		return ThrowException(String::Concat(String::New("listen() Error: "), String::New(strerror(errno))));
	}
	return scope.Close(Integer::New(sock));
}

static JSVAL net_accept(JSARGS args) {
    HandleScope scope;
	struct sockaddr_in their_addr;
	
	int sock = args[0]->IntegerValue();
	
	socklen_t sock_size = sizeof (struct sockaddr_in);
	bzero(&their_addr, sizeof (their_addr));

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(sock, &fds);
//	struct timeval timeout;
//	timeout.tv_sec = 5;
//	timeout.tv_usec = 0;
	switch (select(sock+1, &fds, NULL, NULL, NULL)) {
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
//	int yes = 1;
//#ifdef USE_CORK
//	setsockopt( sock, IPPROTO_TCP, TCP_CORK, (char *)&yes, sizeof(yes) );
//#else
//	setsockopt( sock, IPPROTO_TCP, TCP_NODELAY, (char *)&yes, sizeof(yes) );
//#endif
//	{
//		int x;
//		x = fcntl(sock, F_GETFL, 0);
//		fcntl(sock, F_SETFL, x | O_NONBLOCK);
//	}
	strcpy(remote_addr, inet_ntoa(their_addr.sin_addr));

	return scope.Close(Integer::New(sock));
}

static JSVAL net_remote_addr(JSARGS args) {
	HandleScope scope;
	return scope.Close(String::New(remote_addr));
}

static JSVAL net_cork(JSARGS args) {
    HandleScope scope;
	int fd = args[0]->IntegerValue();
	int flag = args[1]->IntegerValue();
	setsockopt(fd, IPPROTO_TCP, TCP_CORK, &flag, sizeof(flag));
	return Undefined();
}
static JSVAL net_close(JSARGS args) {
    HandleScope scope;
	int fd = args[0]->IntegerValue();
	close(fd);
	return Undefined();
}

static JSVAL net_read(JSARGS args) {
    HandleScope scope;
	int fd = args[0]->IntegerValue();
	long size = args[1]->IntegerValue();
	
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	switch (select(fd+1, &fds, NULL, NULL, &timeout)) {
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
	return scope.Close(s);
}

static JSVAL net_write(JSARGS args) {
    HandleScope scope;
	int fd = args[0]->IntegerValue();
	String::Utf8Value buf(args[1]);
	long size = args[2]->IntegerValue();
	long written = 0;
	char *s = *buf;
	while (size > 0) {
		long count = write(fd, s, size);
		if (count < 0) {
			return ThrowException(String::Concat(String::New("Write Error: "), String::New(strerror(errno))));
		}
		size -= count;
		s += count;
		written += count;;
	}
	return scope.Close(Integer::New(written));
}

static JSVAL net_writebuffer(JSARGS args) {
	HandleScope scope;
	int fd = args[0]->IntegerValue();
	Local<External>wrap = Local<External>::Cast(args[1]);
	Buffer *buf = (Buffer *)wrap->Value();
	
	long size = buf->length();
	long written = 0;
	unsigned char *s = buf->data();
	while (size > 0) {
		long count = write(fd, s, size);
		if (count < 0) {
			return ThrowException(String::Concat(String::New("Write Error: "), String::New(strerror(errno))));
		}
		size -= count;
		s += count;
		written += count;;
	}
	int flag = 0;
	setsockopt(fd, IPPROTO_TCP, TCP_CORK, &flag, sizeof(flag));
	flag = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_CORK, &flag, sizeof(flag));
	return scope.Close(Integer::New(written));
}
/**
 * 
 * @param sock
 * @param filename
 * @param offset
 * @param size
 * @return 
 */
static JSVAL net_sendfile(JSARGS args) {
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
			return handle_scope.Close(False());
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
	setsockopt(fd, IPPROTO_TCP, TCP_CORK, &flag, sizeof(flag));
	flag = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_CORK, &flag, sizeof(flag));

    return Undefined();
}


void init_net_object() {
	HandleScope scope;
	
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

	builtinObject->Set(String::New("net"), net);
}
