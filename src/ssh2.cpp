#include "SilkJS.h"
#include <libssh2.h>

class SSH2 {
private:
	int mSock;
	LIBSSH2_SESSION *mSession;
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
		libssh2_exit();
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

static JSVAL ssh2_alive(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	SSH2 *ssh2 = (SSH2 *)wrap->Value();
	return scope.Close(Boolean::New(ssh2->Alive()));
}

static JSVAL ssh2_error(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	SSH2 *ssh2 = (SSH2 *)wrap->Value();
	return scope.Close(String::New(ssh2->ErrorMessage()));
}

static JSVAL ssh2_close(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	SSH2 *ssh2 = (SSH2 *)wrap->Value();
	delete ssh2;
	return scope.Close(Undefined());
}

static JSVAL ssh2_timeout(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	SSH2 *ssh2 = (SSH2 *)wrap->Value();
	ssh2->SetTimeout(args[1]->IntegerValue());
	return scope.Close(Undefined());
}

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

static JSVAL ssh2_exit_code(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	SSH2 *ssh2 = (SSH2 *)wrap->Value();
	return scope.Close(Integer::New(ssh2->GetExitCode()));
}

static JSVAL ssh2_response(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	SSH2 *ssh2 = (SSH2 *)wrap->Value();
	return scope.Close(String::New(ssh2->GetResponse()));
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

	globalObject->Set(String::New("ssh2"), ssh);
}
