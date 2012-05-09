/** @ignore */

"use strict";

var cairo = require('builtin/cairo');

var CanvasTransformation = {
    // transformations (default transform is the identity matrix)
    scale: function(x, y) {
        cairo.context_scale(this._context, x, y);
    },
    rotate: function(angle) {
        cairo.context_rotate(this._context, angle);
    },
    translate: function(x, y) {
        cairo.context_translate(this._context, x, y);
    },
    transform: function(a, b, c, d, e, f) {
        var matrix = cairo.matrix_create();
        cairo.matrix_init(matrix, a, b, c, d, e, f);
        cairo.context_transform(this._context, matrix);
        cairo.matrix_destroy(matrix);
    },
    setTransform: function(a, b, c, d, e, f) {
        cairo.context_identity_matrix(this._context);
        var matrix = cairo.matrix_create();
        cairo.matrix_init(matrix, a, b, c, d, e, f);
        cairo.context_transform(this._context, matrix);
        cairo.matrix_destroy(matrix);
    },
    resetTransform: function() {
        cairo.context_identity_matrix(this._context);
    }
};

exports.extend({
    CanvasTransformation: CanvasTransformation
});
