/** @ignore */

var cairo = require('builtin/cairo');

function Image(filename) {
    this._surface = cairo.image_surface_create_from_png(filename);
    this._pattern = cairo.pattern_create_for_surface(this._surface);
}
Image.prototype.extend({
    getPattern: function() {
        return this._pattern;
    },
    destroy: function() {
        cairo.pattern_destroy(this._pattern);
        cairo.surface_destroy(this._surface);
    }
});

exports.extend({
    Image: Image
});
