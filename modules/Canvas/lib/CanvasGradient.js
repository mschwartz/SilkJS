/** @ignore */

var cairo = require('builtin/cairo');

function CanvasGradient(pattern) {
    this._pattern = pattern;
}
CanvasGradient.prototype.extend({
    addColorStop: function(offset, color) {
        
    },
    destroy: function() {
        cairo.pattern_destroy(this._pattern);
    }
});

exports = CanvasGradient;
