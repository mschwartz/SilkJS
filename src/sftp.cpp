#include "SilkJS.h"
#include <libssh2.h>
#include <libssh2_sftp.h>


struct SFTP {
    int sock;
    LIBSSH2_SESSION *session;
    LIBSSH2_SFTP *sftp_session;
};

static SFTP *HANDLE(Handle<Value> arg) {
	return (SFTP *)JSEXTERN(arg);
}


static JSVAL sftp_connect(JSARGS args) {
    HandleScope scope;
	String::Utf8Value host(args[0]);
	String::Utf8Value username(args[1]);
	String::Utf8Value password(args[2]);
	String::Utf8Value path(args[2]);
	int port = 22;
	if (args.Length() > 4) {
		port = args[4]->IntegerValue();
	}
    
    struct hostent *hp = gethostbyname(*host);
    if (!hp) {
        return scope.Close(String::New("Failed to get IP address of server"));
    }

    struct sockaddr_in s;
    s.sin_addr = *(struct in_addr *)hp->h_addr_list[0];
    s.sin_family = hp->h_addrtype;
    s.sin_port = htons(port);
    
	int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        char msg[4096];
        sprintf(msg, "Failed to create socket: %s", strerror(errno));
        return scope.Close(String::New(msg));
    }
    if (connect(sock, (struct sockaddr *)&s, sizeof(s)) < 0) {
        char msg[4096];
        sprintf(msg, "Failed to connect: %s", strerror(errno));
        close(sock);
        return scope.Close(String::New(msg));
    }
    LIBSSH2_SESSION *session = libssh2_session_init();
    if (!session) {
        close(sock);
        return scope.Close(String::New("Could not initialize SSH2 session"));
    }
    libssh2_session_set_blocking(session, 1);
    if (libssh2_userauth_password(session, *username, *password)) {
        close(sock);
        libssh2_session_free(session);
        return scope.Close(String::New("Invalid credentials"));
    }
    
    LIBSSH2_SFTP *sftp = libssh2_sftp_init(session);
    if (!sftp) {
        char *errmsg;
        int errlen;
        libssh2_session_last_error(session, &errmsg, &errlen, 0);
        libssh2_session_free(session);
        close(sock);
        return scope.Close(String::New(errmsg));
    }
    
    SFTP *p = new SFTP;
    p->sock = sock;
    p->session = session;
    p->sftp_session = sftp;
    return scope.Close(External::New(p));
}

JSVAL sftp_close(JSARGS args) {
    HandleScope scope;
    SFTP *handle = HANDLE(args[0]);
    if (handle->sftp_session) {
        libssh2_sftp_shutdown(handle->sftp_session);
    }
    if (handle->session) {
        libssh2_session_disconnect(handle->session, "SilkJS SFTP Disconnecting");
        libssh2_session_free(handle->session);
    }
    if (handle->sock > 0) {
        close(handle->sock);
    }
    delete handle;
    return Undefined();
}

/**
 * @function sftp.readDir
 * 
 * ### Synopsis
 * 
 * var files = sftp.readDir(handle, path);
 * 
 * Read a directory from remote server, return an array of objects of the form:
 * 
 * + name: name of file.
 * + longentry: a string?
 * + size: size of file in bytes.
 * + permissions: file permissions.
 * + uid: uid of owner of file.
 * + gid: gid of owner of file.
 * + mtime: modification time (timestamp) of file.
 * + atime: access time (timestamp) of file
 * 
 * The above values may be undefined (except for name), if the SSH2 library and remote server don't allow extended information.
 * 
 * @param {object} handle - opaque handle to existing SFTP connection (already connected).
 * @param {string} path - path on remote server to directory to get listing of.
 * @return {array} files - array of objects as specifed above, or string containing error message.
 */
JSVAL sftp_readdir(JSARGS args) {
    HandleScope scope;
    SFTP *handle = HANDLE(args[0]);
    String::Utf8Value path(args[1]);

    LIBSSH2_SFTP_HANDLE *sftp_handle = libssh2_sftp_opendir(handle->sftp_session, *path);
    if (!sftp_handle) {
        return scope.Close(String::New("Could not open remote directory"));
    }
    
    char mem[512], 
        longentry[512];
    LIBSSH2_SFTP_ATTRIBUTES attrs;
    
    Handle<String> _name = String::New("name");
    Handle<String> _longentry = String::New("longentry");
    Handle<String> _permissions = String::New("permissions");
    Handle<String> _uid = String::New("uid");
    Handle<String> _gid = String::New("gid");
    Handle<String> _size = String::New("size");
    Handle<String> _mtime = String::New("mtime");
    Handle<String> _atime = String::New("atime");
    
	Handle<Array>a = Array::New();
    int aIndex = 0;
    while (1) {
        if (libssh2_sftp_readdir_ex(sftp_handle, mem, sizeof(mem), longentry, sizeof(longentry), &attrs) <= 0) {
            break;
        }
        JSOBJ o = Object::New();
        o->Set(_name, String::New(mem));
        o->Set(_longentry, String::New(longentry));
        if (attrs.flags & LIBSSH2_SFTP_ATTR_SIZE) {
            o->Set(_size, Integer::New(attrs.filesize));
        }
        if (attrs.flags & LIBSSH2_SFTP_ATTR_PERMISSIONS) {
            o->Set(_permissions, Integer::New(attrs.permissions));
        }
        if (attrs.flags & LIBSSH2_SFTP_ATTR_UIDGID) {
            o->Set(_uid, Integer::New(attrs.uid));
            o->Set(_gid, Integer::New(attrs.gid));
        }
        if (attrs.flags & LIBSSH2_SFTP_ATTR_ACMODTIME) {
            o->Set(_mtime, Integer::New(attrs.mtime));
            o->Set(_atime, Integer::New(attrs.atime));
        }
        a->Set(aIndex, o);
    }
    libssh2_sftp_closedir(sftp_handle);
    return scope.Close(a);
}

/**
 * @function sftp.stat
 * 
 * ### Synopsis
 * 
 * var stat = sftp.stat(handle, path);
 * 
 * Get attributes of a remote file.
 * 
 * The returned object will be of the form:
 * 
 * + size: size of file in bytes.
 * + permissions: file permissions.
 * + uid: uid of owner of file.
 * + gid: gid of owner of file.
 * + mtime: modification time (timestamp) of file.
 * + atime: access time (timestamp) of file
 * 
 * @param {object} handle - opaque handle to existing SFTP connection (already connected).
 * @param {string} path - path on remote server to directory to get status of.
 * @returns {object} stat - object of the form described above.
 */
JSVAL sftp_stat(JSARGS args) {
    HandleScope scope;
    SFTP *handle = HANDLE(args[0]);
    String::Utf8Value path(args[1]);
    LIBSSH2_SFTP_HANDLE *sftp_handle = libssh2_sftp_open(handle->sftp_session, *path, LIBSSH2_FXF_READ, 0);
    if (!sftp_handle) {
        return scope.Close(String::New("Could not get status for remote file"));
    }
    LIBSSH2_SFTP_ATTRIBUTES attrs;
    if (libssh2_sftp_fstat_ex(sftp_handle, &attrs, 0) < 0) {
        libssh2_sftp_close(sftp_handle);
        return scope.Close(String::New("Could not get status for remote file"));
    }
    
    Handle<String> _permissions = String::New("permissions");
    Handle<String> _uid = String::New("uid");
    Handle<String> _gid = String::New("gid");
    Handle<String> _size = String::New("size");
    Handle<String> _mtime = String::New("mtime");
    Handle<String> _atime = String::New("atime");
    
    JSOBJ o = Object::New();
    if (attrs.flags & LIBSSH2_SFTP_ATTR_SIZE) {
        o->Set(_size, Integer::New(attrs.filesize));
    }
    if (attrs.flags & LIBSSH2_SFTP_ATTR_PERMISSIONS) {
        o->Set(_permissions, Integer::New(attrs.permissions));
    }
    if (attrs.flags & LIBSSH2_SFTP_ATTR_UIDGID) {
        o->Set(_uid, Integer::New(attrs.uid));
        o->Set(_gid, Integer::New(attrs.gid));
    }
    if (attrs.flags & LIBSSH2_SFTP_ATTR_ACMODTIME) {
        o->Set(_mtime, Integer::New(attrs.mtime));
        o->Set(_atime, Integer::New(attrs.atime));
    }

    libssh2_sftp_close(sftp_handle);
    return scope.Close(o);
}

/**
 * @function sftp.mkdir
 * 
 * ### Synopsis
 * 
 * var success = sftp.mkdir(handle, path, mode);
 * 
 * Make a directory on the remote host.
 * 
 * @param {object} handle - opaque handle to existing SFTP connection (already connected).
 * @param {string} path - path on remote server to create.
 * @returns {boolean} success - true if directory was created.
 */
JSVAL sftp_mkdir(JSARGS args) {
    HandleScope scope;
    SFTP *handle = HANDLE(args[0]);
    String::Utf8Value path(args[1]);
    int mode = 0755;
    if (args.Length() > 2) {
        mode = args[2]->IntegerValue();
    }
    if (libssh2_sftp_mkdir(handle->sftp_session, *path, mode)) {
        return scope.Close(String::New("Could not create directory"));
    }
    return scope.Close(True());
}

/**
 * @function sftp.rmdir
 * 
 * ### Synopsis
 * 
 * var success = sftp.rmdir(handle, path);
 * 
 * Remove a directory on the remote host.
 * 
 * @param {object} handle - opaque handle to existing SFTP connection (already connected).
 * @param {string} path - path on remote server to remove.
 * @returns {boolean} success - true if directory was removed.
 */
JSVAL sftp_rmdir(JSARGS args) {
    HandleScope scope;
    SFTP *handle = HANDLE(args[0]);
    String::Utf8Value path(args[1]);
    if (libssh2_sftp_rmdir(handle->sftp_session, *path)) {
        return scope.Close(String::New("Could not remove directory"));
    }
    return scope.Close(True());
}

/**
 * @function sftp.unlink
 * 
 * ### Synopsis
 * 
 * var success = sftp.unlink(handle, path);
 * 
 * Remove a file on the remote host.
 * 
 * @param {object} handle - opaque handle to existing SFTP connection (already connected).
 * @param {string} path - path to file on remote server to remove.
 * @returns {boolean} success - true if directory was removed.
 */
JSVAL sftp_unlink(JSARGS args) {
    HandleScope scope;
    SFTP *handle = HANDLE(args[0]);
    String::Utf8Value path(args[1]);
    if (libssh2_sftp_unlink(handle->sftp_session, *path)) {
        return scope.Close(String::New("Could not remove file"));
    }
    return scope.Close(True());
}

/**
 * @function sftp.writeFile
 * 
 * ### Synopsis
 * 
 * var status = sftp.writeFile(handle, srcPath, dstPath);
 * var status = sftp.writeFile(handle, srcPath, dstPath, mode);
 * 
 * Write file to remote server via SFTP.
 * 
 * @param {object} handle - opaque handle to already open SFTP connection.
 * @param {string} srcPath - path to file in local file system to send.
 * @param {string} dstPath - path to file in remote file system to create.
 * @param {int} mode - desired resulting file permissions of file on remote end.
 * @return {boolean} success - true if the transfer succeeded.
 * 
 * ### Note
 * If mode is not provided, the file mode of the file being sent will be used.
 */
static JSVAL sftp_writeFile(JSARGS args) {
	HandleScope scope;
    SFTP *handle = HANDLE(args[0]);
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
    LIBSSH2_SFTP_HANDLE *sftp_handle = libssh2_sftp_open(handle->sftp_session, *dstPath, LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC, mode);
    
    char mem[1024];
    ssize_t toWrite = fileinfo.st_size;
    while (toWrite > 0) {
        ssize_t nRead = read(fd, mem, 1024);
        if (nRead < 0) {
            int eNum = errno;
            libssh2_sftp_close(sftp_handle);
            close(fd);
            errno = eNum;
            return scope.Close(False());
        }
        int rc = libssh2_sftp_write(sftp_handle, mem, nRead);
        if (rc < 0) {
            libssh2_sftp_close(sftp_handle);
            close(fd);
            return scope.Close(False());
        }
        toWrite -= nRead;
    }
    close(fd);
    libssh2_sftp_close(sftp_handle);
    return scope.Close(True());
}

void init_sftp_object() {
	HandleScope scope;
	
	Handle<ObjectTemplate>sftp  = ObjectTemplate::New();
	sftp->Set(String::New("connect"), FunctionTemplate::New(sftp_connect));
	sftp->Set(String::New("close"), FunctionTemplate::New(sftp_close));
	sftp->Set(String::New("readdir"), FunctionTemplate::New(sftp_readdir));
	sftp->Set(String::New("stat"), FunctionTemplate::New(sftp_stat));
	sftp->Set(String::New("mkdir"), FunctionTemplate::New(sftp_mkdir));
	sftp->Set(String::New("rmdir"), FunctionTemplate::New(sftp_rmdir));
    sftp->Set(String::New("unlink"), FunctionTemplate::New(sftp_unlink));
    sftp->Set(String::New("writeFile"), FunctionTemplate::New(sftp_writeFile));

	builtinObject->Set(String::New("sftp"), sftp);
}
