Error.stackTraceLimit = 50;

Error.prepareStackTrace = function(error, structuredStackTrace) {
	var stack = [];
	forstructuredStackTrace.each(function(item) {
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
	var ex = e;
	var res = global.res || false;
	if (res) {
		res.reset();
		res.status = 500;
	}
	if (!e.message) {
		try {
			throw new SilkException(e);
		}
		catch (e) {
			ex = e;
		}
	}
	if (res) {
		res.write([
			'<head><title>Server Exception</title></head><body>',
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
	log(ex.message);
	if (e.query) {
		if (res) {
			res.write('<pre>'+ex.query+'</pre>\n');
		}
		log(ex.query);
	}
	if (ex.stack) {
//		ex.stack.pop();
		var stack = '';
		ex.stack.each(function(item) {
			if (res) {
				stack += '<li>';
			}
			else {
				stack += '* ';
			}
			stack += item.fileName + ':' + item.lineNumber;
			if (item.methodName) {
				stack += ' ('+item.functionName + ')';
			}
			if (res) {
				stack += '</li>';
			}
			else {
				stack += '\n';
			}
		});
		if (res) {
			res.write([
				'<h2>Stack Trace</h2>',
				'<ul>'+stack+'</ul>'
			].join('\n'));
		}
//		else {
			log('Stack Trace');
			log(stack);
//		}
	}
	if (res) {
		res.write([
			'</body></html>'
		].join('\n'));
	}
}

