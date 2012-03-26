/**
 * @class Image
 * 
 * ### Synopsis
 * 
 * var Image = require('Image');
 * 
 * ### Example
 * 
 * var img = new Image(path_to_file);
 * console.log(img.width + 'x' + img.height + ' px');
 * img.resize(64, 64);
 * img.saveAs(path_to_new_file);
 * img.dispose();		// always dispose of your Images!
 * ### Notes
 * 
 * GD is an amazingly rich graphics library.  This class doesn't even scratch the surface of what's possible.
 * 
 * The current functionality of this class is to allow an image to be loaded, resized/cropped, and then written back to disk.
 * 
 */
"use strict";

var gd = require('builtin/gd'),
    fs = require('builtin/fs');

/**
 * @constructor Image
 * 
 * ### Synopsis
 * 
 * var img = new Image(path);
 * 
 * Create an image instance, loading the data from disk.
 * 
 * @param {string} path - path to image file in file system
 * @return {object} img - an instance of the Image
 * 
 * ### Notes
 * 
 * Upon return, img.width and img.height are the width and height of the image in pixels.
 */
var Image = function(path) {
    this.handle = gd.imageCreateFromJpeg(path) ||
                  gd.imageCreateFromPng(path) ||
                  gd.imageCreateFromGif(path);

    if (!this.handle) {
        error('Could not load image ' + path);
    }
    this.width = gd.imageSX(this.handle);
    this.height = gd.imageSY(this.handle);
}

Image.prototype.extend({
	/**
	 * @function Image.resize
	 * 
	 * ### Synopsis
	 * 
	 * img.resize(newWidth, newHeight);
	 * 
	 * Resize (resample, too) the image to new dimensions.
	 * 
	 * @param {int} newWidth - new width for the image
	 * @param {int} newHeight - new height for the image
	 * 
	 * ### Notes
	 * Upon return, img.width and img.height are the new width and height of the (resized) image.
	 */
    resize: function(width, height) {
        if (!this.handle) {
            error('Image.resize - not constructed.');
        }
        if (width == this.width && height == this.height) {
            return;
        }
        var newImage = gd.imageCreateTrueColor(width, height);
        gd.imageCopyResampled(newImage, this.handle, 0,0, 0,0, width, height, this.width, this.height);
        gd.imageDestroy(this.handle);
        this.handle = newImage;
        this.width = width;
        this.height = height;
    },
	
	/**
	 * @function Image.crop
	 * 
	 * ### Synopsis
	 * 
	 * img.crop(x,y, w, h);
	 * 
	 * Crop image.  The x,y coordinates are of the upper left hand corner of the crop position, and w,h are the resulting width and height.
	 * 
	 * @param {int} x - x position of upper left hand corner of crop position in image
	 * @param {int} y - y position of upper left hand corner of crop position in image
	 * @param {int} w - width of cropped image
	 * @param {int} h - height of cropped image
	 * 
	 * ### Notes
	 * Upon return, img.width and img.height are the new width and height of the (resized) image.
	 * 
	 * The rectangle specified by x,y,w,h becomes the resulting image.
	 */
    crop: function(x,y, width, height) {
        if (!this.handle) {
            error('Image.crop - not constructed.');
        }
        var newImage = gd.imageCreateTrueColor(width, height);
        gd.imageCopy(newImage, this.handle, 0, 0, x, y, width, height);
        gd.imageDestroy(this.handle);
        this.handle = newImage;
        this.width = width;
        this.height = height;
    },
	
	/**
	 * @function Image.saveAs
	 * 
	 * ### Synopsis
	 * 
	 * img.saveAs(filename);
	 * 
	 * Save image to a disk file.
	 * 
	 * @param {string} filename - path to file to create
	 * 
 	 * ### Example
	 * ```
	 * try {
	 *   img.saveAs(filename);
	 * }
	 * catch (e) {
	 *	// file could not be created.
	 * }
	 * ```
	 */
    saveAs: function(filename) {
        if (!this.handle) {
            error('Image.resize - not constructed.');
        }
        if (!filename) {
            error('Image.resize - no filename argument provided.')
        }
        if (filename.indexOf('.') == -1) {
            error('Image.resize - invalid filename argument (no extension) ' + filename)
        }
        var extension = filename.split('.').pop().toLowerCase();
        switch (extension) {
            case 'jpg':
            case 'jpeg':
                if (!gd.imageJpeg(this.handle, filename, -1)) {
                    error('Could not write ' + filename + '.  Error: ' + fs.error());
                }
                break;
            case 'png':
                if (!gd.imagePngEx(this.handle, filename, -1)) {
                    error('Could not write ' + filename + '.  Error: ' + fs.error());
                }
                break;
            case 'gif':
                if (!gd.imageGif(this.handle, filename)) {
                    error('Could not write ' + filename + '.  Error: ' + fs.error());
                }
                break;
            default:  
                error('Could not write ' + filename + '. Files of .'+extension+' not supported.');
                break;
        }
        
    },
    dispose: function() {
        if (!this.handle) {
            error('Image.resize - not constructed.');
        }
        gd.imageDestroy(this.handle);
        this.handle = null;
    }
})

if (exports) {
    exports = Image;
}