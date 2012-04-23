/** @ignore */

#include "SilkJS.h"
#include <cairo/cairo.h>

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
    return External::New(cairo_image_surface_create(format, width, height));
}

/**
 * @function cairo.surface_create_similar
 * 
 * ### Synopsis
 * 
 * var newSurface = cairo.surface_create_similar(handle, content, width, height);
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
    int format = args[0]->IntegerValue();
    int width = args[1]->IntegerValue();
    int height = args[1]->IntegerValue();
    return External::New(cairo_surface_create_similar(format, width, height));
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
    cairo_device_t *device = cairo_surface_get_font_options(surface, options);
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
 * cairo.SURFACE_TYPE_IMAGE - The surface is of type image
 * cairo.SURFACE_TYPE_PDF - The surface is of type pdf
 * cairo.SURFACE_TYPE_PS - The surface is of type ps
 * cairo.SURFACE_TYPE_XLIB - The surface is of type xlib
 * cairo.SURFACE_TYPE_XCB - The surface is of type xcb
 * cairo.SURFACE_TYPE_GLITZ - The surface is of type glitz
 * cairo.SURFACE_TYPE_QUARTZ - The surface is of type quartz
 * cairo.SURFACE_TYPE_WIN32 - The surface is of type win32
 * cairo.SURFACE_TYPE_BEOS - The surface is of type beos
 * cairo.SURFACE_TYPE_DIRECTFB - The surface is of type directfb
 * cairo.SURFACE_TYPE_SVG - The surface is of type svg
 * cairo.SURFACE_TYPE_OS2 - The surface is of type os2
 * cairo.SURFACE_TYPE_WIN32_PRINTING - The surface is a win32 printing surface
 * cairo.SURFACE_TYPE_QUARTZ_IMAGE - The surface is of type quartz_image
 * cairo.SURFACE_TYPE_SCRIPT - The surface is of type script, since 1.10
 * cairo.SURFACE_TYPE_QT - The surface is of type Qt, since 1.10
 * cairo.SURFACE_TYPE_RECORDING - The surface is of type recording, since 1.10
 * cairo.SURFACE_TYPE_VG - The surface is a OpenVG surface, since 1.10
 * cairo.SURFACE_TYPE_GL - The surface is of type OpenGL, since 1.10
 * cairo.SURFACE_TYPE_DRM - The surface is of type Direct Render Manager, since 1.10
 * cairo.SURFACE_TYPE_TEE - The surface is of type 'tee' (a multiplexing surface), since 1.10
 * cairo.SURFACE_TYPE_XML - The surface is of type XML (for debugging), since 1.10
 * cairo.SURFACE_TYPE_SKIA - The surface is of type Skia, since 1.10
 * cairo.SURFACE_TYPE_SUBSURFACE - The surface is a subsurface created with cairo_surface_create_for_rectangle(), since 1.10
 * cairo.SURFACE_TYPE_COGL - This surface is of type Cogl, since 1.12
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
 * cairo.surface_shas_show_text_glyphs(surface);
 * 
 * Returns whether the surface supports sophisticated cairo.show_text_glyphs() operations. That is, whether it actually uses the provided text and cluster data to a cairo.show_text_glyphs() call.
 * 
 * Note: Even if this function returns false, a cairo.show_text_glyphs() operation targeted at surface will still succeed. It just will act like a cairo.show_glyphs() operation. 
 * 
 * Users can use this function to avoid computing UTF-8 text and cluster mapping if the target surface does not use it.
 * 
 * @param {object} surface - opaque handle to a cairo surface.
 */
static JSVAL surface_has_show_text_glyphs(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return cairo_surface_has_show_text_glyphs(surface) ? True() : False();
}







static JSVAL context_create(JSARGS args) {
    cairo_surface_t *surface = (cairo_surface_t *) JSEXTERN(args[0]);
    return External::New(cairo_create(surface));
}

static JSVAL context_destroy(JSARGS args) {
    cairo_t *context = (cairo_t *) JSEXTERN(args[0]);
    cairo_destroy(context);
    return Undefined();
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
    

//    net->Set(String::New("sendFile"), FunctionTemplate::New(net_sendfile));

    builtinObject->Set(String::New("cairo"), cairo);
}
