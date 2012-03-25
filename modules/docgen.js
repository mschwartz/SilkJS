/**
 * @module docgen
 *
 * ### Synopsis
 * 
 * Documentation generator for SilkJS API.  Generates documentation from jsdoc-like comments in JS and C++ files.
 *  
 * ### Usage
 * 
 * var docgen = require('docgen');
 */

fs = require('builtin/fs');
console = require('console');
markdown = require('github-flavored-markdown').parse;

function readLine(lines) {
    if (!lines.length) {
        return false;
    }
    return lines.shift();

}

function readLineComment(lines) {
    var line = readLine(lines);
    if (line === false) {
        return false;
    }
    if (line.indexOf('*/') != -1) {
        return false;
    }
    line = line.replace(/^\s*\*\s/, '');
    line = line.replace(/^\s*\*$/, '');
    return line;
}

/**
 * @function processFile
 * 
 * ### Synopsis
 * 
 * var docgen = require('docgen');
 * var info = docgen(fn);
 * 
 * Reads a file and processes the docgen formatted coments.
 * 
 * @param {string} fn - path to source file to process
 * @return {object} info - object containing information for generating documentation
 */
function processFile(fn) {
    var items = [];
    var lines = fs.readFile(fn).split(/\n/),
    line,
    parts,
    tag,
    content,
    params;
    while ((line = readLine(lines)) !== false) {
        if (line === '/** @ignore */') {
            return [{
                tag: 'ignore'
            }];
        }
        //console.log('line: ' + line);
        if (line.search(/\s*\/\*\*/) == -1) {
            continue;
        }
        var item = {};
        content = '';
        params = false;
        while ((line = readLineComment(lines)) !== false) {
            if (line.search(/^\s*\@/) !== -1) {
                line = line.replace(/^\s*\@/, '');
                parts = line.split(/\s+/);
                tag = parts.shift().toLowerCase();
                switch (tag) {
                    case 'function':
					case 'constructor':
                    case 'method':
                    case 'class':
                    case 'singleton':
                    case 'namespace':
                    case 'module':
                        item.tag = tag;
                        item.name = parts.join(' ');
                        //                        content += '+ ' + parts.join(' ') + '\n';
                        break;
                    case 'param':
                        if (!params) {
                            content += '### Arguments\n';
                            params = true;
                        }
                        content += '+ ' + parts.join(' ') + '\n';
                        break;
                    case 'return':
                    case 'returns':
                        content += '### Returns\n';
                        content += '+ ' + parts.join(' ') + '\n';
                        break;
                    case 'constant':
                        item.tag = tag;
                        content += parts.join(' ') + '\n';
                        break;
                    default:
                        content += line + '\n';
                        break;
                }
            }
            else {
                content += line + '\n';
            }
        }
        item.content = markdown(content);
        items.push(item);
    }
    return items;
}

if (require.main !== module) {
    exports = processFile;
}
else {
    function main() {
        arguments.each(function(filename) {
            if (fs.isDir(filename)) {
                console.log('dir ' + filename);
            }
            else {
                var items = processFile(filename);
                console.dir(items);
            }
        });
    }
}
