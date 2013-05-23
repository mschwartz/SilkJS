OSX Installer
=============

All is automated by running the create-installer.js script:

```
cd SilkJS/osx-installer
./create-installer.js
```

The result will be a self installing OSX executable named silkjs-installer-osx.

If the silkjs-installer-osx binary is run on an OSX system, it will automatically
unpack and install SilkJS in /usr/local/silkjs and will create a couple of 
softlinks in /usr/local/bin.


