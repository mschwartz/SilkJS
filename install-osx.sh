#!/bin/sh

if  [ "$OSTYPE" != "darwin11" ] ; then
    echo "This install script is for OSX Lion only."
    exit 1
fi

# exit immediately if a simple command exits with a non-zero status.
set -e
trap "echo Installation failed.  Aborting."  EXIT

INSTALLDIR = "/usr/local"
SILKJS = "$INSTALLDIR/silkjs"

if [ -e "$SILKJS" ] ; then
    echo "Updating SilkJS in $SILKJS"
else 
    echo "Installing SilkJS in $SILKJS"
fi

# assure INSTALLDIR and SILKJS directories exist and have proper permissions
# a neat trick here is that on Ubuntu and OSX, the user is in group admin
# so we can chmod 775 /usr/local and chgrp it to admin and write to it
# without sudo (after the install)

if [ ! -d "$INSTALLDIR" ] ; then
    echo
    echo "$INSTALLDIR doesn't exist.  Creating it with sudo, so you may be asked for your password."
    echo
    sudo /bin/mkdir "$INSTALLDIR"
    sudo /usr/bin/chgrp admin "$INSTALLDIR"
    sudo /bin/chmod 775 "$PARENT"
elif [ ! -w "$INSTALLDIR" -o ! -w "$INSTALLDIR/bin" ]; then
    echo
    echo "Setting permissions on $INSTALLDIR,  and $INSTALLDIR/bin"
    echo "You may be asked for your password."
    echo
    sudo /bin/mkdir -p "$INSTALLDIR/bin"
    sudo /bin/chmod g+rwx "$INSTALLDIR"
    sudo /bin/chmod g+rwx "$INSTALLDIR/bin"
fi

# 