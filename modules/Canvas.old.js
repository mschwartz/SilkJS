/** @ignore */

/**
 * @class Canvas
 * 
 * ## Synopsis
 * 
 * Implementation of Canvas for SilkJS.  
 * 
 * With this class, you should be able to render server-side what you would normally be able to render client-side using Canvas.
 * 
 * ## See also
 * 
 * http://www.w3.org/TR/2dcontext/
 */

var cairo = require('builtin/cairo');

var valid_lineCap = [ 'butt', 'round', 'square' ];
var valid_lineJoin = [ 'bevel', 'round', 'miter' ];
var valid_textAlign = [ 'start', 'end', 'left', 'right', 'center' ];
var valid_textBaseline = [ 'top', 'hanging', 'middle', 'alphabetic', 'ideographic', 'bottom' ];

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

function Canvas(width, height) {
    this.width = width;
    this.height = height;
    this.surface = cairo.image_surface_create (cairo.FORMAT_ARGB32, width, height);
}
Canvas.prototype.extend({
    getContext: function(type) {
        if (type !== '2d') {
            return null;
        }
        return new CanvasRenderingContext2D(this);
    }
});

function CanvasRenderingContext2D(canvas) {
    this._canvas = canvas;
    this._context = cairo.context_create(canvas.surface);
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
        
    },
    set globalAlpha(value) {
        
    },
    get globalCompositeOperation() {
        
    },
    set globalCompositeOperation(value) {
        
    },
    // colors and styles (see also CanvasDrawingStyles)
    get strokeStyle() {
        
    },
    set strokeStyle(value) {
        
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
    scrollPathIntoView: functioN(path) {
        
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
        
    },
    strokeText: function(text, x, y, maxWidth) {
        
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
CanvasRenderingContext2D.prototype.extend(CanvasDrawingStyles.prototype);
CanvasRenderingContext2D.prototype.extend(CanvasPathMethods.prototype);


function CanvasGradient() {
    
}
CanvasGradient.prototype.extend({
    addColorStop: function(offset, color) {
        
    }
});

function CanvasPattern() {
    
}
CanvasPattern.prototype.extend({
    setTransform: function(transform) {
        
    }
});

function CanvasDrawingStyles() {
    
}
CanvasDrawingStyles.prototype.extend({
    
});

function TextMetrics() {
    
}
TextMetrics.prototype.extend({
    get width() {
        
    },
    get actualBoundingBoxLeft() {
        
    },
    get actualBoundingBoxRight() {
        
    },
    get fontBoundingBoxAscent() {
        
    },
    get fontBoundingBoxDescent() {
        
    },
    get actualBoundingBoxAscent() {
        
    },
    get actualBoundingBoxDescent() {
        
    },
    get emHeightAscent() {
        
    },
    get emHeightDescent() {
        
    },
    get hangingBaseline() {
        
    },
    get alphabeticBaseline() {
        
    },
    get ideographicBaseline() {
        
    }
});

function ImageData() {
    
}
ImageData.prototype.extend({
    get width() {
        
    },
    get height() {
        
    },
    get data() {
        
    }
});

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
    },
    get lineCap() {
        return this._lineCap;
    },
    set lineCap(value) {
        if (valid_lineCap.indexOf(value) !== -1) {
            this._lineCap = value;
        }
    },
    get lineJoin() {
        return this._lineJoin;
    },
    set lineJoin(value) {
        if (valid_lineJoin.indexOf(value) !== -1) {
            this._lineJoin = value;
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
    }
});

function CanvasPathMethods(context) {
    this._context = context;
}
CanvasPathMethods.extend({
    // shared path API methods
    closePath: function() {
        cairo.context_close_path(this._context);
    },
    moveTo: function(x, y) {
        cairo.context_move_to(this._context, x, y);
    },
    lineTo: function(x, y) {
        cairo.context_line_to(this._context, x, y);
    },
    quadraticCurveTo: function(x1, y1, x2, y2) {
        var point = cairo.context_get_current_point(this._context);
        var x = point.x || x,
            y = point.y || y;
            
        cairo.context_curve_to(
            this._context, 
            x  + 2.0 / 3.0 * (x1 - x),  y  + 2.0 / 3.0 * (y1 - y),
            x2 + 2.0 / 3.0 * (x1 - x2), y2 + 2.0 / 3.0 * (y1 - y2), 
            x2,
            y2
        );
    },
    bezierCurveTo: function(cp1x, cp1y, cp2x, cp2y, x, y) {
        cairo.curve_to(this.context, cp1x, cp1y, cp2x, cp2y, x, y);
    },
    arcTo: function(x1, y1, x2, y2, radius) {
        var M_PI = Math.PI;
        
        // Current path point
        var ctx = this._context;
        var p0 = cairo.context_get_current_point(ctx),
            p1 = { x: x1, y: y1 },
            p2 = { x: x2, y: y2 };

        if ((p1.x == p0.x && p1.y == p0.y)
            || (p1.x == p2.x && p1.y == p2.y)
            || radius == 0) {
            cairo.context_line_to(ctx, p1.x, p1.y);
            return;
        }
        var p1p0 = { x: p0.x - p1.x, y: p0.y - p1.y },
            p1p2 = { x: p2.x - p1.x, y: p2.y - p1.y }
            p1p0_length = Math.sqrt(p1p0.x * p1p0.x + p1p0.y * p1p0.y),
            p1p2_length = Math.sqrt(p1p2.x * p1p2.x + p1p2.y * p1p2.y),
            cos_phi = (p1p0.x * p1p2.x + p1p0.y * p1p2.y) / (p1p0_length * p1p2_length);

        // all points on a line logic
        if (-1 == cos_phi) {
            cairo.context_line_to(ctx, p1.x, p1.y);
            return;
        }

        if (1 == cos_phi) {
            // add infinite far away point
            var max_length = 65535,
                factor_max = max_length / p1p0_length;
            cairo.context_line_to(ctx, p0.x + factor_max * p1p0.x, p0.y + factor_max * p1p0.y);
            return;
        }
        
        var tangent = radius / tan(Math.acos(cos_phi) / 2),
            factor_p1p0 = tangent / p1p0_length,
            t_p1p0 = {x: p1.x + factor_p1p0 * p1p0.x, y: p1.y + factor_p1p0 * p1p0.y };
            
        var orth_p1p0 = { x: p1p0.y, y: -p1p0.x},
            orth_p1p0_length = Math.sqrt(orth_p1p0.x * orth_p1p0.x + orth_p1p0.y * orth_p1p0.y),
            factor_ra = radius / orth_p1p0_length;
            
        var cos_alpha = (orth_p1p0.x * p1p2.x + orth_p1p0.y * p1p2.y) / (orth_p1p0_length * p1p2_length);
        if (cos_alpha < 0) {
            orth_p1p0 = { x: -orth_p1p0.x, y: -orth_p1p0.y};
        }

        var p = { x:t_p1p0.x + factor_ra * orth_p1p0.x, y: t_p1p0.y + factor_ra * orth_p1p0.y };

        orth_p1p0 = { x: -orth_p1p0.x, y: -orth_p1p0.y };
        var sa = Math.acos(orth_p1p0.x / orth_p1p0_length);
        if (orth_p1p0.y < 0) {
            sa = 2 * M_PI - sa;
        }

        var anticlockwise = false;

        var factor_p1p2 = tangent / p1p2_length,
            t_p1p2 = { x: p1.x + factor_p1p2 * p1p2.x, y: p1.y + factor_p1p2 * p1p2.y },
            orth_p1p2 = { x: t_p1p2.x - p.x, y: t_p1p2.y - p.y },
            orth_p1p2_length = math.sqrt(orth_p1p2.x * orth_p1p2.x + orth_p1p2.y * orth_p1p2.y},
            ea = Math.acos(orth_p1p2.x / orth_p1p2_length);

        if (orth_p1p2.y < 0) {
            ea = 2 * M_PI - ea;
        }
        if ((sa > ea) && ((sa - ea) < M_PI)) {
            anticlockwise = true;
        }
        if ((sa < ea) && ((ea - sa) > M_PI)) {
            anticlockwise = true;
        }

        cairo.context_line_to(ctx, t_p1p0.x, t_p1p0.y);

        if (anticlockwise && M_PI * 2 != radius) {
            cairo.context_arc_negative(ctx
            , p.x
            , p.y
            , radius
            , sa
            , ea);
        } else {
            cairo.context_arc(ctx
            , p.x
            , p.y
            , radius
            , sa
            , ea);
        }        
    },
    rect: function(x,y, w, h) {
        
    },
    arc: function(x,y, radius, startAngle, endAngle, anticlockwise) {
        
    },
    ellipse: function(x,y, radiusX, radiusY, rotation, startAngle, endAngle, antiClockwise) {
        
    }
});

function CanvasTransformation(context) {
    this._context = context;
}
CanvasTransformation.prototype.extend({
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
        cairo.matrix_init(a, b, c, d, e, f);
        cairo_context_transform(this._context, matrix);
        cairo_matrix_destroy(matrix);
    },
    setTransform: function(a, b, c, d, e, f) {
        cairo.context_identity_matrix(this._context);
        var matrix = cairo.matrix_create();
        cairo.matrix_init(a, b, c, d, e, f);
        cairo_context_transform(this._context, matrix);
        cairo_matrix_destroy(matrix);
    },
    resetTransform: function() {
        cairo.context_identity_matrix(this._context);
    }
});


function DrawingStyle(scope) {
    
}
DrawingStyle.prototype.extend(CanvasDrawingStyles.prototype);

function Path(path) {
    
}
Path.prototype.extend({
    addPath: function(path, transformation) {
        
    },
    addPathByStrokingPath: function(path, styles, transformation) {
        
    },
    addText: function(text, styles, transformation, x, y, maxWidth) {
        
    },
    addPathByStrokingText: function(text, styles, transformation, x, y, maxWidth) {
        
    }
});
Path.prototype.extend(CanvasPathMethods.prototype);






//function CanvasDrawingStyles() {
//    this._lineWidth = 1.0;
//    this._lineCap = 'butt';
//    this._lineJoin = 'miter';
//    this._miterLimit = 10.0;
//    this._lineDashes = [];
//    this._lineDashOffset = 0.0;
//    this._font = null;
//    this._fontString = '10px sans-serif';
//    this._textAlign = 'start';
//    this._textBaseline = 'alphabetic';
//}
//CanvasDrawingStyles.prototype.extend({
//    // line caps/joins
//    get lineWidth() {
//        return this._lineWidth;
//    },
//    set lineWidth(value) {
//        if (value <= 0 || isNaN(value) || value == Number.NEGATIVE_INFINITY || value == Number.POSITIVE_INFINITY) {
//            return;
//        }
//        this._lineWidth = value;
//    },
//    get lineCap() {
//        return this._lineCap;
//    },
//    set lineCap(value) {
//        if (valid_lineCap.indexOf(value) !== -1) {
//            this._lineCap = value;
//        }
//    },
//    get lineJoin() {
//        return this._lineJoin;
//    },
//    set lineJoin(value) {
//        if (valid_lineJoin.indexOf(value) !== -1) {
//            this._lineJoin = value;
//        }
//    },
//    get miterLimit() {
//        return this._miterLimit;
//    },
//    set miterLimit(value) {
//        if (value <= 0 || isNaN(value) || value == Number.NEGATIVE_INFINITY || value == Number.POSITIVE_INFINITY) {
//            return;
//        }
//        this._miterLimit = value;
//    },
//    setLineDash: function(segments) {
//        var valid = true;
//        segments.each(function(segment) {
//            if (segment <= 0 || isNaN(segment) || segment == Number.NEGATIVE_INFINITY || segment == Number.POSITIVE_INFINITY) {
//                valid = false;
//                return false;
//            }
//        });
//        if (!valid) {
//            return;
//        }
//        var dashes = segements.slice();
//        if (segments.length & 1) {
//            dashes = dashes.concat(dashes);
//        }
//        this._lineDashes = dashes;
//    },
//    getLineDash() {
//        return this._lineDashes.slice();
//    },
//    get lineDashOffset() {
//        return this._lineDashOffset;
//    },
//    set lineDashOffset(value) {
//        if (value <= 0 || isNaN(value) || value == Number.NEGATIVE_INFINITY || value == Number.POSITIVE_INFINITY) {
//            return;
//        }
//        this._lineDashOffset = value;
//    },
//    get font() {
//        return this._fontString;
//    },
//    set font(value) {
//        if (font = parseFont(value)) {
//            this._fontString = value;
//            this._font = font;
//        }
//    },
//    get textAlign() {
//        return this._textAlign;
//    },
//    set textAlign(value) {
//        if (valid_textAlign.indexOf(value) !== -1) {
//            this._textAlign = value;
//        }
//    },
//    get textBaseline() {
//        return this._textBaseline;
//    },
//    set textBaseline(value) {
//        if (valid_textBaseline.indexOf(value) !== -1) {
//            this._textBaseline = value;
//        }
//    }
//});

