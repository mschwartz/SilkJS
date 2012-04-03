/**
 * @module builtin/ftp
 * 
 * ### Synopsis
 * 
 * var ftp = require('builtin/ftp');
 * 
 * Interface to ftplib, access to ftp functionality from SilkJS.
 */

#include "SilkJS.h"
#include <ftplib.h>

static inline netbuf *HANDLE(Handle<Value>v) {
    if (v->IsNull()) {
        ThrowException(String::New("Handle is NULL"));
        return NULL;
    }
    netbuf *nControl = (netbuf *)JSEXTERN(v);
    return nControl;
}


/**
 * @function ftp.init
 * 
 * ### Synopsis
 * 
 * ftp.init();
 * 
 * Performs any required initialization for the library.
 */
static JSVAL ftp_init(JSARGS args) {
    HandleScope scope;
    FtpInit();
    return Undefined();
}

/**
 * @function ftp.site
 * 
 * ### Synopsis
 * 
 * var success = ftp.site(handle, cmd);
 * 
 * @param {object} handle - handle returned by ftp.connect().
 * @param {string} cmd - a string containing a 'SITE' subcommand.
 * @return {int} success - 1 if successful or 0 on error.
 */
static JSVAL ftp_site(JSARGS args) {
    HandleScope scope;
    netbuf *n = HANDLE(args[0]);
	String::Utf8Value cmd(args[1]->ToString());
    return scope.Close(Integer::New(FtpSite(*cmd, n)));
}

/**
 * @function ftp.lastResponse
 * 
 * ### Synopsis
 * 
 * var response = ftp.lastResponse(handle);
 * 
 * Returns a pointer to the last response string sent by the server. This can be parsed by the user program to determine more information about the last request or can be displayed along with an error message.
 * 
 * @param {object} handle - handle returned by ftp.connect().
 * @return {string} response - last response string or NULL if handle is not valid.
 */
static JSVAL ftp_lastResponse(JSARGS args) {
    HandleScope scope;
    netbuf *n = HANDLE(args[0]);
    return scope.Close(String::New(FtpLastResponse(n)));
}

/**
 * @function ftp.sysType
 * 
 * ### Synopsis
 * 
 * var response = ftp.sysType(handle);
 * 
 * Determine system type of remote server.
 * 
 * Issues a SYST command to the remote system and attempts to parse the system type out of the response and returns it.
 * 
 * @param {object} handle - handle returned by ftp.connect().
 * @return {string} response - system type or false if an error occurred.
 */
static JSVAL ftp_systemType(JSARGS args) {
    HandleScope scope;
    netbuf *n = HANDLE(args[0]);
    char buf[2048];
    if (!FtpSysType(buf, 2048, n)) {
        return scope.Close(False());
    }
    return scope.Close(String::New(buf));
}

/**
 * @function ftp.size
 * 
 * var size = ftp.size(handle, path, mode);
 * 
 * Determine size of remote file.
 * 
 * Some servers may not support the SIZE command. 
 * 
 * If this request fails, the size may be available in the response to a RETR (ftp.open() with type=ftp.FILE_READ).
 * 
 * @param {object} handle - handle returned by ftp.connect().
 * @param {string} path - path to file to get size of.
 * @param {int} mode - either ftp.ASCII or ftp.BINARY.
 * @return {int} size - size of remote file or false if an error occurred.
 */
static JSVAL ftp_size(JSARGS args) {
    HandleScope scope;
    netbuf *n = HANDLE(args[0]);
	String::Utf8Value path(args[1]->ToString());
    char mode = (char)args[2]->IntegerValue();
    int size;
    if (FtpSize(*path, &size, mode, n)) {
        return scope.Close(Integer::New(size));
    }
    else {
        return False();
    }
}

/**
 * @function ftp.fileModified
 * 
 * var size = ftp.fileModified(handle, path);
 * 
 * Determine last modification time of remote file.  The date and time are returned as a string in the format 'YYYYMMDDHHMMSS'
 * 
 * Some servers may not support the MDTM command.
 * 
 * @param {object} handle - handle returned by ftp.connect().
 * @param {string} path - path to file to get modification time of.
 * @return {int} size - size of remote file or false if an error occurred.
 */
static JSVAL ftp_fileModified(JSARGS args) {
    HandleScope scope;
    netbuf *n = HANDLE(args[0]);
	String::Utf8Value path(args[1]->ToString());
    char buf[2048];
    if (!FtpModDate(*path, buf, 2048, n)) {
        return scope.Close(False());
    }
    return scope.Close(String::New(buf));
}

/**
 * @function ftp.connect
 * 
 * var size = ftp.connect(host);
 * 
 * Connect to an FTP server.
 * 
 * Establishes a connection to the FTP server on the specified machine and returns a handle which can be used to initiate data transfers. 
 * 
 * The host name should be specified as <host> or <host>:<port>. <host> may be either a host name or ip address. <port> may be either a service name or a port number.
 * 
 * @param {string} host - host to connect to.
 * @return {object} handle - handle to open connection, or false if an error occurred.
 */
static JSVAL ftp_connect(JSARGS args) {
    HandleScope scope;
    netbuf *n;
	String::Utf8Value host(args[0]->ToString());
    if (!FtpConnect(*host, &n)) {
        return scope.Close(False());
    }
    return scope.Close(External::New(n));
}

/**
 * @function ftp.login
 * 
 * var success = ftp.login(handle, username, password);
 * 
 * Login to remote system.
 * 
 * @param {object} handle - handle returned by ftp.connect().
 * @param {string} username - username on remote system.
 * @param {string} password - password on remote system.
 * @return {int} success - 1 if successful, 0 if an error occurred.
 */
static JSVAL ftp_login(JSARGS args) {
    HandleScope scope;
    netbuf *n = HANDLE(args[0]);
	String::Utf8Value username(args[1]->ToString());
	String::Utf8Value password(args[2]->ToString());
    return scope.Close(Integer::New(FtpLogin(*username, *password, n)));
}

/**
 * @function ftp.quit
 * 
 * ### Synopsis
 * 
 * ftp.quit(handle);
 * 
 * Closes the connection to the remote server and frees any resources associated with the connection.
 * 
 * @param {object} handle - handle returned by ftp.connect().
 */
static JSVAL ftp_quit(JSARGS args) {
    HandleScope scope;
    netbuf *n = HANDLE(args[0]);
    FtpQuit(n);
    return Undefined();
}

/**
 * @function ftp.pasv
 * 
 * var success = ftp.pasv(handle);
 * 
 * Set connection to PASV mode.
 * 
 * @param {object} handle - handle returned by ftp.connect().
 * @return {int} success - 1 if successful, 0 if an error occurred.
 */
static JSVAL ftp_pasv(JSARGS args) {
    HandleScope scope;
    netbuf *n = HANDLE(args[0]);
    return scope.Close(Integer::New(FtpOptions(FTPLIB_CONNMODE, FTPLIB_PASSIVE, n)));
}

/**
 * @function ftp.active
 * 
 * var success = ftp.active(handle);
 * 
 * Set connection to ACTIVE mode.
 * 
 * @param {object} handle - handle returned by ftp.connect().
 * @return {int} success - 1 if successful, 0 if an error occurred.
 */
static JSVAL ftp_active(JSARGS args) {
    HandleScope scope;
    netbuf *n = HANDLE(args[0]);
    return scope.Close(Integer::New(FtpOptions(FTPLIB_CONNMODE, FTPLIB_PORT, n)));
}

/**
 * @function ftp.chdir
 * 
 * var success = ftp.chdir(handle, path);
 * 
 * Sends a change working directory request to the server using the specified path.
 * 
 * @param {object} handle - handle returned by ftp.connect().
 * @param {string} path - path of new desired working directory.
 * @return {int} success - 1 if successful, 0 if an error occurred.
 */
static JSVAL ftp_chdir(JSARGS args) {
    HandleScope scope;
    netbuf *n = HANDLE(args[0]);
	String::Utf8Value path(args[1]->ToString());
    return scope.Close(Integer::New(FtpChdir(*path, n)));
}

/**
 * @function ftp.mkdir
 * 
 * var success = ftp.mkdir(handle, path);
 * 
 * Sends a make directory request to the server using the specified path.
 * 
 * @param {object} handle - handle returned by ftp.connect().
 * @param {string} path - path of new desired directory.
 * @return {int} success - 1 if successful, 0 if an error occurred.
 */
static JSVAL ftp_mkdir(JSARGS args) {
    HandleScope scope;
    netbuf *n = HANDLE(args[0]);
	String::Utf8Value path(args[1]->ToString());
    return scope.Close(Integer::New(FtpMkdir(*path, n)));
}

/**
 * @function ftp.rmdir
 * 
 * var success = ftp.rmdir(handle, path);
 * 
 * Sends a remove directory request to the server using the specified path.
 * 
 * @param {object} handle - handle returned by ftp.connect().
 * @param {string} path - path of remote directory to remove.
 * @return {int} success - 1 if successful, 0 if an error occurred.
 */
static JSVAL ftp_rmdir(JSARGS args) {
    HandleScope scope;
    netbuf *n = HANDLE(args[0]);
	String::Utf8Value path(args[1]->ToString());
    return scope.Close(Integer::New(FtpRmdir(*path, n)));
}

/**
 * @function ftp.dir
 * 
 * var success = ftp.dir(handle, path, output);
 * 
 * Sends a LIST command to the server with the specified path.
 * 
 * @param {object} handle - handle returned by ftp.connect().
 * @param {string} path - path of remote directory to list.
 * @param {string} output - path to local file to write listing to.
 * @return {int} success - 1 if successful, 0 if an error occurred.
 */
static JSVAL ftp_dir(JSARGS args) {
    HandleScope scope;
    netbuf *n = HANDLE(args[0]);
	String::Utf8Value path(args[1]->ToString());
	String::Utf8Value output(args[2]->ToString());
    return scope.Close(Integer::New(FtpDir(*output, *path, n)));
}

/**
 * @function ftp.getcwd
 * 
 * ### Synopsis
 * 
 * var path = ftp.getcwd(handle);
 * 
 * @param {object} handle - handle returned by ftp.connect().
 * @return {string} path - path of current remote directory.
 */
static JSVAL ftp_getcwd(JSARGS args) {
    HandleScope scope;
    netbuf *n = HANDLE(args[0]);
    char buf[2048];
    if (!FtpPwd(buf, 2048, n)) {
        return scope.Close(False());
    }
    return scope.Close(String::New(buf));
}

void init_ftp_object() {
	HandleScope scope;

	JSOBJT ftpObject = ObjectTemplate::New();
	ftpObject->Set(String::New("init"), FunctionTemplate::New(ftp_init));
	ftpObject->Set(String::New("site"), FunctionTemplate::New(ftp_site));
	ftpObject->Set(String::New("lastResponse"), FunctionTemplate::New(ftp_lastResponse));
	ftpObject->Set(String::New("systemType"), FunctionTemplate::New(ftp_systemType));
	ftpObject->Set(String::New("size"), FunctionTemplate::New(ftp_size));
	ftpObject->Set(String::New("fileModified"), FunctionTemplate::New(ftp_fileModified));
	ftpObject->Set(String::New("connect"), FunctionTemplate::New(ftp_connect));
	ftpObject->Set(String::New("login"), FunctionTemplate::New(ftp_login));
	ftpObject->Set(String::New("quit"), FunctionTemplate::New(ftp_quit));
	ftpObject->Set(String::New("pasv"), FunctionTemplate::New(ftp_pasv));
	ftpObject->Set(String::New("active"), FunctionTemplate::New(ftp_active));
	ftpObject->Set(String::New("chdir"), FunctionTemplate::New(ftp_chdir));
	ftpObject->Set(String::New("mkdir"), FunctionTemplate::New(ftp_mkdir));
	ftpObject->Set(String::New("rmdir"), FunctionTemplate::New(ftp_rmdir));
	ftpObject->Set(String::New("dir"), FunctionTemplate::New(ftp_dir));
	ftpObject->Set(String::New("getcwd"), FunctionTemplate::New(ftp_getcwd));

    builtinObject->Set(String::New("ftp"), ftpObject);
}
