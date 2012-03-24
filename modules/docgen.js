/**
 * @module docgen
 *
 * Documentation Generator
 * =======================
 *
 * Generate documentation from jsdoc-like comments in JS and C++ files.
 */

fs = require('builtin/fs');
console = require('console');
markdown = require('github-flavored-markdown').parse;

/**
 * @function readLine
 * `var line = readLine(lines);`
 *
 * Attempt to read the next line from a file.  The file has been split into an array
 * of lines.  Lines are removed from the front of the array as processed.
 *
 * @param lines {Array} remaining file content as array of lines
 * @return {mixed} next line or false
 */
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
					case '@ignore':
						item.tag = tag;
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
