/** @ignore */

"use strict";

var cairo = require('builtin/cairo');

function CanvasPattern(pattern, surface) {
    this._pattern = pattern;
    this._surface = surface;
}
CanvasPattern.proto = {}.extend({
    destroy: function() {
        cairo.pattern_destroy(this._pattern);
    }
});
CanvasPattern.prototype.extend(CanvasPattern.proto);

exports.extend({
    CanvasPattern: CanvasPattern
});
