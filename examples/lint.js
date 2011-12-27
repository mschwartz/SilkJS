#!/usr/bin/env silkjs
// lint.js 20111225 (C) markc@renta.net (AGPLv3)
// A simple script to test Javascript according to http://www.jslint.com/
// Usage: [silkjs] lint.js file_to_check.js (ie; silkjs lint.js lint.js)

/*global fs: false, include: false, JSLINT: false */

'use strict';

var main = function () {
  var argv = Array.prototype.slice.call(arguments),
    argc = argv.length,
    str = "",
    res = "";

  if (fs.exists("jslint.js")) {
    include("jslint.js");

    if (argc) {
      str = fs.readFile(argv[0]);
      res = JSLINT(str.replace(/[\w\W]+?\n+?/, ''), {
        node: true,
        nomen: false,
        indent: 2
      });

      if (res) {
        console.log(argv[0] + " passed the lint test");
      } else {
        console.log(argv[0] + " did NOT pass, check at http://www.jslint.com");
      }
    } else {
      console.log("Usage: [silkjs] lint.js file_to_check.js");
    }
  } else {
    console.log("wget https://raw.github.com/douglascrockford/JSLint/master/jslint.js");
  }
};
