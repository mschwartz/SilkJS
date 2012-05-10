/** @ignore */

"use strict";

/*global exports */

var cairo = require('builtin/cairo'),
    CanvasTransformation = require('CanvasTransformation').CanvasTransformation,
    CanvasPathMethods = require('CanvasPathMethods').CanvasPathMethods,
    CanvasGradient = require('CanvasGradient').CanvasGradient,
    CanvasPattern = require('CanvasPattern').CanvasPattern,
    CanvasLineStyles = require('CanvasLineStyles').CanvasLineStyles,
    CanvasText = require('CanvasText').CanvasText,
    Image = require('Image').Image,
    parseColor = require('CanvasGradient').parseColor,
    sprintf = require('phpjs').sprintf,
    console = require('console');

var debug = global.debug;

var patternQualities = {
    fast: cairo.FILTER_QUALITY_FAST,
    good: cairo.FILTER_QUALITY_GOOD,
    best: cairo.FILTER_QUALITY_BEST
};

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

function colorToString(color) {
    if (color.a == 1) {
        return sprintf("#%.2x%.2x%.2x", color.r, color.g, color.b);
    }
    else {
        return sprintf("rgba(%d,%d,%d, %.2f)", color.r, color.g, color.b, color.a);
    }
}

function setTextPath(context, str, x, y) {
    var ctx = context._context,
        extents;
    switch (context._textAlign) {
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

function hasShadow(ctx) {
    return ctx._shadowColor.a && (ctx._shadowBlur || ctx._shadowOffsetX || ctx._shadowOffsetY);
}

function shadow(ctx, fn) {
    var c= ctx._context;
    var path = cairo.context_copy_path_flat(c);
    cairo.context_save(c);
    cairo.context_translate(c, ctx._shadowOffsetX, ctx._shadowOffsetY);
    cairo.context_push_group(c);
    cairo.context_new_path(c);
    cairo.context_append_path(c, path);
    fn(c);

    if (ctx._shadowBlur) {
        cairo.surface_blur(cairo.context_get_group_target(c), ctx._shadowBlur);
    }

    cairo.context_pop_group_to_source(c);
    cairo.context_paint(c);

    cairo.context_restore(c);
    cairo.context_new_path(c);
    cairo.context_append_path(c, path);
    fn(c);
    cairo.path_destroy(path);
}

function savePath(ctx) {
    ctx._savedPath = cairo.context_copy_path_flat(ctx._context);
    cairo.context_new_path(ctx._context);
}

function restorePath(ctx) {
    cairo.context_new_path(ctx._context);
    cairo.context_append_path(ctx._context, ctx._savedPath);
    cairo.path_destroy(ctx._savedPath);
    ctx.savedPath = null;
}

//CanvasRenderingContext2D.prototype.extend(CanvasTransformation.prototype);
//CanvasRenderingContext2D.prototype.extend(CanvasLineStyles.prototype);
//CanvasRenderingContext2D.prototype.extend(CanvasPathMethods.prototype);
//CanvasRenderingContext2D.prototype.extend(CanvasText.prototype);


function CanvasRenderingContext2D(canvas) {
    debug('new CanvasRenderingContext2D');
    var transparent = { r: 0, g: 0, b: 0, a: 1},
        transparent_black = { r: 0, g: 0, b: 0, a: 0};
    this._canvas = canvas;
    this._context = cairo.context_create(canvas.surface);
    this._globalAlpha = 1;
    this._globalCompositeOperation = 'source-over';
    this._strokeStyle = null;
    this._strokeColor = transparent;
    this._fillStyle = null;
    this._fillColor = transparent;
    this._patternQuality = 'good';
    this._shadowOffsetX = this._shadowOffsetY = 0;
    this._shadowBlur = 0;
    this._shadowColor = transparent_black;
    
    this.initCanvasLineStyles();
    this.initCanvasText();
}
CanvasRenderingContext2D.prototype.extend({
    // back-reference to the canvas
    get canvas() {
        return this._canvas;
    },
    // state
    save: function() {
        debug('save');
        cairo.context_save(this._context);
    },
    restore: function() {
        debug('restore');
        cairo.context_restore(this._context);
    },
    // compositing
    get globalAlpha() {
        debug('get globalAlpha');
        return this._globalAlpha;
    },
    set globalAlpha(value) {
        debug('set globalAlpha ' + value);
        if (value >= 0 && value <= 1) {
            this._globalAlpha = value;
        }
    },
    get globalCompositeOperation() {
        debug('get globalCompositeOperation');
        return this._globalCompositeOperation;
    },
    set globalCompositeOperation(value) {
        debug('set globalCompositeOperation ' + value);
        var ndx = globalCompositeOperations.indexOf(value);
        if (ndx == -1) {
            return;
        }
        this._globalCompositeOperation = value;
        cairo.context_set_operator(this._context, globalCompositeOperations[ndx+1]);
    },
    // colors and styles
    get strokeStyle() {
        debug('get strokeStyle');
        return this._strokeStyle || colorToString(this._strokeColor);
    },
    set strokeStyle(value) {
        debug('set strokeStlye');
        if (!value) {
            return;
        }
        if (typeof value === 'string') {
            debug(value);
            var color = parseColor(value);
            if (!color) {
                return;
            }
            this._strokeStyle = null;
            this._strokeColor = color;
            return;
        }
        this._strokeStyle = value;
    },
    get fillStyle() {
        debug('get fillStyle');
        return this._fillStyle || colorToString(this._fillColor);
    },
    set fillStyle(value) {
        debug('set fillStyle');
        if (!value) {
            return;
        }
        if (typeof value === 'string') {
            var color = parseColor(value);
            if (!value) {
                return;
            }
            this._fillStyle = null;
            this._fillColor = color;
            return;
        }
        this._fillStyle = value;
    },
    get patternQuality() {
        debug('get patternQuality');
        return this._patternQuality;
    },
    set patternQuality(value) {
        debug('set fillStyle ' + value);
        if (value === 'fast' || value === 'good' || value === 'best') {
            this._patternQuality = value;
        }
    },
    createLinearGradient: function(x0,y0, x1,y1) {
        debug('createLinearGradient ' + [x0,y0,x1,y1].join(','));
        var pattern = cairo.pattern_create_linear(x0,y0, x1,y1);
        return this.canvas.addPattern(new CanvasGradient(this, pattern));
    },
    createRadialGradient: function(x0, y0, r0, x1, y1, r1) {
        debug('createRadialGradient');
        var pattern = cairo.pattern_create_radial(x0,y0,r0, x1,y1,r1);
        return this.canvas.addPattern(new CanvasGradient(this, pattern));
    },
    /**
     * @function CanvasRenderingContext2D.createPattern
     *
     * ### Synopsis
     *
     * var pattern = ctx.createPattern(image);
     *
     * @param {object} image - canvas or image
     */
    createPattern: function(image) {
        debug('createPattern');
        return image.getPattern();
    },
    // shadows
    get shadowOffsetX() {
        debug('get shadowOffsetX');
        return this._shadowOffsetX;
    },
    set shadowOffsetX(value) {
        debug('set shadowOffsetX ' + value);
        this._shadowOffsetX = value;
    },
    get shadowOffsetY() {
        debug('get shadowOffsetY');
        return this._shadowOffsetY;
    },
    set shadowOffsetY(value) {
        debug('set shadowOffsetX ' + value);
        this._shadowOffsetY = value;
    },
    get shadowBlur() {
        debug('get shadowBlur');
        return this._shadowBlur;
    },
    set shadowBlur(value) {
        debug('set shadowBlur ' + value);
        this._shadowBlur = value;
    },
    get shadowColor() {
        debug('get shadowColor');
        return colorToString(this._shadowColor);
    },
    set shadowColor(value) {
        debug('set shadowColor ' + value);
        var color = parseColor(value);
        if (color) {
            this._shadowColor = color;
        }
    },
    // rects
    clearRect: function(x,y, w,h) {
        debug('clearRect ' + [x,y,w,h].join(','));
        var ctx = this._context;
        cairo.context_save(ctx);
        savePath(this);
        cairo.context_rectangle(ctx, x,y,w,h);
        cairo.context_set_operator(ctx, cairo.OPERATOR_CLEAR);
        cairo.context_fill(ctx);
        restorePath(this);
        cairo.context_restore(ctx);
    },
    fillRect: function(x,y, w,h) {
        debug('fillRect ' + [x,y,w,h].join(','));
        var ctx = this._context;
        savePath(this);
        cairo.context_rectangle(ctx, x,y,w,h);
        cairo.context_fill(ctx);
        restorePath(this);
    },
    strokeRect: function(x,y, w,h) {
        debug('strokeRect ' + [x,y,w,h].join(','));
        var ctx = this._context;
        savePath(this);
        cairo.context_rectangle(ctx, x,y,w,h);
        cairo.context_stroke(ctx);
        restorePath(this);
    },
    // [path API (see also CanvasPathMethods)
    beginPath: function() {
        debug('beginPath');
        cairo.context_new_path(this._context);
    },
    // fill and apply shadow
    fill: function(preserve) {
        debug('fill');
        if (this._fillStyle) {
            if ('CanvasGradient' === this._fillStyle.constructor.name) {
                debug('fill gradient');
                cairo.pattern_set_filter(this._fillStyle._pattern, patternQualities[this._patternQuality]);
                cairo.context_set_source(this._context, this._fillStyle._pattern);
            }
            else if ('CanvasPattern' === this._fillStyle.constructor.name) {
                debug('fill pattern');
                cairo.context_set_source(this._context, this._fillStyle._pattern);
                cairo.pattern_set_extend(cairo.context_get_source(this._context), cairo.EXTEND_REPEAT);
            }
        }
        else {
            var color = this._fillColor;
            debug('fill color');
            cairo.context_set_source_rgba(this._context, color.r/255, color.g/255, color.b/255, color.a/255);
        }

        if (preserve) {
            hasShadow(this) ? shadow(this, cairo.context_fill_preserve) : cairo.context_fill_preserve(this._context);
        }
        else {
            hasShadow(this) ? shadow(this, cairo.context_fill) : cairo.context_fill(this._context);
        }
    },
    stroke: function(preserve) {
        debug('stroke');
        if (this._strokeStyle) {
            if ('CanvasGradient' === this._strokeStyle.constructor.name) {
                cairo.pattern_set_filter(this._strokeStyle._pattern, patternQualities[this._patternQuality]);
                cairo.context_set_source(this._context, this._strokeStyle._pattern);
            }
            else if ('CanvasPattern' === this._strokeStyle.constructor.name) {
                cairo.context_set_source(this._context, this._strokeStyle._pattern);
                cairo.pattern_set_extend(cairo.context_get_source(this._context), cairo.EXTEND_REPEAT);
            }
        }
        else {
            var color = this._strokeColor;
            cairo.context_set_source_rgba(this._context, color.r/255, color.g/255, color.b/255, color.a/255);
        }

        if (preserve) {
            hasShadow(this) ? shadow(this, cairo.context_stroke_preserve) : cairo.context_stroke_preserve(this._context);
        }
        else {
            hasShadow(this) ? shadow(this, cairo.context_stroke) : cairo.context_stroke(this._context);
        }
    },
    clip: function() {
        debug('clip');
        cairo.context_clip_preserve(this._context);
    },
//    resetClip: function() {
//        
//    },
    isPointInPath: function(x, y) {
        debug('isPointInPath ' + x + ',' + y);
        return cairo.context_in_fill(this._context, x, y) || cairo.context_in_stroke(this._context, x, y);
    },
    // text (see also CanvasDrawingStyles)
    fillText: function(text, x, y, maxWidth) {
        debug('fillText ' + text + ' ' + x + ',' + y);
        var ctx = this._context;
        cairo.context_save(ctx);
        if (this._fillStyle) {
            if ('CanvasGradient' === this._fillStyle.constructor.name) {
                debug('fill gradient');
                cairo.pattern_set_filter(this._fillStyle._pattern, patternQualities[this._patternQuality]);
                cairo.context_set_source(this._context, this._fillStyle._pattern);
            }
            else if ('CanvasPattern' === this._fillStyle.constructor.name) {
                debug('fill pattern');
                cairo.context_set_source(this._context, this._fillStyle._pattern);
                cairo.pattern_set_extend(cairo.context_get_source(this._context), cairo.EXTEND_REPEAT);
            }
        }
        else {
            var color = this._fillColor;
            debug('fill color');
            cairo.context_set_source_rgba(this._context, color.r/255, color.g/255, color.b/255, color.a/255);
        }

        if (false) {
            hasShadow(this) ? shadow(this, cairo.context_fill_preserve) : cairo.context_fill_preserve(this._context);
        }
        else {
            hasShadow(this) ? shadow(this, cairo.context_fill) : cairo.context_fill(this._context);
        }

        setTextPath(this, text, x, y);
        cairo.context_fill(ctx);
        cairo.context_restore(ctx);
    },
    strokeText: function(text, x, y, maxWidth) {
        debug('strokeText ' + x + ',' + y);
        var ctx = this._context;
        cairo.context_save(ctx);
        setTextPath(this, text, x, y);
        cairo.context_stroke(ctx);
        cairo.context_restore(ctx);
    },
    measureText: function(str) {
        debug('measureText ' + str);
        var ctx = this._context;
        var te = cairo.context_text_extents(ctx, str),
            fe = cairo.context_font_extents(ctx);

        var x_offset = 0.0;
        switch (this._textAlign) {
            case 'center':
                x_offset = te.width / 2;
                break;
            case 'right':
            case 'end':
                x_offset = te.width;
                break;
        }

        var y_offset = 0.0;
        switch (this._textBaseline) {
            case 'top':
            case 'hanging':
                y_offset = fe.ascent;
                break;
            case 'middle':
                y_offset = (fe.ascent - fe.descent) / 2;
                break;
            case 'bottom':
                y_offset = -fe.descent;
                break;
        }

        return {
            width: te.x_advance,
            actualBoundingBoxLeft: x_offset - te.x_bearing,
            actualBoundingBoxRight: (te.x_bearing + te.width) - x_offset,
            actualBoundingBoxAscent: -(te.y_bearing + y_offset),
            actualBoundingBoxDescent: te.height + te.y_bearing + y_offset,
            emHeightAscent: fe.ascent - y_offset,
            emHeightdescent: fe.descent + y_offset,
            alphabeticBaseline: -y_offset
        };

    },
    // drawing images
    drawImage: function(element) {
        var surface = null,
            sx = 0,
            sy = 0,
            sw = 0,
            sh = 0,
            dx, dy, dw, dh,
            type = 'Canvas';
        
        if ('Image' === element.constructor.name) {
            type = 'Image ' + element._filename;
            surface = element._surface;
            sw = element.width;
            sh = element.height;
        }
        else if ('Canvas' === element.constructor.name) {
            surface = element.surface;
            sw = element.width;
            sh = element.height;
        }
        else {
            throw 'drawImage - unsupported element type ' + element.constructor.name;
        }
        debug('drawImage ' + type);
        switch (arguments.length) {
            case 9:
                sx = arguments[1];
                sy = arguments[2];
                sw = arguments[3];
                sh = arguments[4];
                dx = arguments[5];
                dy = arguments[6];
                dw = arguments[7];
                dh = arguments[8];
                break;
            case 5:
                dx = arguments[1];
                dy = arguments[2];
                dw = arguments[3];
                dh = arguments[4];
                break;
            case 3:
                dx = arguments[1];
                dy = arguments[2];
                dw = sw;
                dh = sh;
                break;
            default:
                throw 'Invalid arguments';
        }
        
        var ctx = this._context;
        cairo.context_save(ctx);
        savePath(this);
        cairo.context_rectangle(ctx, dx,dy, dw,dh);
        cairo.context_clip(ctx);
        restorePath(this);
        // scale src
        if (dw != sw || dh != sh) {
            var fx = dw / sw;
            var fy = dh / sh;
            cairo.context_scale(ctx, fx, fy);
            dx /= fx;
            dy /= fy;
        }
        
        cairo.context_set_source_surface(ctx, surface, dx-sx, dy-sy);
        cairo.pattern_set_filter(cairo.context_get_source(ctx), this._patternQuality);
        cairo.context_paint_with_alpha(ctx, this._globalAlpha);
        cairo.context_restore(ctx);
    },
    // hit regions
//    addHitRegion: function(options) {
//
//    },
    // pixel manipulation
    createImageData: function(sw, dh) {
        throw 'Not implemented';
    },
    getImageData: function(sx, sy, sw, sh) {
        debug('getImageData ' + [sx,sy,sw,sh].join(','));
        return cairo.image_surface_get_data(this._canvas.surface, sx, sy, sw, sh);
    },
    putImageData: function(imagedata, dx,dy, dirtyX, dirtyY, dirtyWidth, dirtyHeight) {
        throw 'Not implemented';
    },
    //
    destroy: function() {
        cairo.context_destroy(this._context);
    }
});
CanvasRenderingContext2D.prototype.extend(CanvasTransformation);
CanvasRenderingContext2D.prototype.extend(CanvasLineStyles);
CanvasRenderingContext2D.prototype.extend(CanvasPathMethods);
CanvasRenderingContext2D.prototype.extend(CanvasText);

exports.extend({
    CanvasRenderingContext2D: CanvasRenderingContext2D
});

