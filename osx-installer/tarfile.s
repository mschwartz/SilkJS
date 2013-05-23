    cpu         x64
    section     .data
    global      _tarfile,_tarfile_size

_tarfile:
        incbin 'silkjs.tgz'
_tarfile_size   dd $-_tarfile

