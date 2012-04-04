/**
 *  @module builtin.SSH2
 * 
 * ### Synopsis
 * 
 * var ssh = require('builtin/ssh2');
 * 
 * SSH and SCP interface.
 * 
 * ### Example
 * 
 * ```
 * var ssh = require('builtin/ssh2');
 * var conn = ssh.connect('somehost', 'someuser', 'somepassword');
 * ssh.exec(conn, 'ls -l');
 * ```
 * 
 */
#include "SilkJS.h"
#include <libssh2.h>

class SSH2 {
public:
	LIBSSH2_SESSION *mSession;
private:
	int mSock;
	bool mSessionOK;
	char mErrorMessage[1024];
	int mExitCode;
	char *mResponse;
	long mTimeout;
public:
	SSH2(const char *aHostName, const char *aUserName, const char *aPassword, const int aPortNumber = 22) {
		mSock = -1;
		mSessionOK = false;
		mResponse = NULL;
		mTimeout = 10000;
		strcpy(mErrorMessage, "");

		// connect
		struct hostent *hp = gethostbyname(aHostName);
		if (!hp) {
			strcpy(mErrorMessage, "Failed to get IP address of server");
			return;
		}
		struct sockaddr_in s;
		s.sin_addr = *(struct in_addr *)hp->h_addr_list[0];
		s.sin_family = hp->h_addrtype;
		s.sin_port = htons(aPortNumber);
		mSock = socket(AF_INET, SOCK_STREAM, 0);
		if (mSock < 0) {
			sprintf(mErrorMessage, "Failed to create socket: %s", strerror(errno));
			return;
		}
		if (connect(mSock, (struct sockaddr *)&s, sizeof(s)) < 0) {
			sprintf(mErrorMessage, "Failed to connect: %s", strerror(errno));
			close(mSock);
			mSock = -1;
			return;
		}
		mSession = libssh2_session_init();
		if (libssh2_session_startup(mSession, mSock)) {
			strcpy(mErrorMessage, "Could not initialize SSH2 session");
			close(mSock);
			mSock = -1;
			return;
		}
		if (libssh2_userauth_password(mSession, aUserName, aPassword)) {
			strcpy(mErrorMessage, "Invalid credentials");
			close(mSock);
			mSock = -1;
			return;
		}
		mSessionOK = true;
	}
	~SSH2() {
		if (mSessionOK) {
			libssh2_session_disconnect(mSession, "Goodbye from SilkJS");
			libssh2_session_free(mSession);
		}
		if (mSock > 0) {
			close(mSock);
			mSock = -1;
		}
#ifdef libssh2_exit
		libssh2_exit();
#endif
	}
public:
	bool Alive() {
		return mSock != -1;
	}
	char *ErrorMessage() {
		return mErrorMessage;
	}
	void SetTimeout(long aTimeout) {
		mTimeout = aTimeout;
	}
	int GetExitCode() {
		return mExitCode;
	}
	char *GetResponse() {
		return mResponse;
	}
public:
	bool RunCommand(const char *aCommand) {
		if (!mSessionOK) {
			return false;
		}
		LIBSSH2_CHANNEL *channel = libssh2_channel_open_session(mSession);
		if (channel == NULL) {
			strcpy(mErrorMessage, "Could not open SSH2 channel");
			return false;
		}
#ifdef libssh2_session_set_timeout
		libssh2_session_set_timeout(mTimeout);
#endif
		if (libssh2_channel_exec(channel, aCommand) == -1) {
			strcpy(mErrorMessage, "Could not execute SSH2 command");
			return false;
		}
		std::string response;
		char buf[1025];
		ssize_t size = 1024;
		while (size == 1024) {
			size = libssh2_channel_read(channel, buf, 1024);
			buf[size] = '\0';
			response += buf;
		}
		if (mResponse) {
			delete [] mResponse;
		}
		mResponse = new char[response.size()+1];
		strcpy(mResponse, response.c_str());
		libssh2_channel_close(channel);
		mExitCode = libssh2_channel_get_exit_status(channel);
		libssh2_channel_free(channel);
		return true;
	}
	
};

static SSH2 *HANDLE(Handle<Value> arg) {
	return (SSH2 *)JSEXTERN(arg);
}

/**
 * @function ssh.connect
 * 
 * ### Synopsis
 * 
 * var conn = ssh.connect(host, username, password);
 * var conn = ssh.connect(host, username, password, port);
 * 
 * Connect to a remote host via SSH.
 * 
 * @param {string} host - name of remote host.
 * @param {string} username - username for password authentication at remote host.
 * @param {string} password - password for authentication at remote host.
 * @param {int} port - SSH port at remote host (defaults to 21)..
 * @return {object} conn - connection ready to use for other methods.
 */
static JSVAL ssh2_connect(JSARGS args) {
    HandleScope scope;
	String::Utf8Value host(args[0]);
	String::Utf8Value username(args[1]);
	String::Utf8Value password(args[2]);
	int port = 22;
	if (args.Length() > 3) {
		port = args[3]->IntegerValue();
	}
	SSH2 *ssh2 = new SSH2(*host, *username, *password, port);
	return scope.Close(External::New(ssh2));
}

/**
 * @function ssh.alive
 * 
 * ### Synopsis
 * 
 * var is_alive = ssh.alive(conn);
 * 
 * Determine if SSH connection is alive.
 * 
 * @param {object} conn - connection returned from ssh.connect()
 * @return {boolean} is_alive - true if connection is alive.
 */
static JSVAL ssh2_alive(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	SSH2 *ssh2 = (SSH2 *)wrap->Value();
	return scope.Close(Boolean::New(ssh2->Alive()));
}

/**
 * @function ssh.error
 * 
 * ### Synopsis
 * 
 * var msg = ssh.error(conn);
 * 
 * Get last SSH error message.
 * 
 * @param {object} conn - connection returned from ssh.connect()
 * @return {string} msg - last SSH error message.
 */
static JSVAL ssh2_error(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	SSH2 *ssh2 = (SSH2 *)wrap->Value();
	return scope.Close(String::New(ssh2->ErrorMessage()));
}

/**
 * @function ssh.close
 * 
 * ### Synopsis
 * 
 * ssh.close(conn);
 * 
 * Close an SSH connection, free all resources used.
 * 
 * @param {object} conn - connection returned from ssh.connect()
 */
static JSVAL ssh2_close(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	SSH2 *ssh2 = (SSH2 *)wrap->Value();
	delete ssh2;
	return scope.Close(Undefined());
}

/**
 * @function ssh2.timeout
 * 
 * ### Synopsis
 * 
 * ssh.timeout(conn, msec);
 * 
 * Set the timeout in milliseconds for how long a blocking the SSH function calls may wait until they consider the situation an error and return a timeout error code.
 *
 * By default or if you set the timeout to zero, there will be no timeout for blocking functions
 * 
 * @param {object} conn - connection returned from ssh.connect()
 * @param {int} msec - milliseconds to set timeout value to
 */
static JSVAL ssh2_timeout(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	SSH2 *ssh2 = (SSH2 *)wrap->Value();
	ssh2->SetTimeout(args[1]->IntegerValue());
	return scope.Close(Undefined());
}

/**
 * @function ssh2.exec
 * 
 * ### Synopsis
 * 
 * var success = ssh2.exec(conn, command);
 * 
 * Execute command at remote host.
 * 
 * @param {object} conn - connection returned from ssh.connect()
 * @param {string} command - command line to execute on remote host
 * @return {boolean} success - true if the command was successfuly executed.
 */
static JSVAL ssh2_exec(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	SSH2 *ssh2 = (SSH2 *)wrap->Value();
	String::Utf8Value command(args[1]);
	if (!ssh2->RunCommand(*command)) {
		return scope.Close(False());
	}
	return scope.Close(True());
}

/**
 * @function ss2.exit_code
 * 
 * ### Synopsis
 * 
 * var code = ssh.exit_code(conn);
 * 
 * Get the return code from the last executed remote command.  This is equivalent to the argument that command passed to exit().
 * 
 * @param {object} conn - connection returned from ssh.connect()
 * @return {int} code - exit code from remote program
 */
static JSVAL ssh2_exit_code(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	SSH2 *ssh2 = (SSH2 *)wrap->Value();
	return scope.Close(Integer::New(ssh2->GetExitCode()));
}

/**
 * @function ssh.response
 * 
 * ### Synopsis
 * 
 * var text = ssh.response(conn);
 * 
 * Get output (stdout) of remote command execution as a string.  If the output is multiple lines, the returned string will be multiple lines separated by newline.
 * 
 * @param {object} conn - connection returned from ssh.connect()
 * @return {string} text - output of remote command.
 */
static JSVAL ssh2_response(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	SSH2 *ssh2 = (SSH2 *)wrap->Value();
	return scope.Close(String::New(ssh2->GetResponse()));
}

/**
 * @function ssh.writeFile
 * 
 * ### Synopsis
 * 
 * var status = ssh.writeFile(handle, srcPath, dstPath);
 * var status = ssh.writeFile(handle, srcPath, dstPath, mode);
 * 
 * Write file to remote server via SCP.
 * 
 * @param {object} handle - opaque handle to already open SSH2 connection.
 * @param {string} srcPath - path to file in local file system to send.
 * @param {string} dstPath - path to file in remote file system to create.
 * @param {int} mode - desired resulting file permissions of file on remote end.
 * @return {boolean} success - true if the transfer succeeded, string error message if transfer failed.
 * 
 * ### Note
 * If mode is not provided, the file mode of the file being sent will be used.
 */
static JSVAL ssh2_scp_send(JSARGS args) {
	HandleScope scope;
    SSH2 *ssh2 = HANDLE(args[0]);
	String::Utf8Value srcPath(args[1]);
	String::Utf8Value dstPath(args[2]);
    int mode;
    struct stat fileinfo;
    if (stat(*srcPath, &fileinfo) != 0) {
        return scope.Close(String::New(strerror(errno)));
    }
    if (args.Length() > 3) {
        mode = args[3]->IntegerValue();
    }
    else {
        mode = fileinfo.st_mode;
    }
    mode &= 0777;
    int fd = open(*srcPath, O_RDONLY);
    if (fd < 0) {
        return scope.Close(String::New(strerror(errno)));
    }
#ifdef libssh2_scp_send64
    LIBSSH2_CHANNEL *channel = libssh2_scp_send64(ssh2->mSession, *dstPath, mode, fileinfo.st_size, 0, 0);
#else
    LIBSSH2_CHANNEL *channel = libssh2_scp_send(ssh2->mSession, *dstPath, mode, fileinfo.st_size);
#endif
    if (!channel) {
        char *errmsg;
        int errlen;
        libssh2_session_last_error(ssh2->mSession, &errmsg, &errlen, 0);
        return scope.Close(String::New(errmsg, errlen));
    }
    
    char mem[1024];
    ssize_t toWrite = fileinfo.st_size;
    while (toWrite > 0) {
        ssize_t nRead = read(fd, mem, 1024);
        if (nRead < 0) {
            int eNum = errno;
            libssh2_channel_free(channel);
            close(fd);
            return scope.Close(String::New(strerror(eNum)));
        }
        int rc = libssh2_channel_write(channel, mem, nRead);
        if (rc < 0) {
            char *errmsg;
            int errlen;
            libssh2_session_last_error(ssh2->mSession, &errmsg, &errlen, 0);
            libssh2_channel_free(channel);
            close(fd);
            return scope.Close(String::New(errmsg));
        }
        toWrite -= nRead;
    }
    close(fd);
    libssh2_channel_free(channel);
    return scope.Close(True());
}

void init_ssh_object() {
	HandleScope scope;
	
	Handle<ObjectTemplate>ssh  = ObjectTemplate::New();
	ssh->Set(String::New("connect"), FunctionTemplate::New(ssh2_connect));
	ssh->Set(String::New("alive"), FunctionTemplate::New(ssh2_alive));
	ssh->Set(String::New("error"), FunctionTemplate::New(ssh2_error));
	ssh->Set(String::New("close"), FunctionTemplate::New(ssh2_close));
	ssh->Set(String::New("timeout"), FunctionTemplate::New(ssh2_timeout));
	ssh->Set(String::New("exec"), FunctionTemplate::New(ssh2_exec));
	ssh->Set(String::New("exit_code"), FunctionTemplate::New(ssh2_exit_code));
	ssh->Set(String::New("response"), FunctionTemplate::New(ssh2_response));
    ssh->Set(String::New("writeFile"), FunctionTemplate::New(ssh2_scp_send));

	builtinObject->Set(String::New("ssh2"), ssh);
}
