/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

var cairo = require('builtin/cairo'),
    console = require('console'),
    canvas = require('Canvas');
        
function test1() {
    var surface = cairo.image_surface_create(cairo.FORMAT_ARGB32, 120, 120);
    var context = cairo.context_create(surface);
	cairo.context_scale (context, 120, 120);

    var radpat = cairo.pattern_create_radial (0.25, 0.25, 0.1,  0.5, 0.5, 0.5);
    cairo.pattern_add_color_stop_rgb (radpat, 0,  1.0, 0.8, 0.8);
    cairo.pattern_add_color_stop_rgb (radpat, 1,  0.9, 0.0, 0.0);

    for (var i=1; i<10; i++) {
        for (var j=1; j<10; j++) {
            cairo.context_rectangle (context, i/10.0 - 0.04, j/10.0 - 0.04, 0.08, 0.08);
        }
    }
    cairo.context_set_source (context, radpat);
    cairo.context_fill (context);

    var linpat = cairo.pattern_create_linear (0.25, 0.35, 0.75, 0.65);
    cairo.pattern_add_color_stop_rgba (linpat, 0.00,  1, 1, 1, 0);
    cairo.pattern_add_color_stop_rgba (linpat, 0.25,  0, 1, 0, 0.5);
    cairo.pattern_add_color_stop_rgba (linpat, 0.50,  1, 1, 1, 0);
    cairo.pattern_add_color_stop_rgba (linpat, 0.75,  0, 0, 1, 0.5);
    cairo.pattern_add_color_stop_rgba (linpat, 1.00,  1, 1, 1, 0);

    cairo.context_rectangle (context, 0.0, 0.0, 1, 1);
    cairo.context_set_source (context, linpat);
    cairo.context_fill (context);

    cairo.context_destroy(context);
    cairo.surface_write_to_png(surface, "foo.png");
    cairo.surface_destroy(surface);
}

function test2() {
    var surface = cairo.image_surface_create (cairo.FORMAT_ARGB32, 240, 80);
    var cr = cairo.context_create (surface);

    cairo.context_select_font_face (cr, "serif", cairo.FONT_SLANT_NORMAL, cairo.FONT_WEIGHT_BOLD);
    cairo.context_set_font_size (cr, 32.0);
    cairo.context_set_source_rgb (cr, 0.0, 0.0, 1.0);
    cairo.context_move_to (cr, 10.0, 50.0);
    cairo.context_show_text (cr, "Hello, world");

    cairo.context_destroy (cr);
    cairo.surface_write_to_png (surface, "hello.png");
    cairo.surface_destroy (surface);
    return 0;    
}

function test3() {
    var surface = cairo.image_surface_create (cairo.FORMAT_ARGB32, 240, 80);
    var cr = cairo.context_create (surface);
    cairo.context_set_source_rgb (cr, 0.5, 0.5, 0.5);
    cairo.context_rectangle (cr, 10,10, 50, 50);
    cairo.context_fill (cr);
    cairo.surface_write_to_png(surface, "foo.png");
}

function test4() {
    var surface = cairo.image_surface_create (cairo.FORMAT_ARGB32, 120, 120);
    var cr = cairo.context_create (surface);
	/* Examples are in 1.0 x 1.0 coordinate space */
	cairo.context_scale (cr, 120, 120);

	/* Drawing code goes here */
	cairo.context_set_source_rgb (cr, 0, 0, 0);
	cairo.context_move_to (cr, 0, 0);
	cairo.context_line_to (cr, 1, 1);
	cairo.context_move_to (cr, 1, 0);
	cairo.context_line_to (cr, 0, 1);
	cairo.context_set_line_width (cr, 0.2);
	cairo.context_stroke (cr);

	cairo.context_rectangle (cr, 0, 0, 0.5, 0.5);
	cairo.context_set_source_rgba (cr, 1, 0, 0, 0.80);
	cairo.context_fill (cr);

	cairo.context_rectangle (cr, 0, 0.5, 0.5, 0.5);
	cairo.context_set_source_rgba (cr, 0, 1, 0, 0.60);
	cairo.context_fill (cr);

	cairo.context_rectangle (cr, 0.5, 0, 0.5, 0.5);
	cairo.context_set_source_rgba (cr, 0, 0, 1, 0.40);
	cairo.context_fill (cr);

	/* Write output and clean up */
    cairo.surface_write_to_png (surface, "setsourcergba.png");
    cairo.context_destroy (cr);
    cairo.surface_destroy (surface);
}

function test5() {
    var surface = cairo.image_surface_create (cairo.FORMAT_ARGB32, 640, 400);
    var cr = cairo.context_create (surface);
    var img = cairo.image_surface_create_from_png('watermark3.png')    ;
    cairo.context_save(cr);
    cairo.context_set_source_surface(cr, img, 10, 10);
    cairo.context_paint_with_alpha(cr, 1);
//    cairo.context_paint(cr);
    cairo.context_restore(cr);
	/* Write output and clean up */
    cairo.surface_write_to_png (surface, "test5.png");
    cairo.context_destroy (cr);
    cairo.surface_destroy (surface);
}
function main() {
    test5();
}
