/**
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

/**
 * @function gd.imageCreate
 * 
 * ### Synopsis
 * 
 * var handle = gd.imageCreate(sx, sy);
 * 
 * gd.imageCreate is called to create palette-based images, with no more than 256 colors. 
 * 
 * Invoke gdImageCreate with the x and y dimensions of the desired image. gdImageCreate returns an opaque handle to the new image, or NULL if unable to allocate the image. 
 * 
 * The image must eventually be destroyed using gd.imageDestroy().
 * 
 * @param {int} sx - width of new image
 * @param {int} sy - height of new image
 * @return {object} handle - opaque handle to image, or null if the image could not be created.
 */
static JSVAL gd_imageCreate (JSARGS args) {
    HandleScope scope;
    int sx = args[0]->IntegerValue();
    int sy = args[1]->IntegerValue();
    gdImagePtr im = gdImageCreate(sx, sy);
    if (!im) {
        return Null();
    }
    return scope.Close(External::New(im));
}

/**
 * @function gd.imageCreateTrueColor
 * 
 * ### Synopsis
 * 
 * var handle = gd.imageCreateTrueColor(sx, sy);
 * 
 * gd.imageCreateTrueColor is called to create truecolor images, with an essentially unlimited number of colors. Invoke gd.imageCreateTrueColor with the x and y dimensions of the desired image. gdImageCreateTrueColor returns an opaque handle to the new image, or NULL if unable to allocate the image. 
 * 
 * The image must eventually be destroyed using gd.imageDestroy().
 * 
 * Truecolor images are always filled with black at creation time. There is no concept of a "background" color index.
 * 
 * @param {int} sx - width of new image
 * @param {int} sy - height of new image
 * @return {object} handle - opaque handle to image, or null if the image could not be created.
 */
static JSVAL gd_imageCreateTrueColor (JSARGS args) {
    HandleScope scope;
    int sx = args[0]->IntegerValue();
    int sy = args[1]->IntegerValue();
    gdImagePtr im = gdImageCreateTrueColor(sx, sy);
    if (!im) {
        return Null();
    }
    return scope.Close(External::New(im));
}

/**
 * @function gd.imageCreateFromJpeg
 * 
 * ### Synopsis
 * 
 * var handle = gd.imageCreateFromJpeg(filename);
 * 
 * Load a JPEG image from a file.
 * 
 * ### Description
 * 
 * This function examines the first 12 bytes of the file to assure it is truly a JPEG format file.
 * 
 * @param {string} filename - name of file to load.
 * @return {object} handle - opaque handle to image, or null if the image could not be created.
 */
static JSVAL gd_ImageCreateFromJpeg (JSARGS args) {
    HandleScope scope;
    String::Utf8Value data(args[0]);
    FILE *in = fopen(*data, "rb");
    if (!in) {
        return Null();
    }
    // test binary header of file to assure it's JPEG
    {
        unsigned char buf[12];
        ssize_t size;
        if ((size = fread((char *) buf, 1, 12, in)) != 12) {
//            printf("gd_ImageCreateFromJpeg: could not read 12 bytes %d\n", size);
            fclose(in);
            return Null();
        }
        if (buf[0] != 0xff || buf[1] != 0xd8 || buf[2] != 0xff || (strcasecmp((char *) &buf[6], "JFIF") && strcasecmp((char *) &buf[6], "EXIF"))) {
            //            printf("gd_ImageCreateFromJpeg: %s signature match failed\n", *data);
            fclose(in);
            return Null();
        }
        //        printf("%s Is a JPEG\n", *data);
        fseek(in, 0, SEEK_SET);
    }

    gdImagePtr im = gdImageCreateFromJpeg(in);
    fclose(in);
    if (!im) {
        return Null();
    }
    return scope.Close(External::New(im));
}

/**
 * @function gd.imageCreateFromJpeg64
 * 
 * ### Synopsis
 * 
 * var handle = gd.imageCreateFromJpeg64(s);
 * 
 * Create image from base64 encoded string.
 * 
 * @param {string} s - base64 encoded string, when decoded it is binary data of a JPEG image.
 * @return {object} handle - opaque handle to image, or null if the image could not be created.
 */
static JSVAL gd_imageCreateFromJpeg64 (JSARGS args) {
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

/**
 * @function gd.imageCreateFromPng
 * 
 * ### Synopsis
 * 
 * var handle = gd.imageCreateFromPng(filename);
 * 
 * Load a PNG image from a file.
 * 
 * @param {string} filename - name of file to load.
 * @return {object} handle - opaque handle to image, or null if the image could not be created.
 */
static JSVAL gd_imageCreateFromPng (JSARGS args) {
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
 * @function gd.imageCreateFromPng64
 * 
 * ### Synopsis
 * 
 * var handle = gd.imageCreateFromPng64(s);
 * 
 * Create image from base64 encoded string.
 * 
 * @param {string} s - base64 encoded string, when decoded it is binary data of a PNG image.
 * @return {object} handle - opaque handle to image, or null if the image could not be created.
 */
static JSVAL gd_imageCreateFromPng64 (JSARGS args) {
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

/**
 * @function gd.imageCreateFromGif
 * 
 * ### Synopsis
 * 
 * var handle = gd.imageCreateFromGif(filename);
 * 
 * Load a GIF image from a file.
 * 
 * @param {string} filename - name of file to load.
 * @return {object} handle - opaque handle to image, or null if the image could not be created.
 */
static JSVAL gd_imageCreateFromGif (JSARGS args) {
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
 * @function gd.imageCreateFromGif64
 * 
 * ### Synopsis
 * 
 * var handle = gd.imageCreateFromGif64(s);
 * 
 * Create image from base64 encoded string.
 * 
 * @param {string} s - base64 encoded string, when decoded it is binary data of a GIF image.
 * @return {object} handle - opaque handle to image, or null if the image could not be created.
 */
static JSVAL gd_imageCreateFromGif64 (JSARGS args) {
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

/**
 * @function gd.imageCreateFromGd
 * 
 * ### Synopsis
 * 
 * var handle = gd.imageCreateFromGd(filename);
 * 
 * Load a GD image from a file.
 * 
 * @param {string} filename - name of file to load.
 * @return {object} handle - opaque handle to image, or null if the image could not be created.
 */
static JSVAL gd_imageCreateFromGd (JSARGS args) {
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
 * @function gd.imageCreateFromGd64
 * 
 * ### Synopsis
 * 
 * var handle = gd.imageCreateFromGd64(s);
 * 
 * Create image from base64 encoded string.
 * 
 * @param {string} s - base64 encoded string, when decoded it is binary data of a GD image.
 * @return {object} handle - opaque handle to image, or null if the image could not be created.
 */
static JSVAL gd_imageCreateFromGd64 (JSARGS args) {
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

/**
 * @function gd.imageCreateFromWBMP
 * 
 * ### Synopsis
 * 
 * var handle = gd.imageCreateFromWBMP(filename);
 * 
 * Load a WBMP image from a file.
 * 
 * @param {string} filename - name of file to load.
 * @return {object} handle - opaque handle to image, or null if the image could not be created.
 */
static JSVAL gd_imageCreateFromWBMP (JSARGS args) {
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
 * @function gd.imageCreateFromWBMP64
 * 
 * ### Synopsis
 * 
 * var handle = gd.imageCreateFromWBMP64(s);
 * 
 * Create image from base64 encoded string.
 * 
 * @param {string} s - base64 encoded string, when decoded it is binary data of a WBMP image.
 * @return {object} handle - opaque handle to image, or null if the image could not be created.
 */
static JSVAL gd_imageCreateFromWBMP64 (JSARGS args) {
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

/**
 * @function gd.imageCreateFromXpm
 * 
 * ### Synopsis
 * 
 * var handle = gd.imageCreateFromXpm(filename);
 * 
 * Load a XPM image from a file.
 * 
 * @param {string} filename - name of file to load.
 * @return {object} handle - opaque handle to image, or null if the image could not be created.
 */
static JSVAL gd_imageCreateFromXpm (JSARGS args) {
    HandleScope scope;
    String::Utf8Value data(args[0]);
    gdImagePtr im = gdImageCreateFromXpm(*data);
    return scope.Close(External::New(im));
}

/**
 * @function gd.imageCreateFromXbm
 * 
 * ### Synopsis
 * 
 * var handle = gd.imageCreateFromXbm(filename);
 * 
 * Load a Xbm image from a file.
 * 
 * @param {string} filename - name of file to load.
 * @return {object} handle - opaque handle to image, or null if the image could not be created.
 */
static JSVAL gd_imageCreateFromXbm (JSARGS args) {
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

/**
 * @function gd.imageDestroy
 * 
 * ### Synopsis
 * 
 * gd.imageDestroy(handle);
 * 
 * gd.imageDestroy is used to free the memory associated with an image. 
 * 
 * It is important to invoke gd.imageDestroy before exiting your program or allowing the handle handle to be garbage collected.
 * 
 * @param {object} handle - opaque handle to a GD image.
 */
static JSVAL gd_imageDestroy (JSARGS args) {
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
 * @param {object} handle - opaque handle to a GD image.
 * @param {string} filename - file to write
 * @param {int} quality - see note above
 * @return {boolean} success - true if the file was written, false if there was an error.
 */
static JSVAL gd_imageJpeg (JSARGS args) {
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

/**
 * @function gd.imageJpeg64
 * 
 * ### Synopsis
 * 
 * var base64String = gd.imageJpeg64(handle, quality);
 * 
 * Returns image as a base64 encoded string.
 * 
 * If quality is negative, the default IJG JPEG quality value (which should yield a good general quality / size tradeoff for most situations) is used. Otherwise, for practical purposes, quality should be a value in the range 0-95, higher quality values usually implying both higher quality and larger image sizes.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} quality - see note above
 * @return {string} base64String - image as a base64 encoded string.
 */
static JSVAL gd_imageJpeg64 (JSARGS args) {
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
 * @param {object} handle - opaque handle to a GD image.
 * @param {string} filename - file to write.
 * @return {boolean} success - true if the file was written, false if there was an error..
 */
static JSVAL gd_imageGif (JSARGS args) {
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

/**
 * @function gd.imageGif64
 * 
 * ### Synopsis
 * 
 * var base64String = gd.imageGif64(handle);
 * 
 * Returns image as a base64 encoded string.
 * 
 * GIF does not support true color; GIF images can contain a maximum of 256 colors. If the image to be written is a truecolor image, such as those created with gd.imageCreateTrueColor() or loaded from a JPEG or a truecolor PNG image file, a palette-based temporary image will automatically be created internally using the gd.imageCreatePaletteFromTrueColor() function. The original image pixels are not modified. This conversion produces high quality palettes but does require some CPU time. If you are regularly converting truecolor to palette in this way, you should consider creating your image as a palette-based image in the first place.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @return {string} base64String - image as a base64 encoded string.
 */
static JSVAL gd_imageGif64 (JSARGS args) {
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
 * @param {object} handle - opaque handle to a GD image.
 * @param {string} filename - file to write.
 * @return {boolean} success - true if the file was written, false if there was an error..
 */
static JSVAL gd_imagePng (JSARGS args) {
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

/**
 * @function gd.imagePng64
 * 
 * ### Synopsis
 * 
 * var base64String = gd.imagePng64(handle);
 * 
 * Returns image as a base64 encoded string.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @return {string} base64String - image as a base64 encoded string.
 */
static JSVAL gd_imagePng64 (JSARGS args) {
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
 * This function allows the level of compression to be specified. 
 * 
 * A compression level of 0 means "no compression." A compression level of 1 means "compressed, but as quickly as possible." 
 * 
 * A compression level of 9 means "compressed as much as possible to produce the smallest possible file." 
 * 
 * A compression level of -1 will use the default compression level at the time zlib was compiled on your system.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {string} filename - file to write.
 * @param {int} level - compression level, see notes above.
 * @return {boolean} success - true if the file was written, false if there was an error.
 */
static JSVAL gd_imagePngEx (JSARGS args) {
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

/**
 * @function gd.imagePng64Ex
 * 
 * ### Synopsis
 * 
 * var base64String = gd.imagePng64Ex(handle, level);
 * 
 * Returns image as a base64 encoded string.
 * 
 * This function allows the level of compression to be specified. 
 * 
 * A compression level of 0 means "no compression." A compression level of 1 means "compressed, but as quickly as possible." 
 * 
 * A compression level of 9 means "compressed as much as possible to produce the smallest possible file." 
 * 
 * A compression level of -1 will use the default compression level at the time zlib was compiled on your system.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} level - compression level, see notes above.
 * @return {string} base64String - image as a base64 encoded string.
 */
static JSVAL gd_imagePng64Ex (JSARGS args) {
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

/**
 * @function gd.imageWBMP
 * 
 * ### Synopsis
 * 
 * var success = gd.imageWBMP(handle, filename, fg);
 * 
 * Outputs the specified image to the specified file in WBMP format.
 * 
 * _WBMP file support is black and white only._
 * 
 * The color index specified by the fg argument is the "foreground," and only pixels of this color will be set in the WBMP file. All other pixels will be considered "background."
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {string} filename - file to write.
 * @param {int} fg - foreground color index.
 * @return {boolean} success - true if the file was written, false if there was an error..
 */
static JSVAL gd_imageWBMP (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    String::Utf8Value data(args[1]);
    int fg = args[2]->IntegerValue();

    FILE *out = fopen(*data, "wb");
    if (!out) {
        return False();
    }
    gdImageWBMP(im, fg, out);
    fclose(out);
    return True();
}

/**
 * @function gd.imageWBMP64
 * 
 * ### Synopsis
 * 
 * var base64String = gd.imageWBMP64(handle, fg);
 * 
 * Returns image as a base64 encoded string.
 * 
 * _WBMP file support is black and white only._
 * 
 * The color index specified by the fg argument is the "foreground," and only pixels of this color will be set in the WBMP file. All other pixels will be considered "background."
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} fg - foreground color index.
 * @return {string} base64String - image as a base64 encoded string.
 */
static JSVAL gd_imageWBMP64 (JSARGS args) {
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

/**
 * @function gd.imageGd
 * 
 * ### Synopsis
 * 
 * var success = gd.imageGd(handle, filename);
 * 
 * Outputs the specified image to the specified file in GD format.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {string} filename - file to write.
 * @return {boolean} success - true if the file was written, false if there was an error..
 */
static JSVAL gd_imageGd (JSARGS args) {
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

/**
 * @function gd.imageGD64
 * 
 * ### Synopsis
 * 
 * var base64String = gd.imageGd64(handle);
 * 
 * Returns image as a base64 encoded string.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @return {string} base64String - image as a base64 encoded string.
 */
static JSVAL gd_imageGd64 (JSARGS args) {
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

/**
 * @function gd.imageTrueColorToPalette
 * 
 * ### Synopsis
 * 
 * gd.imageTrueColorToPalette(handle, ditherFlag, colorsWanted);
 * 
 * The function converts a truecolor image to a palette-based image, using a high-quality two-pass quantization routine. 
 * 
 * If ditherFlag is set, the image will be dithered to approximate colors better, at the expense of some obvious "speckling." 
 * 
 * colorsWanted can be anything up to 256. 
 * 
 * If the original source image includes photographic information or anything that came out of a JPEG, 256 is strongly recommended. 100% transparency of a single transparent color in the original truecolor image will be preserved. There is no other support for preservation of alpha channel or transparency in the destination image.
 * 
 * For best results, don't use this function -- write real truecolor PNGs and JPEGs. The disk space gain of conversion to palette is not great (for small images it can be negative) and the quality loss is ugly. 
 * 
 * However, the version of this function included in version 2.0.12 and later does do a better job than the version included prior to 2.0.12.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {boolean} ditherFlag - see note above.
 * @param {int} colorsWanted - see note above.
 */
static JSVAL gd_imageTrueColorToPalette (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int ditherFlag = args[1]->IntegerValue();
    int colorsWanted = args[2]->IntegerValue();
    gdImageTrueColorToPalette(im, ditherFlag, colorsWanted);
    return Undefined();
}

/**
 * @function gd.imageCreatePaletteFromTrueColor
 * 
 * ### Synopsis
 * 
 * var newHandle = gd.imageTrueColorToPalette(handle, ditherFlag, colorsWanted);
 * 
 * The function converts a truecolor image to a palette-based image, using a high-quality two-pass quantization routine. This is identical to gd.imageTrueColorToPallete(), except this version returns a handle to a new image, and the old image is unaltered.
 * 
 * If ditherFlag is set, the image will be dithered to approximate colors better, at the expense of some obvious "speckling." 
 * 
 * colorsWanted can be anything up to 256. 
 * 
 * If the original source image includes photographic information or anything that came out of a JPEG, 256 is strongly recommended. 100% transparency of a single transparent color in the original truecolor image will be preserved. There is no other support for preservation of alpha channel or transparency in the destination image.
 * 
 * For best results, don't use this function -- write real truecolor PNGs and JPEGs. The disk space gain of conversion to palette is not great (for small images it can be negative) and the quality loss is ugly. 
 * 
 * However, the version of this function included in version 2.0.12 and later does do a better job than the version included prior to 2.0.12.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {boolean} ditherFlag - see note above.
 * @param {int} colorsWanted - see note above.
 * @returns {object} newHandle - handle to a new palettized image.
 */
static JSVAL gd_imageCreatePaletteFromTrueColor (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int ditherFlag = args[1]->IntegerValue();
    int colorsWanted = args[2]->IntegerValue();
    gdImagePtr newImage = gdImageCreatePaletteFromTrueColor(im, ditherFlag, colorsWanted);
    return scope.Close(External::New(newImage));
}

/**
 * @function gd.imageSetPixel
 * 
 * ### Synopsis
 * 
 * gd.imageSetPixel(x,y, color);
 * 
 * Sets a pixel to a particular color index.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} x - x coordinate of pixel
 * @param {int} y - y coordinate of pixel
 * @param {int} color - color index to set pixel
 */
static JSVAL gd_imageSetPixel (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int x = args[1]->IntegerValue();
    int y = args[2]->IntegerValue();
    int color = args[3]->IntegerValue();
    gdImageSetPixel(im, x, y, color);
    return Undefined();
}

/**
 * @function gd.imageLine
 * 
 * ### Synopsis
 * 
 * gd.imageLine(x1,y1, x2,y2, color);
 * 
 * Draws a line between two endpoints (x1,y1) and (x2,y2).  The line is drawn using the color index specified.
 * 
 * Note that the color index can be an actual color returned by gd.imageColorAllocate() or one of gd.Styled, gd.Brushed or gd.StyledBrushed.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} x1 - x coordinate of first endpoint
 * @param {int} y1 - y coordinate of first endpoint
 * @param {int} x2 - x coordinate of second endpoint
 * @param {int} y2 - y coordinate of second endpoint
 * @param {int} color - color index to draw line.
 */
static JSVAL gd_imageLine (JSARGS args) {
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
 * @function gd.imagePolygon
 * 
 * gd.imagePolygon(image, points, color);
 * 
 * Draw a _closed_ polygon with the given vertices (at least 3) using the specified color index.
 * 
 * This function will draw the last line to close the polygon, if required.
 * 
 * points is something like:
 * ```
 * [
 *		{ x: xVal, y: yVal },
 *		{ x: xVal, y: yVal },
 *		{ x: xVal, y: yVal },
 *		...
 * } 
 * ```
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {array} points - see note above.
 * @param {int} color - color index to draw polygon.
 */
static JSVAL gd_imagePolygon (JSARGS args) {
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
 * @function gd.imageOpenPolygon
 * 
 * gd.imageOpenPolygon(image, points, color);
 * 
 * Draw a sequence of lines with the vertices (at least 3) specified, using the color index specified. 
 * 
 * Unlike gdImagePolygon, the enpoints of the line sequence are not connected to a closed polygon.
 * 
 * points is something like:
 * ```
 * [
 *		{ x: xVal, y: yVal },
 *		{ x: xVal, y: yVal },
 *		{ x: xVal, y: yVal },
 *		...
 * } 
 * ```
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {array} points - see note above.
 * @param {int} color - color index to draw polygon.
 */
static JSVAL gd_imageOpenPolygon (JSARGS args) {
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

/**
 * @function gd.imageRectangle
 * 
 * ### Synopsis
 * 
 * gd.imageRectangle(x1,y1, x2,y2, color);
 * 
 * Draws a rectangle given the upper left (x1,y1) and lower right (x2,y2) coordinates.  The rectangle is drawn using the color index specified.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} x1 - x coordinate of upper left corner.
 * @param {int} y1 - y coordinate of upper left corner.
 * @param {int} x2 - x coordinate of lower right corner.
 * @param {int} y2 - y coordinate of lower right corner.
 * @param {int} color - color index to draw rectangle.
 */
static JSVAL gd_imageRectangle (JSARGS args) {
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
 * @function gd.imageFilledPolygon
 * 
 * gd.imageFilledPolygon(image, points, color);
 * 
 * Draw a _filled_ polygon with the given vertices (at least 3) using the specified color index.
 * 
 * This function will draw the last line to close the polygon, if required.
 * 
 * points is something like:
 * ```
 * [
 *		{ x: xVal, y: yVal },
 *		{ x: xVal, y: yVal },
 *		{ x: xVal, y: yVal },
 *		...
 * } 
 * ```
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {array} points - see note above.
 * @param {int} color - color index to fill the polygon.
 */
static JSVAL gd_imageFilledPolygon (JSARGS args) {
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

/**
 * @function gd.imageFilledRectangle
 * 
 * ### Synopsis
 * 
 * gd.imageFilledRectangle(x1,y1, x2,y2, color);
 * 
 * Draws a filled rectangle given the upper left (x1,y1) and lower right (x2,y2) coordinates.  The rectangle is filled using the color index specified.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} x1 - x coordinate of upper left corner.
 * @param {int} y1 - y coordinate of upper left corner.
 * @param {int} x2 - x coordinate of lower right corner.
 * @param {int} y2 - y coordinate of lower right corner.
 * @param {int} color - color index to fill rectangle.
 */
static JSVAL gd_imageFilledRectangle (JSARGS args) {
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

/**
 * @function gd.imageArc
 * 
 * ### Synopsis
 * 
 * gd.imageArc(handle, cx, cy, w, h, s, e, color);
 * 
 * Draw a partial ellipse centered at the given point, with the specified width and height in pixels. 
 * 
 * The arc begins at the position in degrees specified by s and ends at the position specified by e. 
 * 
 * The arc is drawn in the color specified by the last argument. A circle can be drawn by beginning from 0 degrees and ending at 360 degrees, with width and height being equal. 
 * 
 * e must be greater than s. 
 * 
 * Values greater than 360 are interpreted modulo 360.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} cx - x coordinate of center of arc.
 * @param {int} cy - y coordinate of center of arc.
 * @param {int} w - width of arc in pixels.
 * @param {int} h - height of arc in pixels.
 * @param {int} s - start position in degrees.
 * @param {int} e - end position in degrees.
 * @param {int} color - color index to render arc.
 */
static JSVAL gd_imageArc (JSARGS args) {
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

/**
 * @function gd.imageFilledArc
 * 
 * ### Synopsis
 * 
 * gd.imageFilledArc(handle, cx, cy, w, h, s, e, color, style);
 * 
 * Draw a partial ellipse centered at the given point, with the specified width and height in pixels. 
 * 
 * The arc begins at the position in degrees specified by s and ends at the position specified by e. 
 * 
 * The arc is filled in the color specified by the color argument. 
 * 
 * A circle can be drawn by beginning from 0 degrees and ending at 360 degrees, with width and height being equal. e must be greater than s. 
 * 
 * Values greater than 360 are interpreted modulo 360. 
 * 
 * The style argument is a bitwise OR of the following possibilities:
 * 
 * + gd.Arc
 * + gd.Chord
 * + gd.Pie (synonym for gd.Chord)
 * + gd.NoFill
 * + gd.Edged
 * 
 * gd.Arc and gd.Chord are mutually exclusive; gd.Chord just connects the starting and ending angles with a straight line, while gd.Arc produces a rounded edge. 
 * 
 * gd.Pie is a synonym for gd.Arc. 
 * 
 * gd.NoFill indicates that the arc or chord should be outlined, not filled. 
 * 
 * gd.Edged, used together with gd.NoFill, indicates that the beginning and ending angles should be connected to the center; this is a good way to outline (rather than fill) a 'pie slice'.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} cx - x coordinate of center of arc.
 * @param {int} cy - y coordinate of center of arc.
 * @param {int} w - width of arc in pixels.
 * @param {int} h - height of arc in pixels.
 * @param {int} s - start position in degrees.
 * @param {int} e - end position in degrees.
 * @param {int} color - color index to render arc.
 * @param {int} style - color index to fill arc.
 */
static JSVAL gd_imageFilledArc (JSARGS args) {
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

/**
 * @function gd.imageFilledEllipse
 * 
 * ### Synopsis
 * 
 * gd.imageFilledEllipse(handle, cx, cy, w, h, color);
 * 
 * Draw an ellipse centered at the given point, with the specified width and height in pixels. The ellipse is filled in the color specified by the color argument.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} cx - x coordinate of center of arc.
 * @param {int} cy - y coordinate of center of arc.
 * @param {int} w - width of arc in pixels.
 * @param {int} h - height of arc in pixels.
 * @param {int} color - color index to render arc.
 */
static JSVAL gd_imageFilledEllipse (JSARGS args) {
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

/**
 * @function gd.imageFillToBorder
 * 
 * ### Synopsis
 * 
 * gd.imageFillToBorder(handle, x, y, border, color);
 * 
 * Floods a portion of the image with the specified color, beginning at the specified point and stopping at the specified border color. 
 * 
 * For a way of flooding an area defined by the color of the starting point, see gd.ImageFill().
 * 
 * The border color cannot be a special color such as gdTiled; it must be a proper solid color. The fill color can be, however.
 * 
 * Note that gdImageFillToBorder is recursive. It is not the most naive implementation possible, and the implementation is expected to improve, but there will always be degenerate cases in which the stack can become very deep.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} x - x coordinate to start fill.
 * @param {int} y - y coordinate to start fill.
 * @param {int} border - color index of border.
 * @param {int} color - color index to fill with.
 */
static JSVAL gd_imageFillToBorder (JSARGS args) {
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

/**
 * @function gd.imageFill
 * 
 * ### Synopsis
 * 
 * gd.imageFill(handle, x, y, color);
 * 
 * Floods a portion of the image with the specified color, beginning at the specified point and flooding the surrounding region of the same color as the starting point. 
 * 
 * For a way of flooding a region defined by a specific border color rather than by its interior color, see gd.ImageFillToBorder().
 * 
 * The fill color can be gd.Tiled, resulting in a tile fill using another image as the tile. However, the tile image cannot be transparent. If the image you wish to fill with has a transparent color index, call gd.ImageTransparent() on the tile image and set the transparent color index to -1 to turn off its transparency.
 * 
 * Note that gdImageFill is recursive. It is not the most naive implementation possible, and the implementation is expected to improve, but there will always be degenerate cases in which the stack can become very deep.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} x - x coordinate to start fill.
 * @param {int} y - y coordinate to start fill.
 * @param {int} color - color index to fill with.
 */
static JSVAL gd_imageFill (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int x = args[1]->IntegerValue();
    int y = args[2]->IntegerValue();
    int color = args[3]->IntegerValue();
    gdImageFill(im, x, y, color);
    return Undefined();
}

/**
 * @function gd.imageSetAntiAliased
 * 
 * ### Synopsis
 * 
 * gd.imageSetAntiAliased(handle, c);
 * 
 * This function is used to specify the actual foreground color to be used when drawing antialiased lines. You may set any color to be the foreground, however as of GD version 2.0.12 an alpha channel component is not supported.
 * 
 * "Antialiasing" is a process by which jagged edges associated with line drawing can be reduced by blending the foreground color with an appropriate percentage of the background, depending on how much of the pixel in question is actually within the boundaries of the line being drawn. 
 * 
 * All line-drawing functions, such as gd.imageLine(), gd.imageOpenPolygon() and gd.imagePolygon(), will draw antialiased lines if the special "color" gd.AntiAliased is used when calling them.
 * 
 * Antialiased lines can be drawn on both truecolor and palette-based images. However, attempts to draw antialiased lines on highly complex palette-based backgrounds may not give satisfactory results, due to the limited number of colors available in the palette. Antialiased line-drawing on simple backgrounds should work well with palette-based images; otherwise create or fetch a truecolor image instead.
 * 
 * You need not take any special action when you are finished with antialised line drawing.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} color - Actual color of the foreground to be used when drawing antialised lines.
 */
static JSVAL gd_imageSetAntiAliased (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int c = args[1]->IntegerValue();
    gdImageSetAntiAliased(im, c);
    return Undefined();
}

/**
 * @function gd.imageSetAntiAliasedDontBlend
 * 
 * ### Synopsis
 * 
 * gd.imageSetAntiAliasedDontBlend(handle, c);
 * 
 * Normally, when drawing lines with the special gd.AntiAliased "color," blending with the background to reduce jagged edges is the desired behavior. However, when it is desired that lines not be blended with one particular color when it is encountered in the background, the gd.ImageSetAntiAliasedDontBlend() function can be used to indicate the special color that the foreground should stand out more clearly against.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} color - special color that the foreground should stand out mroe clearly against.
 */
static JSVAL gd_imageSetAntiAliasedDontBlend (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int c = args[1]->IntegerValue();
    gdImageSetAntiAliased(im, c);
    return Undefined();
}

/**
 * @function gd.imageSetBrush
 * 
 * ### Synopsis
 * 
 * gd.imageSetBrush(handle, brushHandle);
 * 
 * gd.imageSetBrush() is used to specify the brush to be used in a particular image. 
 * 
 * A "brush" is an image used to draw wide, shaped strokes in another image. Just as a paintbrush is not a single point, a brush image need not be a single pixel. 
 * 
 * Any gd image can be used as a brush, and by setting the transparent color index of the brush image with gd.imageColorTransparent(), a brush of any shape can be created. 
 * 
 * All line-drawing functions, such as gd.imageLine(), gd.imageOpenPolygon() and gd.imagePolygon(), will use the current brush if the special "color" gd.Brushed or gd.StyledBrushed is used when calling them.
 * 
 * You can set any image to be the brush. If the brush image does not have the same color map as the first image, any colors missing from the first image will be allocated. If not enough colors can be allocated, the closest colors already available will be used. This allows arbitrary PNGs to be used as brush images. It also means, however, that you should not set a brush unless you will actually use it; if you set a rapid succession of different brush images, you can quickly fill your color map, and the results will not be optimal.
 * 
 * You need not take any special action when you are finished with a brush. As for any other image, if you will not be using the brush image for any further purpose, you should call gd.imageDestroy(). 
 * 
 * You must not use the color gd.Brushed if the current brush has been destroyed; you can of course set a new brush to replace it.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {object} brushHandle - opaque handle to a GD image to be used as the brush.
 */
static JSVAL gd_imageSetBrush (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdImagePtr brush = (gdImagePtr) wrap->Value();
    gdImageSetBrush(im, brush);
    return Undefined();
}

/**
 * @function gd.imageSetTile
 * 
 * gd.imageSetTile(handle, tileHandle);
 * 
 * gd.imageSetTile() is used to specify the tile to be used in a particular image.
 * 
 * A "tile" is an image used to fill an area with a repeated pattern. Any gd image can be used as a tile, and by setting the transparent color index of the tile image with gd.imageColorTransparent(), a tile that allows certain parts of the underlying area to shine through can be created. 
 * 
 * All region-filling functions, such as gd.imageFill() and gd.imageFilledPolygon(), will use the current tile if the special "color" gd.Tiled is used when calling them.
 * 
 * You can set any image to be the tile. If the tile image does not have the same color map as the first image, any colors missing from the first image will be allocated. If not enough colors can be allocated, the closest colors already available will be used. This allows arbitrary PNGs to be used as tile images. It also means, however, that you should not set a tile unless you will actually use it; if you set a rapid succession of different tile images, you can quickly fill your color map, and the results will not be optimal.
 * 
 * You need not take any special action when you are finished with a tile. As for any other image, if you will not be using the tile image for any further purpose, you should call gd.imageDestroy(). You must not use the color gd.Tiled if the current tile has been destroyed; you can of course set a new tile to replace it.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {object} tileHandle - opaque handle to a GD image to be used as the tile.
 */
static JSVAL gd_imageSetTile (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdImagePtr brush = (gdImagePtr) wrap->Value();
    gdImageSetTile(im, brush);
    return Undefined();
}

/**
 * @function gd.imageSetStyle
 * 
 * ### Synopsis
 * 
 * gd.imageSetStyle(handle, style);
 * 
 * It is often desirable to draw dashed lines, dotted lines, and other variations on a broken line. gd.imageSetStyle() can be used to set any desired series of colors, including a special color that leaves the background intact, to be repeated during the drawing of a line.
 * 
 * To use gd.imageSetStyle(), create an array of integers and assign them the desired series of color values to be repeated. You can assign the special color value gd.Transparent to indicate that the existing color should be left unchanged for that particular pixel (allowing a dashed line to be attractively drawn over an existing image).
 * 
 * Then, to draw a line using the style, use the normal gd.imageLine() function with the special color value gd.Styled.
 * 
 * You can also combine styles and brushes to draw the brush image at intervals instead of in a continuous stroke. When creating a style for use with a brush, the style values are interpreted differently: zero (0) indicates pixels at which the brush should not be drawn, while one (1) indicates pixels at which the brush should be drawn. To draw a styled, brushed line, you must use the special color value gd.StyledBrushed.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {array} style - array of integers as described above.
 */
static JSVAL gd_imageSetStyle (JSARGS args) {
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

/**
 * @function gd.imageSetThickness
 * 
 * ### Synopsis
 * 
 * gd.imageSetThickness(handle, thickness);
 * 
 * Set the width of lines drawn by the gd.imageLine(), gd.imagePolygon() and related functions, in pixels.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} thickness - thickness of lines to be drawn, in pixels.
 */
static JSVAL gd_imageSetThickness (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int thickness = args[1]->IntegerValue();
    gdImageSetThickness(im, thickness);
    return Undefined();
}

/**
 * @function gd.imageAlphaBlending
 * 
 * ### Synopsis
 * 
 * gd.imageAlphaBlending(handle, mode);
 * 
 * The gd.imageAlphaBlending() function allows for two different modes of drawing on truecolor images. 
 * 
 * In blending mode, which is on by default (gd 2.0.2 and above), the alpha channel component of the color supplied to all drawing functions, such as gd.imageSetPixel(), determines how much of the underlying color should be allowed to shine through. As a result, gd automatically blends the existing color at that point with the drawing color, and stores the result in the image. The resulting pixel is opaque. 
 * 
 * In non-blending mode, the drawing color is copied literally with its alpha channel information, replacing the destination pixel. 
 * 
 * _Blending mode is not available when drawing on palette images._
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} mode - 1 to enable blending mode, 0 to enable non-blending mode..
 */
static JSVAL gd_imageAlphaBlending (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int blending = args[1]->IntegerValue();
    gdImageAlphaBlending(im, blending);
    return Undefined();
}

/**
 * @function gd.imageSaveAlpha
 * 
 * ### Synopsis
 * 
 * gd.imageSaveAlpha(handle, saveFlag);
 * 
 * By default, gd 2.0.2 and above do not attempt to save full alpha channel information (as opposed to single-color transparency) when saving PNG images. (PNG is currently the only output format supported by gd which can accommodate alpa channel information.) 
 * 
 * This saves space in the output file. 
 * 
 * If you wish to create an image with alpha channel information for use with tools that support it, call gd.imageSaveAlpha(handle, 1) to turn on saving of such information, and call gd.imageAlphaBlending(handle, 0) to turn off alpha blending within the library so that alpha channel information is actually stored in the image rather than being composited immediately at the time that drawing functions are invoked.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param args
 * @return 
 */
static JSVAL gd_imageSaveAlpha (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int saveFlag = args[1]->IntegerValue();
    gdImageSaveAlpha(im, saveFlag);
    return Undefined();
}

static JSVAL gd_imageSetClip (JSARGS args) {
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

static JSVAL gd_imageGetClip (JSARGS args) {
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

static JSVAL gd_imageAlpha (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int color = args[1]->IntegerValue();
    int alpha = gdImageAlpha(im, color);
    return scope.Close(Integer::New(alpha));
}

static JSVAL gd_imageGetPixel (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int x = args[1]->IntegerValue();
    int y = args[2]->IntegerValue();
    int color = gdImageGetPixel(im, x, y);
    return scope.Close(Integer::New(color));
}

static JSVAL gd_imageBoundsSafe (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int x = args[1]->IntegerValue();
    int y = args[2]->IntegerValue();
    return gdImageBoundsSafe(im, x, y) ? True() : False();
}

static JSVAL gd_imageBlue (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int color = args[1]->IntegerValue();
    int blue = gdImageBlue(im, color);
    return scope.Close(Integer::New(blue));
}

static JSVAL gd_imageGreen (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int color = args[1]->IntegerValue();
    int green = gdImageGreen(im, color);
    return scope.Close(Integer::New(green));
}

static JSVAL gd_imageRed (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int color = args[1]->IntegerValue();
    int red = gdImageRed(im, color);
    return scope.Close(Integer::New(red));
}

static JSVAL gd_imageSX (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    return scope.Close(Integer::New(gdImageSX(im)));
}

static JSVAL gd_imageSY (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    return scope.Close(Integer::New(gdImageSY(im)));
}

// Fonts and text-handling functions

static JSVAL gd_fontGetSmall (JSARGS args) {
    HandleScope scope;
    return scope.Close(External::New(gdFontGetSmall()));
}

static JSVAL gd_fontGetLarge (JSARGS args) {
    HandleScope scope;
    return scope.Close(External::New(gdFontGetLarge()));
}

static JSVAL gd_fontGetMediumBold (JSARGS args) {
    HandleScope scope;
    return scope.Close(External::New(gdFontGetMediumBold()));
}

static JSVAL gd_fontGetGiant (JSARGS args) {
    HandleScope scope;
    return scope.Close(External::New(gdFontGetGiant()));
}

static JSVAL gd_fontGetTiny (JSARGS args) {
    HandleScope scope;
    return scope.Close(External::New(gdFontGetTiny()));
}

static JSVAL gd_imageChar (JSARGS args) {
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

static JSVAL gd_imageCharUp (JSARGS args) {
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

static JSVAL gd_imageString (JSARGS args) {
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

static JSVAL gd_imageString16 (JSARGS args) {
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

static JSVAL gd_imageStringUp (JSARGS args) {
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

static JSVAL gd_imageStringUp16 (JSARGS args) {
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

static JSVAL gd_FTUseFontConfig (JSARGS args) {
    HandleScope scope;
    int flag = args[0]->IntegerValue();
    return scope.Close(Integer::New(gdFTUseFontConfig(flag)));
}



// TODO: finish font/text functions

// Color handling functions

static JSVAL gd_imageColorAllocate (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    return scope.Close(Integer::New(gdImageColorAllocate(im, red, green, blue)));
}

static JSVAL gd_imageColorAllocateAlpha (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    int alpha = args[4]->IntegerValue();
    return scope.Close(Integer::New(gdImageColorAllocateAlpha(im, red, green, blue, alpha)));
}

static JSVAL gd_imageColorClosest (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    return scope.Close(Integer::New(gdImageColorClosest(im, red, green, blue)));
}

static JSVAL gd_imageColorClosestAlpha (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    int alpha = args[4]->IntegerValue();
    return scope.Close(Integer::New(gdImageColorClosestAlpha(im, red, green, blue, alpha)));
}

static JSVAL gd_imageColorClosestHWB (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    return scope.Close(Integer::New(gdImageColorClosestHWB(im, red, green, blue)));
}

static JSVAL gd_imageColorExact (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    return scope.Close(Integer::New(gdImageColorExact(im, red, green, blue)));
}

static JSVAL gd_imageColorResolve (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    return scope.Close(Integer::New(gdImageColorResolve(im, red, green, blue)));
}

static JSVAL gd_imageColorResolveAlpha (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    int alpha = args[4]->IntegerValue();
    return scope.Close(Integer::New(gdImageColorResolveAlpha(im, red, green, blue, alpha)));
}

static JSVAL gd_imageColorsTotal (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    return scope.Close(Integer::New(gdImageColorsTotal(im)));
}

static JSVAL gd_imageGetInterlaced (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    return scope.Close(Integer::New(gdImageGetInterlaced(im)));
}

static JSVAL gd_imageGetTransparent (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    return scope.Close(Integer::New(gdImageGetTransparent(im)));
}

static JSVAL gd_imageColorDeallocate (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int color = args[1]->IntegerValue();
    gdImageColorDeallocate(im, color);
    return Undefined();
}

static JSVAL gd_imageColorTransparent (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im = (gdImagePtr) wrap->Value();
    int color = args[1]->IntegerValue();
    gdImageColorTransparent(im, color);
    return Undefined();
}

static JSVAL gd_TrueColor (JSARGS args) {
    HandleScope scope;
    int red = args[0]->IntegerValue();
    int green = args[1]->IntegerValue();
    int blue = args[2]->IntegerValue();
    return Integer::New(gdTrueColor(red, green, blue));
}

static JSVAL gd_TrueColorAlpha (JSARGS args) {
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
static JSVAL gd_imageCopy (JSARGS args) {
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
static JSVAL gd_imageCopyResized (JSARGS args) {
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
static JSVAL gd_imageCopyResampled (JSARGS args) {
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

static JSVAL gd_imageCopyRotated (JSARGS args) {
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

static JSVAL gd_imageCopyMerge (JSARGS args) {
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

static JSVAL gd_imageCopyMergeGray (JSARGS args) {
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

static JSVAL gd_imagePaletteCopy (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr dst = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdImagePtr src = (gdImagePtr) wrap->Value();
    gdImagePaletteCopy(dst, src);
    return Undefined();
}

static JSVAL gd_imageSquareToCircle (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr dst = (gdImagePtr) wrap->Value();
    int radius = args[1]->IntegerValue();
    gdImageSquareToCircle(dst, radius);
    return Undefined();
}

static JSVAL gd_imageSharpen (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr dst = (gdImagePtr) wrap->Value();
    int pct = args[1]->IntegerValue();
    gdImageSharpen(dst, pct);
    return Undefined();
}

static JSVAL gd_imageCompare (JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    gdImagePtr im1 = (gdImagePtr) wrap->Value();
    wrap = Local<External>::Cast(args[1]);
    gdImagePtr im2 = (gdImagePtr) wrap->Value();
    return scope.Close(Integer::New(gdImageCompare(im1, im2)));
}

static JSVAL gd_imageInterlace (JSARGS args) {
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

void init_gd_object () {
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
    gd->Set(String::New("Arc"), Integer::New(gdArc));
    gd->Set(String::New("Chord"), Integer::New(gdChord));
    gd->Set(String::New("Pie"), Integer::New(gdPie));
    gd->Set(String::New("NoFill"), Integer::New(gdNoFill));
    gd->Set(String::New("Edged"), Integer::New(gdEdged));

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
    gd->Set(String::New("imageCreateFromXpm"), FunctionTemplate::New(gd_imageCreateFromXpm));
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
