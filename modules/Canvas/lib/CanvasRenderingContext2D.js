/** @ignore */

var cairo = require('builtin/cairo'),
    CanvasTransformation = require('CanvasTransformation'),
    CanvasLineStyles = require('CanvasLineStyles'),
    CanvasPathMethods = require('CanvasPathMethods'),
    CanvasText = require('CanvasText');


function setTextPath(context, str, x, y) {
    var ctx = context._context,
        extents;

    switch (context._textAlignment) {
        case 'center':
            extents = cairo.context_text_extents(ctx, str);
            x -= extents.width / 2;
            break;
        case 'right':
        case 'end':
            extents = cairo.context_text_extents(ctx, str);
            x -= extents.width;
            break;
    }
//    var valid_textBaseline = [ 'top', 'hanging', 'middle', 'alphabetic', 'ideographic', 'bottom' ];

    switch (context._textBaseline) {
        case 'top':
        case 'hanging':
            extents = cairo.context_font_extents(ctx);
            y += extents.ascent;
            break;
        case 'middle':
            extents = cairo.context_font_extents(ctx);
            y += extents.ascent / 2;
            break;
        case 'bottom':
            extents = cairo.context_font_extents(ctx);
            y -= extents.height - extents.ascent;
            break;
    }
    cairo.context_move_to(ctx, x, y);
    cairo.context_text_path(ctx, str);
}

var globalCompositeOperations = [
    'source-over', cairo.OPERATOR_OVER,
    'source-atop', cairo.OPERATOR_ATOP,
    'source-in', cairo.OPERATOR_IN,
    'source-out', cairo.OPERATOR_OUT,
    'xor', cairo.OPERATOR_XOR,
    'destination-atop', cairo.OPERATOR_DEST_ATOP,
    'destination-in', cairo.OPERATOR_DEST_IN,
    'destination-out', cairo.OPERATOR_DEST_OUT,
    'destination-over', cairo.OPERATOR_DEST_OVER,
    'clear', cairo.OPERATOR_CLEAR,
    'source', cairo.OPERATOR_SOURCE,
    'dest', cairo.OPERATOR_DEST,
    'over', cairo.OPERATOR_OVER,
    'saturate', cairo.OPERATOR_SATURATE
];
if (cairo.VERSION_MINOR >= 10) {
    globalCompositeOperations = globalCompositeOperations.concat([
        'lighter',cairo.OPERATOR_LIGHTEN,
        'darker', cairo.OPERATOR_DARKEN,
        'multiply', cairo.OPERATOR_MULTIPLY,
        'screen', cairo.OPERATOR_SCREEN,
        'overlay', cairo.OPERATOR_OVERLAY,
        'hard-light', cairo.OEPRATOR_HARD_LIGHT,
        'soft-light', cairo.OPERATOR_SOFT_LIGHT,
        'hsl-hue', cairo.OPERATOR_HSL_HUE,
        'hsl-saturation', cairo.OPERATOR_HSL_SATURATION,
        'hsl-color', cairo.OPERATOR_HSL_COLOR,
        'hsl-luminosity', cairo.OPERATOR_HSL_LUMINOSITY,
        'color-dodge', cairo.OPERATOR_COLOR_DODGE,
        'color-burn', cairo.OPERATOR_COLOR_BURN,
        'difference', cairo.OPERATOR_DIFFERENCE,
        'exclusion', cairo.OPERATOR_EXCLUSION
    ]);
}
else {
    globalCompositeOperations = globalCompositeOperations.concat([ 'lighter', cairo.OPERATOR_ADD ]);
}
//CanvasRenderingContext2D.prototype.extend(CanvasTransformation.prototype);
//CanvasRenderingContext2D.prototype.extend(CanvasLineStyles.prototype);
//CanvasRenderingContext2D.prototype.extend(CanvasPathMethods.prototype);
//CanvasRenderingContext2D.prototype.extend(CanvasText.prototype);


function CanvasRenderingContext2D(canvas) {
    this._canvas = canvas;
    this._context = cairo.context_create(canvas.surface);
    this._globalAlpha = 1;
    this._globalCompositeOperation = 'source-over';
    CanvasTransformation.call(this, arguments);
    CanvasLineStyles.call(this, arguments);
    CanvasPathMethods.call(this, arguments);
    CanvasText.call(this, arguments);
}
CanvasRenderingContext2D.prototype.extend({
    // back-reference to the canvas
    get canvas() {
        return this._canvas;
    },
    // state
    save: function() {
        cairo.context_save(this._context);
    },
    restore: function() {
        cairo.context_restore(this._context);
    },
    // compositing
    get globalAlpha() {
        return this._globalAlpha;
    },
    set globalAlpha(value) {
        if (value >= 0 && value <= 1) {
            this._globalAlpha = value;
        }
    },
    get globalCompositeOperation() {
        return this._globalCompositeOperation;
    },
    set globalCompositeOperation(value) {
        var ndx = globalCompositeOperations.indexOf(value);
        if (ndx == -1) {
            return;
        }
        this._globalCompositeOperation = value;
        cairo.context_set_operator(this._context, globalCompositeOperations[ndx+1]);
    },
    // colors and styles
    get strokeStyle() {

    },
    set strokeStyle(value) {
        if (!value) {
            return;
        }
//        if ('C')

    },
    get fillStyle() {
        
    },
    set fillStyle(value) {
        
    },
    createLinearGradient: function(x0,y0, x1,y1) {
        
    },
    createRadialGradient: function(x0, y0, r0, x1, y1, r1) {
        
    },
    createPattern: function(element) {
        
    },
    // shadows
    get shadowOffsetX() {
        
    },
    set shadowOffsetX(value) {
        
    },
    get shadowOffsetY() {
        
    },
    set shadowOffsetY(value) {
        
    },
    get shadowBlur() {
        
    },
    set shadowBlur(value) {
        
    },
    get shadowColor() {
        
    },
    set shadowColor(value) {
        
    },
    // rects
    clearRect: function(x,y, w,h) {
        
    },
    fillRect: function(x,y, w,h) {
        
    },
    strokeRect: function(x,y, w,h) {
        
    },
    // [path API (see also CanvasPathMethods)
    beingPath: function() {
        
    },
    fill: function(path) {
        
    },
    stroke: function(path) {
        
    },
    drawSystemFocusRing: function(path, element) {
        
    },
    drawCustomFocusRing: function(path, element) {
        
    },
    scrollPathIntoView: function(path) {
        
    },
    clip: function(path) {
        
    },
//    resetClip: function() {
//        
//    },
    isPointInPath: function(path, x, y) {
        
    },
    // text (see also CanvasDrawingStyles)
    fillText: function(text, x, y, maxWidth) {
        var ctx = this._context;
        cairo.context_save(ctx);
        setTextPath(this, x, x, y);
        cairo.context_fill(ctx);
        cairo.context_restore(ctx);
    },
    strokeText: function(text, x, y, maxWidth) {
        var ctx = this._context;
        cairo.context_save(ctx);
        setTextPath(this, x, x, y);
        cairo.context_stroke(ctx);
        cairo.context_restore(ctx);
    },
    measureText: function(text) {
        
    },
    // drawing images
    drawImage: function(element, sx, sy, sw, sh, dx, dy, dw, dh) {
        
    },
    // hit regions
    addHitRegion: function(options) {
        
    },
    // pixel manipulation
    createImageData: function(sw, dh) {
        
    },
    getImageData: function(sx, sy, sw, sh) {
        
    },
    putImageData: function(imagedata, dx,dy, dirtyX, dirtyY, dirtyWidth, dirtyHeight) {
        
    }
});
CanvasRenderingContext2D.prototype.extend(CanvasTransformation.prototype);
CanvasRenderingContext2D.prototype.extend(CanvasLineStyles.prototype);
CanvasRenderingContext2D.prototype.extend(CanvasPathMethods.prototype);
CanvasRenderingContext2D.prototype.extend(CanvasText.prototype);

exports = CanvasRenderingContext2D;
