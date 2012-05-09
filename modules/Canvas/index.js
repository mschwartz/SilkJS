/** @ignore */

// http://www.w3.org/TR/2dcontext/

var console = require('console');

exports.Canvas = require('lib/Canvas').Canvas;
exports.Image = require('lib/Image').Image;
exports.debug = function(s) {
    console.dir(s);
}