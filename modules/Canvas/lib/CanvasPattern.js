/** @ignore */

"use strict";

var cairo = require('builtin/cairo');

function CanvasPattern(pattern, surface) {
    this._pattern = pattern;
    this._surface = surface;
}
CanvasPattern.prototype.extend({
    destroy: function() {
        cairo.pattern_destroy(this._pattern);
    }
});

exports.extend({
    CanvasPattern: CanvasPattern
});
