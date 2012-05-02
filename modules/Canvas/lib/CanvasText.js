/** @ignore */

var cairo = require('builtin/cairo'),
    parseFont = require('Utils').parseFont;

function CanvasText(context) {
    this._context = context;
}
CanvasText.prototype.extend({
    get font() {
        return this._fontString;
    },
    set font(value) {
        if (font = parseFont(value)) {
            this._fontString = value;
            this._font = font;
        }
    },
    get textAlign() {
        return this._textAlign;
    },
    set textAlign(value) {
        if (valid_textAlign.indexOf(value) !== -1) {
            this._textAlign = value;
        }
    },
    get textBaseline() {
        return this._textBaseline;
    },
    set textBaseline(value) {
        if (valid_textBaseline.indexOf(value) !== -1) {
            this._textBaseline = value;
        }
    }
});

exports = CanvasText;
