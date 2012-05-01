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
 * http://www.whatwg.org/specs/web-apps/current-work/multipage/the-canvas-element.html
 */

var cairo = require('builtin/cairo');

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
        
    },
    restore: function() {
        
    },
    // transformations (default transform is the identity matrix)
    get currentTransform() {
        
    },
    set currentTransform(value) {
        
    },
    scale: function(x, y) {
        
    },
    rotate: function(angle) {
        
    },
    translate: function(x, y) {
        
    },
    transform: function(a, b, c, d, e, f) {
        
    },
    setTransform: function(a, b, c, d, e, f) {
        
    },
    resetTransform: function() {
        
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
    get shadowBlur: function() {
        
    },
    set shadowBlur: function(value) {
        
    },
    get shadowColor: function() {
        
    },
    set shadowColor: function(value) {
        
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
    resetClip: function() {
        
    },
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
