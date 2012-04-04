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
 * stdin.close();
 * ```
 */

var editline = require('builtin/editline');

/** 
 * @constructor ReadLine
 * 
 * ### Synopsis:
 * 
 * var stdin = new ReadLine(program_name);
 * 
 * @param {string} program_name - name of program (see man editline).
 */
function ReadLine(prog) {
    prog = prog || 'SilkJS';
    this.handle = editline.init(prog);
    editline.prompt(this.handle, '> ');
}
ReadLine.prototype.extend({
    /**
     * @function stdin.prompt
     * 
     * ### Synopsis
     * 
     * stdin.prompt(str);
     * 
     * Set the prompt string.  The prompt is displayed on lines the user is to type in.  The default prompt is '> '.
     * 
     * @param {string} str - new prompt string.
     * 
     */
    prompt: function(txt) {
        editline.prompt(this.handle, txt);
    },
    
    /**
     * @function stdin.gets
     * 
     * ### Synopsis
     * 
     * var line = stdin.gets();
     * 
     * Read a line from stdin with prompt.
     * 
     * @return {string} line - text that user entered, or false if EOF (e.g. user hit ^D).
     */
    gets: function() {
        return editline.gets(this.handle);
    },
    
    /**
     * @function stdin.close
     * 
     * ### Synopsis
     * 
     * stdin.close();
     * 
     * Close ReadLine instance and free any resources used.
     */
    close: function() {
        editline.end(this.handle);
    }
});

if (exports) {
    exports = ReadLine;
}
