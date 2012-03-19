Error.stackTraceLimit = 50;

Error.prepareStackTrace = function(error, structuredStackTrace) {
	var stack = [];
	structuredStackTrace.each(function(item) {
		stack.push({
			scriptName: item.getScriptNameOrSourceURL(),
			typeName: item.getTypeName(),
			functionName: item.getFunctionName(),
			methodName: item.getMethodName(),
			fileName: item.getFileName(),
			lineNumber: item.getLineNumber(),
			columnNumer: item.getColumnNumber(),
			evalOrgin: item.getEvalOrigin(),
			isTopLevel: item.isToplevel(),
			isEval: item.isEval(),
			isNative: item.isNative(),
			isConstructor: item.isConstructor()
		});
	});
	return stack;
}

var SQLException = function(msg, query) {
	Error.captureStackTrace(this, SQLException);
	this.name = 'SQL Error';
	this.message = msg;
	this.query = query;
};
SQLException.prototype.toString = function() {
	return this.message;
}

var SilkException = function(msg) {
	Error.captureStackTrace(this, SilkException);
	this.message = msg;
};
SilkException.prototype.toString = function() {
	return this.message;
};

function error(s) {
	try {
		throw new SilkException(s);
	}
	catch (e) {
		e.stack.shift();
		throw e;
	}
}

Error.exceptionHandler = function(e) {
    e.dump();
    var spaces = '        ';
    function formatLineNumber(n) {
        n = ''+n;
        return spaces.substr(0, 8 - n.length) + n;
    }
	var ex = e;
	var res = global.res || false;
	if (res) {
		res.reset();
		res.status = 500;
	}
	if (!e.message || !e.stack) {
		try {
			throw new SilkException(e);
		}
		catch (e) {
			ex = e;
		}
	}
	log(print_r(ex));
	if (res) {
		res.write([
			'<head>',
			'<title>Server Exception</title>',
			'<style>',
			'body {',
			'  padding: 10px;',
			'}',
			'h1, h2, h3, h4, h5, h6, {',
			'  padding: 0;',
			'  margin: 0;',
			'}',
			'</style>',
			'</head><body>',
			'<h1>Software Exception</h1>'
		].join('\n'));
	}
//	else {
		log('Server Exception');
//	}
	if (ex.name) {
		if (res) {
			res.write('<h2>'+ex.name+'</h2>\n');
		}
		log(ex.name);
	}
	if (res) {
		res.write('<b>'+ex.message+'</b>\n');
	}
//	log(ex.message);
//    log(ex.stack);
	if (e.query) {
		if (res) {
			res.write('<pre>'+ex.query+'</pre>\n');
		}
//		log(ex.query);
	}
	var filename = null,
		lineNumber = 0;
	if (ex.stack) {
		var stack = '',
			conStack = '';
		ex.stack.each(function(item) {
			filename = filename || item.fileName;
			lineNumber = lineNumber || item.lineNumber;
            if (stack == '' && require.isRequiredFile(filename)) {
                lineNumber -= 6;
                item.lineNumber -= 6;
            }
            stack += '<li>';
			conStack += '* ';
			stack += item.fileName + ':' + item.lineNumber;
			conStack += item.fileName + ':' + item.lineNumber;
			if (item.methodName) {
				stack += ' ('+item.functionName + ')';
				conStack += ' (' + item.functionName + ')';
			}
			if (res) {
				stack += '</li>';
			}
			conStack += '\n';
		});
		if (res) {
			res.write([
				'<h2>Stack Trace</h2>',
				'<ul>'+stack+'</ul>'
			].join('\n'));
		}
//		else {
			log('Stack Trace\n'+conStack);
//		}
	}
	
	if (res && filename) {
		res.write('<h2>' + filename + '</h2>');
		var fs = require('builtin/fs');
		var lines;
		if (global.HttpChild && filename.endsWith('.coffee')) {
			lines = HttpChild.getCoffeeScript(filename);
			if (lines) {
				lines = lines.compiled.split(/\n/);
			}
			else {
				lines = fs.readFile(filename).split(/\n/);
			}
		}
		else {
			lines = fs.readFile(filename);
            if (lines) {
                lines = lines.split(/\n/);
            }
            else {
                log('could not read ' + filename);
                lines = false;
            }
		}
        if (lines) {
            var startLine = lineNumber-10;
            if (startLine < 0) {
                startLine = 0;
            }
            var endLine = startLine + 21;
            if (endLine >= lines.length) {
                endLine = lines.length-1;
            }
            res.writeln('<pre style="border: 1px solid black; background: #efefef; padding: 5px;">');
            for (var lineNum=startLine; lineNum<endLine; lineNum++) {
                if (lineNum == lineNumber) {
                    res.write('<div style="background: red; color: white;">');
                }
                res.write('<span style="background: black; color: white;">' + formatLineNumber(lineNum) + '</span>');
                res.writeln(lines[lineNum-1].replace(/</igm, '&lt;'));
                if (lineNum == lineNumber) {
                    res.write('</div>');
                }
            }
            res.writeln('</pre>');
        }
	}
	
	if (res) {
		res.write([
			'</body></html>'
		].join('\n'));
	}
}

