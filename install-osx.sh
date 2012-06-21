#!/bin/sh

if  [ "$OSTYPE" != "darwin11" ] ; then
    echo "This install script is for OSX Lion only."
    exit 1
fi

# exit immediately if a simple command exits with a non-zero status.
#set -e
#trap "echo Installation failed.  Aborting."  EXIT

INSTALLDIR="/usr/local"
SILKJS="$INSTALLDIR/silkjs"

if [ -e "$SILKJS" ] ; then
    echo "Updating SilkJS in $SILKJS"
    rm -f /usr/local/bin/silkjs /usr/local/bin/httpd-silk.js
    rm -rf /usr/local/silkjs
else 
    echo "Installing SilkJS in $SILKJS"
    echo
    echo "You may be asked to enter your password."
    echo "This is so the /usr/local directory can be made writable for the install."
    echo
    sudo chgrp admin /usr/local /usr/local/bin 
    sudo chmod g+rwx /usr/local /usr/local/bin
fi

# curl --progress-bar https://github.com/downloads/mschwartz/SilkJS/silkjs-lion-x64.tgz | tar -C "$INSTALLDIR" -xzvf -
echo "Downloading SilkJS"
curl --progress-bar --location https://github.com/downloads/mschwartz/SilkJS/silkjs-lion-x64.tgz | tar -C "$INSTALLDIR" -xzf -

ln -sf /usr/local/silkjs/bin/silkjs /usr/local/bin
ln -sf /usr/local/silkjs/httpd/main.js /usr/local/bin/httpd-silk.js

echo "Installation complete."
echo
echo "You need to add /usr/local/bin to your PATH environment variable."
echo "This can be done by adding this line to your .bashrc file:"
echo "export PATH=/usr/local/bin:\$PATH"
echo
echo "You can run SilkJS with the following command:"
echo "$ silkjs"
echo
echo "You can run the SilkJS HTTP Server with the following command:"
echo "$ httpd-silk.js yourapp.js"
echo
echo "For instructions on setting up a WWW site for httpd-silk.js, see"
echo "http://silkjs.org/http-server/"
echo