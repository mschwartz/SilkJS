/** @ignore */

var cairo = require('builtin/cairo');

var cache = {};

var weights = 'normal|bold|bolder|lighter|[1-9]00', 
    styles = 'normal|italic|oblique', 
    units = 'px|pt|pc|in|cm|mm|%', 
    string = '\'([^\']+)\'|"([^"]+)"|[\\w-]+';

var fontre = new RegExp('^ *'
    + '(?:(' + weights + ') *)?'
    + '(?:(' + styles + ') *)?'
    + '([\\d\\.]+)(' + units + ') *'
    + '((?:' + string + ')( *, *(?:' + string + '))*)'
);
 
 var parseFont = exports.parseFont = function(str) {
    var font = {}, 
    captures = fontre.exec(str);
    
    // Invalid
    if (!captures) {
        return false;
    }
    
    // Cached
    if (cache[str]) {
        return cache[str];
    }
    
    // Populate font object
    font.weight = captures[1] || 'normal';
    font.style = captures[2] || 'normal';
    font.size = parseFloat(captures[3]);
    font.unit = captures[4];
    font.family = captures[5].replace(/["']/g, '');
    
    // TODO: dpi
    // TODO: remaining unit conversion
    switch (font.unit) {
        case 'pt':
            font.size /= .75;
            break;
        case 'in':
            font.size *= 96;
            break;
        case 'mm':
            font.size *= 96.0 / 25.4;
            break;
        case 'cm':
            font.size *= 96.0 / 2.54;
            break;
    }
    
    return cache[str] = font;
};

var valid_textAlign = [ 'start', 'end', 'left', 'right', 'center' ];
var valid_textBaseline = [ 'top', 'hanging', 'middle', 'alphabetic', 'ideographic', 'bottom' ];

function CanvasText(context) {
    this._context = context;
    this._font = null;
    this._fontString = '10px sans-serif';
    this._textAlign = 'start';
    this._textBaseline = 'alphabetic';
}
CanvasText.prototype.extend({
    get font() {
        return this._fontString;
    },
    set font(value) {
        var font;
        if (font = parseFont(value)) {
            var ctx = this._context;
            this._fontString = value;
            this._font = font;
            cairo.context_set_font_size(ctx, font.size);
            switch (font.style) {
                case 'italic':
                    cairo.context_select_font_face(ctx, family, cairo.FONT_SLANT_ITALIC, font.weight === 'bold' ? cairo.FONT_WEIGHT_BOLD : cairo.FONT_WEIGHT_NORMAL);
                    break;
                case 'oblique':
                    cairo.context_select_font_face(ctx, family, cairo.FONT_SLANT_OBLIQUE, font.weight === 'bold' ? cairo.FONT_WEIGHT_BOLD : cairo.FONT_WEIGHT_NORMAL);
                    break;
                case 'normal':
                default:
                    cairo.context_select_font_face(ctx, family, cairo.FONT_SLANT_NORMAL, font.weight === 'bold' ? cairo.FONT_WEIGHT_BOLD : cairo.FONT_WEIGHT_NORMAL);
                    break;
            }
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
