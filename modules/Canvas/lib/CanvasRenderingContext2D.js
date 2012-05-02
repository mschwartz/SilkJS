/** @ignore */

var cairo = require('builtin/cairo'),
    CanvasTransformation = require('CanvasTransformation'),
    CanvasLineStyles = require('CanvasLineStyles'),
    CanvasPathMethods = require('CanvasPathMethods'),
    CanvasText = require('CanvasText');

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
CanvasRenderingContext2D.prototype.extend(CanvasLineStyles.prototype);
CanvasRenderingContext2D.prototype.extend(CanvasPathMethods.prototype);
CanvasRenderingContext2D.prototype.extend(CanvasText.prototype);

exports = CanvasRenderingContext2D;
