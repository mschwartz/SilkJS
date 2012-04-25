/**
 * @module builtin/cairo
 * 
 * ### Synopsis
 * 
 * Interface to libcairo graphics library.
 * 
 */
#include "SilkJS.h"
#include <cairo/cairo.h>

/**
 * @function cairo.surface_create_similar
 * 
 * ### Synopsis
 * 
 * var newSurface = cairo.surface_create_similar(handle, format, width, height);
 * 
 * Create a new surface that is as compatible as possible with an existing surface. 
 * 
 * For example the new surface will have the same fallback resolution and font options as other. Generally, the new surface will also use the same backend as other, unless that is not possible for some reason. The type of the returned surface may be examined with cairo.surface_get_type().
 * 
 * Initially the surface contents are all 0 (transparent if contents have transparency, black otherwise.)
 * 
 * Use cairo.surface_create_similar_image() if you need an image surface which can be painted quickly to the target surface.
 *
 * The format parameter is one of the following:
 * 
 * + cairo.CONTENT_COLOR - the surface will hold color content only.
 * + cairo.CONTENT_ALPHA - the surface will hold alpha content only.
 * + cairo.CONTENT_COLOR_ALPHA - the surface will hold color and alpha content.
 *
 * @param {object} surface - opaque handle to a cairo surface.
 * @param {int} format - format of pixels in the surface to create.
 * @param {int} width - width of the surface, in pixels
 * @param {int} height - height of the surface, in pixels.
 * @return {object} newSurface - opaque handle to the newly created surface. The caller owns the surface and should call cairo.surface_destroy() when done with it. 
 */
static JSVAL surface_create_similar(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    int format = args[1]->IntegerValue();
    int width = args[2]->IntegerValue();
    int height = args[3]->IntegerValue();
    return External::New(cairo_surface_create_similar(surface, (cairo_content_t)format, width, height));
}

/**
 * @function cairo.surface_reference
 * 
 * ### Synopsis
 * 
 * var surface = cairo.surface_reference(surface);
 * 
 * Increases the reference count on surface by one. This prevents surface from being destroyed until a matching call to cairo.surface_destroy() is made.
 * 
 * The number of references to a cairo surface can be get using cairo.surface_get_reference_count().
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {object} surface - the referenced cairo surface.
 */
static JSVAL surface_reference(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return External::New(cairo_surface_reference(surface));
}

/**
 * @function cairo.surface_status
 * 
 * ### Synopsis
 * 
 * var status = cairo.surface_status(surface);
 * 
 * Checks whether an error has previously occurred for this surface.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {int} status - one of cairo.STATUS_SUCCESS, cairo.STATUS_NULL_POINTER, cairo.STATUS_NO_MEMORY, cairo.STATUS_READ_ERROR, cairo.STATUS_INVALID_CONTENT, cairo.STATUS_INVALID_FORMAT, or cairo.STATUS_INVALID_VISUAL.
 */
static JSVAL surface_status(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_surface_status(surface));
}

/**
 * @function cairo.surface_destroy
 * 
 * ### Synopsis
 * 
 * cairo.surface_destroy(surface);
 * 
 * Decreases the reference count on surface by one. If the result is zero, then surface and all associated resources are freed. 
 * 
 * See cairo.surface_reference().
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 */
static JSVAL surface_destroy(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    cairo_surface_destroy(surface);
    return Undefined();
}

/**
 * @function cairo.surface_finish
 * 
 * ### Synopsis
 * 
 * cairo.surface_finish(surface);
 * 
 * This function finishes the surface and drops all references to external resources. 
 * 
 * For example, for the Xlib backend it means that cairo will no longer access the drawable, which can be freed. 
 * 
 * After calling cairo.surface_finish() the only valid operations on a surface are getting and setting user, referencing and destroying, and flushing and finishing it. Further drawing to the surface will not affect the surface but will instead trigger a cairo.STATUS_SURFACE_FINISHED error.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 */
static JSVAL surface_finish(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    cairo_surface_finish(surface);
    return Undefined();
}

/**
 * @function cairo.surface_flush
 * 
 * ### Synopsis
 * 
 * cairo.surface_flush(surface);
 * 
 * Do any pending drawing for the surface and also restore any temporary modifications cairo has made to the surface's state. 
 * 
 * This function must be called before switching from drawing on the surface with cairo to drawing on it directly with native APIs. 
 * 
 * If the surface doesn't support direct access, then this function does nothing.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 */
static JSVAL surface_flush(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    cairo_surface_flush(surface);
    return Undefined();
}

/**
 * @function cairo.surface_get_device
 * 
 * ### Synopsis
 * 
 * var device = cairo.surface_get_device(surface);
 * 
 * This function returns the device for a surface.
 * 
 * See device methods.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {object} device - opaque handle to the device for the surface, or null if the surface does not have an associated device.
 */
static JSVAL surface_get_device(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    cairo_device_t *device = cairo_surface_get_device(surface);
    if (device == NULL) {
        return Null();
    }
    return External::New(device);
}

/**
 * @function cairo.surface_get_font_options
 * 
 * ### Synopsis
 * 
 * var options = cairo.surface_get_font_options(surface);
 * 
 * Retrieves the default font rendering options for the surface. 
 * 
 * This allows display surfaces to report the correct subpixel order for rendering on them, print surfaces to disable hinting of metrics and so forth. 
 * 
 * The result can then be used with cairo.scaled_font_create().
 * 
 * NOTE: the resulting options object should be freed with cairo.font_options_destroy().
 * 
 * See font_options methods.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {object} options - opaque handle to the font options for the surface, or null if the surface does not have an associated device.
 */
static JSVAL surface_get_font_options(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    cairo_font_options_t *options = cairo_font_options_create();
    cairo_surface_get_font_options(surface, options);
    return External::New(options);
}

/**
 * @function cairo.surface_get_content
 * 
 * ### Synopsis
 * 
 * var content = cairo.surface_get_content(surface);
 * 
 * This function returns the content type of surface which indicates whether the surface contains color and/or alpha information.
 * 
 * The content type returned is one of:
 * 
 * + cairo.CONTENT_COLOR
 * + cairo.CONTENT_ALPHA
 * + cairo.CONTENT_COLOR_ALPHA
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {int} content - type content type of surface.
 */
static JSVAL surface_get_content(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_surface_get_content(surface));
}

/**
 * @function cairo.surface_mark_dirty
 * 
 * ### Synopsis
 * 
 * cairo.surface_mark_dirty(surface);
 * 
 * Tells cairo that drawing has been done to surface using means other than cairo, and that cairo should reread any cached areas. 
 * 
 * Note that you must call cairo.surface_flush() before doing such drawing.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 */
static JSVAL surface_mark_dirty(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    cairo_surface_mark_dirty(surface);
    return Undefined();
}

/**
 * @function cairo.surface_mark_dirty_rectangle
 * 
 * ### Synopsis
 * 
 * cairo.surface_mark_dirty(surface, x,y, w,h);
 * 
 * Like cairo.surface_mark_dirty(), but drawing has been done only to the specified rectangle, so that cairo can retain cached contents for other parts of the surface.
 * 
 * Any cached clip set on the surface will be reset by this function, to make sure that future cairo calls have the clip set that they expect.
 * 
 * Note that you must call cairo.surface_flush() before doing such drawing.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @param {int} x - upper left x coordinate of dirty rectangle
 * @param {int} y - upper left y coordinate of dirty rectangle
 * @param {int} w - width of dirty rectangle
 * @param {int} h - height of dirty rectangle
 */
static JSVAL surface_mark_dirty_rectangle(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    int x = args[1]->IntegerValue();
    int y = args[2]->IntegerValue();
    int w = args[3]->IntegerValue();
    int h = args[4]->IntegerValue();
    cairo_surface_mark_dirty_rectangle(surface, x,y, w,h);
    return Undefined();
}

/**
 * @function cairo.surface_set_device_offset
 * 
 * ### Synopsis
 * 
 * cairo.surface_set_device_offset(surface, x_offset, y_offset);
 * 
 * Sets an offset that is added to the device coordinates determined by the CTM when drawing to surface. 
 * 
 * One use case for this function is when we want to create a cairo surface that redirects drawing for a portion of an on-screen surface to an offscreen surface in a way that is completely invisible to the user of the cairo API. Setting a transformation via cairo.translate() isn't sufficient to do this, since functions like cairo.device_to_user() will expose the hidden offset.
 * 
 * Note that the offset affects drawing to the surface as well as using the surface in a source pattern.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @param {number} x_offset - the offset in the X direction, in device units
 * @param {number} y_offset - the offset in the Y direction, in device units
 */
static JSVAL surface_set_device_offset(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    double dx = args[1]->NumberValue();
    double dy = args[2]->NumberValue();
    cairo_surface_set_device_offset(surface, dx, dy);
    return Undefined();
}

/**
 * @function cairo.surface_get_device_offset
 * 
 * ### Synopsis
 * 
 * var offsets = cairo.surface_get_device_offset(surface);
 * 
 * This function returns the previous device offset set by cairo.surface_set_device_offset().
 * 
 * The object returned is of the form:
 * 
 * + {number} x_offset: the offset in the X direction, in device units
 * + {number} y_offset: the offset in the Y direction, in device units
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {object} offsets - offsets.x,offsets.y are floating point x_offset,y_offset values.
 */
static JSVAL surface_get_device_offset(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    double dx;
    double dy;
    cairo_surface_get_device_offset(surface, &dx, &dy);
    JSOBJ o = Object::New();
    o->Set(String::New("x_offset"), Number::New(dx));
    o->Set(String::New("y_offset"), Number::New(dy));
    return o;
}

/**
 * @function cairo.surface_set_fallback_resolution
 * 
 * ### Synopsis
 * 
 * cairo.surface_set_fallback_resolution(surface, x_pixels_per_inch, y_pixels_per_inch);
 * 
 * Set the horizontal and vertical resolution for image fallbacks.
 * 
 * When certain operations aren't supported natively by a backend, cairo will fallback by rendering operations to an image and then overlaying that image onto the output. For backends that are natively vector-oriented, this function can be used to set the resolution used for these image fallbacks, (larger values will result in more detailed images, but also larger file sizes).
 * 
 * Some examples of natively vector-oriented backends are the ps, pdf, and svg backends.
 * 
 * For backends that are natively raster-oriented, image fallbacks are still possible, but they are always performed at the native device resolution. So this function has no effect on those backends.
 * 
 * Note: The fallback resolution only takes effect at the time of completing a page (with cairo.show_page() or cairo.copy_page()) so there is currently no way to have more than one fallback resolution in effect on a single page.
 * 
 * The default fallback resoultion is 300 pixels per inch in both dimensions.
 *  
 * @param {object} surface - opaque handle to a cairo surface.
 * @param {number} x_pixels_per_inch - horizontal setting for pixels per inch.
 * @param {number} y_pixels_per_inch - vertical setting for pixels per inch.
 */
static JSVAL surface_set_fallback_resolution(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    double dx = args[1]->NumberValue();
    double dy = args[2]->NumberValue();
    cairo_surface_set_fallback_resolution(surface, dx, dy);
    return Undefined();
}

/**
 * @function cairo.surface_get_fallback_resolution
 * 
 * ### Synopsis
 * 
 * var resolution = cairo.surface_get_fallback_resolution(surface);
 * 
 * This function returns the previous device offset set by cairo.surface_set_fallback_resolution().
 * 
 * The object returned is of the form:
 * 
 * + {number} x_pixels_per_inch: the offset in the X direction, in device units
 * + {number} y_pixels_per_inch: the offset in the Y direction, in device units
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {object} resolution - resolution.x,resolution.y are floating point pixels_per_inch values.
 */
static JSVAL surface_get_fallback_resolution(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    double dx;
    double dy;
    cairo_surface_get_fallback_resolution(surface, &dx, &dy);
    JSOBJ o = Object::New();
    o->Set(String::New("x_offset"), Number::New(dx));
    o->Set(String::New("y_offset"), Number::New(dy));
    return o;
}

/**
 * @function cairo.surface_get_type
 * 
 * ### Synopsis
 * 
 * var type = cairo.surface_get_type(surface);
 * 
 * This function returns the type of the backend used to create a surface.
 * 
 * The returned type will be one of the following:
 * 
 * + cairo.SURFACE_TYPE_IMAGE - The surface is of type image
 * + cairo.SURFACE_TYPE_PDF - The surface is of type pdf
 * + cairo.SURFACE_TYPE_PS - The surface is of type ps
 * + cairo.SURFACE_TYPE_XLIB - The surface is of type xlib
 * + cairo.SURFACE_TYPE_XCB - The surface is of type xcb
 * + cairo.SURFACE_TYPE_GLITZ - The surface is of type glitz
 * + cairo.SURFACE_TYPE_QUARTZ - The surface is of type quartz
 * + cairo.SURFACE_TYPE_WIN32 - The surface is of type win32
 * + cairo.SURFACE_TYPE_BEOS - The surface is of type beos
 * + cairo.SURFACE_TYPE_DIRECTFB - The surface is of type directfb
 * + cairo.SURFACE_TYPE_SVG - The surface is of type svg
 * + cairo.SURFACE_TYPE_OS2 - The surface is of type os2
 * + cairo.SURFACE_TYPE_WIN32_PRINTING - The surface is a win32 printing surface
 * + cairo.SURFACE_TYPE_QUARTZ_IMAGE - The surface is of type quartz_image
 * + cairo.SURFACE_TYPE_SCRIPT - The surface is of type script, since 1.10
 * + cairo.SURFACE_TYPE_QT - The surface is of type Qt, since 1.10
 * + cairo.SURFACE_TYPE_RECORDING - The surface is of type recording, since 1.10
 * + cairo.SURFACE_TYPE_VG - The surface is a OpenVG surface, since 1.10
 * + cairo.SURFACE_TYPE_GL - The surface is of type OpenGL, since 1.10
 * + cairo.SURFACE_TYPE_DRM - The surface is of type Direct Render Manager, since 1.10
 * + cairo.SURFACE_TYPE_TEE - The surface is of type 'tee' (a multiplexing surface), since 1.10
 * + cairo.SURFACE_TYPE_XML - The surface is of type XML (for debugging), since 1.10
 * + cairo.SURFACE_TYPE_SKIA - The surface is of type Skia, since 1.10
 * + cairo.SURFACE_TYPE_SUBSURFACE - The surface is a subsurface created with cairo_surface_create_for_rectangle(), since 1.10
 * + cairo.SURFACE_TYPE_COGL - This surface is of type Cogl, since 1.12
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {int} type - one of the types above.
 */
static JSVAL surface_get_type(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_surface_get_type(surface));
}

/**
 * @function cairo.surface_get_reference_count
 * 
 * ### Synopsis
 * 
 * var count = cairo.surface_get_reference_count(surface);
 * 
 * Returns the current reference count of surface.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {int} count - reference count of surface.  If the object is a nil object, 0 will be returned.
 */
static JSVAL surface_get_reference_count(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_surface_get_reference_count(surface));
}

/**
 * @function cairo.surface_copy_page
 * 
 * ### Synopsis
 * 
 * cairo.surface_copy_page(surface);
 * 
 * Emits the current page for backends that support multiple pages, but doesn't clear it, so that the contents of the current page will be retained for the next page. 
 * 
 * Use cairo.surface_show_page() if you want to get an empty page after the emission.
 * 
 * There is a convenience function for this that takes a Context, namely cairo.copy_page().
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 */
static JSVAL surface_copy_page(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    cairo_surface_copy_page(surface);
    return Undefined();
}

/**
 * @function cairo.surface_show_page
 * 
 * ### Synopsis
 * 
 * cairo.surface_show_page(surface);
 * 
 * Emits and clears the current page for backends that support multiple pages. Use cairo.surface_copy_page() if you don't want to clear the page.
 * 
 * There is a convenience function for this that takes a Context, namely cairo.show_page().
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 */
static JSVAL surface_show_page(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    cairo_surface_show_page(surface);
    return Undefined();
}

/**
 * @function cairo.surface_has_show_text_glyphs
 * 
 * ### Synopsis
 * 
 * var status = cairo.surface_has_show_text_glyphs(surface);
 * 
 * Returns whether the surface supports sophisticated cairo.show_text_glyphs() operations. That is, whether it actually uses the provided text and cluster data to a cairo.show_text_glyphs() call.
 * 
 * Note: Even if this function returns false, a cairo.show_text_glyphs() operation targeted at surface will still succeed. It just will act like a cairo.show_glyphs() operation. 
 * 
 * Users can use this function to avoid computing UTF-8 text and cluster mapping if the target surface does not use it.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {boolean} status - true if the surface supports show_text_glyphs().
 */
static JSVAL surface_has_show_text_glyphs(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return cairo_surface_has_show_text_glyphs(surface) ? True() : False();
}


/**
 * @function cairo.image_surface_create
 * 
 * ### Synopsis
 * 
 * var surface = cairo.image_surface_create(format, width, height);
 * 
 * Creates an image surface of the specified format and dimensions. Initially the surface contents are all 0. (Specifically, within each pixel, each color or alpha channel belonging to format will be 0. The contents of bits within a pixel, but not belonging to the given format are undefined).
 * 
 * This function always returns a valid handle, but it will return a handle to a "nil" surface if an error such as out of memory occurs. 
 * 
 * You can use cairo.surface_status() to check for this.
 * 
 * The format parameter is one of the following:
 * 
 * + cairo.CONTENT_COLOR - the surface will hold color content only.
 * + cairo.CONTENT_ALPHA - the surface will hold alpha content only.
 * + cairo.CONTENT_COLOR_ALPHA - the surface will hold color and alpha content.
 *
 * @param {int} format - format of pixels in the surface to create.
 * @param {int} width - width of the surface, in pixels
 * @param {int} height - height of the surface, in pixels.
 * @return {object} surface - opaque handle to the newly created surface. The caller owns the surface and should call cairo.surface_destroy() when done with it. 
 */
static JSVAL image_surface_create(JSARGS args) {
    int format = args[0]->IntegerValue();
    int width = args[1]->IntegerValue();
    int height = args[1]->IntegerValue();
    return External::New(cairo_image_surface_create((cairo_format_t)format, width, height));
}

/**
 * @functino cairo.image_surface_get_format
 * 
 * ### Synopsis
 * 
 * var format = cairo.image_surface_get_format(surface);
 * 
 * Get the format of the surface.
 * 
 * The format returned will be one of the following:
 * 
 * + cairo.FORMAT_ARGB32
 * + cairo.FORMAT_RGB24
 * + cairo.FORMAT_A8
 * + cairo.FORMAT_A1
 * + cairo.FORMAT_RGB16_565
 * + cairo.FORMAT_RGB30
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {int} format - one of the above values.
 */
static JSVAL image_surface_get_format(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_image_surface_get_format(surface));
}

/**
 * @functino cairo.image_surface_get_width
 * 
 * ### Synopsis
 * 
 * var width = cairo.image_surface_get_width(surface);
 * 
 * Get the width of the surface.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {int} width - width of the surface in pixels.
 */
static JSVAL image_surface_get_width(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_image_surface_get_width(surface));
}

/**
 * @functino cairo.image_surface_get_height
 * 
 * ### Synopsis
 * 
 * var height = cairo.image_surface_get_height(surface);
 * 
 * Get the height of the surface.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {int} height - height of the surface in pixels.
 */
static JSVAL image_surface_get_height(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_image_surface_get_height(surface));
}


////////////////////// CONTEXTS

/**
 * @function cairo.context_create
 * 
 * ### Synopsis
 * 
 * var context = cairo.context_create(surface);
 * 
 * Creates a new context with all graphics state parameters set to default values and with surface as a target surface. The target surface should be constructed with a backend-specific function such as cairo.image_surface_create() (or any other cairo.backend_surface_create() variant).
 * 
 * This function references surface, so you can immediately call cairo.surface_destroy() on it if you don't need to maintain a separate reference to it.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 * @return {object} context - opaque handle to a cairo context.
 */
static JSVAL context_create(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return External::New(cairo_create(surface));
}

/**
 * @function cairo.context_reference
 * 
 * ### Synopsis
 * 
 * var context = cairo.context_reference(context);
 * 
 * Increases the reference count on context by one. This prevents context from being destroyed until a matching call to cairo.context_destroy() is made.
 * 
 * The number of references to a context can be get using cairo.context_get_reference_count().
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} context - opaque handle to the referenced cairo context.
 */
static JSVAL context_reference(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return External::New(cairo_reference(context));
}

/**
 * @function cairo.context_get_reference_count
 * 
 * ### Synopsis
 * 
 * var count = cairo.context_get_reference_count(context);
 * 
 * Returns the current reference count of context.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {int} count - reference count of the context.
 */
static JSVAL context_get_reference_count(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_get_reference_count(context));
}

/**
 * @function cairo.context_destroy
 * 
 * ### Synopsis
 * 
 * cairo.context_destroy(context);
 * 
 * Decreases the reference count on context by one. If the result is zero, then context and all associated resources are freed. 
 * 
 * See cairo.context_reference().
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_destroy(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_destroy(context);
    return Undefined();
}

/**
 * @function cairo.context_status
 * 
 * ### Synopsis
 * 
 * var status = cairo.context_status(context);
 * 
 * Returns the previous error status code for this context.
 * 
 * The status returned is one of the following:
 *  
 * + cairo.STATUS_SUCCESS - no error has occurred
 * + cairo.STATUS_NO_MEMORY - out of memory
 * + cairo.STATUS_INVALID_RESTORE - cairo_restore() called without matching cairo_save()
 * + cairo.STATUS_INVALID_POP_GROUP - no saved group to pop, i.e. cairo_pop_group() without matching cairo_push_group()
 * + cairo.STATUS_NO_CURRENT_POINT - no current point defined
 * + cairo.STATUS_INVALID_MATRIX - invalid matrix (not invertible)
 * + cairo.STATUS_INVALID_STATUS - invalid value for an input + cairo.STATUS_t
 * + cairo.STATUS_NULL_POINTER - NULL pointer
 * + cairo.STATUS_INVALID_STRING - input string not valid UTF-8
 * + cairo.STATUS_INVALID_PATH_DATA - input path data not valid
 * + cairo.STATUS_READ_ERROR - error while reading from input stream
 * + cairo.STATUS_WRITE_ERROR - error while writing to output stream
 * + cairo.STATUS_SURFACE_FINISHED - target surface has been finished
 * + cairo.STATUS_SURFACE_TYPE_MISMATCH - the surface type is not appropriate for the operation
 * + cairo.STATUS_PATTERN_TYPE_MISMATCH - the pattern type is not appropriate for the operation
 * + cairo.STATUS_INVALID_CONTENT - invalid value for an input cairo_content_t
 * + cairo.STATUS_INVALID_FORMAT - invalid value for an input cairo_format_t
 * + cairo.STATUS_INVALID_VISUAL - invalid value for an input Visual*
 * + cairo.STATUS_FILE_NOT_FOUND - file not found
 * + cairo.STATUS_INVALID_DASH - invalid value for a dash setting
 * + cairo.STATUS_INVALID_DSC_COMMENT - invalid value for a DSC comment (Since 1.2)
 * + cairo.STATUS_INVALID_INDEX - invalid index passed to getter (Since 1.4)
 * + cairo.STATUS_CLIP_NOT_REPRESENTABLE - clip region not representable in desired format (Since 1.4)
 * + cairo.STATUS_TEMP_FILE_ERROR - error creating or writing to a temporary file (Since 1.6)
 * + cairo.STATUS_INVALID_STRIDE - invalid value for stride (Since 1.6)
 * + cairo.STATUS_FONT_TYPE_MISMATCH - the font type is not appropriate for the operation (Since 1.8)
 * + cairo.STATUS_USER_FONT_IMMUTABLE - the user-font is immutable (Since 1.8)
 * + cairo.STATUS_USER_FONT_ERROR - error occurred in a user-font callback function (Since 1.8)
 * + cairo.STATUS_NEGATIVE_COUNT - negative number used where it is not allowed (Since 1.8)
 * + cairo.STATUS_INVALID_CLUSTERS - input clusters do not represent the accompanying text and glyph array (Since 1.8)
 * + cairo.STATUS_INVALID_SLANT - invalid value for an input cairo_font_slant_t (Since 1.8)
 * + cairo.STATUS_INVALID_WEIGHT - invalid value for an input cairo_font_weight_t (Since 1.8)
 * + cairo.STATUS_INVALID_SIZE - invalid value (typically too big) for the size of the input (surface, pattern, etc.) (Since 1.10)
 * + cairo.STATUS_USER_FONT_NOT_IMPLEMENTED - user-font method not implemented (Since 1.10)
 * + cairo.STATUS_DEVICE_TYPE_MISMATCH - the device type is not appropriate for the operation (Since 1.10)
 * + cairo.STATUS_DEVICE_ERROR - an operation to the device caused an unspecified error (Since 1.10)
 * + cairo.STATUS_INVALID_MESH_CONSTRUCTION - a mesh pattern construction operation was used outside of a cairo_mesh_pattern_begin_patch()/cairo_mesh_pattern_end_patch() pair (Since 1.12)
 * + cairo.STATUS_DEVICE_FINISHED - target device has been finished (Since 1.12)
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {int} status - see possible values above.
 */
static JSVAL context_status(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_status(context));
}

/**
 * @function cairo.context_save
 * 
 * ### Synopsis
 * 
 * cairo.context_save(context);
 * 
 * Makes a copy of the current state of context and saves it on an internal stack of saved states for context. 
 * 
 * When cairo.context_restore() is called, context will be restored to the saved state. 
 * 
 * Multiple calls to cairo.context_save() and cairo.context_restore() can be nested; each call to cairo.context_restore() restores the state from the matching paired cairo.context_save().
 * 
 * It isn't necessary to clear all saved states before a context is freed. 
 * 
 * If the reference count of a context drops to zero in response to a call to cairo.context_destroy(), any saved states will be freed along with the context.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_save(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_save(context);
    return Undefined();
}

/**
 * @function cairo.context_restore
 * 
 * ### Synopsis
 * 
 * cairo.context_restore(context);
 * 
 * Restores context to the state saved by a preceding call to cairo.context_save() and removes that state from the stack of saved states.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_restore(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_restore(context);
    return Undefined();
}

/**
 * @function cairo.context_get_target
 * 
 * ### Synopsis
 * 
 * var surface = cairo.context_get_target(context);
 * 
 * Gets the target surface for the cairo context as passed to cairo.context_create().
 * 
 * This function will always return a valid pointer, but the result can be a "nil" surface if cr is already in an error state, (ie. cairo.context_status() != cairo.STATUS_SUCCESS).
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} surface - opaque handle to the cairo surface.
 */
static JSVAL context_get_target(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return External::New(cairo_get_target(context));
}

/**
 * @function cairo.context_push_group
 * 
 * ### Synopsis
 * 
 * cairo.context_push_group(context);
 * 
 * Temporarily redirects drawing to an intermediate surface known as a group. 
 * 
 * The redirection lasts until the group is completed by a call to cairo.context_pop_group() or cairo.context_pop_group_to_source(). 
 * 
 * These calls provide the result of any drawing to the group as a pattern, (either as an explicit object, or set as the source pattern).
 * 
 * This group functionality can be convenient for performing intermediate compositing. One common use of a group is to render objects as opaque within the group, (so that they occlude each other), and then blend the result with translucence onto the destination.
 * 
 * Groups can be nested arbitrarily deep by making balanced calls to cairo.context_push_group()/cairo.context_pop_group(). Each call pushes/pops the new target group onto/from a stack.
 * 
 * The cairo.context_push_group() function calls cairo.context_save() so that any changes to the graphics state will not be visible outside the group, (the cairo.content_pop_group() functions call cairo.context__restore()).
 * 
 * By default the intermediate group will have a content type of cairo.CONTENT_COLOR_ALPHA. Other content types can be chosen for the group by using cairo.context_push_group_with_content() instead.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * 
 * ### Example
 * 
 * As an example, here is how one might fill and stroke a path with translucence, but without any portion of the fill being visible under the stroke:
 * 
 * ```
 * cairo.context_push_group (context);
 * cairo.context_set_source (context, fill_pattern);
 * cairo.context_fill_preserve (context);
 * cairo.context_set_source (context, stroke_pattern);
 * cairo.context_stroke (context);
 * cairo.context_pop_group_to_source (context);
 * cairo.context_paint_with_alpha (context, alpha);
 * ```
 */
static JSVAL context_push_group(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_push_group(context);
    return Undefined();
}

/**
 * @function cairo.context_push_group_with_content
 * 
 * ### Synopsis
 * 
 * cairo.context_push_group_with_content(context, content);
 * 
 * Temporarily redirects drawing to an intermediate surface known as a group. The redirection lasts until the group is completed by a call to cairo.context_pop_group() or cairo.context_pop_group_to_source(). These calls provide the result of any drawing to the group as a pattern, (either as an explicit object, or set as the source pattern).
 * 
 * The group will have a content type of content. The ability to control this content type is the only distinction between this function and cairo.content_push_group() which you should see for a more detailed description of group rendering.
 * 
 * The content parameter is one of the following value:
 * 
 * + cairo.CONTENT_COLOR - the surface will hold color content only.
 * + cairo.CONTENT_ALPHA - the surface will hold alpha content only.
 * + cairo.CONTENT_COLOR_ALPHA - the surface will hold color and alpha content.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_push_group_with_content(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_push_group_with_content(context, (cairo_content_t)args[0]->IntegerValue());
    return Undefined();
}

/**
 * @function cairo.context_pop_group
 * 
 * ### Synopsis
 * 
 * var pattern = cairo.context_pop_group(context);
 * 
 * Terminates the redirection begun by a call to cairo.context_push_group() or cairo.context_push_group_with_content() and returns a new pattern containing the results of all drawing operations performed to the group.
 * 
 * The cairo.context_pop_group() function calls cairo.context_restore(), (balancing a call to cairo.context_save() by the cairo.context_push_group() function), so that any changes to the graphics state will not be visible outside the group.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} pattern - opaque handle to a cairo pattern.
 */
static JSVAL context_pop_group(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return External::New(cairo_pop_group(context));
}

/**
 * @function cairo.context_pop_group_to_source
 * 
 * ### Synopsis
 * 
 * cairo.context_pop_group_to_source(context);
 * 
 * Terminates the redirection begun by a call to cairo.context_push_group() or cairo,context_push_group_with_content() and installs the resulting pattern as the source pattern in the given cairo context.
 * 
 * The behavior of this function is equivalent to the sequence of operations:
 * 
 * ```
 * var group = cairo.context_pop_group (context);
 * cairo.context_set_source (context, group);
 * cairo.pattern_destroy (group);
 * ```
 * but is more convenient as their is no need for a variable to store the short-lived pointer to the pattern.
 * 
 * The cairo.context_pop_group() function calls cairo.context_restore(), (balancing a call to cairo.context_save() by the cairo.context_push_group()function), so that any changes to the graphics state will not be visible outside the group.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_pop_group_to_source(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_pop_group_to_source(context);
    return Undefined();
}

/**
 * @function cairo.context_get_group_target
 * 
 * ### Synopsis
 * 
 * var surface = cairo.context_get_group_target(context);
 * 
 * Gets the current destination surface for the context. This is either the original target surface as passed to cairo.context_create() or the target surface for the current group as started by the most recent call to cairo.context_push_group() or cairo.context_push_group_with_content().
 * 
 * This function will always return a valid pointer, but the result can be a "nil" surface if context is already in an error state, (ie. cairo.context_status() != cairo.STATUS_SUCCESS).
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} surface - opaque handle to the cairo surface.
 */
static JSVAL context_get_group_target(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return External::New(cairo_get_group_target(context));
}

/**
 * @function cairo.context_set_source_rgb
 * 
 * ### Synopsis
 * 
 * cairo.context_set_source_rgb(context, red, green, blue);
 * 
 * Sets the source pattern within context to an opaque color. 
 * 
 * This opaque color will then be used for any subsequent drawing operation until a new source pattern is set.
 * 
 * The color components are floating point numbers in the range 0 to 1. If the values passed in are outside that range, they will be clamped.
 * 
 * The default source pattern is opaque black, (that is, it is equivalent to cairo.context_set_source_rgb(context, 0.0, 0.0, 0.0)).
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} red - red component of color
 * @param {number} green - green component of color
 * @param {number} blue - blue component of color
 */
static JSVAL context_set_source_rgb(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_set_source_rgb(context, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());
    return Undefined();
}

/**
 * @function cairo.context_set_source_rgba
 * 
 * ### Synopsis
 * 
 * cairo.context_set_source_rgba(context, red, green, blue, alpha);
 * 
 * Sets the source pattern within context to a translucent color. 
 * 
 * This color will then be used for any subsequent drawing operation until a new source pattern is set.
 * 
 * The color and alpha components are floating point numbers in the range 0 to 1. If the values passed in are outside that range, they will be clamped.
 * 
 * The default source pattern is opaque black, (that is, it is equivalent to cairo.context_set_source_rgba(context, 0.0, 0.0, 0.0, 1.0)).
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} red - red component of color
 * @param {number} green - green component of color
 * @param {number} blue - blue component of color
 * @param {number} alpha - alpha component of color
 */
static JSVAL context_set_source_rgba(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_set_source_rgba(context, args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue(), args[4]->NumberValue());
    return Undefined();
}

/**
 * @function cairo.context_set_source
 * 
 * ### Synopsis
 * 
 * cairo.context_set_source(context, pattern);
 * 
 * Sets the source pattern within context to pattern. 
 * 
 * This pattern will then be used for any subsequent drawing operation until a new source pattern is set.
 * 
 * Note: The pattern's transformation matrix will be locked to the user space in effect at the time of cairo.context_set_source(). This means that further modifications of the current transformation matrix will not affect the source pattern. See cairo.pattern_set_matrix().
 * 
 * The default source pattern is a solid pattern that is opaque black, (that is, it is equivalent to cairo.context_set_source_rgb(context, 0.0, 0.0, 0.0)).
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {object} pattern - opaque handle to a cairo pattern.
 */
static JSVAL context_set_source(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[1]);
    cairo_set_source(context, pattern);
    return Undefined();
}

/**
 * @function cairo.context_set_source_surface
 * 
 * ### Synopsis
 * 
 * cairo.context_set_source_surface(context, surface, x, y);
 * 
 * This is a convenience function for creating a pattern from surface and setting it as the source in context with cairo.context_set_source().
 * 
 * The x and y parameters give the user-space coordinate at which the surface origin should appear. (The surface origin is its upper-left corner before any transformation has been applied.) The x and y parameters are negated and then set as translation values in the pattern matrix.
 * 
 * Other than the initial translation pattern matrix, as described above, all other pattern attributes, (such as its extend mode), are set to the default values as in cairo.pattern_create_for_surface(). The resulting pattern can be queried with cairo.context_get_source() so that these attributes can be modified if desired, (eg. to create a repeating pattern with cairo.pattern_set_extend()).
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {object} surface - opaque handle to a cairo surface..
 * @param {number} x - x coordinate of surface origin.
 * @param {number} y - y coordinate of surface origin.
 */
static JSVAL context_set_source_surface(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[1]);
    double x = args[2]->NumberValue();
    double y = args[3]->NumberValue();
    cairo_set_source_surface(context, surface, x, y);
    return Undefined();
}

/**
 * @function cairo.context_get_source
 * 
 * ### Synopsis
 * 
 * var pattern = cairo.context_get_source(context, pattern);
 * 
 * Gets the current source pattern within context. 
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} pattern - opaque handle to a cairo pattern.
 */
static JSVAL context_get_source(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return External::New(cairo_get_source(context));
}

/**
 * @function cairo.context_set_antialias
 * 
 * ### Synopsis
 * 
 * cairo.context_set_antialias(context, mode);
 * 
 * Set the antialiasing mode of the rasterizer used for drawing shapes. 
 * 
 * This value is a hint, and a particular backend may or may not support a particular value. At the current time, no backend supports cairo.ANTIALIAS_SUBPIXEL when drawing shapes.
 * 
 * Note that this option does not affect text rendering, instead see cairo.context_font_options_set_antialias().
 * 
 * Valid modes are:
 * 
 * + cairo.ANTIALIAS_DEFAULT - use the default antialiasing for the subsystem and target device.
 * + cairo.ANTIALIAS_NONE - use a bilevel alpha mask.
 * + cairo.ANTIALIAS_GRAY - perform single-color antialiasing (using shades of gray for black text on a white background, for example).
 * + cairo.ANTIALIAS_SUBPIXEL - perform antialiasing by taking advantage of the order of subpixel elements on devices such as LCD panels.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {int} mode - one of the above modes.
 */
static JSVAL context_set_antialias(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_set_antialias(context, (cairo_antialias_t)args[1]->IntegerValue());
    return Undefined();
}

/**
 * @function cairo.context_get_antialias
 * 
 * ### Synopsis
 * 
 * var mode = cairo.context_get_antialias(context);
 * 
 * Gets the current shape antialiasing mode, as set by cairo.context_set_antialias().
 * 
 * The return value is a mode:
 * 
 * + cairo.ANTIALIAS_DEFAULT - use the default antialiasing for the subsystem and target device.
 * + cairo.ANTIALIAS_NONE - use a bilevel alpha mask.
 * + cairo.ANTIALIAS_GRAY - perform single-color antialiasing (using shades of gray for black text on a white background, for example).
 * + cairo.ANTIALIAS_SUBPIXEL - perform antialiasing by taking advantage of the order of subpixel elements on devices such as LCD panels.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {int} mode - see list above.
 */
static JSVAL context_get_antialias(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_get_antialias(context));
}

/**
 * @function cairo.content_set_dash
 * 
 * ### Synopsis
 * 
 * cairo.context_set_dash(context, dashes, offset);
 * 
 * Sets the dash pattern to be used by cairo.context_stroke(). 
 * 
 * A dash pattern is specified by dashes, an array of positive values. 
 * 
 * Each value provides the length of alternate "on" and "off" portions of the stroke. 
 * 
 * The offset specifies an offset into the pattern at which the stroke begins.
 * 
 * Each "on" segment will have caps applied as if the segment were a separate sub-path. In particular, it is valid to use an "on" length of 0.0 with cairo.LINE_CAP_ROUND or cairo.LINE_CAP_SQUARE in order to distributed dots or squares along a path.
 * 
 * Note: The length values are in user-space units as evaluated at the time of stroking. This is not necessarily the same as the user space at the time of cairo.context_set_dash()..
 * 
 * If the dashes array has length 0, dashing is disabled.
 * 
 * If the dashes array has a single element,  a symmetric pattern is assumed with alternating on and off portions of the size specified by the single value in dashes.
 * 
 * If any value in dashes is negative, or if all values are 0, then context will be put into an error state with a status of cairo.STATUS_INVALID_DASH.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {array} dashes - array of dash information, described above.
 * @param {number} offset - offset into pattern at which the stroke begins.
 */
static JSVAL context_set_dash(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    Handle<Array>dashes = Handle<Array>::Cast(args[1]->ToObject());
    double offset = args[2]->NumberValue();
    
    int numDashes = dashes->Length();
    if (numDashes == 0) {
        cairo_set_dash(context, NULL, 0, offset);
    }
    double *dashArray = new double[numDashes];
    for (int i=0; i<numDashes; i++) {
        dashArray[i] = dashes->Get(i)->NumberValue();
    }
    cairo_set_dash(context, dashArray, numDashes, offset);
    delete [] dashArray;
    
    return Undefined();
}

/**
 * @function cairo.context_get_dash_count
 * 
 * ### Synopsis
 * 
 * var count = cairo.context_get_dash_count(context);
 * 
 * Returns the length of the dash array in the context, or 0 if dashing is not in effect.
 * 
 * See also cairo.context_set_dash() and cairo_context.get_dash().
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {int} count - length of dashes array.
 */
static JSVAL context_get_dash_count(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_get_dash_count(context));
}

/**
 * @function cairo.context_get_dash
 * 
 * ### Synopsis
 * 
 * var dashes = cairo.context_get_dash(context);
 * 
 * Gets the current dash array.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {array} dashes = array of Number values of the dash array for the context.
 */
static JSVAL context_get_dash(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    
    int count = cairo_get_dash_count(context);
    double *dashes = new double[count];
    double offset;
    cairo_get_dash(context, dashes, &offset);
    Handle<Array>a = Array::New(count);
    for (int i=0; i<count; i++) {
        a->Set(i, Number::New(dashes[i]));
    }
    delete [] dashes;
    return a;
}

/**
 * @function cairo.context_set_fill_rule
 * 
 * ### Synopsis
 * 
 * cairo.context_set_fill_rule(context, rule);
 * 
 * Set the current fill rule within the cairo context. The fill rule is used to determine which regions are inside or outside a complex (potentially self-intersecting) path. The current fill rule affects both cairo.context_fill() and cairo.context_clip().
 * 
 * The rule argument may be:
 * 
 * + cairo.FILL_RULE_WINDING
 * + cairo.FILL_RULE_EVEN_ODD
 * 
 * For both fill rules, whether or not a point is included in the fill is determined by taking a ray from that point to infinity and looking at intersections with the path. The ray can be in any direction, as long as it doesn't pass through the end point of a segment or have a tricky intersection such as intersecting tangent to the path. (Note that filling is not actually implemented in this way. This is just a description of the rule that is applied.)
 * 
 * cairo.FILL_RULE_WINDING
 *   If the path crosses the ray from left-to-right, counts +1. If the path crosses the ray from right to left, counts -1. (Left and right are determined from the perspective of looking along the ray from the starting point.) If the total count is non-zero, the point will be filled.
 * cairo.FILL_RULE_EVEN_ODD
 *  Counts the total number of intersections, without regard to the orientation of the contour. If the total number of intersections is odd, the point will be filled.
 * 
 * The default fill rule is cairo.FILL_RULE_WINDING.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {int} rule - one of the above fill rules.
 */
static JSVAL context_set_fill_rule(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    int rule = args[1]->IntegerValue();
    cairo_set_fill_rule(context, (cairo_fill_rule_t)rule);
    return Undefined();
}

/**
 * @function cairo.context_get_fill_rule
 * 
 * ### Synopsis
 * 
 * var rule = cairo.context_get_fill_rule(context);
 * 
 * Gets the current fill rule, as set by cairo.context_set_fill_rule().
 * 
 * See cairo.context_set_fill_rule() for description of the fill rule values.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {int} rule - fill rule
 */
static JSVAL context_get_fill_rule(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_get_fill_rule(context));
}

/**
 * @function cairo.context_set_line_cap
 * 
 * ### Synopsis
 * 
 * cairo.context_set_line_cap(context, line_cap);
 * 
 * Sets the current line cap style within the cairo context. 
 * 
 * As with the other stroke parameters, the current line cap style is examined by cairo.context_stroke(), cairo.context_stroke_extents(), and cairo.context_stroke_to_path(), but does not have any effect during path construction.
 * 
 * The line_cap values may be:
 * 
 * cairo.LINE_CAP_BUTT
 *   start(stop) the line exactly at the start(end) point
 * cairo.LINE_CAP_ROUND
 *   use a round ending, the center of the circle is the end point
 * cairo.LINE_CAP_SQUARE
 *   use squared ending, the center of the square is the end point
 * 
 * The default line cap style is cairo.LINE_CAP_BUTT.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {int} line_cap - one of the above line_cap values.
 */
static JSVAL context_set_line_cap(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    int line_cap = args[1]->IntegerValue();
    cairo_set_line_cap(context, (cairo_line_cap_t)line_cap);
    return Undefined();
}

/**
 * @function cairo.context_get_line_cap
 * 
 * ### Synopsis
 * 
 * var line_cap = cairo.context_get_line_cap(context);
 * 
 * Gets the current line cap style, as set by cairo.context_set_line_cap().
 * 
 * See cairo.context_set_line_cap() for description of the line_cap values.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {int} line_cap - line cap style.
 */
static JSVAL context_get_line_cap(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_get_line_cap(context));
}

/**
 * @function cairo.context_set_line_join
 * 
 * ### Synopsis
 * 
 * cairo.context_set_line_join(context, line_join);
 * 
 * Sets the current line join style within the cairo context. 
 * 
 * As with the other stroke parameters, the current line join style is examined by cairo.context_stroke(), cairo.context_stroke_extents(), and cairo.context_stroke_to_path(), but does not have any effect during path construction.
 * 
 * Line join styles may be:
 * 
 * cairo.LINE_JOIN_MITER
 *   use a sharp (angled) corner, see cairo.context_set_miter_limit().
 * cairo.LINE_JOIN_ROUND
 *   use a rounded join, the center of the circle is the joint point.
 * cairo.LINE_JOIN_BEVEL
 *   use a cut-off join, the join is cut off at half the line width from the joint point
 * 
 * The default line join style is cairo.LINE_JOIN_MITER.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {int} line_join - one of the above line_join values.
 */
static JSVAL context_set_line_join(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    int line_join = args[1]->IntegerValue();
    cairo_set_line_join(context, (cairo_line_join_t)line_join);
    return Undefined();
}

/**
 * @function cairo.context_get_line_join
 * 
 * ### Synopsis
 * 
 * var line_join = cairo.context_get_line_join(context);
 * 
 * Gets the current line join style, as set by cairo.context_set_line_join().
 * 
 * See cairo.context_set_line_join() for description of the line_join values.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {int} line_join - line join style.
 */
static JSVAL context_get_line_join(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_get_line_join(context));
}

/**
 * @function cairo.context_set_line_width
 * 
 * ### Synopsis
 * 
 * cairo.context_set_line_width(context, width);
 * 
 * Sets the current line width within the cairo context. The line width value specifies the diameter of a pen that is circular in user space, (though device-space pen may be an ellipse in general due to scaling/shear/rotation of the CTM).
 * 
 * Note: When the description above refers to user space and CTM it refers to the user space and CTM in effect at the time of the stroking operation, not the user space and CTM in effect at the time of the call to cairo_set_line_width(). The simplest usage makes both of these spaces identical. That is, if there is no change to the CTM between a call to cairo_set_line_width() and the stroking operation, then one can just pass user-space values to cairo.context_set_line_width() and ignore this note.
 * 
 * As with the other stroke parameters, the current line width is examined by cairo.context_stroke(), cairo.context_stroke_extents(), and cairo.context_stroke_to_path(), but does not have any effect during path construction.
 * 
 * The default line width value is 2.0.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} width - setting for line width.
 */
static JSVAL context_set_line_width(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    double width = args[1]->NumberValue();
    cairo_set_line_width(context, width);
    return Undefined();
}

/**
 * @function cairo.context_get_line_width
 * 
 * ### Synopsis
 * 
 * var width = cairo.context_get_line_width(context);
 * 
 * This function returns the current line width value exactly as set by cairo.context_set_line_width(). 
 * 
 * Note that the value is unchanged even if the CTM has changed between the calls to cairo.context_set_line_width() and cairo.context_get_line_width().
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {number} width - current line width.
 */
static JSVAL context_get_line_width(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Number::New(cairo_get_line_width(context));
}

/**
 * @function cairo.context_set_miter_limit
 * 
 * ### Synopsis
 * 
 * cairo.context_set_miter_limit(context, limit);
 * 
 * Sets the current miter limit within the cairo context.
 * 
 * If the current line join style is set to CAIRO_LINE_JOIN_MITER (see cairo.context_set_line_join()), the miter limit is used to determine whether the lines should be joined with a bevel instead of a miter. Cairo divides the length of the miter by the line width. If the result is greater than the miter limit, the style is converted to a bevel.
 * 
 * As with the other stroke parameters, the current line miter limit is examined by cairo.context_stroke(), cairo.context_stroke_extents(), and cairo.context_stroke_to_path(), but does not have any effect during path construction.
 * 
 * The default miter limit value is 10.0, which will convert joins with interior angles less than 11 degrees to bevels instead of miters. For reference, a miter limit of 2.0 makes the miter cutoff at 60 degrees, and a miter limit of 1.414 makes the cutoff at 90 degrees.
 * 
 * A miter limit for a desired angle can be computed as: miter limit = 1/sin(angle/2)
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} limit - setting for miter limit.
 */
static JSVAL context_set_miter_limit(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    double limit = args[1]->NumberValue();
    cairo_set_miter_limit(context, limit);
    return Undefined();
}

/**
 * @function cairo.context_get_miter_limit
 * 
 * ### Synopsis
 * 
 * var limit = cairo.context_get_miter_limit(context);
 * 
 * This function returns the current miter limit value exactly as set by cairo.context_set_miter_limit(). 
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {number} width - current line width.
 */
static JSVAL context_get_miter_limit(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Number::New(cairo_get_miter_limit(context));
}

/**
 * @function cairo.context_set_operator
 * 
 * ### Synopsis
 * 
 * cairo.context_set_operator(context, operator);
 * 
 * Sets the compositing operator to be used for all drawing operations.
 * 
 * The default operator is CAIRO_OPERATOR_OVER.
 * 
 * The operator parameter may be one of the following values:
 * 
 * cairo.OPERATOR_CLEAR - clear destination layer (bounded)
 * cairo.OPERATOR_SOURCE - replace destination layer (bounded)
 * cairo.OPERATOR_OVER - draw source layer on top of destination layer (bounded)
 * cairo.OPERATOR_IN - draw source where there was destination content (unbounded)
 * cairo.OPERATOR_OUT - draw source where there was no destination content (unbounded)
 * cairo.OPERATOR_ATOP - draw source on top of destination content and only there
 * cairo.OPERATOR_DEST - ignore the source
 * cairo.OPERATOR_DEST_OVER - draw destination on top of source
 * cairo.OPERATOR_DEST_IN - leave destination only where there was source content (unbounded)
 * cairo.OPERATOR_DEST_OUT - leave destination only where there was no source content
 * cairo.OPERATOR_DEST_ATOP - leave destination on top of source content and only there (unbounded)
 * cairo.OPERATOR_XOR - source and destination are shown where there is only one of them
 * cairo.OPERATOR_ADD - source and destination layers are accumulated
 * cairo.OPERATOR_SATURATE - like over, but assuming source and dest are disjoint geometries
 * cairo.OPERATOR_MULTIPLY - source and destination layers are multiplied. This causes the result to be at least as dark as the darker inputs.
 * cairo.OPERATOR_SCREEN - source and destination are complemented and multiplied. This causes the result to be at least as light as the lighter inputs.
 * cairo.OPERATOR_OVERLAY - multiplies or screens, depending on the lightness of the destination color.
 * cairo.OPERATOR_DARKEN - replaces the destination with the source if it is darker, otherwise keeps the source.
 * cairo.OPERATOR_LIGHTEN - replaces the destination with the source if it is lighter, otherwise keeps the source.
 * cairo.OPERATOR_COLOR_DODGE - brightens the destination color to reflect the source color.
 * cairo.OPERATOR_COLOR_BURN - darkens the destination color to reflect the source color.
 * cairo.OPERATOR_HARD_LIGHT - Multiplies or screens, dependent on source color.
 * cairo.OPERATOR_SOFT_LIGHT - Darkens or lightens, dependent on source color.
 * cairo.OPERATOR_DIFFERENCE - Takes the difference of the source and destination color.
 * cairo.OPERATOR_EXCLUSION - Produces an effect similar to difference, but with lower contrast.
 * cairo.OPERATOR_HSL_HUE - Creates a color with the hue of the source and the saturation and luminosity of the target.
 * cairo.OPERATOR_HSL_SATURATION - Creates a color with the saturation of the source and the hue and luminosity of the target. Painting with this mode onto a gray area produces no change.
 * cairo.OPERATOR_HSL_COLOR - Creates a color with the hue and saturation of the source and the luminosity of the target. This preserves the gray levels of the target and is useful for coloring monochrome images or tinting color images.
 * cairo.OPERATOR_HSL_LUMINOSITY - Creates a color with the luminosity of the source and the hue and saturation of the target. This produces an inverse effect to cairo.OPERATOR_HSL_COLOR.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {int} operator - one of the above operators.
 */
static JSVAL context_set_operator(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_set_operator(context, (cairo_operator_t)args[1]->IntegerValue());
    return Undefined();
}

/**
 * @function cairo.context_get_operator
 * 
 * ### Synopsis
 * 
 * var operator = cairo.context_get_operator(context);
 * 
 * Gets the current compositing operator for a cairo context.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {int} operator - one of the operators, as described in cairo.context_set_operator().
 */
static JSVAL context_get_operator(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Integer::New(cairo_get_operator(context));
}

/**
 * @function cairo.context_set_tolerance
 * 
 * ### Synopsis
 * 
 * cairo.context_set_tolerance(context, tolerance);
 * 
 * Sets the tolerance used when converting paths into trapezoids. 
 * 
 * Curved segments of the path will be subdivided until the maximum deviation between the original path and the polygonal approximation is less than tolerance. 
 * 
 * The default value is 0.1. 
 * 
 * A larger value will give better performance, a smaller value, better appearance. (Reducing the value from the default value of 0.1 is unlikely to improve appearance significantly.) 
 * 
 * The accuracy of paths within Cairo is limited by the precision of its internal arithmetic, and the prescribed tolerance is restricted to the smallest representable internal value.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} tolerance - setting for tolerance, in device units (typically pixels).
 */
static JSVAL context_set_tolerance(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    double tolerance = args[1]->NumberValue();
    cairo_set_tolerance(context, tolerance);
    return Undefined();
}

/**
 * @function cairo.context_get_tolerance
 * 
 * ### Synopsis
 * 
 * var tolerance = cairo.context_get_tolerance(context);
 * 
 * This function returns the current tolerance value exactly as set by cairo.context_set_tolerance(). 
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {number} tolerance - current tolerance value.
 */
static JSVAL context_get_tolerance(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return Number::New(cairo_get_tolerance(context));
}

/**
 * @function cairo.context_clip
 * 
 * ### Synopsis
 * 
 * cairo.context_clip(context);
 * 
 * Establishes a new clip region by intersecting the current clip region with the current path as it would be filled by cairo.context_fill() and according to the current fill rule (see cairo.context_set_fill_rule()).
 * 
 * After cairo.context_clip(), the current path will be cleared from the cairo context.
 * 
 * The current clip region affects all drawing operations by effectively masking out any changes to the surface that are outside the current clip region.
 * 
 * Calling cairo.context_clip() can only make the clip region smaller, never larger. But the current clip is part of the graphics state, so a temporary restriction of the clip region can be achieved by calling cairo.context_clip() within a cairo.context_save()/cairo.context_restore() pair. The only other means of increasing the size of the clip region is cairo.context_reset_clip().
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_clip(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_clip(context);
    return Undefined();
}

/**
 * @function cairo.context_clip_preserve
 * 
 * ### Synopsis
 * 
 * cairo.context_clip_preserve(context);
 * 
 * Establishes a new clip region by intersecting the current clip region with the current path as it would be filled by cairo.context_fill() and according to the current fill rule (see cairo.context_set_fill_rule()).
 * 
 * Unlike cairo.context_clip(), cairo.context_clip_preserve() preserves the path within the cairo context.
 * 
 * The current clip region affects all drawing operations by effectively masking out any changes to the surface that are outside the current clip region.
 * 
 * Calling cairo.context_clip_preserve() can only make the clip region smaller, never larger. But the current clip is part of the graphics state, so a temporary restriction of the clip region can be achieved by calling cairo.context_clip_preserve() within a cairo.context_save()/cairo.context_restore() pair. The only other means of increasing the size of the clip region is cairo.context_reset_clip().
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_clip_preserve(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_clip_preserve(context);
    return Undefined();
}

/**
 * @function cairo.context_clip_extents
 * 
 * ### Synopsis
 * 
 * var extents = cairo.context_clip_extents(context);
 * 
 * Computes a bounding box in user coordinates covering the area inside the current clip.
 * 
 * The object returned has the following members:
 * 
 * + x1: x coordinate of the top left corner of the resulting extents.
 * + y1: y coordinate of the top left corner of the resulting extents.
 * + x2: x coordinate of the lower right corner of the resulting extents.
 * + y2: y coordinate of the lower right corner of the resulting extents.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} extents - see object description above.
 */
static JSVAL context_clip_extents(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    double x1,y1, x2,y2;
    cairo_clip_extents(context, &x1,&y1, &x2,&y2);
    
    JSOBJ o = Object::New();
    o->Set(String::New("x1"), Number::New(x1));
    o->Set(String::New("y1"), Number::New(y1));
    o->Set(String::New("x2"), Number::New(x2));
    o->Set(String::New("y2"), Number::New(y2));
    return o;
}

/**
 * @function cairo.context_in_clip
 * 
 * ### Synopsis
 * 
 * var inClip = cairo.context_in_clip(context, x, y);
 * 
 * Tests whether the given point is inside the area that would be visible through the current clip, i.e. the area that would be filled by a cairo.context_paint() operation.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {boolean} inClip - true if the point is inside the clip rectangle, false if not.
 */
static JSVAL context_in_clip(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return cairo_in_clip(context, args[1]->NumberValue(), args[2]->NumberValue()) ? True() : False();
}

/**
 * @function cairo.context_reset_clip
 * 
 * ### Synopsis
 * 
 * cairo.context_reset_clip(context);
 * 
 * Reset the current clip region to its original, unrestricted state. 
 * 
 * That is, set the clip region to an infinitely large shape containing the target surface. Equivalently, if infinity is too hard to grasp, one can imagine the clip region being reset to the exact bounds of the target surface.
 * 
 * Note that code meant to be reusable should not call cairo.context_reset_clip() as it will cause results unexpected by higher-level code which calls cairo.context_clip(). Consider using cairo.context_save() and cairo.context_restore() around cairo.context_clip() as a more robust means of temporarily restricting the clip region.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_reset_clip(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_reset_clip(context);
    return Undefined();
}

// rectangle list stuff not implemented (yet)

/**
 * @function cairo.context_fill
 * 
 * ### Synopsis
 * 
 * cairo.context_fill(context);
 * 
 * A drawing operator that fills the current path according to the current fill rule, (each sub-path is implicitly closed before being filled). 
 * 
 * After cairo.context_fill(), the current path will be cleared from the cairo context. 
 * 
 * See cairo.context_set_fill_rule() and cairo.context_fill_preserve().
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_fill(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_fill(context);
    return Undefined();
}

/**
 * @function cairo.context_fill_preserve
 * 
 * ### Synopsis
 * 
 * cairo.context_fill_preserve(context);
 * 
 * A drawing operator that fills the current path according to the current fill rule, (each sub-path is implicitly closed before being filled). 
 * 
 * Unlike cairo.context_fill(), cairo.context_fill_preserve() preserves the path within the cairo context.
 * 
 * See cairo.context_set_fill_rule() and cairo.context_fill().
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_fill_preserve(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_fill_preserve(context);
    return Undefined();
}

/**
 * @function cairo.context_fill_extents
 * 
 * ### Synopsis
 * 
 * var extents = cairo.context_fill_extents(context);
 * 
 * Computes a bounding box in user coordinates covering the area that would be affected, (the "inked" area), by a cairo.context_fill() operation given the current path and fill parameters. If the current path is empty, returns an empty rectangle ((0,0), (0,0)). Surface dimensions and clipping are not taken into account.
 * 
 * Contrast with cairo.context_path_extents(), which is similar, but returns non-zero extents for some paths with no inked area, (such as a simple line segment).
 * 
 * Note that cairo.context_fill_extents() must necessarily do more work to compute the precise inked areas in light of the fill rule, so cairo.context_path_extents() may be more desirable for sake of performance if the non-inked path extents are desired.
 * 
 * See cairo.context_fill(), cairo.context_set_fill_rule() and cairo.context_fill_preserve().
 * 
 * The object returned has the following members:
 * 
 * + x1: x coordinate of the top left corner of the resulting extents.
 * + y1: y coordinate of the top left corner of the resulting extents.
 * + x2: x coordinate of the lower right corner of the resulting extents.
 * + y2: y coordinate of the lower right corner of the resulting extents.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} extents - see object description above.
 */
static JSVAL context_fill_extents(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    double x1,y1, x2,y2;
    cairo_fill_extents(context, &x1,&y1, &x2,&y2);
    
    JSOBJ o = Object::New();
    o->Set(String::New("x1"), Number::New(x1));
    o->Set(String::New("y1"), Number::New(y1));
    o->Set(String::New("x2"), Number::New(x2));
    o->Set(String::New("y2"), Number::New(y2));
    return o;
}

/**
 * @function cairo.context_in_fill
 * 
 * ### Synopsis
 * 
 * var inFill = cairo.context_in_fill(context, x, y);
 * 
 * Tests whether the given point is inside the area that would be affected by a cairo.context_fill() operation given the current path and filling parameters. Surface dimensions and clipping are not taken into account.
 * 
 * See cairo.context_fill(), cairo.context_set_fill_rule() and cairo.context_fill_preserve().
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {boolean} inFill - true if the point is inside the area that would be affected, false if not.
 */
static JSVAL context_in_fill(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return cairo_in_fill(context, args[1]->NumberValue(), args[2]->NumberValue()) ? True() : False();
}

/**
 * @function cairo.context_mask
 * 
 * ### Synopsis
 * 
 * cairo.context_mask(context, pattern);
 * 
 * A drawing operator that paints the current source using the alpha channel of pattern as a mask. 
 * 
 * Opaque areas of pattern are painted with the source, transparent areas are not painted.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} pattern - opaque handle to a cairo pattern.
 */
static JSVAL context_mask(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_pattern_t *pattern = (cairo_pattern_t *) JSEXTERN(args[1]);
    cairo_mask(context, pattern);
    return Undefined();
}

/**
 * @function cairo.context_mask_surface
 * 
 * ### Synopsis
 * 
 * cairo.context_mask_surface(context, surface, surface_x, surface_y);
 * 
 * A drawing operator that paints the current source using the alpha channel of surface as a mask. 
 * 
 * Opaque areas of surface are painted with the source, transparent areas are not painted.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {object} surface - opaque handle to a cairo surface.
 * @param {number} surface_x - x coordinate at which to place the origin of surface.
 * @param {number} surface_y - y coordinate at which to place the origin of surface.
 */
static JSVAL context_mask_surface(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[1]);
    cairo_mask_surface(context, surface, args[2]->NumberValue(), args[3]->NumberValue());
    return Undefined();
}

/**
 * @function cairo.context_paint
 * 
 * ### Synopsis
 * 
 * cairo.context_paint(context);
 * 
 * A drawing operator that paints the current source everywhere within the current clip region.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_paint(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_paint(context);
    return Undefined();
}

/**
 * @function cairo.context_paint_with_alpha
 * 
 * ### Synopsis
 * 
 * cairo.context_paint_with_alpha(context, alpha);
 * 
 * A drawing operator that paints the current source everywhere within the current clip region using a mask of constant alpha value alpha. 
 * 
 * The effect is similar to cairo.context_paint(), but the drawing is faded out using the alpha value.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @param {number} alpha - alpha value, between 0 (transparent) and 1 (opaque).
 */
static JSVAL context_paint_with_alpha(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_paint_with_alpha(context, args[1]->NumberValue());
    return Undefined();
}

/**
 * @function cairo.context_stroke
 * 
 * ### Synopsis
 * 
 * cairo.context_stroke(context);
 * 
 * A drawing operator that strokes the current path according to the current line width, line join, line cap, and dash settings. After cairo.context_stroke(), the current path will be cleared from the cairo context. 
 * 
 * See cairo.context_set_line_width(), cairo.context_set_line_join(), cairo.context_set_line_cap(), cairo.context_set_dash(), and cairo.context_stroke_preserve().
 * 
 * Note: Degenerate segments and sub-paths are treated specially and provide a useful result. These can result in two different situations:
 * 
 * 1. Zero-length "on" segments set in cairo.context_set_dash(). If the cap style is cairo.LINE_CAP_ROUND or cairo.LINE_CAP_SQUARE then these segments will be drawn as circular dots or squares respectively. In the case of cairo.LINE_CAP_SQUARE, the orientation of the squares is determined by the direction of the underlying path.
 * 2. A sub-path created by cairo.context_move_to() followed by either a cairo.context_close_path() or one or more calls to cairo.context_line_to() to the same coordinate as the cairo_move_to(). If the cap style is cairo._LINE_CAP_ROUND then these sub-paths will be drawn as circular dots. Note that in the case of cairo.LINE_CAP_SQUARE a degenerate sub-path will not be drawn at all, (since the correct orientation is indeterminate).
 * 
 * In no case will a cap style of cairo.LINE_CAP_BUTT cause anything to be drawn in the case of either degenerate segments or sub-paths.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_stroke(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_stroke(context);
    return Undefined();
}

/**
 * @function cairo.context_stroke_preserve
 * 
 * ### Synopsis
 * 
 * cairo.stroke_preserve(context);
 * 
 * A drawing operator that strokes the current path according to the current line width, line join, line cap, and dash settings. 
 * 
 * Unlike cairo.context_stroke(), cairo.context_stroke_preserve() preserves the path within the cairo context.
 * 
 * See cairo.context_set_line_width(), cairo.context_set_line_join(), cairo.context_set_line_cap(), and cairo.context_set_dash().
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_stroke_preserve(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_stroke_preserve(context);
    return Undefined();
}

/**
 * @function cairo.context_stroke_extents
 * 
 * ### Synopsis
 * 
 * var extents = cairo.context_stroke_extents(context);
 * 
 * Computes a bounding box in user coordinates covering the area that would be affected, (the "inked" area), by a cairo.context_stroke() operation given the current path and stroke parameters. If the current path is empty, returns an empty rectangle ((0,0), (0,0)). Surface dimensions and clipping are not taken into account.
 * 
 * Note that if the line width is set to exactly zero, then cairo.context_stroke_extents() will return an empty rectangle. Contrast with cairo.context_path_extents() which can be used to compute the non-empty bounds as the line width approaches zero.
 * 
 * Note that cairo.context_stroke_extents() must necessarily do more work to compute the precise inked areas in light of the stroke parameters, so cairo.context_path_extents() may be more desirable for sake of performance if non-inked path extents are desired.
 * 
 * See cairo.context_stroke(), cairo.context_set_line_width(), cairo.context_set_line_join(), cairo.context_set_line_cap(), cairo.context_set_dash(), and cairo.context_stroke_preserve().
 * 
 * The object returned has the following members:
 * 
 * + x1: x coordinate of the top left corner of the resulting extents.
 * + y1: y coordinate of the top left corner of the resulting extents.
 * + x2: x coordinate of the lower right corner of the resulting extents.
 * + y2: y coordinate of the lower right corner of the resulting extents.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} extents - see object description above.
 */
static JSVAL context_stroke_extents(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    double x1,y1, x2,y2;
    cairo_stroke_extents(context, &x1,&y1, &x2,&y2);
    
    JSOBJ o = Object::New();
    o->Set(String::New("x1"), Number::New(x1));
    o->Set(String::New("y1"), Number::New(y1));
    o->Set(String::New("x2"), Number::New(x2));
    o->Set(String::New("y2"), Number::New(y2));
    return o;
}

/**
 * @function cairo.context_in_stroke
 * 
 * ### Synopsis
 * 
 * var inStroke = cairo.context_in_stroke(context, x, y);
 * 
 * Tests whether the given point is inside the area that would be affected by a cairo.context_stroke() operation given the current path and stroking parameters. 
 * 
 * Surface dimensions and clipping are not taken into account.
 * 
 * See cairo.context_stroke(), cairo.context_set_line_width(), cairo.context_set_line_join(), cairo.context_set_line_cap(), cairo.context_set_dash(), and cairo.context_stroke_preserve().
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {boolean} inStroke - true if the point is inside the area that would be affected, false if not.
 */
static JSVAL context_in_stroke(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return cairo_in_stroke(context, args[1]->NumberValue(), args[2]->NumberValue()) ? True() : False();
}

/**
 * @function cairo.context_copy_page
 * 
 * ### Synopsis
 * 
 * cairo.context_copy_page(context);
 * 
 * Emits the current page for backends that support multiple pages, but doesn't clear it, so, the contents of the current page will be retained for the next page too. Use cairo.conext_show_page() if you want to get an empty page after the emission.
 * 
 * This is a convenience function that simply calls cairo.surface_copy_page() on context's target.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_copy_page(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_copy_page(context);
    return Undefined();
}

/**
 * @function cairo.context_show_page
 * 
 * ### Synopsis
 * 
 * cairo.context_show_page(context);
 * 
 * Emits and clears the current page for backends that support multiple pages. Use cairo.context_copy_page() if you don't want to clear the page.
 * 
 * This is a convenience function that simply calls cairo.surface_show_page() on context's target.
 * 
 * @param {object} context - opaque handle to a cairo context.
 */
static JSVAL context_show_page(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_show_page(context);
    return Undefined();
}


////////////////////////// PATHS
// http://www.cairographics.org/manual/cairo-Paths.html

// functions to return cario_path_t and manipulate them not done.

/**
 * @function cairo.context_has_current_point
 * 
 * ### Synopsis
 * 
 * var hasPoint = cairo.context_has_current_point(context);
 * 
 * Returns whether a current point is defined on the current path. See cairo.context_get_current_point() for details on the current point.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {boolean} hasPoint - true if a current point is defined.
 */
static JSVAL context_has_current_point(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    return cairo_has_current_point(context) ? True() : False();
}

/**
 * @function cairo.context_get_current_point
 * 
 * ### Synopsis
 * 
 * var point = cairo.context_get_current_point(context);
 * 
 * Gets the current point of the current path, which is conceptually the final point reached by the path so far.
 * 
 * The current point is returned in the user-space coordinate system. If there is no defined current point or if context is in an error status, x and y will both be set to 0.0. It is possible to check this in advance with cairo.context_has_current_point().
 * 
 * Most path construction functions alter the current point. 
 * 
 * See the following for details on how they affect the current point: cairo.context_new_path(), cairo.context_new_sub_path(), cairo.context_append_path(), cairo.context_close_path(), cairo.context_move_to(), cairo.context_line_to(), cairo.context_curve_to(), cairo.context_rel_move_to(), cairo.context_rel_line_to(), cairo.context_rel_curve_to(), cairo.context_arc(), cairo.context_arc_negative(), cairo.context_rectangle(), cairo.context_text_path(), cairo.context_glyph_path(), cairo_stroke_to_path().
 * 
 * Some functions use and alter the current point but do not otherwise change current path: cairo.context_show_text().
 * 
 * Some functions unset the current path and as a result, current point: cairo.context_fill(), cairo.context_stroke().
 * 
 * The object returned has just two members:
 * 
 * + {number} x: x coordinate of the current point
 * + {number} y: y coordinate of the current point
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return {object} point - object of the form described above.
 */
static JSVAL context_get_current_point(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    double x,y;
    
    cairo_get_current_point(context, &x, &y);
    JSOBJ o = Object::New();
    o->Set(String::New("x"), Number::New(x));
    o->Set(String::New("y"), Number::New(y));
    return o;
}

/**
 * @function cairo.context_new_path
 * 
 * ### Synopsis
 * 
 * cairo.context_new_path(context);
 * 
 * Clears the current path. After this call there will be no path and no current point.
 * 
 * @param {object} context - opaque handle to a cairo context.
 * @return 
 */
static JSVAL context_new_path(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_new_path(context);
    return Undefined();
}

////////////////////////// MISC

/**
 * @function cairo.status_to_string
 * 
 * ### Synopsis
 * 
 * var message = cairo.status_to_string(status);
 * 
 * Provides a human-readable description of a cairo.STATUS_* code.
 * 
 * @param {int} status - one of the cairo.STATUS_* codes, as returned by a function like cairo.context_status().
 * @return {string} message - description of the error/status code.
 */
static JSVAL status_to_string(JSARGS args) {
    return String::New(cairo_status_to_string((cairo_status_t)args[0]->IntegerValue()));
}

void init_cairo_object () {
    Handle<ObjectTemplate>cairo = ObjectTemplate::New();

    cairo->Set(String::New("FORMAT_INVALID"), Integer::New(CAIRO_FORMAT_INVALID));
    cairo->Set(String::New("FORMAT_ARGB32"), Integer::New(CAIRO_FORMAT_ARGB32));
    cairo->Set(String::New("FORMAT_RGB24"), Integer::New(CAIRO_FORMAT_RGB24));
    cairo->Set(String::New("FORMAT_A8"), Integer::New(CAIRO_FORMAT_A8));
    cairo->Set(String::New("FORMAT_A1"), Integer::New(CAIRO_FORMAT_A1));
    cairo->Set(String::New("FORMAT_RGB16_565"), Integer::New(CAIRO_FORMAT_RGB16_565));
    
    cairo->Set(String::New("STATUS_SUCCESS"), Integer::New(CAIRO_STATUS_SUCCESS));
    cairo->Set(String::New("STATUS_NO_MEMORY"), Integer::New(CAIRO_STATUS_NO_MEMORY));
    cairo->Set(String::New("STATUS_INVALID_RESTORE"), Integer::New(CAIRO_STATUS_INVALID_RESTORE));
    cairo->Set(String::New("STATUS_INVALID_POP_GROUP"), Integer::New(CAIRO_STATUS_INVALID_POP_GROUP));
    cairo->Set(String::New("STATUS_NO_CURRENT_POINT"), Integer::New(CAIRO_STATUS_NO_CURRENT_POINT));
    cairo->Set(String::New("STATUS_INVALID_MATRIX"), Integer::New(CAIRO_STATUS_INVALID_MATRIX));
    cairo->Set(String::New("STATUS_INVALID_STATUS"), Integer::New(CAIRO_STATUS_INVALID_STATUS));
    cairo->Set(String::New("STATUS_NULL_POINTER"), Integer::New(CAIRO_STATUS_NULL_POINTER));
    cairo->Set(String::New("STATUS_INVALID_STRING"), Integer::New(CAIRO_STATUS_INVALID_STRING));
    cairo->Set(String::New("STATUS_INVALID_PATH_DATA"), Integer::New(CAIRO_STATUS_INVALID_PATH_DATA));
    cairo->Set(String::New("STATUS_READ_ERROR"), Integer::New(CAIRO_STATUS_READ_ERROR));
    cairo->Set(String::New("STATUS_WRITE_ERROR"), Integer::New(CAIRO_STATUS_WRITE_ERROR));
    cairo->Set(String::New("STATUS_SURFACE_FINISHED"), Integer::New(CAIRO_STATUS_SURFACE_FINISHED));
    cairo->Set(String::New("STATUS_SURFACE_TYPE_MISMATCH"), Integer::New(CAIRO_STATUS_SURFACE_TYPE_MISMATCH));
    cairo->Set(String::New("STATUS_PATTERN_TYPE_MISMATCH"), Integer::New(CAIRO_STATUS_PATTERN_TYPE_MISMATCH));
    cairo->Set(String::New("STATUS_INVALID_CONTENT"), Integer::New(CAIRO_STATUS_INVALID_CONTENT));
    cairo->Set(String::New("STATUS_INVALID_FORMAT"), Integer::New(CAIRO_STATUS_INVALID_FORMAT));
    cairo->Set(String::New("STATUS_INVALID_VISUAL"), Integer::New(CAIRO_STATUS_INVALID_VISUAL));
    cairo->Set(String::New("STATUS_FILE_NOT_FOUND"), Integer::New(CAIRO_STATUS_FILE_NOT_FOUND));
    cairo->Set(String::New("STATUS_INVALID_DASH"), Integer::New(CAIRO_STATUS_INVALID_DASH));
    cairo->Set(String::New("STATUS_INVALID_DSC_COMMENT"), Integer::New(CAIRO_STATUS_INVALID_DSC_COMMENT));
    cairo->Set(String::New("STATUS_INVALID_INDEX"), Integer::New(CAIRO_STATUS_INVALID_INDEX));
    cairo->Set(String::New("STATUS_CLIP_NOT_REPRESENTABLE"), Integer::New(CAIRO_STATUS_CLIP_NOT_REPRESENTABLE));
    cairo->Set(String::New("STATUS_TEMP_FILE_ERROR"), Integer::New(CAIRO_STATUS_TEMP_FILE_ERROR));
    cairo->Set(String::New("STATUS_INVALID_STRIDE"), Integer::New(CAIRO_STATUS_INVALID_STRIDE));
    cairo->Set(String::New("STATUS_FONT_TYPE_MISMATCH"), Integer::New(CAIRO_STATUS_FONT_TYPE_MISMATCH));
    cairo->Set(String::New("STATUS_USER_FONT_IMMUTABLE"), Integer::New(CAIRO_STATUS_USER_FONT_IMMUTABLE));
    cairo->Set(String::New("STATUS_USER_FONT_ERROR"), Integer::New(CAIRO_STATUS_USER_FONT_ERROR));
    cairo->Set(String::New("STATUS_NEGATIVE_COUNT"), Integer::New(CAIRO_STATUS_NEGATIVE_COUNT));
    cairo->Set(String::New("STATUS_INVALID_CLUSTERS"), Integer::New(CAIRO_STATUS_INVALID_CLUSTERS));
    cairo->Set(String::New("STATUS_INVALID_SLANT"), Integer::New(CAIRO_STATUS_INVALID_SLANT));
    cairo->Set(String::New("STATUS_INVALID_WEIGHT"), Integer::New(CAIRO_STATUS_INVALID_WEIGHT));
    cairo->Set(String::New("STATUS_INVALID_SIZE"), Integer::New(CAIRO_STATUS_INVALID_SIZE));
    cairo->Set(String::New("STATUS_USER_FONT_NOT_IMPLEMENTED"), Integer::New(CAIRO_STATUS_USER_FONT_NOT_IMPLEMENTED));
    cairo->Set(String::New("STATUS_DEVICE_TYPE_MISMATCH"), Integer::New(CAIRO_STATUS_DEVICE_TYPE_MISMATCH));
    cairo->Set(String::New("STATUS_DEVICE_ERROR"), Integer::New(CAIRO_STATUS_DEVICE_ERROR));

    cairo->Set(String::New("CONTENT_COLOR"), Integer::New(CAIRO_CONTENT_COLOR));
    cairo->Set(String::New("CONTENT_ALPHA"), Integer::New(CAIRO_CONTENT_ALPHA));
    cairo->Set(String::New("CONTENT_COLOR_ALPHA"), Integer::New(CAIRO_CONTENT_COLOR_ALPHA));
    
    cairo->Set(String::New("SURFACE_TYPE_IMAGE"), Integer::New(CAIRO_SURFACE_TYPE_IMAGE));
    cairo->Set(String::New("SURFACE_TYPE_PDF"), Integer::New(CAIRO_SURFACE_TYPE_PDF));
    cairo->Set(String::New("SURFACE_TYPE_PS"), Integer::New(CAIRO_SURFACE_TYPE_PS));
    cairo->Set(String::New("SURFACE_TYPE_XLIB"), Integer::New(CAIRO_SURFACE_TYPE_XLIB));
    cairo->Set(String::New("SURFACE_TYPE_XCB"), Integer::New(CAIRO_SURFACE_TYPE_XCB));
    cairo->Set(String::New("SURFACE_TYPE_GLITZ"), Integer::New(CAIRO_SURFACE_TYPE_GLITZ));
    cairo->Set(String::New("SURFACE_TYPE_QUARTZ"), Integer::New(CAIRO_SURFACE_TYPE_QUARTZ));
    cairo->Set(String::New("SURFACE_TYPE_WIN32"), Integer::New(CAIRO_SURFACE_TYPE_WIN32));
    cairo->Set(String::New("SURFACE_TYPE_BEOS"), Integer::New(CAIRO_SURFACE_TYPE_BEOS));
    cairo->Set(String::New("SURFACE_TYPE_DIRECTFB"), Integer::New(CAIRO_SURFACE_TYPE_DIRECTFB));
    cairo->Set(String::New("SURFACE_TYPE_SVG"), Integer::New(CAIRO_SURFACE_TYPE_SVG));
    cairo->Set(String::New("SURFACE_TYPE_OS2"), Integer::New(CAIRO_SURFACE_TYPE_OS2));
    cairo->Set(String::New("SURFACE_TYPE_WIN32_PRINTING"), Integer::New(CAIRO_SURFACE_TYPE_WIN32_PRINTING));
    cairo->Set(String::New("SURFACE_TYPE_QUARTZ_IMAGE"), Integer::New(CAIRO_SURFACE_TYPE_QUARTZ_IMAGE));
    cairo->Set(String::New("SURFACE_TYPE_SCRIPT"), Integer::New(CAIRO_SURFACE_TYPE_SCRIPT));
    cairo->Set(String::New("SURFACE_TYPE_QT"), Integer::New(CAIRO_SURFACE_TYPE_QT));
    cairo->Set(String::New("SURFACE_TYPE_RECORDING"), Integer::New(CAIRO_SURFACE_TYPE_RECORDING));
    cairo->Set(String::New("SURFACE_TYPE_VG"), Integer::New(CAIRO_SURFACE_TYPE_VG));
    cairo->Set(String::New("SURFACE_TYPE_GL"), Integer::New(CAIRO_SURFACE_TYPE_GL));
    cairo->Set(String::New("SURFACE_TYPE_DRM"), Integer::New(CAIRO_SURFACE_TYPE_DRM));
    cairo->Set(String::New("SURFACE_TYPE_TEE"), Integer::New(CAIRO_SURFACE_TYPE_TEE));
    cairo->Set(String::New("SURFACE_TYPE_XML"), Integer::New(CAIRO_SURFACE_TYPE_XML));
    cairo->Set(String::New("SURFACE_TYPE_SKIA"), Integer::New(CAIRO_SURFACE_TYPE_SKIA));
    cairo->Set(String::New("SURFACE_TYPE_SUBSURFACE"), Integer::New(CAIRO_SURFACE_TYPE_SUBSURFACE));

    cairo->Set(String::New("MIME_TYPE_JPEG"), String::New(CAIRO_MIME_TYPE_JPEG));
    cairo->Set(String::New("MIME_TYPE_PNG"), String::New(CAIRO_MIME_TYPE_PNG));
    cairo->Set(String::New("MIME_TYPE_JP2"), String::New(CAIRO_MIME_TYPE_JP2));
    cairo->Set(String::New("MIME_TYPE_URI"), String::New(CAIRO_MIME_TYPE_URI));

    cairo->Set(String::New("ANTIALIAS_DEFAULT"), Integer::New(CAIRO_ANTIALIAS_DEFAULT));
    cairo->Set(String::New("ANTIALIAS_NONE"), Integer::New(CAIRO_ANTIALIAS_NONE));
    cairo->Set(String::New("ANTIALIAS_GRAY"), Integer::New(CAIRO_ANTIALIAS_GRAY));
    cairo->Set(String::New("ANTIALIAS_SUBPIXEL"), Integer::New(CAIRO_ANTIALIAS_SUBPIXEL));

    cairo->Set(String::New("LINE_CAP_BUTT"), Integer::New(CAIRO_LINE_CAP_BUTT));
    cairo->Set(String::New("LINE_CAP_ROUND"), Integer::New(CAIRO_LINE_CAP_ROUND));
    cairo->Set(String::New("LINE_CAP_SQUARE"), Integer::New(CAIRO_LINE_CAP_SQUARE));
    
    cairo->Set(String::New("FILL_RULE_WINDING"), Integer::New(CAIRO_FILL_RULE_WINDING));
    cairo->Set(String::New("FILL_RULE_EVEN_ODD"), Integer::New(CAIRO_FILL_RULE_EVEN_ODD));
    
    cairo->Set(String::New("LINE_JOIN_MITER"), Integer::New(CAIRO_LINE_JOIN_MITER));
    cairo->Set(String::New("LINE_JOIN_ROUND"), Integer::New(CAIRO_LINE_JOIN_ROUND));
    cairo->Set(String::New("LINE_JOIN_BEVEL"), Integer::New(CAIRO_LINE_JOIN_BEVEL));
    
    cairo->Set(String::New("OPERATOR_CLEAR"), Integer::New(CAIRO_OPERATOR_CLEAR));
    cairo->Set(String::New("OPERATOR_SOURCE"), Integer::New(CAIRO_OPERATOR_SOURCE));
    cairo->Set(String::New("OPERATOR_OVER"), Integer::New(CAIRO_OPERATOR_OVER));
    cairo->Set(String::New("OPERATOR_IN"), Integer::New(CAIRO_OPERATOR_IN));
    cairo->Set(String::New("OPERATOR_OUT"), Integer::New(CAIRO_OPERATOR_OUT));
    cairo->Set(String::New("OPERATOR_ATOP"), Integer::New(CAIRO_OPERATOR_ATOP));
    cairo->Set(String::New("OPERATOR_DEST"), Integer::New(CAIRO_OPERATOR_DEST));
    cairo->Set(String::New("OPERATOR_DEST_OVER"), Integer::New(CAIRO_OPERATOR_DEST_OVER));
    cairo->Set(String::New("OPERATOR_DEST_IN"), Integer::New(CAIRO_OPERATOR_DEST_IN));
    cairo->Set(String::New("OPERATOR_DEST_OUT"), Integer::New(CAIRO_OPERATOR_DEST_OUT));
    cairo->Set(String::New("OPERATOR_DEST_ATOP"), Integer::New(CAIRO_OPERATOR_DEST_ATOP));
    cairo->Set(String::New("OPERATOR_XOR"), Integer::New(CAIRO_OPERATOR_XOR));
    cairo->Set(String::New("OPERATOR_ADD"), Integer::New(CAIRO_OPERATOR_ADD));
    cairo->Set(String::New("OPERATOR_SATURATE"), Integer::New(CAIRO_OPERATOR_SATURATE));
    cairo->Set(String::New("OPERATOR_MULTIPLY"), Integer::New(CAIRO_OPERATOR_MULTIPLY));
    cairo->Set(String::New("OPERATOR_SCREEN"), Integer::New(CAIRO_OPERATOR_SCREEN));
    cairo->Set(String::New("OPERATOR_OVERLAY"), Integer::New(CAIRO_OPERATOR_OVERLAY));
    cairo->Set(String::New("OPERATOR_DARKEN"), Integer::New(CAIRO_OPERATOR_DARKEN));
    cairo->Set(String::New("OPERATOR_LIGHTEN"), Integer::New(CAIRO_OPERATOR_LIGHTEN));
    cairo->Set(String::New("OPERATOR_COLOR_DODGE"), Integer::New(CAIRO_OPERATOR_COLOR_DODGE));
    cairo->Set(String::New("OPERATOR_COLOR_BURN"), Integer::New(CAIRO_OPERATOR_COLOR_BURN));
    cairo->Set(String::New("OPERATOR_HARD_LIGHT"), Integer::New(CAIRO_OPERATOR_HARD_LIGHT));
    cairo->Set(String::New("OPERATOR_SOFT_LIGHT"), Integer::New(CAIRO_OPERATOR_SOFT_LIGHT));
    cairo->Set(String::New("OPERATOR_DIFFERENCE"), Integer::New(CAIRO_OPERATOR_DIFFERENCE));
    cairo->Set(String::New("OPERATOR_EXCLUSION"), Integer::New(CAIRO_OPERATOR_EXCLUSION));
    cairo->Set(String::New("OPERATOR_HSL_HUE"), Integer::New(CAIRO_OPERATOR_HSL_HUE));
    cairo->Set(String::New("OPERATOR_HSL_SATURATION"), Integer::New(CAIRO_OPERATOR_HSL_SATURATION));
    cairo->Set(String::New("OPERATOR_HSL_COLOR"), Integer::New(CAIRO_OPERATOR_HSL_COLOR));
    cairo->Set(String::New("OPERATOR_HSL_LUMINOSITY"), Integer::New(CAIRO_OPERATOR_HSL_LUMINOSITY));

    
    
//    net->Set(String::New("sendFile"), FunctionTemplate::New(net_sendfile));

    builtinObject->Set(String::New("cairo"), cairo);
}
