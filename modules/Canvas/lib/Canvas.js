/** @ignore */

global.debug = function(s) {
//    console.dir(s);
}

var cairo = require('builtin/cairo'),
    console = require('console');

var CanvasRenderingContext2D = require('CanvasRenderingContext2D').CanvasRenderingContext2D;

function Canvas(width, height) {
    debug('new Canvas');
    this.width = width;
    this.height = height;
    this.surface = cairo.image_surface_create (cairo.FORMAT_ARGB32, width, height);
    this._context = null;
}
Canvas.prototype.extend({
    getContext: function(type) {
        if (type !== '2d') {
            return null;
        }
        if (this._context) {
            this._context.destroy();
        }
//        if (!this._context) {
            this._context = new CanvasRenderingContext2D(this);
//        }
        return this._context;
    },
    getSurface: function() {
        return this.surface;
    },
    writeToFile: function(filename) {
        cairo.surface_write_to_png(this.surface, filename);

    },
    destroy: function() {
        if (this._context) {
            this._context.destroy();
        }
        cairo.surface_destroy(this.surface);
    }
});

exports.extend({
    Canvas: Canvas,
    debug: debug
});
