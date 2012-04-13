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

var editline = require('builtin/editline'),
    process = require('builtin/process');

/** 
 * @constructor ReadLine
 * 
 * ### Synopsis:
 * 
 * var stdin = new ReadLine(program_name);
 * var stdin = new ReadLine(program_name, historySize);
 * 
 * Create a line editor instance.  A history file is written to $HOME/.program_namerc.
 * 
 * 
 * @param {string} program_name - name of program (see man editline).
 * @param {int} historySize - number of lines of history to keep around. Defaults to 50.
 */
function ReadLine(prog, historySize) {
    prog = prog || 'SilkJS';
    this.prog = prog;
    this.historyFile = process.env().HOME + '/' + '.'+prog+'rc';
    editline.loadHistory(this.historyFile);
    this.promptString = '> ';
//    this.handle = editline.init(prog, historySize || 50);
//    editline.prompt(this.handle, '> ');
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
        this.promptString = txt;
//        editline.prompt(this.handle, txt);
    },
    
    /**
     * @function stdin.gets
     * 
     * ### Synopsis
     * 
     * var line = stdin.gets(addHistory);
     * 
     * Read a line from stdin with prompt.
     *
     * @param {boolean} addHistory - set to false to not add input to history.
     * @return {string} line - text that user entered, or false if EOF (e.g. user hit ^D).
     */
    gets: function(addHistory) {
        if (addHistory === undefined || addHistory === true) {
            addHistory = true;
        }
        else {
            addHistory = false;
        }
        var ret = editline.gets(this.promptString, addHistory );
        if (ret === -1) {
            throw 'SIGINT';
        }
        else if (ret === -2) {
            throw 'SIGQUIT';
        }
        else if (ret === -3) {
            throw 'SIGHUP';
        }
        else if (ret === -4) {
            throw 'SIGTERM';
        }
        if (ret === false) {
            console.log('^D hit');
        }
        if (addHistory) {
            editline.saveHistory(this.historyFile);
        }
        return ret;
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
//        editline.end(this.handle);
    }
});

if (exports) {
    exports = ReadLine;
}
