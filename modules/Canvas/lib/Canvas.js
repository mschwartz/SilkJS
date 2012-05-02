/** @ignore */

var cairo = require('builtin/cairo'),
    CanvasRenderingContext2D = require('CanvasRenderingContext2D');

function Canvas(width, height) {
    this.width = width;
    this.height = height;
    this.surface = cairo.image_surface_create (cairo.FORMAT_ARGB32, width, height);
}
Canvas.prototype.extend({
    getContext: function(type) {
        if (type !== '2d') {
            return null;
        }
        return new CanvasRenderingContext2D(this);
    }
});

exports = Canvas;
