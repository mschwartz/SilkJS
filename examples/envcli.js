#!/usr/bin/env silkjs
// envcli.js 2011125 (C) markc@renta.net (AGPLv3)
// An example of how to use the object returned by process.env()

//console.log(JSON.stringify(process.env(), null, 2));
//console.log(process.env()["FOOBAR"] || "env var not set");

"use strict";

var e = '', env = process.env();

for (e in env) {
  if (env.hasOwnProperty(e)) {
    console.log(e + " = " + env[e]);
  }
}
