/** @ignore */

var cairo = require('builtin/cairo');

function CanvasLineStyles(context) {
    this._context = context;
    this._lineWidth = 1.0;
    this._lineCap = 'butt';
    this._lineJoin = 'miter';
    this._miterLimit = 10.0;
}
CanvasLineStyles.prototype.extend({
    // line caps/joins
    get lineWidth() {
        return this._lineWidth;
    },
    set lineWidth(value) {
        if (value <= 0 || isNaN(value) || value == Number.NEGATIVE_INFINITY || value == Number.POSITIVE_INFINITY) {
            return;
        }
        this._lineWidth = value;
        cairo.context_set_line_width(this._context, value);
    },
    get lineCap() {
        return this._lineCap;
    },
    set lineCap(value) {
        switch (value) {
            case 'butt':
                this._lineCap = value;
                cairo.context_set_line_cap(this._context, cairo.LINE_CAP_BUTT);
                break;
            case 'round':
                this._lineCap = value;
                cairo.context_set_line_cap(this._context, cairo.LINE_CAP_ROUND);
                break;
            case 'square':
                this._lineCap = value;
                cairo.context_set_line_cap(this._context, cairo.LINE_CAP_SQUARE);
                break;
        }
    },
    get lineJoin() {
        return this._lineJoin;
    },
    set lineJoin(value) {
        switch (value) {
            case 'bevel':
                this._lineJoin = value;
                cairo.context_set_line_join(this._context, cairo.LINE_JOIN_BEVEL);
                break;
            case 'round':
                this._lineJoin = value;
                cairo.context_set_line_join(this._context, cairo.LINE_JOIN_ROUND);
                break;
            case 'miter':
                this._lineJoin = value;
                cairo.context_set_line_join(this._context, cairo.LINE_JOIN_MITER);
                break;
        }
    },
    get miterLimit() {
        return this._miterLimit;
    },
    set miterLimit(value) {
        if (value <= 0 || isNaN(value) || value == Number.NEGATIVE_INFINITY || value == Number.POSITIVE_INFINITY) {
            return;
        }
        this._miterLimit = value;
        cairo.context_set_miter_limit(this._context, value);
    }
});

exports = CanvasLineStyles;
