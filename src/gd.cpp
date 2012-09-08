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
    int sx = args[0]->IntegerValue();
    int sy = args[1]->IntegerValue();
    gdImagePtr im = gdImageCreate(sx, sy);
    if (!im) {
        return Null();
    }
    return Opaque::New(im);
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
    int sx = args[0]->IntegerValue();
    int sy = args[1]->IntegerValue();
    gdImagePtr im = gdImageCreateTrueColor(sx, sy);
    if (!im) {
        return Null();
    }
    return Opaque::New(im);
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
    return Opaque::New(im);
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
    String::Utf8Value data(args[0]);
    unsigned char buf[data.length()];
    int decoded = decode_base64(buf, *data);
    gdImagePtr im = gdImageCreateFromJpegPtr(decoded, (void *) buf);
    //    string s = Base64Decode(*data);
    //    gdImagePtr im = gdImageCreateFromJpegPtr(s.size(), (void *) s.c_str());
    if (!im) {
        return Null();
    }
    return Opaque::New(im);
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
    return Opaque::New(im);
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
    String::Utf8Value data(args[0]);
    unsigned char buf[data.length()];
    int decoded = decode_base64(buf, *data);
    gdImagePtr im = gdImageCreateFromPngPtr(decoded, (void *) buf);

    //    string s = Base64Decode(*data);
    //    gdImagePtr im = gdImageCreateFromPngPtr(s.size(), (void *) s.c_str());
    if (!im) {
        return Null();
    }
    return Opaque::New(im);
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
    return Opaque::New(im);
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
    String::Utf8Value data(args[0]);

    unsigned char buf[data.length()];
    int decoded = decode_base64(buf, *data);
    gdImagePtr im = gdImageCreateFromGifPtr(decoded, (void *) buf);

    //    string s = Base64Decode(*data);
    //    gdImagePtr im = gdImageCreateFromGifPtr(s.size(), (void *) s.c_str());
    if (!im) {
        return Null();
    }
    return Opaque::New(im);
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
    return Opaque::New(im);
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
    String::Utf8Value data(args[0]);
    unsigned char buf[data.length()];
    int decoded = decode_base64(buf, *data);
    gdImagePtr im = gdImageCreateFromGdPtr(decoded, (void *) buf);
    //    string s = Base64Decode(*data);
    //    gdImagePtr im = gdImageCreateFromGdPtr(s.size(), (void *) s.c_str());
    if (!im) {
        return Null();
    }
    return Opaque::New(im);
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
    return Opaque::New(im);
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
    String::Utf8Value data(args[0]);
    unsigned char buf[data.length()];
    int decoded = decode_base64(buf, *data);
    gdImagePtr im = gdImageCreateFromWBMPPtr(decoded, (void *) buf);
    //    string s = Base64Decode(*data);
    //    gdImagePtr im = gdImageCreateFromWBMPPtr(s.size(), (void *) s.c_str());
    if (!im) {
        return Null();
    }
    return Opaque::New(im);
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
    String::Utf8Value data(args[0]);
    gdImagePtr im = gdImageCreateFromXpm(*data);
    return Opaque::New(im);
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
    return Opaque::New(im);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int ditherFlag = args[1]->IntegerValue();
    int colorsWanted = args[2]->IntegerValue();
    gdImagePtr newImage = gdImageCreatePaletteFromTrueColor(im, ditherFlag, colorsWanted);
    return Opaque::New(newImage);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    gdImagePtr brush = (gdImagePtr)JSOPAQUE(args[1]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    gdImagePtr brush = (gdImagePtr)JSOPAQUE(args[1]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
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
 * @param {int} saveFlag - set to save alpha information
 * @return 
 */
static JSVAL gd_imageSaveAlpha (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int saveFlag = args[1]->IntegerValue();
    gdImageSaveAlpha(im, saveFlag);
    return Undefined();
}

/**
 * @function gd.imageSetClip
 * 
 * ### Synopsis
 * 
 * gd.imageSetClip(handle, x1,y1, x2,y2);
 * 
 * Establishes a clipping rectangle. Once gd.imageSetClip() has been called, all future drawing operations will remain within the specified clipping area, until a new gdImageSetClip call takes place. 
 * 
 * For instance, if a clipping rectangle of 25, 25, 75, 75 has been set within a 100x100 image, a diagonal line from 0,0 to 99,99 will appear only between 25,25 and 75,75.
 * 
 * If gd.imageSetClip() is never called, the clipping area will be the entire image.
 * 
 * The parameters passed to gd.imageSetClip() are checked against the dimensions of the image and limited to "safe" values.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} x1 - x coordinate of upper left corner.
 * @param {int} y1 - y coordinate of upper left corner.
 * @param {int} x2 - x coordinate of lower right corner.
 * @param {int} y2 - y coordinate of lower right corner.
 */
static JSVAL gd_imageSetClip (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int x1 = args[1]->IntegerValue();
    int y1 = args[2]->IntegerValue();
    int x2 = args[3]->IntegerValue();
    int y2 = args[4]->IntegerValue();
    gdImageSetClip(im, x1, y1, x2, y2);
    return Undefined();
}

/**
 * @function gd.imageGetClip
 * 
 * ### Synopsis
 * 
 * var clipInfo = gd.imageGetClip(handle);
 * 
 * Fetches the boundaries of the current clipping rectangle.
 * 
 * The returned clipInfo object has the form:
 * 
 * x1: x coordinate of upper left corner.
 * y1: y coordinate of upper left corner.
 * x2: x coordinate of lower right corner.
 * y2: y coordinate of lower right corner.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @return {object} clipInfo - object with coordinates of upper left/lower right of clip rectangle.
 */
static JSVAL gd_imageGetClip (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int x1, y1, x2, y2;
    gdImageGetClip(im, &x1, &y1, &x2, &y2);
    Handle<Object>o = Object::New();
    o->Set(String::New("x1"), Integer::New(x1));
    o->Set(String::New("y1"), Integer::New(y1));
    o->Set(String::New("x2"), Integer::New(x2));
    o->Set(String::New("y2"), Integer::New(y2));
    return o;
}

// Query Functions

/**
 * @function gd.imageAlpha
 * 
 * ### Synopsis
 * 
 * var alpha = gd.imageAlpha(handle, color);
 * 
 * Returns the alpha channel component of the specified color index. 
 * 
 * Alpha channel values vary between 0 (gd.AlphaOpaque), which does not blend at all with the background, through 127 (gd.AlphaTransparent), which allows the background to shine through 100%. 
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} color - color index.
 * @return {int} alpha - alpha value
 */
static JSVAL gd_imageAlpha (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int color = args[1]->IntegerValue();
    int alpha = gdImageAlpha(im, color);
    return Integer::New(alpha);
}

/**
 * @function gd.imageGetPixel
 * 
 * ### Synopsis
 * 
 * var color = gd.imageGetPixel(handle, x, y);
 * 
 * Retreive the color index of a particular pixel.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} x - x coordinate of pixel to retrieve;
 * @param {int} y - y coordinate of pixel to retrieve;
 * @return {int} color - color of pixel at specified location.
 */
static JSVAL gd_imageGetPixel (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int x = args[1]->IntegerValue();
    int y = args[2]->IntegerValue();
    int color = gdImageGetPixel(im, x, y);
    return Integer::New(color);
}

/**
 * @function gd.imageBoundsSafe
 * 
 * ### Synopsis
 * 
 * var isSafe = gd.imageBoundsSafe(handle, x, y);
 * 
 * Determine if a point is within the clipping rectangle of the specified image.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} x - x coordinate of pixel to test;
 * @param {int} y - y coordinate of pixel to test;
 * @return {boolean} isSafe - true if the specified point is within the current clipping rectangle.
 */
static JSVAL gd_imageBoundsSafe (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int x = args[1]->IntegerValue();
    int y = args[2]->IntegerValue();
    return gdImageBoundsSafe(im, x, y) ? True() : False();
}

/**
 * @function gd.imageBlue
 * 
 * ### Synopsis
 * 
 * var blue = gd.imageBlue(handle, color);
 * 
 * Get the blue component of the specified color index.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} color - color index to get blue component of.
 * @return {int} blue - blue component.
 */
static JSVAL gd_imageBlue (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int color = args[1]->IntegerValue();
    int blue = gdImageBlue(im, color);
    return Integer::New(blue);
}

/**
 * @function gd.imageGreen
 * 
 * ### Synopsis
 * 
 * var green = gd.imageGreen(handle, color);
 * 
 * Get the green component of the specified color index.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} color - color index to get green component of.
 * @return {int} green - green component.
 */
static JSVAL gd_imageGreen (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int color = args[1]->IntegerValue();
    int green = gdImageGreen(im, color);
    return Integer::New(green);
}

/**
 * @function gd.imageRed
 * 
 * ### Synopsis
 * 
 * var red = gd.imageRed(handle, color);
 * 
 * Get the red component of the specified color index.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} color - color index to get red component of.
 * @return {int} red - red component.
 */
static JSVAL gd_imageRed (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int color = args[1]->IntegerValue();
    int red = gdImageRed(im, color);
    return Integer::New(red);
}

/**
 * @function gd.imageSX
 * 
 * ### Synopsis
 * 
 * var width = gd.imageSX(handle);
 * 
 * Get width of image.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @return {int} width - width of the image.
 */
static JSVAL gd_imageSX (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    return Integer::New(gdImageSX(im));
}

/**
 * @function gd.imageSY
 * 
 * ### Synopsis
 * 
 * var height = gd.imageSY(handle);
 * 
 * Get height of image.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @return {int} height - height of the image.
 */
static JSVAL gd_imageSY (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    return Integer::New(gdImageSY(im));
}

// Fonts and text-handling functions

/**
 * @function gd.fontGetSmall
 * 
 * ### Synopsis
 * 
 * var fontHandle = gd.fontGetSmall();
 * 
 * Get a font handle for the "small" gd font.  The returned handle is used as an argument to the gd text rendering functions.
 * 
 * @return {object} fontHandle - opaque handle to the font.
 */
static JSVAL gd_fontGetSmall (JSARGS args) {
    return Opaque::New(gdFontGetSmall());
}

/**
 * @function gd.fontGetLarge
 * 
 * ### Synopsis
 * 
 * var fontHandle = gd.fontGetLarge();
 * 
 * Get a font handle for the "large" gd font.  The returned handle is used as an argument to the gd text rendering functions.
 * 
 * @return {object} fontHandle - opaque handle to the font.
 */
static JSVAL gd_fontGetLarge (JSARGS args) {
    return Opaque::New(gdFontGetLarge());
}

/**
 * @function gd.fontGetMediumBold
 * 
 * ### Synopsis
 * 
 * var fontHandle = gd.fontGetMediumBold();
 * 
 * Get a font handle for the "medium bold" gd font.  The returned handle is used as an argument to the gd text rendering functions.
 * 
 * @return {object} fontHandle - opaque handle to the font.
 */
static JSVAL gd_fontGetMediumBold (JSARGS args) {
    return Opaque::New(gdFontGetMediumBold());
}

/**
 * @function gd.fontGetGiant
 * 
 * ### Synopsis
 * 
 * var fontHandle = gd.fontGetGiant();
 * 
 * Get a font handle for the "giant" gd font.  The returned handle is used as an argument to the gd text rendering functions.
 * 
 * @return {object} fontHandle - opaque handle to the font.
 */
static JSVAL gd_fontGetGiant (JSARGS args) {
    return Opaque::New(gdFontGetGiant());
}

/**
 * @function gd.fontGetTiny
 * 
 * ### Synopsis
 * 
 * var fontHandle = gd.fontGetTiny();
 * 
 * Get a font handle for the "tiny" gd font.  The returned handle is used as an argument to the gd text rendering functions.
 * 
 * @return {object} fontHandle - opaque handle to the font.
 */
static JSVAL gd_fontGetTiny (JSARGS args) {
    return Opaque::New(gdFontGetTiny());
}

/**
 * @function gd.imageChar
 * 
 * ### Synopsis
 * 
 * gd.imageChar(handle, fontHandle, x, y, c, color);
 * 
 * Draws a single character on the image.  The chracter is rendered left to right.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {object} fontHandle - opaque handle to a GD font
 * @param {int} x - x coordinate to render character.
 * @param {int} y - y coordinate to render character.
 * @param {int} c - the character to render.
 * @param {int} color - the color to render the character.
 */
static JSVAL gd_imageChar (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    gdFontPtr font = (gdFontPtr)JSOPAQUE(args[1]);
    int x = args[2]->IntegerValue();
    int y = args[3]->IntegerValue();
    int c = args[4]->IntegerValue();
    int color = args[5]->IntegerValue();
    gdImageChar(im, font, x, y, c, color);
    return Undefined();
}

/**
 * @function gd.imageCharUp
 * 
 * ### Synopsis
 * 
 * gd.imageCharUp(handle, fontHandle, x, y, c, color);
 * 
 * Draws a single character on the image, rotated 90 degrees.  The chracter is rendered bottom to top.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {object} fontHandle - opaque handle to a GD font
 * @param {int} x - x coordinate to render character.
 * @param {int} y - y coordinate to render character.
 * @param {int} c - the character to render.
 * @param {int} color - the color to render the character.
 */
static JSVAL gd_imageCharUp (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    gdFontPtr font = (gdFontPtr)JSOPAQUE(args[1]);
    int x = args[2]->IntegerValue();
    int y = args[3]->IntegerValue();
    int c = args[4]->IntegerValue();
    int color = args[5]->IntegerValue();
    gdImageCharUp(im, font, x, y, c, color);
    return Undefined();
}

/**
 * @function gd.imageString
 * 
 * ### Synopsis
 * 
 * gd.imageString(handle, fontHandle, x, y, s, color);
 * 
 * Draws a string on the image.  The string is rendered left to right.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {object} fontHandle - opaque handle to a GD font
 * @param {int} x - x coordinate to render string.
 * @param {int} y - y coordinate to render string.
 * @param {string} s - the string to render.
 * @param {int} color - the color to render the string.
 */
static JSVAL gd_imageString (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    gdFontPtr font = (gdFontPtr)JSOPAQUE(args[1]);
    int x = args[2]->IntegerValue();
    int y = args[3]->IntegerValue();
    String::AsciiValue s(args[4]);
    int color = args[5]->IntegerValue();
    gdImageString(im, font, x, y, (unsigned char *) *s, color);
    return Undefined();
}

/**
 * @function gd.imageString16
 * 
 * ### Synopsis
 * 
 * gd.imageString16(handle, fontHandle, x, y, s, color);
 * 
 * Draws a string of 16-bit characters on the image.  The string is rendered left to right.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {object} fontHandle - opaque handle to a GD font
 * @param {int} x - x coordinate to render string.
 * @param {int} y - y coordinate to render string.
 * @param {string} s - the string to render.
 * @param {int} color - the color to render the string.
 */
static JSVAL gd_imageString16 (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    gdFontPtr font = (gdFontPtr)JSOPAQUE(args[1]);
    int x = args[2]->IntegerValue();
    int y = args[3]->IntegerValue();
    String::Utf8Value s(args[4]);
    int color = args[5]->IntegerValue();
    gdImageString16(im, font, x, y, (unsigned short *) *s, color);
    return Undefined();
}

/**
 * @function gd.imageStringUp
 * 
 * ### Synopsis
 * 
 * gd.imageStringUp(handle, fontHandle, x, y, s, color);
 * 
 * Draws a string on the image, rotated 90 degrees.  The string is rendered bottom to top.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {object} fontHandle - opaque handle to a GD font
 * @param {int} x - x coordinate to render string.
 * @param {int} y - y coordinate to render string.
 * @param {string} s - the string to render.
 * @param {int} color - the color to render the string.
 */
static JSVAL gd_imageStringUp (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    gdFontPtr font = (gdFontPtr)JSOPAQUE(args[1]);
    int x = args[2]->IntegerValue();
    int y = args[3]->IntegerValue();
    String::AsciiValue s(args[4]);
    int color = args[5]->IntegerValue();
    gdImageStringUp(im, font, x, y, (unsigned char *) *s, color);
    return Undefined();
}

/**
 * @function gd.imageStringUp16
 * 
 * ### Synopsis
 * 
 * gd.imageStringUp16(handle, fontHandle, x, y, s, color);
 * 
 * Draws a string of 16-bit characters on the image, rotated 90 degrees.  The string is rendered bottom to top.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {object} fontHandle - opaque handle to a GD font
 * @param {int} x - x coordinate to render string.
 * @param {int} y - y coordinate to render string.
 * @param {string} s - the string to render.
 * @param {int} color - the color to render the string.
 */
static JSVAL gd_imageStringUp16 (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    gdFontPtr font = (gdFontPtr)JSOPAQUE(args[1]);
    int x = args[2]->IntegerValue();
    int y = args[3]->IntegerValue();
    String::Utf8Value s(args[4]);
    int color = args[5]->IntegerValue();
    gdImageStringUp16(im, font, x, y, (unsigned short *) *s, color);
    return Undefined();
}

static JSVAL gd_FTUseFontConfig (JSARGS args) {
    int flag = args[0]->IntegerValue();
    return Integer::New(gdFTUseFontConfig(flag));
}



// TODO: finish font/text functions

// Color handling functions

/**
 * @function gd.imageColorAllocate
 * 
 * ### Synopsis
 * 
 * var color = gd.imageColorAllocate(handle, r, g, b);
 * 
 * gd.imageColorAllocate() finds the first available color index in the image specified, sets its RGB values to those requested (255 is the maximum for each), and returns the index of the new color table entry, or an RGBA value in the case of a truecolor image; in either case you can then use the returned value as a parameter to drawing functions. When creating a new palette-based image, the first time you invoke this function, you are setting the background color for that image.
 * 
 * In the event that all gdMaxColors colors (256) have already been allocated, gd.imageColorAllocate() will return -1 to indicate failure. (This is not uncommon when working with existing PNG files that already use 256 colors.) 
 * 
 * Note that gdImageColorAllocate does not check for existing colors that match your request; see gd.imageColorExact(), gd.imageColorClosest() and gd.imageColorClosestHWB() for ways to locate existing colors that approximate the color desired in situations where a new color is not available. Also see gd.imageColorResolve(), new in gd-1.6.2.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} r - red component of desired color.
 * @param {int} g - green component of desired color.
 * @param {int} b - blue component of desired color.
 * @return {int} color - color index or rgba value that can be used as color value for other gd functions.
 */
static JSVAL gd_imageColorAllocate (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    return Integer::New(gdImageColorAllocate(im, red, green, blue));
}

/**
 * @function gd.imageColorAllocateAlpha
 * 
 * ### Synopsis
 * 
 * var color = gd.imageColorAllocateAlpha(handle, r, g, b, a);
 * 
 * gd.imageColorAllocateAlpha() finds the first available color index in the image specified, sets its RGBA values to those requested (255 is the maximum for red, green, and blue, and 127 represents full transparency for alpha), and returns the index of the new color table entry, or an RGBA value in the case of a truecolor image; in either case you can then use the returned value as a parameter to drawing functions. When creating a new palette-based image, the first time you invoke this function, you are setting the background color for that image.
 * 
 * In the event that all gdMaxColors colors (256) have already been allocated, gd.imageColorAllocate() will return -1 to indicate failure. (This is not uncommon when working with existing PNG files that already use 256 colors.) 
 * 
 * Note that gd.imageColorAllocateAlpha() does not check for existing colors that match your request; see gd.imageColorExactAlpha() and gd.imageColorClosestAlpha() for ways to locate existing colors that approximate the color desired in situations where a new color is not available. Also see gd.imageColorResolveAlpha().
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} r - red component of desired color.
 * @param {int} g - green component of desired color.
 * @param {int} b - blue component of desired color.
 * @param {int} a - alpha component of desired color.
 * @return {int} color - color index or rgba value that can be used as color value for other gd functions.
 */
static JSVAL gd_imageColorAllocateAlpha (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    int alpha = args[4]->IntegerValue();
    return Integer::New(gdImageColorAllocateAlpha(im, red, green, blue, alpha));
}

/**
 * @function gd.imageColorClosest
 * 
 * ### Synopsis
 * 
 * var color = gd.imageColorClosest(handle, r, g, b);
 * 
 * gd.imageColorClosest() searches the colors which have been defined thus far in the image specified and returns the index of the color with RGB values closest to those of the request. (Closeness is determined by Euclidian distance, which is used to determine the distance in three-dimensional color space between colors.)
 * 
 * If no colors have yet been allocated in the image, gd.imageColorClosest() returns -1.
 * 
 * When applied to a truecolor image, this function always succeeds in returning the desired color.
 * 
 * This function is most useful as a backup method for choosing a drawing color when an image already contains gd.MaxColors (256) colors and no more can be allocated. (This is not uncommon when working with existing PNG files that already use many colors.) 
 * 
 * See gd.imageColorExact() for a method of locating exact matches only.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} r - red component of desired color.
 * @param {int} g - green component of desired color.
 * @param {int} b - blue component of desired color.
 * @return {int} color - color index or rgba value that can be used as color value for other gd functions.
 */
static JSVAL gd_imageColorClosest (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    return Integer::New(gdImageColorClosest(im, red, green, blue));
}

/**
 * @function gd.imageColorClosestAlpha
 * 
 * ### Synopsis
 * 
 * var color = gd.imageColorClosestAlpha(handle, r, g, b, a);
 * 
 * gd.imageColorClosestAlpha searches the colors which have been defined thus far in the image specified and returns the index of the color with RGBA values closest to those of the request. (Closeness is determined by Euclidian distance, which is used to determine the distance in four-dimensional color/alpha space between colors.)
 * 
 * If no colors have yet been allocated in the image, gd.imageColorClosestAlpha() returns -1.
 * 
 * When applied to a truecolor image, this function always succeeds in returning the desired color.
 * 
 * This function is most useful as a backup method for choosing a drawing color when a palette-based image already contains gd.MaxColors (256) colors and no more can be allocated. (This is not uncommon when working with existing palette-based PNG files that already use many colors.) 
 * 
 * See gd.imageColorExactAlpha() for a method of locating exact matches only.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} r - red component of desired color.
 * @param {int} g - green component of desired color.
 * @param {int} b - blue component of desired color.
 * @param {int} a - alpha component of desired color.
 * @return {int} color - color index or rgba value that can be used as color value for other gd functions.
 */
static JSVAL gd_imageColorClosestAlpha (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    int alpha = args[4]->IntegerValue();
    return Integer::New(gdImageColorClosestAlpha(im, red, green, blue, alpha));
}

/**
 * @function gd.imageColorClosestHWB
 * 
 * ### Synopsis
 * 
 * var color = gd.imageColorClosestHWB(handle, r, g, b);
 * 
 * gd.imageColorClosestHWB() searches the colors which have been defined thus far in the image specified and returns the index of the color with hue, whiteness and blackness closest to the requested color. This scheme is typically superior to the Euclidian distance scheme used by gd.imageColorClosest().
 * 
 * If no colors have yet been allocated in the image, gd.imageColorClosestHWB() returns -1.
 * 
 * When applied to a truecolor image, this function always succeeds in returning the desired color.
 * 
 * This function is most useful as a backup method for choosing a drawing color when an image already contains gdMaxColors (256) colors and no more can be allocated. (This is not uncommon when working with existing PNG files that already use many colors.) 
 * 
 * See gd.imageColorExact() for a method of locating exact matches only.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} r - red component of desired color.
 * @param {int} g - green component of desired color.
 * @param {int} b - blue component of desired color.
 * @return {int} color - color index or rgba value that can be used as color value for other gd functions.
 */
static JSVAL gd_imageColorClosestHWB (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    return Integer::New(gdImageColorClosestHWB(im, red, green, blue));
}

/**
 * @function gd.imageColorExact
 * 
 * ### Synopsis
 * 
 * var color = gd.imageColorExact(handle, r, g, b);
 * 
 * gd.imageColorExact() searches the colors which have been defined thus far in the image specified and returns the index of the first color with RGB values which exactly match those of the request. 
 * 
 * If no allocated color matches the request precisely, gd.imageColorExact() returns -1. 
 * 
 * See gd.imageColorClosest() for a way to find the color closest to the color requested.
 * 
 * When applied to a truecolor image, this function always succeeds in returning the desired color.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} r - red component of desired color.
 * @param {int} g - green component of desired color.
 * @param {int} b - blue component of desired color.
 * @return {int} color - color index or rgba value that can be used as color value for other gd functions.
 */
static JSVAL gd_imageColorExact (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    return Integer::New(gdImageColorExact(im, red, green, blue));
}

/**
 * @function gd.imageColorResolve
 * 
 * ### Synopsis
 * 
 * var color = gd.imageColorResolve(handle, r, g, b);
 * 
 * gd.imageColorResolve() searches the colors which have been defined thus far in the image specified and returns the index of the first color with RGB values which exactly match those of the request. 
 * 
 * If no allocated color matches the request precisely, then gd.imageColorResolve() tries to allocate the exact color. 
 * 
 * If there is no space left in the color table then gd.imageColorResolve() returns the closest color (as in gd.imageColorClosest()). 
 * 
 * This function always returns an index of a color.
 * 
 * When applied to a truecolor image, this function always succeeds in returning the desired color.
 *  
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} r - red component of desired color.
 * @param {int} g - green component of desired color.
 * @param {int} b - blue component of desired color.
 * @return {int} color - color index or rgba value that can be used as color value for other gd functions.
 */
static JSVAL gd_imageColorResolve (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    return Integer::New(gdImageColorResolve(im, red, green, blue));
}

/**
 * @function gd.imageColorResolveAlpha
 * 
 * ### Synopsis
 * 
 * var color = gd.imageColorResolveAlpha(handle, r, g, b, a);
 * 
 * gd.imageColorResolveAlpha() searches the colors which have been defined thus far in the image specified and returns the index of the first color with RGBA values which exactly match those of the request. 
 * 
 * If no allocated color matches the request precisely, then gd.imageColorResolveAlpha() tries to allocate the exact color. 
 * 
 * If there is no space left in the color table then gdImageColorResolveAlpha returns the closest color (as in gdImageColorClosestAlpha()). 
 * 
 * This function always returns an index of a color. * 
 * 
 * When applied to a truecolor image, this function always succeeds in returning the desired color.
 *  
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} r - red component of desired color.
 * @param {int} g - green component of desired color.
 * @param {int} b - blue component of desired color.
 * @param {int} a - alpha component of desired color.
 * @return {int} color - color index or rgba value that can be used as color value for other gd functions.
 */
static JSVAL gd_imageColorResolveAlpha (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int red = args[1]->IntegerValue();
    int green = args[2]->IntegerValue();
    int blue = args[3]->IntegerValue();
    int alpha = args[4]->IntegerValue();
    return Integer::New(gdImageColorResolveAlpha(im, red, green, blue, alpha));
}

/**
 * @function gd.imageColorsTotal
 * 
 * ### Synopsis
 * 
 * var colors = gd.imageColorsTotal(handle);
 * 
 * Returns the total number of colors currently allocated in a palette image.  For truecolor images, the result of this function is undefined and should not be used.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @return {int} colors - number of colors used.
 */
static JSVAL gd_imageColorsTotal (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    return Integer::New(gdImageColorsTotal(im));
}

/**
 * @function gd.imageGetInterlaced
 * 
 * ### Synopsis
 * 
 * var isInterlaced = gd.imageGetInterlaced(handle);
 * 
 * Determine if the the image is interlaced.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @return {boolean} isInterlaced - true if image is interlaced.
 */
static JSVAL gd_imageGetInterlaced (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    return Integer::New(gdImageGetInterlaced(im));
}

/**
 * @function gd.imageGetTransparent
 * 
 * ### Synopsis
 * 
 * var color = gd.imageGetTransparent(handle);
 * 
 * Get the current transparent color index in the image.  If there is no transparent color, this function returns -1.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @return {int} color - color index of transparent color or -1.
 */
static JSVAL gd_imageGetTransparent (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    return Integer::New(gdImageGetTransparent(im));
}

/**
 * @function gd.imageColorDeallocate
 * 
 * ### Synopsis
 * 
 * gd.imageColorDeallocate(handle, color);
 * 
 * Marks the specified color as being available for reuse. It does not attempt to determine whether the color index is still in use in the image. 
 * 
 * After a call to this function, the next call to gdImageColorAllocate for the same image will set new RGB values for that color index, changing the color of any pixels which have that index as a result. 
 * 
 * If multiple calls to gd.imageColorDeallocate() are made consecutively, the lowest-numbered index among them will be reused by the next gd.imageColorAllocate() call.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} color - color index to deallocate.
 */
static JSVAL gd_imageColorDeallocate (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int color = args[1]->IntegerValue();
    gdImageColorDeallocate(im, color);
    return Undefined();
}

/**
 * @function gd.imageColorTransparent
 * 
 * ### Synopsis
 * 
 * gd.imageColorTransparent(handle, color);
 * 
 * Sets the transparent color index for the specified image to the specified index. 
 * 
 * To indicate that there should be no transparent color, invoke gd.imageColorTransparent() with a color index of -1. 
 * 
 * Note that JPEG images do not support transparency, so this setting has no effect when writing JPEG images.
 * 
 * The color index used should be an index allocated by gd.imageColorAllocate(), whether explicitly invoked by your code or implicitly invoked by loading an image. 
 * 
 * In order to ensure that your image has a reasonable appearance when viewed by users who do not have transparent background capabilities (or when you are writing a JPEG-format file, which does not support transparency), be sure to give reasonable RGB values to the color you allocate for use as a transparent color, even though it will be transparent on systems that support PNG transparency.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} color - color index to set as the transparent color.
 */
static JSVAL gd_imageColorTransparent (JSARGS args) {
    gdImagePtr im = (gdImagePtr)JSOPAQUE(args[0]);
    int color = args[1]->IntegerValue();
    gdImageColorTransparent(im, color);
    return Undefined();
}

/**
 * @function gd.imageTrueColor
 * 
 * ### Synopsis
 * 
 * var color = gd.imageTrueColor(red, green, blue);
 * 
 * Returns an RGBA color value for use when drawing on a truecolor image. Red, green, and blue are all in the range between 0 (off) and 255 (maximum). 
 * 
 * This function should not be used with palette-based images. 
 * 
 * If you need to write code which is compatible with both palette-based and truecolor images, use gd.imageColorResolve().
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} red - red component of desired color.
 * @param {int} green - green component of desired color.
 * @param {int} blue - blue component of desired color.
 * @return {int} color - rgba color value for use with drawing functions.
 */
static JSVAL gd_TrueColor (JSARGS args) {
    int red = args[0]->IntegerValue();
    int green = args[1]->IntegerValue();
    int blue = args[2]->IntegerValue();
    return Integer::New(gdTrueColor(red, green, blue));
}

/**
 * @function gd.imageTrueColorAlpha
 * 
 * ### Synopsis
 * 
 * var color = gd.imageTrueColorAlpha(red, green, blue, alpha);
 * 
 * Returns an RGBA color value for use when drawing on a truecolor image with alpha channel transparency. 
 * 
 * Red, green, and blue are all in the range between 0 (off) and 255 (maximum). Alpha is in the range between 0 (opaque) and 127 (fully transparent). 
 * 
 * This function should not be used with palette-based images. 
 * 
 * If you need to write code which is compatible with both palette-based and truecolor images, use gd.imageColorResolveAlpha().
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} red - red component of desired color.
 * @param {int} green - green component of desired color.
 * @param {int} blue - blue component of desired color.
 * @param {int} alpha - alpha component of desired color.
 * @return {int} color - rgba color value for use with drawing functions.
 */
static JSVAL gd_TrueColorAlpha (JSARGS args) {
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
 */
static JSVAL gd_imageCopy (JSARGS args) {
    gdImagePtr dst = (gdImagePtr)JSOPAQUE(args[0]);
    gdImagePtr src = (gdImagePtr)JSOPAQUE(args[1]);
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
 */
static JSVAL gd_imageCopyResized (JSARGS args) {
    gdImagePtr dst = (gdImagePtr)JSOPAQUE(args[0]);
    gdImagePtr src = (gdImagePtr)JSOPAQUE(args[1]);
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
 * Copy a rectangular portion of one image to another image, smoothly interpolating pixel values so that, in particular, reducing the size of an image still retains a great deal of clarity. 
 * 
 * The X and Y dimensions of the original region and the destination region can vary, resulting in stretching or shrinking of the region as appropriate.
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
 */
static JSVAL gd_imageCopyResampled (JSARGS args) {
    gdImagePtr dst = (gdImagePtr)JSOPAQUE(args[0]);
    gdImagePtr src = (gdImagePtr)JSOPAQUE(args[1]);
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

/**
 * @function gd.imageCopyRotated
 * 
 * ### Synopsis
 * 
 * gd.imageCopyRotated(dstImage, srcImage, dstX, dstY, srcX, srcY, srcW, srcH, angle);
 * 
 * Copy a rectangular portion of one image to another image, or to another region of the same image. 
 * 
 * The srcX and srcY coordinates specify the upper left corner of the source area; however, the dstX and dstY coordinates specify the CENTER of the destination area. This important distinction is made because the rotated rectangle may may or may not be parallel to the X and Y axes. 
 * 
 * The destination coordinates may be floating point, as the center of the desired destination area may lie at the center of a pixel (0.5 pixels) rather than its upper left corner. 
 * 
 * The angle specified is an integer number of degrees, between 0 and 360, with 0 degrees causing no change, and counterclockwise rotation as the angle increases.
 * 
 * When you copy a region from one location in an image to another location in the same image, gd.imageCopyRotated() will perform as expected unless the regions overlap, in which case the result is unpredictable. If this presents a problem, create a scratch image in which to keep intermediate results.
 * 
 * Important note on copying between images: since palette-based images do not necessarily have the same color tables, pixels are not simply set to the same color index values to copy them. If the destination image is not a truecolor image, gd.imageColorResolveAlpha() is used to choose the destination pixel.
 * 
 * @param {object} dstImage - destination for resized image
 * @param {object} srcImage - source for resized image
 * @param {number} dstX - X coordinate of the upper left corner of the destination region
 * @param {number} dstY - Y coordinate of the upper left corner of the destination region
 * @param {int} srcX - X coordinate of the upper left corner of the source region
 * @param {int} srcY - Y coordinate of the upper left corner of the source region
 * @param {int} srcW - width of the source region
 * @param {int} srcH - height of the source region
 * @param {int} angle - angle to rotate image.
 */
static JSVAL gd_imageCopyRotated (JSARGS args) {
    gdImagePtr dst = (gdImagePtr)JSOPAQUE(args[0]);
    gdImagePtr src = (gdImagePtr)JSOPAQUE(args[1]);
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

/**
 * @function gd.imageCopyMerge
 * 
 * ### Synopsis
 * 
 * gd.imageCopyMerge(dstImage, srcImage, dstX, dstY, srcX, srcY, w, h, pct);
 * 
 * gd.imageCopyMerge() is almost identical to gdImageCopy, except that it 'merges' the two images by an amount specified in the last parameter. 
 * 
 * If the last parameter is 100, then it will function identically to gdImageCopy - the source image replaces the pixels in the destination.
 * 
 * If, however, the pct parameter is less than 100, then the two images are merged. With pct = 0, no action is taken.
 * 
 * This feature is most useful to 'highlight' sections of an image by merging a solid color with pct = 50:
 * 
 * @param {object} dstImage - destination for resized image
 * @param {object} srcImage - source for resized image
 * @param {int} dstX - X coordinate of the upper left corner of the destination region
 * @param {int} dstY - Y coordinate of the upper left corner of the destination region
 * @param {int} srcX - X coordinate of the upper left corner of the source region
 * @param {int} srcY - Y coordinate of the upper left corner of the source region
 * @param {int} w - width of the region
 * @param {int} h - height of the region
 * @param {int} h - height of the region
 * @param {int} pct - pct weight to merge pixels.
 */
static JSVAL gd_imageCopyMerge (JSARGS args) {
    gdImagePtr dst = (gdImagePtr)JSOPAQUE(args[0]);
    gdImagePtr src = (gdImagePtr)JSOPAQUE(args[1]);
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

/**
 * @function gd.imageCopyMergeGray
 * 
 * ### Synopsis
 * 
 * gd.imageCopyMergeGray(dstImage, srcImage, dstX, dstY, srcX, srcY, w, h, pct);
 * 
 * gd.imageCopyMergeGray() is almost identical to gd.imageCopyMerge(), except that when merging images it preserves the hue of the source by converting the destination pixels to grey scale before the copy operation.
 * 
 * @param {object} dstImage - destination for resized image
 * @param {object} srcImage - source for resized image
 * @param {int} dstX - X coordinate of the upper left corner of the destination region
 * @param {int} dstY - Y coordinate of the upper left corner of the destination region
 * @param {int} srcX - X coordinate of the upper left corner of the source region
 * @param {int} srcY - Y coordinate of the upper left corner of the source region
 * @param {int} w - width of the region
 * @param {int} h - height of the region
 * @param {int} h - height of the region
 * @param {int} pct - pct weight to merge pixels.
 */
static JSVAL gd_imageCopyMergeGray (JSARGS args) {
    gdImagePtr dst = (gdImagePtr)JSOPAQUE(args[0]);
    gdImagePtr src = (gdImagePtr)JSOPAQUE(args[1]);
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

/**
 * @function gd.imagePaletteCopy
 * 
 * ### Synopsis
 * 
 * gd.imagePaletteCopy(dstImage, srcImage);
 * 
 * Copies a palette from one image to another, attempting to match the colors in the target image to the colors in the source palette.
 * 
 * @param {object} dstImage - destination for resized image
 * @param {object} srcImage - source for resized image
 */
static JSVAL gd_imagePaletteCopy (JSARGS args) {
    gdImagePtr dst = (gdImagePtr)JSOPAQUE(args[0]);
    gdImagePtr src = (gdImagePtr)JSOPAQUE(args[1]);
    gdImagePaletteCopy(dst, src);
    return Undefined();
}

/**
 * @function gd.imageSquareToCircle
 * 
 * ### Synopsis
 * 
 * gd.imageSquareToCircule(handle, radius);
 * 
 * Returns a new image of width and height radius * 2, in which the X axis of the original has been remapped to theta (angle) and the Y axis of the original has been remapped to rho (distance from center). This is known as a "polar coordinate transform."
 * 
 * The source image must be square, but can have any size.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} radius - radious of circls.
 * @return {object} newHandle - handle to a new image.
 */
static JSVAL gd_imageSquareToCircle (JSARGS args) {
    gdImagePtr dst = (gdImagePtr)JSOPAQUE(args[0]);
    int radius = args[1]->IntegerValue();
    return Opaque::New(gdImageSquareToCircle(dst, radius));
}

/**
 * @function gd.imageSharpen
 * 
 * ### Synopsis
 * 
 * gd.imageSharpen(handle, pct);
 * 
 * Sharpens the specified image. The pct parameter is a sharpening percentage, and can be greater than 100. 
 * 
 * Silently does nothing to non-truecolor images. 
 * 
 * Silently does nothing for pct less than 0. Transparency/alpha channel are not altered.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} pct - sharpening percentage
 */
static JSVAL gd_imageSharpen (JSARGS args) {
    gdImagePtr dst = (gdImagePtr)JSOPAQUE(args[0]);
    int pct = args[1]->IntegerValue();
    gdImageSharpen(dst, pct);
    return Undefined();
}

/**
 * @function gd.imagePaletteCompare
 * 
 * ### Synopsis
 * 
 * var mask = gd.imagePaletteCompare(dstImage, srcImage);
 * 
 * Returns a bitmap indicating if the two images are different. 
 * 
 * The members of the bitmap are defined in gd.h, but the most important is GD_CMP_IMAGE, which indicated that the images will actually appear different when displayed. 
 * 
 * Other, less important, differences relate to pallette entries. Any difference in the transparent colour is assumed to make images display differently, even if the transparent colour is not used.
 * 
 * The mask bits are:
 * 
 * + gd.CMP_IMAGE               1	Actual image IS different
 * + gd.CMP_NUM_COLORS          2   Number of Colours in pallette differ
 * + gd.CMP_COLOR               4   Image colours differ
 * + gd.CMP_SIZE_X              8   Image width differs
 * + gd.CMP_SIZE_Y              16  Image heights differ
 * + gd.CMP_TRANSPARENT         32  Transparent colour
 * + gd.CMP_BACKGROUND          64  Background colour
 * + gd.CMP_INTERLACE           128 Interlaced setting
 * + gd.CMP_TRUECOLOR           256 Truecolor vs palette differs
 * 
 * @param {object} dstImage - destination for resized image
 * @param {object} srcImage - source for resized image
 * @return {int} mask - see bits defined above.
 */
static JSVAL gd_imageCompare (JSARGS args) {
    gdImagePtr im1 = (gdImagePtr)JSOPAQUE(args[0]);
    gdImagePtr im2 = (gdImagePtr)JSOPAQUE(args[1]);
    return Integer::New(gdImageCompare(im1, im2));
}

/**
 * @function gd.imageInterlace
 * 
 * ### Synopsis
 * 
 * gd.imageInterlace(handle, interlace);
 * 
 * gd.imageInterlace() is used to determine whether an image should be stored in a linear fashion, in which lines will appear on the display from first to last, or in an interlaced fashion, in which the image will "fade in" over several passes. 
 * 
 * By default, images are not interlaced. (When writing JPEG images, interlacing implies generating progressive JPEG files, which are represented as a series of scans of increasing quality. Noninterlaced gd images result in regular [sequential] JPEG data streams.)
 * 
 * A nonzero value for the interlace argument turns on interlace; a zero value turns it off. Note that interlace has no effect on other functions, and has no meaning unless you save the image in PNG or JPEG format; the gd and xbm formats do not support interlace.
 * 
 * When a PNG is loaded with gd.imageCreateFromPng() or a JPEG is loaded with gd.imageCreateFromJpeg(), interlace will be set according to the setting in the PNG or JPEG file.
 * 
 * Note that many PNG and JPEG viewers and web browsers do not support interlace or the incremental display of progressive JPEGs. However, the interlaced PNG or progressive JPEG should still display; it will simply appear all at once, just as other images do.
 * 
 * @param {object} handle - opaque handle to a GD image.
 * @param {int} interlace - set to 1 to enable interlace for the image, 0 to disable it.
 */
static JSVAL gd_imageInterlace (JSARGS args) {
    gdImagePtr dst = (gdImagePtr)JSOPAQUE(args[0]);
    int interlace = args[1]->IntegerValue();
    gdImageInterlace(dst, interlace);
    return Undefined();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void init_gd_object () {
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
    gd->Set(String::New("AplhaOpaque"), Integer::New(gdAlphaOpaque));
    gd->Set(String::New("AplhaTransparent"), Integer::New(gdAlphaTransparent));
    
    gd->Set(String::New("CMP_IMAGE"), Integer::New(GD_CMP_IMAGE));               // 1: Actual image IS different
    gd->Set(String::New("CMP_NUM_COLORS"), Integer::New(GD_CMP_NUM_COLORS));     // 2: Number of Colours in pallette differ
    gd->Set(String::New("CMP_COLOR"), Integer::New(GD_CMP_COLOR));               // 4: Image colours differ 
    gd->Set(String::New("CMP_SIZE_X"), Integer::New(GD_CMP_SIZE_X));             // 8: Image width differs
    gd->Set(String::New("CMP_SIZE_Y"), Integer::New(GD_CMP_SIZE_Y));             // 16: Image heights differ
    gd->Set(String::New("CMP_TRANSPARENT"), Integer::New(GD_CMP_TRANSPARENT));   // 32: Transparent colour
    gd->Set(String::New("CMP_BACKGROUND"), Integer::New(GD_CMP_BACKGROUND));     // 64: Background colour
    gd->Set(String::New("CMP_INTERLACE"), Integer::New(GD_CMP_INTERLACE));       // 128: Interlaced setting
    gd->Set(String::New("CMP_TRUECOLOR"), Integer::New(GD_CMP_TRUECOLOR));       // 256: Truecolor vs palette differs
    

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
