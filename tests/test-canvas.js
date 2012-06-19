require.path.unshift('../modules');

var cairo = require('builtin/cairo'),
    Canvas = require('Canvas').Canvas,
    Image = require('Canvas').Image;

function test1() {
    var surface = cairo.image_surface_create (cairo.FORMAT_ARGB32, 120, 120);
    var cr = cairo.context_create (surface);
    var img = new Image('watermark3.png')    ;
    cairo.context_save(cr);
    cairo.context_set_source_surface(cr, img._surface, 10, 10);
    cairo.context_paint_with_alpha(cr, 1);
    cairo.context_restore(cr);
	/* Write output and clean up */
    cairo.surface_write_to_png (surface, "yyy.png");
    cairo.context_destroy (cr);
    cairo.surface_destroy (surface);
}

function test2() {
    var canvas = new Canvas(120,120);
    var surface = canvas.surface;
    var ctx = canvas.getContext('2d');
    var cr = ctx._context;
    var img = new Image('watermark3.png')    ;
    cairo.context_save(cr);
    cairo.context_set_source_surface(cr, img._surface, 10, 10);
    cairo.context_paint_with_alpha(cr, 1);
    cairo.context_restore(cr);
	/* Write output and clean up */
    cairo.surface_write_to_png (surface, "yyy.png");
    cairo.context_destroy (cr);
    cairo.surface_destroy (surface);
}

function test3() {
    var c = new Canvas(120,120);
    var ctx = c.getContext('2d');
    var img = new Image('watermark3.png');
    ctx.drawImage(img, 10,10);
    cairo.surface_write_to_png (c.surface, "yyy.png");
}

function test4() {
    var c = new Canvas(640,400);
    var ctx = c.getContext('2d');
    ctx.save();
    var grad = ctx.createLinearGradient(300,0,300,400);
    grad.addColorStop(0, '#e1eaec');
    grad.addColorStop(1, '#edf3f5');
    ctx.fillStyle = grad;
    ctx.strokeStyle = '#000';
    ctx.lineWidth = 0;
    ctx.beginPath();
    ctx.moveTo(0,0);
    ctx.lineTo(600,0);
    ctx.lineTo(600,400);
    ctx.lineTo(0,400);
    ctx.lineTo(0,0);
    ctx.lineTo(1,0);
    ctx.fill();
    ctx.closePath();
    ctx.restore();
    var img = new Image('watermark3.png');
    ctx.drawImage(img, 10,10);
    cairo.surface_write_to_png (c.surface, "yyy.png");
}

function test5() {
    var surface = cairo.image_surface_create (cairo.FORMAT_ARGB32, 640, 400);
    var cr = cairo.context_create (surface);
    cairo.context_save(cr);
    var grad = cairo.pattern_create_linear(300,0,300,400);
    cairo.pattern_add_color_stop_rgba(grad, 0, 225/255, 234/255, 236/255, 255);
    cairo.pattern_add_color_stop_rgba(grad, 1, 237/255, 243/255, 245/255, 255);
    cairo.pattern_set_filter(cr, cairo.FILTER_GOOD);
//    cairo.context_set_source_rgba(cr, 255,0,0, 1);
    cairo.context_set_source(cr, grad);
    cairo.context_new_path(cr);
    cairo.context_move_to(cr, 0, 0);
    cairo.context_line_to(cr, 600, 0);
    cairo.context_line_to(cr, 600, 400);
    cairo.context_line_to(cr, 0, 400);
    cairo.context_line_to(cr, 0, 0);
    cairo.context_line_to(cr, 1, 0);
    cairo.context_set_line_width(cr, 1);
    cairo.context_fill(cr);
    cairo.context_close_path(cr);
    cairo.context_restore();
    cairo.surface_write_to_png (surface, "zzz.png");
}

function test6() {
    var surface = cairo.image_surface_create (cairo.FORMAT_ARGB32, 640, 400);
    var cr = cairo.context_create (surface);
    cairo.context_save(cr);
    cairo.context_move_to(cr, 10, 10);
    cairo.context_line_to(cr, 20, 20);
    cairo.context_set_line_width(cr, 1);
    cairo.context_stroke(cr);
    cairo.context_restore();
    cairo.surface_write_to_png (surface, "zzz.png");
}
function main() {
    test5();
}