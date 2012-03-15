/**
 * SilkJS builtin fs object
 * 
 * Usage:
 * require('builtin/fs');
 * 
 */
#include "SilkJS.h"



// TODO:

static JSVAL fs_error(JSARGS args) {
    HandleScope scope;
    return scope.Close(String::New(strerror(errno)));
}

static JSVAL fs_chdir(JSARGS args) {
    HandleScope scope;
    String::Utf8Value dir(args[0]->ToString());
    return scope.Close(Integer::New(chdir(*dir)));
}

static JSVAL fs_getcwd(JSARGS args) {
    HandleScope scope;
    char *cwd = getcwd(NULL, 0);
    Handle<String>s = String::New(cwd);
    delete[] cwd;
    return scope.Close(s);
}

static JSVAL fs_open(JSARGS args) {
    HandleScope scope;
    String::Utf8Value filename(args[0]->ToString());
    int flags = args[1]->IntegerValue();
    mode_t mode = args[2]->IntegerValue();
    return scope.Close(Integer::New(open(*filename, flags, mode)));
}

static JSVAL fs_close(JSARGS args) {
    HandleScope scope;
    int fd = args[0]->IntegerValue();
    return scope.Close(Integer::New(close(fd)));
}

static JSVAL fs_flock(JSARGS args) {
    HandleScope scope;
    int fd = args[0]->IntegerValue();
    int operation = args[1]->IntegerValue();
    return scope.Close(Integer::New(flock(fd, operation)));
}

static JSVAL fs_lockf(JSARGS args) {
    HandleScope scope;
    int fd = args[0]->IntegerValue();
    int operation = args[1]->IntegerValue();
    return scope.Close(Integer::New(lockf(fd, operation, 0)));
}

static JSVAL fs_rename(JSARGS args) {
    HandleScope scope;
    String::Utf8Value oldpath(args[0]->ToString());
    String::Utf8Value newpath(args[1]->ToString());
    return scope.Close(Integer::New(rename(*oldpath, *newpath)));
}

static JSVAL fs_truncate(JSARGS args) {
    HandleScope scope;
    off_t len = args[1]->IntegerValue();
    if (args[0]->IsString()) {
        String::Utf8Value path(args[0]->ToString());
        return scope.Close(Integer::New(truncate(*path, len)));
    } else {
        int fd = args[0]->IntegerValue();
        return scope.Close(Integer::New(ftruncate(fd, len)));
    }
}

static JSVAL fs_chmod(JSARGS args) {
    HandleScope scope;
    mode_t mode = args[1]->IntegerValue();
    if (args[0]->IsString()) {
        String::Utf8Value path(args[0]->ToString());
        return scope.Close(Integer::New(chmod(*path, mode)));
    } else {
        int fd = args[0]->IntegerValue();
        return scope.Close(Integer::New(fchmod(fd, mode)));
    }
}

static JSOBJ format_stat(struct stat &buf) {
    HandleScope scope;
    JSOBJ o = Object::New();
    o->Set(String::New("dev"), Integer::New(buf.st_dev));
    o->Set(String::New("ino"), Integer::New(buf.st_ino));
    o->Set(String::New("mode"), Integer::New(buf.st_mode));
    o->Set(String::New("nlink"), Integer::New(buf.st_nlink));
    o->Set(String::New("uid"), Integer::New(buf.st_uid));
    o->Set(String::New("gid"), Integer::New(buf.st_gid));
    o->Set(String::New("rdev"), Integer::New(buf.st_rdev));
    o->Set(String::New("size"), Integer::New(buf.st_size));
    o->Set(String::New("blksize"), Integer::New(buf.st_blksize));
    o->Set(String::New("blocks"), Integer::New(buf.st_blocks));
    o->Set(String::New("atime"), Integer::New(buf.st_atime));
    o->Set(String::New("mtime"), Integer::New(buf.st_mtime));
    o->Set(String::New("ctime"), Integer::New(buf.st_ctime));
    return scope.Close(o);
}

static JSVAL fs_stat(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());
    struct stat buf;
    if (stat(*path, &buf) == -1) {
        return scope.Close(Null());
    }
    return scope.Close(format_stat(buf));
}

static JSVAL fs_lstat(const Arguments & args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());
    struct stat buf;
    if (lstat(*path, &buf) == -1) {
        return scope.Close(False());
    }
    return scope.Close(format_stat(buf));
}

static JSVAL fs_fstat(JSARGS args) {
    HandleScope scope;
    int fd = args[0]->IntegerValue();
    struct stat buf;
    if (fstat(fd, &buf) == -1) {
        return scope.Close(False());
    }
    return scope.Close(format_stat(buf));
}

static JSVAL fs_link(JSARGS args) {
    HandleScope scope;
    String::Utf8Value oldpath(args[0]->ToString());
    String::Utf8Value newpath(args[1]->ToString());
    return scope.Close(Integer::New(link(*oldpath, *newpath)));
}

static JSVAL fs_symlink(JSARGS args) {
    HandleScope scope;
    String::Utf8Value oldpath(args[0]->ToString());
    String::Utf8Value newpath(args[1]->ToString());
    return scope.Close(Integer::New(symlink(*oldpath, *newpath)));
}

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static JSVAL fs_readlink(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());

    char pathBuf[PATH_MAX];
    ssize_t size = readlink(*path, pathBuf, PATH_MAX);
    if (size < 0) {
        return scope.Close(False());
    }
    return scope.Close(String::New(pathBuf));
}

static JSVAL fs_realpath(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());

    char *absolutePath = realpath(*path, NULL);
    if (!absolutePath) {
        return scope.Close(False());
    }
    Handle<String>s = String::New(absolutePath);
    free(absolutePath);
    return scope.Close(s);
}

static JSVAL fs_unlink(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());
    if (unlink(*path) == -1) {
        return scope.Close(False());
    }
    return scope.Close(True());
}

static JSVAL fs_rmdir(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());
    if (rmdir(*path) == -1) {
        return scope.Close(False());
    }
    return scope.Close(True());
}

static JSVAL fs_mkdir(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());
    mode_t mode = 0700;
    if (args.Length() > 1) {
        mode = args[1]->IntegerValue();
    }
    if (mkdir(*path, mode) == -1) {
        return scope.Close(False());
    }
    return scope.Close(True());
}

static JSVAL fs_readdir(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());
    DIR *d = opendir(*path);
    if (!d) {
        return scope.Close(Null());
    }
    Handle<Array>a = Array::New();
    int ndx = 0;
    while (struct dirent * e = readdir(d)) {
        if (strcmp(e->d_name, ".") && strcmp(e->d_name, "..")) {
            a->Set(ndx++, String::New(e->d_name));
        }
    }
    closedir(d);
    return scope.Close(a);
}

static JSVAL fs_readfile(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());
    int fd = open(*path, O_RDONLY);
    if (fd == -1) {
        printf("%s\n%s\n", *path, strerror(errno));
        return scope.Close(Null());
    }
    std::string s;
    //	long size = lseek(fd, 0, 2); lseek(fd, 0, 0);
    //	printf("size = %ld\n", size);
    char buf[1024];
    ssize_t count;
    while ((count = read(fd, buf, 1024))) {
        s = s.append(buf, count);
    }
    //	if (read(fd, buf, size) != size) {
    //		return scope.Close(Null());
    //	}
    close(fd);
    Handle<String>ret = String::New(s.c_str(), s.size());
    return scope.Close(ret);
}

static JSVAL fs_readfile64(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());
    int fd = open(*path, O_RDONLY);
    if (fd == -1) {
        printf("%s\n%s\n", *path, strerror(errno));
        return scope.Close(Null());
    }
    long size = lseek(fd, 0, 2);
    lseek(fd, 0, 0);
    unsigned char buf[size];
    if (read(fd, buf, size) != size) {
        return scope.Close(Null());
    }
    close(fd);
    string encoded = Base64Encode(buf, size);
    Handle<String>s = String::New(encoded.c_str(), encoded.size());
    return scope.Close(s);
}

static JSVAL fs_writefile(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());
    String::Utf8Value data(args[1]->ToString());
    ssize_t size;
    if (args.Length() > 2) {
        size = args[2]->IntegerValue();
    } else {
        size = strlen(*data);
    }
    mode_t mode = 0644;
    if (args.Length() > 3) {
        mode = args[3]->IntegerValue();
    }
    int fd = open(*path, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if (fd == -1) {
        return scope.Close(False());
    }
    if (write(fd, *data, size) != size) {
        close(fd);
        return scope.Close(False());
    }
    close(fd);
    return scope.Close(True());
}

static JSVAL fs_writefile64(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());
    String::Utf8Value data(args[1]->ToString());
    mode_t mode = 0644;
    if (args.Length() > 2) {
        mode = args[2]->IntegerValue();
    }
    string out = Base64Decode(*data);
    int fd = open(*path, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if (fd == -1) {
        return scope.Close(False());
    }
    ssize_t size = out.size();
    if (write(fd, out.c_str(), size) != size) {
        close(fd);
        return scope.Close(False());
    }
    close(fd);
    return scope.Close(True());
}

static JSVAL fs_exists(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());

    struct stat buf;
    if (stat(*path, &buf)) {
        return scope.Close(False());
    }
    return scope.Close(True());
}

static JSVAL fs_isfile(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());

    struct stat buf;
    if (stat(*path, &buf)) {
        return scope.Close(False());
    }
    if (S_ISREG(buf.st_mode)) {
        return scope.Close(True());
    } else {
        return scope.Close(False());
    }
}

static JSVAL fs_isdir(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());

    struct stat buf;
    if (stat(*path, &buf)) {
        return scope.Close(False());
    }
    if (S_ISDIR(buf.st_mode)) {
        return scope.Close(True());
    } else {
        return scope.Close(False());
    }
}

static JSVAL fs_filesize(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());

    struct stat buf;
    if (stat(*path, &buf)) {
        return scope.Close(False());
    }
    return scope.Close(Integer::New(buf.st_size));
}

static JSVAL fs_mtime(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());

    struct stat buf;
    if (stat(*path, &buf)) {
        return scope.Close(False());
    }
    return scope.Close(Integer::New(buf.st_mtime));
}

static JSVAL fs_md5(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path(args[0]->ToString());
    unsigned char buf[1024];
    int bytes;
    
    MD5_CTX mdContext;
    MD5Init(&mdContext);
    
    FILE *fp = fopen(*path, "rb");
    if (fp == NULL) {
        return scope.Close(False());
    }
    while ((bytes = fread(buf, 1, 1024, fp)) != 0) {
        MD5Update(&mdContext, buf, bytes);
    }
    fclose(fp);
    MD5Final(&mdContext);
    MD5Digest(&mdContext, (char *)buf);
    return scope.Close(String::New((char *)buf));
}

static JSVAL fs_cmp(JSARGS args) {
    HandleScope scope;
    String::Utf8Value path1(args[0]->ToString());
    String::Utf8Value path2(args[1]->ToString());
    int fd;
    long size1, size2;
    char buf[1024];
    
    fd = open(*path1, O_RDONLY);
    if (fd < 0) {
        sprintf(buf, "Could not open %s", *path1);
        return scope.Close(String::New(buf));
    }
    size1 = lseek(fd, 0, 2); lseek(fd, 0, 0);
    unsigned char buf1[size1];
    if (read(fd, buf1, size1) != size1) {
        close(fd);
        sprintf(buf, "Error reading %s", *path1);
        return scope.Close(String::New(buf));
    }
    close(fd);
    
    fd = open(*path2, O_RDONLY);
    if (fd < 0) {
        sprintf(buf, "Could not open %s", *path2);
        return scope.Close(String::New(buf));
    }
    size2 = lseek(fd, 0, 2); lseek(fd, 0, 0);
    unsigned char buf2[size2];
    if (read(fd, buf2, size2) != size2) {
        close(fd);
        sprintf(buf, "Error reading %s", *path2);
        return scope.Close(String::New(buf));
    }
    close(fd);
    
    if (size1 != size2) {
        return scope.Close(String::New("Files differ in size"));
    }
    
    for (long n=0; n<size1; n++) {
        if (buf1[n] != buf2[n]) {
            sprintf(buf, "Files differ at offset %ld", n);
            return scope.Close(String::New(buf));
        }
    }
    return scope.Close(True());
}

void init_fs_object() {
    HandleScope scope;

    Handle<ObjectTemplate>fs = ObjectTemplate::New();

    fs->Set(String::New("O_RDONLY"), Integer::New(O_RDONLY));
    fs->Set(String::New("O_WRONLY"), Integer::New(O_WRONLY));
    fs->Set(String::New("O_RDWR"), Integer::New(O_RDWR));
    fs->Set(String::New("O_CREAT"), Integer::New(O_CREAT));
    fs->Set(String::New("O_TRUNC"), Integer::New(O_TRUNC));
    // for flock
    fs->Set(String::New("LOCK_EX"), Integer::New(LOCK_EX));
    fs->Set(String::New("LOCK_UN"), Integer::New(LOCK_UN));
    fs->Set(String::New("LOCK_SH"), Integer::New(LOCK_SH));
    // for lockf
    fs->Set(String::New("F_LOCK"), Integer::New(F_LOCK));
    fs->Set(String::New("F_TLOCK"), Integer::New(F_TLOCK));
    fs->Set(String::New("F_ULOCK"), Integer::New(F_ULOCK));
    fs->Set(String::New("F_TEST"), Integer::New(F_TEST));

    fs->Set(String::New("S_IFMT"), Integer::New(S_IFMT));
    fs->Set(String::New("S_IFLNK"), Integer::New(S_IFLNK));
    fs->Set(String::New("S_IFREG"), Integer::New(S_IFREG));
    fs->Set(String::New("S_IFBLK"), Integer::New(S_IFBLK));
    fs->Set(String::New("S_IFDIR"), Integer::New(S_IFDIR));
    fs->Set(String::New("S_IFCHR"), Integer::New(S_IFCHR));
    fs->Set(String::New("S_IFIFO"), Integer::New(S_IFIFO));
    fs->Set(String::New("S_ISUID"), Integer::New(S_ISUID));
    fs->Set(String::New("S_ISGID"), Integer::New(S_ISGID));
    fs->Set(String::New("S_ISVTX"), Integer::New(S_ISVTX));
    fs->Set(String::New("S_IRWXU"), Integer::New(S_IRWXU));
    fs->Set(String::New("S_IRUSR"), Integer::New(S_IRUSR));
    fs->Set(String::New("S_IWUSR"), Integer::New(S_IWUSR));
    fs->Set(String::New("S_IXUSR"), Integer::New(S_IXUSR));
    fs->Set(String::New("S_IRWXG"), Integer::New(S_IRWXG));
    fs->Set(String::New("S_IRGRP"), Integer::New(S_IRGRP));
    fs->Set(String::New("S_IWGRP"), Integer::New(S_IWGRP));
    fs->Set(String::New("S_IRWXO"), Integer::New(S_IRWXO));
    fs->Set(String::New("S_IROTH"), Integer::New(S_IROTH));
    fs->Set(String::New("S_IWOTH"), Integer::New(S_IWOTH));
    fs->Set(String::New("S_IXOTH"), Integer::New(S_IXOTH));


    fs->Set(String::New("error"), FunctionTemplate::New(fs_error));
    fs->Set(String::New("chdir"), FunctionTemplate::New(fs_chdir));
    fs->Set(String::New("getcwd"), FunctionTemplate::New(fs_getcwd));
    fs->Set(String::New("open"), FunctionTemplate::New(fs_open));
    fs->Set(String::New("close"), FunctionTemplate::New(fs_close));
    fs->Set(String::New("flock"), FunctionTemplate::New(fs_flock));
    fs->Set(String::New("lockf"), FunctionTemplate::New(fs_lockf));
    fs->Set(String::New("rename"), FunctionTemplate::New(fs_rename));
    fs->Set(String::New("truncate"), FunctionTemplate::New(fs_truncate));
    fs->Set(String::New("chmod"), FunctionTemplate::New(fs_chmod));
    fs->Set(String::New("stat"), FunctionTemplate::New(fs_stat));
    fs->Set(String::New("lstat"), FunctionTemplate::New(fs_lstat));
    fs->Set(String::New("fstat"), FunctionTemplate::New(fs_fstat));
    fs->Set(String::New("link"), FunctionTemplate::New(fs_link));
    fs->Set(String::New("symlink"), FunctionTemplate::New(fs_symlink));
    fs->Set(String::New("readlink"), FunctionTemplate::New(fs_readlink));
    fs->Set(String::New("realpath"), FunctionTemplate::New(fs_realpath));
    fs->Set(String::New("unlink"), FunctionTemplate::New(fs_unlink));
    fs->Set(String::New("rmdir"), FunctionTemplate::New(fs_rmdir));
    fs->Set(String::New("mkdir"), FunctionTemplate::New(fs_mkdir));
    fs->Set(String::New("readDir"), FunctionTemplate::New(fs_readdir));
    fs->Set(String::New("readFile"), FunctionTemplate::New(fs_readfile));
    fs->Set(String::New("readFile64"), FunctionTemplate::New(fs_readfile64));
    fs->Set(String::New("writeFile"), FunctionTemplate::New(fs_writefile));
    fs->Set(String::New("writeFile64"), FunctionTemplate::New(fs_writefile64));
    fs->Set(String::New("exists"), FunctionTemplate::New(fs_exists));
    fs->Set(String::New("isFile"), FunctionTemplate::New(fs_isfile));
    fs->Set(String::New("isDir"), FunctionTemplate::New(fs_isdir));
    fs->Set(String::New("fileSize"), FunctionTemplate::New(fs_filesize));
    fs->Set(String::New("fileModified"), FunctionTemplate::New(fs_mtime));
    
    fs->Set(String::New("md5"), FunctionTemplate::New(fs_md5));
    fs->Set(String::New("cmp"), FunctionTemplate::New(fs_cmp));

    builtinObject->Set(String::New("fs"), fs);
}
