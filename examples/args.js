#!/usr/bin/env silkjs
// args.js 20111225 (C) markc@renta.net (AGPLv3)
// Demonstrates one way to print out any command line arguments
// chmod +x args.js && args.js a b c

'use strict';

var main = function () {
  Array.prototype.slice.call(arguments).forEach(
    function (e, i) {
      console.log("argv[" + i + "] = " + e);
    }
  );
};
