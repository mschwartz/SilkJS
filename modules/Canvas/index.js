/**
 * @class Canvas
 *
 * ### Synopsis
 *
 * var Canvas = require('Canvas').Canvas;
 * var canvas = new Canvas(width, height);
 * var context = canvas.getContext('2d');
 * ...
 * canvas.writeToFile(path);
 *
 * Server-side implementation of Canvas rendering object.
 *
 * The browser side implementation of Canvas allows dynamic rendering to and interaction with images.  This server-side implementation allows for the rendering of static images using the same API.
 *
 * For complete details on the browser-side Canvas API, see http://www.w3.org/TR/2dcontext/.
 *
 * ### Details
 *
 * The Canvas API is implemented on top of SilkJS' native libcairo methods.
 *
 * The only two context2d methods not implemented are createImageData() and putImageData(), otherwise this is a complete API.
 *
 * Garbage collection is an issue, since libcairo objects are instantiated by the Canvas API that need to be freed/released.  For this reason, the Image, CanvasRenderingContext2D, and Canvas classes all have destroy() methods that you should call when you are done using those objects.
 *
 * The Canvas object implements some garbage collection for patterns.  Those are destroyed() automatically when you call the canvas' destroy() method.
 *
 * In addition to the destroy() methods, the Canvas.writeToFile(path) method is implemented to save your rendered canvas to a PNG file.
 */

// http://www.w3.org/TR/2dcontext/

var console = require('console');

exports.Canvas = require('lib/Canvas').Canvas;
exports.Image = require('lib/Image').Image;
exports.debug = function(s) {
    console.dir(s);
}
