/* *
 * @module builtin/gd
 * 
 * ### Synopsis
 * SilkJS builtin gd object.
 * 
 * ### Description
 * 
 * ### Usage
 * var gd = require('builtin/gd');
 * 
 * ### See Also:
 * http://www.boutell.com/gd/manual2.0.33.html
 */


#include "SilkJS.h"
#include <gd.h>
#include <gdfonts.h>
#include <gdfontl.h>
#include <gdfontmb.h>
#include <gdfontg.h>
#include <gdfontt.h>

// not implemented:
// gd2 file format functions
// animated gif functions

static JSVAL gd_imageCreate(JSARGS args) {
    HandleScope scope;
    int sx = args[0]->IntegerValue();
    int sy = args[1]->IntegerValue();
    gdImagePtr im = gdImageCreate(sx, sy);
    if (!im) {
        return Null();
    }
    return scope.Close(External::New(im));
}

static JSVAL gd_imageCreateTrueColor(JSARGS args) {
    HandleScope scope;
    int sx = args[0]->IntegerValue();
    int sy = args[1]->IntegerValue();
    gdImagePtr im = gdImageCreateTrueColor(sx, sy);
    if (!im) {
        return Null();
    }
    return scope.Close(External::New(im));
}

static JSVAL gd_ImageCreateFromJpeg(JSARGS args) {
    HandleScope scope;
    String::Utf8Value data(args[0]);
    FILE *in = fopen(*data, "rb");
    if (!in) {
        return Null();
    }
    gdImagePtr im = gdImageCreateFromJpeg(in);
    fclose(in);
    if (!im) {
        return Null();
    }
    return scope.Close(External::New(im));
}

/**
 * Create image from base64 encoded string
 * 
 * @param args
 * @return 
 */
static JSVAL gd_imageCreateFromJpeg64(JSARGS args) {
    HandleScope scope;
    String::Utf8Value data(args[0]);
    unsigned char buf[data.length()];
    int decoded = decode_base64(buf, *data);
    gdImagePtr im = gdImageCreateFromJpegPtr(decoded, (void *) buf);
//    string s = Base64Decode(*data);
//    gdImagePtr im = gdImageCreateFromJpegPtr(s.size(), (void *) s.c_str());
    if (!im) {
        return Null();
    }
    return scope.Close(External::New(im));
}

static JSVAL gd_imageCreateFromPng(JSARGS args) {
    HandleScope scope;
    String::Utf8Value data(args[0]);
    FILE *in = fopen(*data, "rb");
    if (!in) {
        return Null();
    }
    gdImagePtr im = gdImageCreateFromPng(in);
    fclose(in);
    if (!im) {
        return Null();
    }
    return scope.Close(External::New(im));
}

/**
 * Create image from base64 encoded string
 * 
 * @param args
 * @return 
 */
static JSVAL gd_imageCreateFromPng64(JSARGS args) {
    HandleScope scope;
    String::Utf8Value data(args[0]);
    unsigned char buf[data.length()];
    int decoded = decode_base64(buf, *data);
    gdImagePtr im = gdImageCreateFromPngPtr(decoded, (void *) buf);

//    string s = Base64Decode(*data);
//    gdImagePtr im = gdImageCreateFromPngPtr(s.size(), (void *) s.c_str());
    if (!im) {
        return Null();
    }
    return scope.Close(External::New(im));
}

static JSVAL gd_imageCreateFromGif(JSARGS args) {
    HandleScope scope;
    String::Utf8Value data(args[0]);
    FILE *in = fopen(*data, "rb");
    if (!in) {
        return Null();
    }
    gdImagePtr im = gdImageCreateFromGif(in);
    fclose(in);
    if (!im) {
        return Null();
    }
    return scope.Close(External::New(im));
}

/**
 * Create image from base64 encoded string
 * 
 * @param args
 * @return 
 */
static JSVAL gd_imageCreateFromGif64(JSARGS args) {
    HandleScope scope;
    String::Utf8Value data(args[0]);
    
    unsigned char buf[data.length()];
    int decoded = decode_base64(buf, *data);
    gdImagePtr im = gdImageCreateFromGifPtr(decoded, (void *) buf);

//    string s = Base64Decode(*data);
//    gdImagePtr im = gdImageCreateFromGifPtr(s.size(), (void *) s.c_str());
    if (!im) {
        return Null();
    }
    return scope.Close(External::New(im));
}

static JSVAL gd_imageCreateFromGd(JSARGS args) {
    HandleScope scope;
    String::Utf8Value data(args[0]);
    FILE *in = fopen(*data, "rb");
    if (!in) {
        return Null();
    }
    gdImagePtr im = gdImageCreateFromGd(in);
    fclose(in);
    if (!im) {
        return Null();
    }
    return scope.Close(External::New(im));
}

/**
 * Create image from base64 encoded string
 * 
 * @param args
 * @return 
 */
static JSVAL gd_imageCreateFromGd64(JSARGS args) {
    HandleScope scope;
    String::Utf8Value data(args[0]);
    unsigned char buf[data.length()];
    int decoded = decode_base64(buf, *data);
    gdImagePtr im = gdImageCreateFromGdPtr(decoded, (void *) buf);
//    string s = Base64Decode(*data);
//    gdImagePtr im = gdImageCreateFromGdPtr(s.size(), (void *) s.c_str());
    if (!im) {
        return Null();
    }
    return scope.Close(External::New(im));
}

static JSVAL gd_imageCreateFromWBMP(JSARGS args) {
    HandleScope scope;
    String::Utf8Value data(args[0]);
    FILE *in = fopen(*data, "rb");
    if (!in) {
        return Null();
    }
    gdImagePtr im = gdImageCreateFromWBMP(in);
    fclose(in);
    if (!im) {
        return Null();
    }
    return scope.Close(External::New(im));
}

/**
 * Create image from base64 encoded string
 * 
 * @param args
 * @return 
 */
static JSVAL gd_imageCreateFromWBMP64(JSARGS args) {
    HandleScope scope;
    String::Utf8Value data(args[0]);
    unsigned char buf[data.length()];
    int decoded = decode_base64(buf, *data);
    gdImagePtr im = gdImageCreateFromWBMPPtr(decoded, (void *) buf);
//    string s = Base64Decode(*data);
//    gdImagePtr im = gdImageCreateFromWBMPPtr(s.size(), (void *) s.c_str());
    if (!im) {
        return Null();
    }
    return scope.Close(External::New(im));
}

static JSVAL gd_imageCreateFromXpm(JSARGS args) {
    HandleScope scope;
    String::Utf8Value data(args[0]);
    gdImagePtr im = gdImageCreateFromXpm(*data);
    return scope.Close(External::New(im));
}

static JSVAL gd_imageCreateFromXbm(JSARGS args) {
    HandleScope scope;
    String::Utf8Value data(args[0]);
    FILE *in = fopen(*data, "rb");
    if (!in) {
        return Null();
    }
    gdImagePtr im = gdImageCreateFromXbm(in);
    fclose(in);
    if (!im) {
        return Null();
    }
    return scope.Close(External::New(im));
}

static JSVAL gd_imageDestroy(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    gdImageDestroy(im);
    return Undefined();
}

/**
 * @function gd.imageJpeg
 * 
 * ### Synopsis
 * 
 * var success = gd.imageJpeg(handle, filename, quality);
 * 
 * Outputs the specified image to the specified file in JPEG format.
 * 
 * If quality is negative, the default IJG JPEG quality value (which should yield a good general quality / size tradeoff for most situations) is used. Otherwise, for practical purposes, quality should be a value in the range 0-95, higher quality values usually implying both higher quality and larger image sizes.
 *
 * If you have set image interlacing using gd.imageInterlace(), this function will interpret that to mean you wish to output a progressive JPEG. Some programs (e.g., Web browsers) can display progressive JPEGs incrementally; this can be useful when browsing over a relatively slow communications link, for example. Progressive JPEGs can also be slightly smaller than sequential (non-progressive) JPEGs.
 * 
 * @param {object} handle - handle to image to write
 * @param {string} filename - file to write
 * @param {int} quality - see note above
 * @return {boolean} success - true if the file was written, false if there was an error.
 */
static JSVAL gd_imageJpeg(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    String::Utf8Value data(args[1]);
    int quality = args[2]->IntegerValue();

    FILE *out = fopen(*data, "wb");
    if (!out) {
        return False();
    }
    gdImageJpeg(im, out, quality);
    fclose(out);
    return True();
}

static JSVAL gd_imageJpeg64(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int quality = args[1]->IntegerValue();
    int size;
    void *ptr = gdImageJpegPtr(im, &size, quality);
    string out = Base64Encode((const unsigned char *) ptr, size);
    gdFree(ptr);
    return String::New(out.c_str(), out.size());
}

/**
 * @function gd.imageGif
 * 
 * ### Synopsis
 * 
 * var success = gd.imageGif(handle, filename);
 * 
 * Outputs the specified image to the specified file in GIF format. 
 * 
 * GIF does not support true color; GIF images can contain a maximum of 256 colors. If the image to be written is a truecolor image, such as those created with gd.imageCreateTrueColor() or loaded from a JPEG or a truecolor PNG image file, a palette-based temporary image will automatically be created internally using the gd.imageCreatePaletteFromTrueColor() function. The original image pixels are not modified. This conversion produces high quality palettes but does require some CPU time. If you are regularly converting truecolor to palette in this way, you should consider creating your image as a palette-based image in the first place.
 * 
 * @param {object} handle - handle to image to write.
 * @param {string} filename - file to write.
 * @return {boolean} success - true if the file was written, false if there was an error..
 */
static JSVAL gd_imageGif(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    String::Utf8Value data(args[1]);

    FILE *out = fopen(*data, "wb");
    if (!out) {
        return False();
    }
    gdImageGif(im, out);
    fclose(out);
    return True();
}

static JSVAL gd_imageGif64(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int size;
    void *ptr = gdImageGifPtr(im, &size);
    string out = Base64Encode((const unsigned char *) ptr, size);
    gdFree(ptr);
    return String::New(out.c_str(), out.size());
}

// animated gif methods not implemented

/**
 * @function gd.imagePng
 * 
 * ### Synopsis
 * 
 * var success = gd.imagePng(handle, filename);
 * 
 * Outputs the specified image to the specified file in PNG format.
 * 
 * @param {object} handle - handle to image to write.
 * @param {string} filename - file to write.
 * @return {boolean} success - true if the file was written, false if there was an error..
 */
static JSVAL gd_imagePng(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    String::Utf8Value data(args[1]);

    FILE *out = fopen(*data, "wb");
    if (!out) {
        return False();
    }
    gdImagePng(im, out);
    fclose(out);
    return True();
}

static JSVAL gd_imagePng64(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int size;
    void *ptr = gdImagePngPtr(im, &size);
    string out = Base64Encode((const unsigned char *) ptr, size);
    gdFree(ptr);
    return String::New(out.c_str(), out.size());
}

/**
 * @function gd.imagePngEx
 * 
 * ### Synopsis
 * 
 * var success = gd.imagePngEx(handle, filename, level);
 * 
 * Outputs the specified image to the specified file in PNG format.
 * 
 * This function allows the level of compression to be specified. A compression level of 0 means "no compression." A compression level of 1 means "compressed, but as quickly as possible." A compression level of 9 means "compressed as much as possible to produce the smallest possible file." A compression level of -1 will use the default compression level at the time zlib was compiled on your system.
 * 
 * @param {object} handle - handle to image to write.
 * @param {string} filename - file to write.
 * @param {int} level - compression level, see notes above.
 * @return {boolean} success - true if the file was written, false if there was an error.
 */
static JSVAL gd_imagePngEx(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    String::Utf8Value data(args[1]);
    int quality = args[2]->IntegerValue();

    FILE *out = fopen(*data, "wb");
    if (!out) {
        return False();
    }
    gdImagePngEx(im, out, quality);
    fclose(out);
    return True();
}

static JSVAL gd_imagePng64Ex(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int quality = args[1]->IntegerValue();
    int size;
    void *ptr = gdImagePngPtrEx(im, &size, quality);
    string out = Base64Encode((unsigned char *) ptr, size);
    gdFree(ptr);
    return String::New(out.c_str(), out.size());
}

static JSVAL gd_imageWBMP(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int fg = args[1]->IntegerValue();
    String::Utf8Value data(args[2]);

    FILE *out = fopen(*data, "wb");
    if (!out) {
        return False();
    }
    gdImageWBMP(im, fg, out);
    fclose(out);
    return True();
}

static JSVAL gd_imageWBMP64(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int fg = args[1]->IntegerValue();
    int size;
    void *ptr = gdImageWBMPPtr(im, &size, fg);
    string out = Base64Encode((const unsigned char *) ptr, size);
    gdFree(ptr);
    return String::New(out.c_str(), out.size());
}

static JSVAL gd_imageGd(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    String::Utf8Value data(args[1]);

    FILE *out = fopen(*data, "wb");
    if (!out) {
        return False();
    }
    gdImageGd(im, out);
    fclose(out);
    return True();
}

static JSVAL gd_imageGd64(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int size;
    void *ptr = gdImageGdPtr(im, &size);
    string out = Base64Encode((unsigned char *) ptr, size);
    gdFree(ptr);
    return String::New(out.c_str(), out.size());
}

// true color / palette functions

static JSVAL gd_imageTrueColorToPalette(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int ditherFlag = args[1]->IntegerValue();
    int colorsWanted = args[2]->IntegerValue();
    gdImageTrueColorToPalette(im, ditherFlag, colorsWanted);
    return Undefined();
}

static JSVAL gd_imageCreatePaletteFromTrueColor(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int ditherFlag = args[1]->IntegerValue();
    int colorsWanted = args[2]->IntegerValue();
    gdImagePtr newImage = gdImageCreatePaletteFromTrueColor(im, ditherFlag, colorsWanted);
    return scope.Close(External::New(newImage));
}

static JSVAL gd_imageSetPixel(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int x = args[1]->IntegerValue();
    int y = args[2]->IntegerValue();
    int color = args[3]->IntegerValue();
    gdImageSetPixel(im, x, y, color);
    return Undefined();
}

static JSVAL gd_imageLine(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int x1 = args[1]->IntegerValue();
    int y1 = args[2]->IntegerValue();
    int x2 = args[3]->IntegerValue();
    int y2 = args[4]->IntegerValue();
    int color = args[5]->IntegerValue();
    gdImageLine(im, x1, y1, x2, y2, color);
    return Undefined();
}

/**
 * gd.imagePolygon(image, points, color);
 * 
 * points is something like:
 * [
 *		{ x: xVal, y: yVal },
 *		{ x: xVal, y: yVal },
 *		{ x: xVal, y: yVal },
 *		...
 * } 
 * @param args
 * @return 
 */
static JSVAL gd_imagePolygon(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    Handle<Array>jPoints = Handle<Array>::Cast(args[1]->ToObject());
    int color = args[2]->IntegerValue();
    int numPoints = jPoints->Length();
    gdPoint points[numPoints];
    Handle<String>x = String::New("x");
    Handle<String>y = String::New("y");

    for (int i = 0; i < numPoints; i++) {
        Handle<Object>p = Handle<Object>::Cast(jPoints->Get(i));
        points[i].x = p->Get(x)->IntegerValue();
        points[i].y = p->Get(y)->IntegerValue();
    }
    gdImagePolygon(im, points, numPoints, color);
    return Undefined();
}

/**
 * gd.imagePolygon(image, points, color);
 * 
 * points is something like:
 * [
 *		{ x: xVal, y: yVal },
 *		{ x: xVal, y: yVal },
 *		{ x: xVal, y: yVal },
 *		...
 * } 
 * @param args
 * @return 
 */
static JSVAL gd_imageOpenPolygon(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    Handle<Array>jPoints = Handle<Array>::Cast(args[1]->ToObject());
    int color = args[2]->IntegerValue();
    int numPoints = jPoints->Length();
    gdPoint points[numPoints];
    Handle<String>x = String::New("x");
    Handle<String>y = String::New("y");

    for (int i = 0; i < numPoints; i++) {
        Handle<Object>p = Handle<Object>::Cast(jPoints->Get(i));
        points[i].x = p->Get(x)->IntegerValue();
        points[i].y = p->Get(y)->IntegerValue();
    }
    gdImageOpenPolygon(im, points, numPoints, color);
    return Undefined();
}

static JSVAL gd_imageRectangle(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int x1 = args[1]->IntegerValue();
    int y1 = args[2]->IntegerValue();
    int x2 = args[3]->IntegerValue();
    int y2 = args[4]->IntegerValue();
    int color = args[5]->IntegerValue();
    gdImageRectangle(im, x1, y1, x2, y2, color);
    return Undefined();
}

/**
 * gd.imagePolygon(image, points, color);
 * 
 * points is something like:
 * [
 *		{ x: xVal, y: yVal },
 *		{ x: xVal, y: yVal },
 *		{ x: xVal, y: yVal },
 *		...
 * } 
 * @param args
 * @return 
 */
static JSVAL gd_imageFilledPolygon(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    Handle<Array>jPoints = Handle<Array>::Cast(args[1]->ToObject());
    int color = args[2]->IntegerValue();
    int numPoints = jPoints->Length();
    gdPoint points[numPoints];
    Handle<String>x = String::New("x");
    Handle<String>y = String::New("y");

    for (int i = 0; i < numPoints; i++) {
        Handle<Object>p = Handle<Object>::Cast(jPoints->Get(i));
        points[i].x = p->Get(x)->IntegerValue();
        points[i].y = p->Get(y)->IntegerValue();
    }
    gdImageFilledPolygon(im, points, numPoints, color);
    return Undefined();
}

static JSVAL gd_imageFilledRectangle(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int x1 = args[1]->IntegerValue();
    int y1 = args[2]->IntegerValue();
    int x2 = args[3]->IntegerValue();
    int y2 = args[4]->IntegerValue();
    int color = args[5]->IntegerValue();
    gdImageFilledRectangle(im, x1, y1, x2, y2, color);
    return Undefined();
}

static JSVAL gd_imageArc(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int cx = args[1]->IntegerValue();
    int cy = args[2]->IntegerValue();
    int w = args[3]->IntegerValue();
    int h = args[4]->IntegerValue();
    int s = args[5]->IntegerValue();
    int e = args[6]->IntegerValue();
    int color = args[7]->IntegerValue();
    gdImageArc(im, cx, cy, w, h, s, e, color);
    return Undefined();
}

static JSVAL gd_imageFilledArc(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int cx = args[1]->IntegerValue();
    int cy = args[2]->IntegerValue();
    int w = args[3]->IntegerValue();
    int h = args[4]->IntegerValue();
    int s = args[5]->IntegerValue();
    int e = args[6]->IntegerValue();
    int color = args[7]->IntegerValue();
    int style = args[8]->IntegerValue();
    gdImageFilledArc(im, cx, cy, w, h, s, e, color, style);
    return Undefined();
}

static JSVAL gd_imageFilledEllipse(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int cx = args[1]->IntegerValue();
    int cy = args[2]->IntegerValue();
    int w = args[3]->IntegerValue();
    int h = args[4]->IntegerValue();
    int color = args[5]->IntegerValue();
    gdImageFilledEllipse(im, cx, cy, w, h, color);
    return Undefined();
}

static JSVAL gd_imageFillToBorder(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int x = args[1]->IntegerValue();
    int y = args[2]->IntegerValue();
    int border = args[3]->IntegerValue();
    int color = args[4]->IntegerValue();
    gdImageFillToBorder(im, x, y, border, color);
    return Undefined();
}

static JSVAL gd_imageFill(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int x = args[1]->IntegerValue();
    int y = args[2]->IntegerValue();
    int color = args[3]->IntegerValue();
    gdImageFill(im, x, y, color);
    return Undefined();
}

static JSVAL gd_imageSetAntiAliased(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int c = args[1]->IntegerValue();
    gdImageSetAntiAliased(im, c);
    return Undefined();
}

static JSVAL gd_imageSetAntiAliasedDontBlend(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int c = args[1]->IntegerValue();
    gdImageSetAntiAliased(im, c);
    return Undefined();
}

static JSVAL gd_imageSetBrush(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdImagePtr brush = (gdImagePtr) wrap->Value();
    gdImageSetBrush(im, brush);
    return Undefined();
}

static JSVAL gd_imageSetTile(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdImagePtr brush = (gdImagePtr) wrap->Value();
    gdImageSetTile(im, brush);
    return Undefined();
}

static JSVAL gd_imageSetStyle(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    Handle<Array>jStyle = Handle<Array>::Cast(args[1]->ToObject());
    int styleLength = jStyle->Length();
    int style[styleLength];
    for (int i = 0; i < styleLength; i++) {
        style[i] = jStyle->Get(i)->IntegerValue();
    }
    gdImageSetStyle(im, style, styleLength);
    return Undefined();
}

static JSVAL gd_imageSetThickness(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int thickness = args[1]->IntegerValue();
    gdImageSetThickness(im, thickness);
    return Undefined();
}

static JSVAL gd_imageAlphaBlending(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int blending = args[1]->IntegerValue();
    gdImageAlphaBlending(im, blending);
    return Undefined();
}

static JSVAL gd_imageSaveAlpha(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int saveFlag = args[1]->IntegerValue();
    gdImageSaveAlpha(im, saveFlag);
    return Undefined();
}

static JSVAL gd_imageSetClip(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int x1 = args[1]->IntegerValue();
    int y1 = args[2]->IntegerValue();
    int x2 = args[3]->IntegerValue();
    int y2 = args[4]->IntegerValue();
    gdImageSetClip(im, x1, y1, x2, y2);
    return Undefined();
}

static JSVAL gd_imageGetClip(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int x1, y1, x2, y2;
    gdImageGetClip(im, &x1, &y1, &x2, &y2);
    Handle<Object>o = Object::New();
    o->Set(String::New("x1"), Integer::New(x1));
    o->Set(String::New("y1"), Integer::New(y1));
    o->Set(String::New("x2"), Integer::New(x2));
    o->Set(String::New("y2"), Integer::New(y2));
    return scope.Close(o);
}

// Query Functions

static JSVAL gd_imageAlpha(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int color = args[1]->IntegerValue();
    int alpha = gdImageAlpha(im, color);
    return scope.Close(Integer::New(alpha));
}

static JSVAL gd_imageGetPixel(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int x = args[1]->IntegerValue();
    int y = args[2]->IntegerValue();
    int color = gdImageGetPixel(im, x, y);
    return scope.Close(Integer::New(color));
}

static JSVAL gd_imageBoundsSafe(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int x = args[1]->IntegerValue();
    int y = args[2]->IntegerValue();
    return gdImageBoundsSafe(im, x, y) ? True() : False();
}

static JSVAL gd_imageBlue(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int color = args[1]->IntegerValue();
    int blue = gdImageBlue(im, color);
    return scope.Close(Integer::New(blue));
}

static JSVAL gd_imageGreen(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int color = args[1]->IntegerValue();
    int green = gdImageGreen(im, color);
    return scope.Close(Integer::New(green));
}

static JSVAL gd_imageRed(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int color = args[1]->IntegerValue();
    int red = gdImageRed(im, color);
    return scope.Close(Integer::New(red));
}

static JSVAL gd_imageSX(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    return scope.Close(Integer::New(gdImageSX(im)));
}

static JSVAL gd_imageSY(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    return scope.Close(Integer::New(gdImageSY(im)));
}

// Fonts and text-handling functions

static JSVAL gd_fontGetSmall(JSARGS args) {
    HandleScope scope;
    return scope.Close(External::New(gdFontGetSmall()));
}

static JSVAL gd_fontGetLarge(JSARGS args) {
    HandleScope scope;
    return scope.Close(External::New(gdFontGetLarge()));
}

static JSVAL gd_fontGetMediumBold(JSARGS args) {
    HandleScope scope;
    return scope.Close(External::New(gdFontGetMediumBold()));
}

static JSVAL gd_fontGetGiant(JSARGS args) {
    HandleScope scope;
    return scope.Close(External::New(gdFontGetGiant()));
}

static JSVAL gd_fontGetTiny(JSARGS args) {
    HandleScope scope;
    return scope.Close(External::New(gdFontGetTiny()));
}

static JSVAL gd_imageChar(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdFontPtr font = (gdFontPtr) wrap->Value();
    int x = args[2]->IntegerValue();
    int y = args[3]->IntegerValue();
    int c = args[4]->IntegerValue();
    int color = args[5]->IntegerValue();
    gdImageChar(im, font, x, y, c, color);
    return Undefined();
}

static JSVAL gd_imageCharUp(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdFontPtr font = (gdFontPtr) wrap->Value();
    int x = args[2]->IntegerValue();
    int y = args[3]->IntegerValue();
    int c = args[4]->IntegerValue();
    int color = args[5]->IntegerValue();
    gdImageCharUp(im, font, x, y, c, color);
    return Undefined();
}

static JSVAL gd_imageString(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdFontPtr font = (gdFontPtr) wrap->Value();
    int x = args[2]->IntegerValue();
    int y = args[3]->IntegerValue();
    String::AsciiValue s(args[4]);
    int color = args[5]->IntegerValue();
    gdImageString(im, font, x, y, (unsigned char *) *s, color);
    return Undefined();
}

static JSVAL gd_imageString16(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdFontPtr font = (gdFontPtr) wrap->Value();
    int x = args[2]->IntegerValue();
    int y = args[3]->IntegerValue();
    String::Utf8Value s(args[4]);
    int color = args[5]->IntegerValue();
    gdImageString16(im, font, x, y, (unsigned short *) *s, color);
    return Undefined();
}

static JSVAL gd_imageStringUp(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdFontPtr font = (gdFontPtr) wrap->Value();
    int x = args[2]->IntegerValue();
    int y = args[3]->IntegerValue();
    String::AsciiValue s(args[4]);
    int color = args[5]->IntegerValue();
    gdImageStringUp(im, font, x, y, (unsigned char *) *s, color);
    return Undefined();
}

static JSVAL gd_imageStringUp16(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdFontPtr font = (gdFontPtr) wrap->Value();
    int x = args[2]->IntegerValue();
    int y = args[3]->IntegerValue();
    String::Utf8Value s(args[4]);
    int color = args[5]->IntegerValue();
    gdImageStringUp16(im, font, x, y, (unsigned short *) *s, color);
    return Undefined();
}

static JSVAL gd_FTUseFontConfig(JSARGS args) {
    HandleScope scope;
    int flag = args[0]->IntegerValue();
    return scope.Close(Integer::New(gdFTUseFontConfig(flag)));
}



// TODO: finish font/text functions

// Color handling functions

static JSVAL gd_imageColorAllocate(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    return scope.Close(Integer::New(gdImageColorAllocate(im, red, green, blue)));
}

static JSVAL gd_imageColorAllocateAlpha(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    int alpha = args[4]->IntegerValue();
    return scope.Close(Integer::New(gdImageColorAllocateAlpha(im, red, green, blue, alpha)));
}

static JSVAL gd_imageColorClosest(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    return scope.Close(Integer::New(gdImageColorClosest(im, red, green, blue)));
}

static JSVAL gd_imageColorClosestAlpha(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    int alpha = args[4]->IntegerValue();
    return scope.Close(Integer::New(gdImageColorClosestAlpha(im, red, green, blue, alpha)));
}

static JSVAL gd_imageColorClosestHWB(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    return scope.Close(Integer::New(gdImageColorClosestHWB(im, red, green, blue)));
}

static JSVAL gd_imageColorExact(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    return scope.Close(Integer::New(gdImageColorExact(im, red, green, blue)));
}

static JSVAL gd_imageColorResolve(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    return scope.Close(Integer::New(gdImageColorResolve(im, red, green, blue)));
}

static JSVAL gd_imageColorResolveAlpha(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    int alpha = args[4]->IntegerValue();
    return scope.Close(Integer::New(gdImageColorResolveAlpha(im, red, green, blue, alpha)));
}

static JSVAL gd_imageColorsTotal(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    return scope.Close(Integer::New(gdImageColorsTotal(im)));
}

static JSVAL gd_imageGetInterlaced(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    return scope.Close(Integer::New(gdImageGetInterlaced(im)));
}

static JSVAL gd_imageGetTransparent(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    return scope.Close(Integer::New(gdImageGetTransparent(im)));
}

static JSVAL gd_imageColorDeallocate(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int color = args[1]->IntegerValue();
    gdImageColorDeallocate(im, color);
    return Undefined();
}

static JSVAL gd_imageColorTransparent(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int color = args[1]->IntegerValue();
    gdImageColorTransparent(im, color);
    return Undefined();
}

static JSVAL gd_TrueColor(JSARGS args) {
    HandleScope scope;
    int red = args[0]->IntegerValue();
    int green = args[1]->IntegerValue();
    int blue = args[2]->IntegerValue();
    return Integer::New(gdTrueColor(red, green, blue));
}

static JSVAL gd_TrueColorAlpha(JSARGS args) {
    HandleScope scope;
    int red = args[0]->IntegerValue();
    int green = args[1]->IntegerValue();
    int blue = args[2]->IntegerValue();
    int alpha = args[3]->IntegerValue();
    return Integer::New(gdTrueColorAlpha(red, green, blue, alpha));
}

// copying and resizing functions

/**
 * @function gd.imageCopy
 * 
 * ### Synopsis
 * 
 * gd.imageCopy(dstImage, srcImage, dstX, dstY, srcX, srcY, w, h);
 * 
 * Copy a rectangular portion of one image to another image.
 * 
 * @param {object} dstImage - destination for resized image
 * @param {object} srcImage - source for resized image
 * @param {int} dstX - X coordinate of the upper left corner of the destination region
 * @param {int} dstY - Y coordinate of the upper left corner of the destination region
 * @param {int} srcX - X coordinate of the upper left corner of the source region
 * @param {int} srcY - Y coordinate of the upper left corner of the source region
 * @param {int} w - width of the region
 * @param {int} h - height of the region
 * @return 
 */
static JSVAL gd_imageCopy(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr dst = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdImagePtr src = (gdImagePtr) wrap->Value();
    int dstX = args[2]->IntegerValue();
    int dstY = args[3]->IntegerValue();
    int srcX = args[4]->IntegerValue();
    int srcY = args[5]->IntegerValue();
    int w = args[6]->IntegerValue();
    int h = args[7]->IntegerValue();
    gdImageCopy(dst, src, dstX, dstY, srcX, srcY, w, h);
    return Undefined();
}

/**
 * @function gd.imageCopyResized
 * 
 * ### Synopsis
 * 
 * gd.imageCopyResized(dstImage, srcImage, dstX, dstY, srcX, srcY, dstW, dstH, srcW, srcH);
 * 
 * Copy a rectangular portion of one image to another image. The X and Y dimensions of the original region and the destination region can vary, resulting in stretching or shrinking of the region as appropriate
 * 
 * @param {object} dstImage - destination for resized image
 * @param {object} srcImage - source for resized image
 * @param {int} dstX - X coordinate of the upper left corner of the destination region
 * @param {int} dstY - Y coordinate of the upper left corner of the destination region
 * @param {int} srcX - X coordinate of the upper left corner of the source region
 * @param {int} srcY - Y coordinate of the upper left corner of the source region
 * @param {int} dstW - width of the destination region
 * @param {int} dstH - height of the destination region
 * @param {int} srcW - width of the source region
 * @param {int} srcH - height of the source region
 * @return 
 */
static JSVAL gd_imageCopyResized(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr dst = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdImagePtr src = (gdImagePtr) wrap->Value();
    int dstX = args[2]->IntegerValue();
    int dstY = args[3]->IntegerValue();
    int srcX = args[4]->IntegerValue();
    int srcY = args[5]->IntegerValue();
    int dstW = args[6]->IntegerValue();
    int dstH = args[7]->IntegerValue();
    int srcW = args[8]->IntegerValue();
    int srcH = args[9]->IntegerValue();
    gdImageCopyResized(dst, src, dstX, dstY, srcX, srcY, dstW, dstH, srcW, srcH);
    return Undefined();
}

/**
 * @function gd.imageCopyResampled
 * 
 * ### Synopsis
 * 
 * gd.imageCopyResampled(dstImage, srcImage, dstX, dstY, srcX, srcY, dstW, dstH, srcW, srcH);
 * 
 * Copy a rectangular portion of one image to another image, smoothly interpolating pixel values so that, in particular, reducing the size of an image still retains a great deal of clarity. The X and Y dimensions of the original region and the destination region can vary, resulting in stretching or shrinking of the region as appropriate.
 * 
 * @param {object} dstImage - destination for resized image
 * @param {object} srcImage - source for resized image
 * @param {int} dstX - X coordinate of the upper left corner of the destination region
 * @param {int} dstY - Y coordinate of the upper left corner of the destination region
 * @param {int} srcX - X coordinate of the upper left corner of the source region
 * @param {int} srcY - Y coordinate of the upper left corner of the source region
 * @param {int} dstW - width of the destination region
 * @param {int} dstH - height of the destination region
 * @param {int} srcW - width of the source region
 * @param {int} srcH - height of the source region
 * @return 
 */
static JSVAL gd_imageCopyResampled(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr dst = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdImagePtr src = (gdImagePtr) wrap->Value();
    int dstX = args[2]->IntegerValue();
    int dstY = args[3]->IntegerValue();
    int srcX = args[4]->IntegerValue();
    int srcY = args[5]->IntegerValue();
    int dstW = args[6]->IntegerValue();
    int dstH = args[7]->IntegerValue();
    int srcW = args[8]->IntegerValue();
    int srcH = args[9]->IntegerValue();
    gdImageCopyResampled(dst, src, dstX, dstY, srcX, srcY, dstW, dstH, srcW, srcH);
    return Undefined();
}

static JSVAL gd_imageCopyRotated(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr dst = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdImagePtr src = (gdImagePtr) wrap->Value();
    double dstX = args[2]->NumberValue();
    double dstY = args[3]->NumberValue();
    int srcX = args[4]->IntegerValue();
    int srcY = args[5]->IntegerValue();
    int srcW = args[6]->IntegerValue();
    int srcH = args[7]->IntegerValue();
    int angle = args[8]->IntegerValue();
    gdImageCopyRotated(dst, src, dstX, dstY, srcX, srcY, srcW, srcH, angle);
    return Undefined();
}

static JSVAL gd_imageCopyMerge(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr dst = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdImagePtr src = (gdImagePtr) wrap->Value();
    int dstX = args[2]->IntegerValue();
    int dstY = args[3]->IntegerValue();
    int srcX = args[4]->IntegerValue();
    int srcY = args[5]->IntegerValue();
    int w = args[6]->IntegerValue();
    int h = args[7]->IntegerValue();
    int pct = args[8]->IntegerValue();
    gdImageCopyMerge(dst, src, dstX, dstY, srcX, srcY, w, h, pct);
    return Undefined();
}

static JSVAL gd_imageCopyMergeGray(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr dst = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdImagePtr src = (gdImagePtr) wrap->Value();
    int dstX = args[2]->IntegerValue();
    int dstY = args[3]->IntegerValue();
    int srcX = args[4]->IntegerValue();
    int srcY = args[5]->IntegerValue();
    int w = args[6]->IntegerValue();
    int h = args[7]->IntegerValue();
    int pct = args[8]->IntegerValue();
    gdImageCopyMergeGray(dst, src, dstX, dstY, srcX, srcY, w, h, pct);
    return Undefined();
}

static JSVAL gd_imagePaletteCopy(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr dst = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdImagePtr src = (gdImagePtr) wrap->Value();
    gdImagePaletteCopy(dst, src);
    return Undefined();
}

static JSVAL gd_imageSquareToCircle(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr dst = (gdImagePtr) wrap->Value();
    int radius = args[1]->IntegerValue();
    gdImageSquareToCircle(dst, radius);
    return Undefined();
}

static JSVAL gd_imageSharpen(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr dst = (gdImagePtr) wrap->Value();
    int pct = args[1]->IntegerValue();
    gdImageSharpen(dst, pct);
    return Undefined();
}

static JSVAL gd_imageCompare(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im1 = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdImagePtr im2 = (gdImagePtr) wrap->Value();
    return scope.Close(Integer::New(gdImageCompare(im1, im2)));
}

static JSVAL gd_imageInterlace(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr dst = (gdImagePtr) wrap->Value();
    int interlace = args[1]->IntegerValue();
    gdImageInterlace(dst, interlace);
    return Undefined();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void init_gd_object() {
    HandleScope scope;

    Handle<ObjectTemplate>gd = ObjectTemplate::New();

    // constants
    gd->Set(String::New("AntiAliased"), Integer::New(gdAntiAliased));
    gd->Set(String::New("Brushed"), Integer::New(gdBrushed));
    gd->Set(String::New("MaxColors"), Integer::New(gdMaxColors));
    gd->Set(String::New("Styled"), Integer::New(gdStyled));
    gd->Set(String::New("StyledBrushed"), Integer::New(gdStyledBrushed));
    gd->Set(String::New("DashSize"), Integer::New(gdDashSize));
    gd->Set(String::New("Tiled"), Integer::New(gdTiled));
    gd->Set(String::New("Transparent"), Integer::New(gdTransparent));


    // fonts
    gd->Set(String::New("fontGetSmall"), FunctionTemplate::New(gd_fontGetSmall));
    gd->Set(String::New("fontGetLarge"), FunctionTemplate::New(gd_fontGetLarge));
    gd->Set(String::New("fontGetMediumBold"), FunctionTemplate::New(gd_fontGetMediumBold));
    gd->Set(String::New("fontGetGiant"), FunctionTemplate::New(gd_fontGetGiant));
    gd->Set(String::New("fontGetTiny"), FunctionTemplate::New(gd_fontGetTiny));

    // images
    gd->Set(String::New("imageCreate"), FunctionTemplate::New(gd_imageCreate));
    gd->Set(String::New("imageCreateTrueColor"), FunctionTemplate::New(gd_imageCreateTrueColor));
    gd->Set(String::New("imageCreateFromJpeg"), FunctionTemplate::New(gd_ImageCreateFromJpeg));
    gd->Set(String::New("imageCreateFromJpeg64"), FunctionTemplate::New(gd_imageCreateFromJpeg64));
    gd->Set(String::New("imageCreateFromPng"), FunctionTemplate::New(gd_imageCreateFromPng));
    gd->Set(String::New("imageCreateFromPng64"), FunctionTemplate::New(gd_imageCreateFromPng64));
    gd->Set(String::New("imageCreateFromGif"), FunctionTemplate::New(gd_imageCreateFromGif));
    gd->Set(String::New("imageCreateFromGif64"), FunctionTemplate::New(gd_imageCreateFromGif64));
    gd->Set(String::New("imageCreateFromGd"), FunctionTemplate::New(gd_imageCreateFromGd));
    gd->Set(String::New("imageCreateFromGd64"), FunctionTemplate::New(gd_imageCreateFromGd64));
    gd->Set(String::New("imageCreateFromWBMP"), FunctionTemplate::New(gd_imageCreateFromWBMP));
    gd->Set(String::New("imageCreateFromWBMP64"), FunctionTemplate::New(gd_imageCreateFromWBMP64));
    gd->Set(String::New("imageCreateFromXpm"), FunctionTemplate::New(gd_imageCreateFromXpm));
    gd->Set(String::New("imageCreateFromXbm"), FunctionTemplate::New(gd_imageCreateFromXbm));
    gd->Set(String::New("imageDestroy"), FunctionTemplate::New(gd_imageDestroy));
    gd->Set(String::New("imageJpeg"), FunctionTemplate::New(gd_imageJpeg));
    gd->Set(String::New("imageJpeg64"), FunctionTemplate::New(gd_imageJpeg64));
    gd->Set(String::New("imageGif"), FunctionTemplate::New(gd_imageGif));
    gd->Set(String::New("imageGif64"), FunctionTemplate::New(gd_imageGif64));
    gd->Set(String::New("imagePng"), FunctionTemplate::New(gd_imagePng));
    gd->Set(String::New("imagePng64"), FunctionTemplate::New(gd_imagePng64));
    gd->Set(String::New("imagePngEx"), FunctionTemplate::New(gd_imagePngEx));
    gd->Set(String::New("imagePng64Ex"), FunctionTemplate::New(gd_imagePng64Ex));
    gd->Set(String::New("imageWBMP"), FunctionTemplate::New(gd_imageWBMP));
    gd->Set(String::New("imageWBMP64"), FunctionTemplate::New(gd_imageWBMP64));
    gd->Set(String::New("imageGd"), FunctionTemplate::New(gd_imageGd));
    gd->Set(String::New("imageGd64"), FunctionTemplate::New(gd_imageGd64));
    gd->Set(String::New("imageTrueColorToPalette"), FunctionTemplate::New(gd_imageTrueColorToPalette));
    gd->Set(String::New("imageCreatePaletteFromTrueColor"), FunctionTemplate::New(gd_imageCreatePaletteFromTrueColor));
    gd->Set(String::New("imageSetPixel"), FunctionTemplate::New(gd_imageSetPixel));
    gd->Set(String::New("imageLine"), FunctionTemplate::New(gd_imageLine));
    gd->Set(String::New("imagePolygon"), FunctionTemplate::New(gd_imagePolygon));
    gd->Set(String::New("imageOpenPolygon"), FunctionTemplate::New(gd_imageOpenPolygon));
    gd->Set(String::New("imageRectangle"), FunctionTemplate::New(gd_imageRectangle));
    gd->Set(String::New("imageFilledPolygon"), FunctionTemplate::New(gd_imageFilledPolygon));
    gd->Set(String::New("imageFilledRectangle"), FunctionTemplate::New(gd_imageFilledRectangle));
    gd->Set(String::New("imageArc"), FunctionTemplate::New(gd_imageArc));
    gd->Set(String::New("imageFilledArc"), FunctionTemplate::New(gd_imageFilledArc));
    gd->Set(String::New("imageFilledEllipse"), FunctionTemplate::New(gd_imageFilledEllipse));
    gd->Set(String::New("imageFillToBorder"), FunctionTemplate::New(gd_imageFillToBorder));
    gd->Set(String::New("imageFill"), FunctionTemplate::New(gd_imageFill));
    gd->Set(String::New("imageSetAntiAliased"), FunctionTemplate::New(gd_imageSetAntiAliased));
    gd->Set(String::New("imageSetAntiAliasedDontBlend"), FunctionTemplate::New(gd_imageSetAntiAliasedDontBlend));
    gd->Set(String::New("imageSetBrush"), FunctionTemplate::New(gd_imageSetBrush));
    gd->Set(String::New("imageSetTile"), FunctionTemplate::New(gd_imageSetTile));
    gd->Set(String::New("imageSetStyle"), FunctionTemplate::New(gd_imageSetStyle));
    gd->Set(String::New("imageSetThickness"), FunctionTemplate::New(gd_imageSetThickness));
    gd->Set(String::New("imageAlphaBlending"), FunctionTemplate::New(gd_imageAlphaBlending));
    gd->Set(String::New("imageSaveAlpha"), FunctionTemplate::New(gd_imageSaveAlpha));
    gd->Set(String::New("imageSetClip"), FunctionTemplate::New(gd_imageSetClip));
    gd->Set(String::New("imageGetClip"), FunctionTemplate::New(gd_imageGetClip));
    gd->Set(String::New("imageAlpha"), FunctionTemplate::New(gd_imageAlpha));
    gd->Set(String::New("imageGetPixel"), FunctionTemplate::New(gd_imageGetPixel));
    gd->Set(String::New("imageBoundsSafe"), FunctionTemplate::New(gd_imageBoundsSafe));
    gd->Set(String::New("imageBlue"), FunctionTemplate::New(gd_imageBlue));
    gd->Set(String::New("imageGreen"), FunctionTemplate::New(gd_imageGreen));
    gd->Set(String::New("imageRed"), FunctionTemplate::New(gd_imageRed));
    gd->Set(String::New("imageSX"), FunctionTemplate::New(gd_imageSX));
    gd->Set(String::New("imageSY"), FunctionTemplate::New(gd_imageSY));
    gd->Set(String::New("imageChar"), FunctionTemplate::New(gd_imageChar));
    gd->Set(String::New("imageCharUp"), FunctionTemplate::New(gd_imageCharUp));
    gd->Set(String::New("imageString"), FunctionTemplate::New(gd_imageString));
    gd->Set(String::New("imageString16"), FunctionTemplate::New(gd_imageString16));
    gd->Set(String::New("imageStringUp"), FunctionTemplate::New(gd_imageStringUp));
    gd->Set(String::New("imageStringUp16"), FunctionTemplate::New(gd_imageStringUp16));
    gd->Set(String::New("useFontConfig"), FunctionTemplate::New(gd_FTUseFontConfig));

    gd->Set(String::New("imageColorAllocate"), FunctionTemplate::New(gd_imageColorAllocate));
    gd->Set(String::New("imageColorAllocateAlpha"), FunctionTemplate::New(gd_imageColorAllocateAlpha));
    gd->Set(String::New("imageColorClosest"), FunctionTemplate::New(gd_imageColorClosest));
    gd->Set(String::New("imageColorClosestAlpha"), FunctionTemplate::New(gd_imageColorClosestAlpha));
    gd->Set(String::New("imageColorClosestHWB"), FunctionTemplate::New(gd_imageColorClosestHWB));
    gd->Set(String::New("imageColorExact"), FunctionTemplate::New(gd_imageColorExact));
    gd->Set(String::New("imageColorResolve"), FunctionTemplate::New(gd_imageColorResolve));
    gd->Set(String::New("imageColorResolveAlpha"), FunctionTemplate::New(gd_imageColorResolveAlpha));
    gd->Set(String::New("imageColorsTotal"), FunctionTemplate::New(gd_imageColorsTotal));
    gd->Set(String::New("imageGetInterlaced"), FunctionTemplate::New(gd_imageGetInterlaced));
    gd->Set(String::New("imageGetTransparent"), FunctionTemplate::New(gd_imageGetTransparent));
    gd->Set(String::New("imageColorDeallocate"), FunctionTemplate::New(gd_imageColorDeallocate));
    gd->Set(String::New("imageColorTransparent"), FunctionTemplate::New(gd_imageColorTransparent));
    gd->Set(String::New("TrueColor"), FunctionTemplate::New(gd_TrueColor));
    gd->Set(String::New("TrueColorAlpha"), FunctionTemplate::New(gd_TrueColorAlpha));

    gd->Set(String::New("imageCopy"), FunctionTemplate::New(gd_imageCopy));
    gd->Set(String::New("imageCopyResized"), FunctionTemplate::New(gd_imageCopyResized));
    gd->Set(String::New("imageCopyResampled"), FunctionTemplate::New(gd_imageCopyResampled));
    gd->Set(String::New("imageCopyRotated"), FunctionTemplate::New(gd_imageCopyRotated));
    gd->Set(String::New("imageCopyMerge"), FunctionTemplate::New(gd_imageCopyMerge));
    gd->Set(String::New("imageCopyMergeGray"), FunctionTemplate::New(gd_imageCopyMergeGray));
    gd->Set(String::New("imagePaletteCopy"), FunctionTemplate::New(gd_imagePaletteCopy));
    gd->Set(String::New("imageSquareToCircle"), FunctionTemplate::New(gd_imageSquareToCircle));
    gd->Set(String::New("imageSharpen"), FunctionTemplate::New(gd_imageSharpen));
    gd->Set(String::New("imageCompare"), FunctionTemplate::New(gd_imageCompare));
    gd->Set(String::New("imageInterlace"), FunctionTemplate::New(gd_imageInterlace));

    builtinObject->Set(String::New("gd"), gd);
}
