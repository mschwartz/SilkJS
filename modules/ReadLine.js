/**
 * @class ReadLine
 *
 * ### Synopsis
 *
 * var ReadLine = require('ReadLine');
 *
 * Provides console command line input with prompt, editing, and history.
 *
 * ### Example
 * ```
 * var ReadLine = require('ReadLine');
 * var stdin = new ReadLine('program_name');
 * while ((var text = stdin.gets())) {
 *      console.log(text);
 * }
 * in.close();
 * ```
 */

var editline = require('builtin/editline');

function ReadLine(prog) {
    prog = prog || 'SilkJS';
    this.handle = editline.init(prog);
    editline.prompt(this.handle, '> ');
}
ReadLine.prototype.extend({
    prompt: function(txt) {
        editline.prompt(this.handle, txt);
    },
    gets: function() {
        return editline.gets(this.handle);
    },
    close: function() {
        editline.end(this.handle);
    }
});

if (exports) {
    exports = ReadLine;
}
