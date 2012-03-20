/**
 * @module Image
 * 
 * 
 */

"use strict";

var gd = require('builtin/gd'),
    fs = require('builtin/fs');

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