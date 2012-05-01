// httpd/child.js

HttpChild = (function() {
    var requestsHandled;
    
    var mimeTypes = require('MimeTypes');

    function errorHandler(e) {
        var spaces = '                 ';
        function lineNumber(n) {
            n = '' + n;
            return spaces.substr(0, 8 - n.length) + n;
        }
        res.reset();
        if (global.error_action) {
            global.error_action(e);
            return;
        }
        res.status = 500;
        res.write([
            '<html>',
            '<head>',
            '<title>Software Exception</title>',
            '<style>',
            '.source {',
            '   padding: 5px;',
            '   border: 1px solid black;',
            '}',
            '.highlight {',
            '   background: red;',
            '   color: white;',
            '   font-weight: bold;',
            '}',
            '.lineNumber {',
            '   padding-right: 5px;',
            '   text-align: right;',
            '}',
            '.line {',
            '   padding-left: 5px;',
            '}',
            '</style>',
            '</head>',
            '</html>'
        ].join('\n'));
        res.write('<h1>Software Exception</h1>');
        res.write('<h3>' + e + '</h3>');
        var stack = e.stack;
        if (stack) {
            stack = stack.split('\n');
            stack.shift();

            var newStack = [];
            stack.each(function(line) {
                var parsed = line.replace(/^\s*at\s*/, '');
                var file = parsed.replace(/.*\s+\(/, '').replace(/\).*$/, '');
                var method = parsed.replace(/\s*\(.*$/, '');
                if (method === file) {
                    method = 'anonymous';
                }
                file = file.split(':');
                var lineNo = file[1];
                var column = file[2];
                file = file[0];
                if (require.isRequiredFile(file)) {
                    lineNo -= 6;
                }
                newStack.push('    at ' + method + ' ' + file + ':' + lineNo + ':' + column);
            });

            var line = stack[0];
            var parsed = line.replace(/^\s*at\s*/, '');
            var file = parsed.replace(/.*\s+\(/, '').replace(/\).*$/, '');
            file = file.split(':');
            var lineNo = file[1];
            var column = file[2];
            file = file[0];
            var method = parsed.replace(/\s*\(.*$/, '');
            res.write('<h2>Stack Trace</h2>');
            res.write('<pre>' + newStack.join('\n') + '</pre>');

            var content = fs.readFile(file);
            if (content) {
                res.write('<h2>' + file + '</h2>');
                var lines = content.split('\n');
                if (require.isRequiredFile(file)) {
                    lineNo -= 6;
                }
                var startLine = lineNo - 10;
                if (startLine < 0) {
                    startLine = 0;
                }
                var endLine = startLine + 10;
                if (endLine > lines.length) {
                    endLine = lines.length;
                }
                lineNo--;
                res.write('<pre class="source">');
                for (var i=startLine; i<endLine; i++) {
                    if (i == lineNo) {
                        res.write('<div class="highlight"><span class="lineNumber">' + lineNumber(i+1) + '</span><span class="line">' + lines[i] + '</span></div>');
                    }
                    else {
                        res.writeln('<span class="lineNumber">' + lineNumber(i+1) + '</span><span class="line">' + lines[i] + '</span>');
                    }
                }
                res.write('</pre>');
            }
            else {
                res.writeln('<div>Could not open ' + file + '</div>');
            }
//            res.write('<pre>' + content + '</pre>');
        }
    }

	function notFound() {
        res.reset();
        global.notFound_action && global.notFound_action();
		res.status = 404;
		res.write('<h1>Not Found</h1>');
		res.stop();
	}

	var jst_cache = {};
	function getCachedJst(fn) {
        if (!fs.exists(fn)) {
            throw 'Cannot find included .jst file ' + fn;
        }
		var jst = jst_cache[fn];
		if (!jst || fs.stat(fn).mtime < jst.mtime) {
			var source = fs.readFile(fn);
			jst = {
				mtime: fs.stat(fn).mtime,
				parsed: Jst.parse(source)
			};
			jst_cache[fn] = jst;
		}
		return jst.parsed;
	}

	function includeJst(fn) {
        if (fn[0] !== '/') {
            fn = '/' + fn;
        }
		var jst = getCachedJst(Config.documentRoot + fn);
		return Jst.includeParsed(jst, {
			include: includeJst
		});
	}

	function runJst(fn) {
		var jst = getCachedJst(fn);
		var out = Jst.executeParsed(jst, {
			include: includeJst
		});
		res.contentLength = out.length;
		res.write(out);
		res.stop();
	}

    function sendFile(fn) {
        res.sendFile(fn);
        res.stop();
    }

	var coffee_cache = {};
	function getCachedCoffee(fn) {
		var coffee = coffee_cache[fn];
		var mtime = fs.stat(fn).mtime;
		if (!coffee || mtime > coffee.mtime) {
			if (coffee) {
				v8.freeScript(coffee.script);
			}
			
			var source = fs.readFile(fn);
			var compiled = CoffeeScript.compile(source, fn);
			var script = v8.compileScript(compiled, fn);
			coffee = {
				mtime: mtime,
				source: source,
				compiled: compiled,
				script: script
			};
			coffee_cache[fn] = coffee;
		}
		return coffee.script;
	}
	
	function runCoffee(fn) {
		var coffee = getCachedCoffee(fn);
		v8.runScript(coffee);
		res.stop();
	}
	
	function runMarkdown(fn) {
		var content = fs.readFile(fn);
		var converter = new Showdown.converter();
		var html = converter.makeHtml(content);
		res.write(html);
		res.stop();
	}

	var less_cache = {};
	function getCachedLess(fn) {
		var cached = less_cache[fn];
		var mtime = fs.fileModified(fn);
		if (!cached || mtime > cached.mtime) {
			var less = require('less-silkjs-1.2.2');
			var content = fs.readFile(fn);
			var parser = new (less.Parser);
			parser.parse(content, function(e, root) {
				if (e) {
					console.dir(e);
				}
				else {
					cached = {
						mtime: mtime,
						css: root.toCSS()
					};
					less_cache[fn] = cached;
				}
			});
		}
		return cached.css;
	}
	function runLess(fn) {
		var css = getCachedLess(fn);
		res.write(css);
		res.stop();
	}

	var sjs_cache = {};
	function getCachedSJS(fn) {
		var cached = sjs_cache[fn];
		var mtime = fs.fileModified(fn);
		if (!cached || mtime > cached.mtime) {
			if (cached) {
				v8.freeScript(cached.script);
			}
			var content = fs.readFile(fn);
			content = [
				'(function() {',
//				'    var print = res.write;',
//				'	 var println = res.writeln;',
				content,
				'}());'
			].join('\n');
			var script;
			try {
				script = v8.compileScript(content, fn);
			}
			catch (e) {
				println(e + '<br/>');
				println(fn + '<br/>\n<pre>\n' + content.replace(/</igm, '&lt;'));
				console.dir(e);
				console.dir(e.prototype);
				console.dir(e.stack);
				res.stop();
			}
			cached = {
				mtime: mtime,
				script: script
			};
			sjs_cache[fn] = cached;
		}
		return cached.script;
	}
	
	function runSJS(fn) {
		var sjs = getCachedSJS(fn);
		v8.runScript(sjs);
		res.stop();
	}
	
	var dynamicHandlers = {
		sjs:	{ contentType: 'text/html',							handler: runSJS },
		less:	{ contentType: 'text/css',		                    handler: runLess },
		coffee:	{ contentType: 'text/html',		                    handler: runCoffee },
		jst:	{ contentType: 'text/html',                         handler: runJst },
		md:		{ contentType: 'text/html',                         handler: runMarkdown }
//		ogg:	{ contentType: 'audio/ogg',                         handler: sendFile },
//		mp3:	{ contentType: 'audio/mpeg3',                       handler: sendFile },
//		png:	{ contentType: 'image/png',                         handler: sendFile },
//		ico:	{ contentType: 'image/ico',                         handler: sendFile },
//		gif:	{ contentType: 'image/gif',                         handler: sendFile },
//		jpg:	{ contentType: 'image/jpeg'                  ,      handler: sendFile },
//		jpeg:	{ contentType: 'image/jpeg',                        handler: sendFile },
//		html:	{ contentType: 'text/html',                         handler: sendFile },
//		htm:	{ contentType: 'text/html',                         handler: sendFile },
//		js:		{ contentType: 'text/javascript',                   handler: sendFile },
//		css:	{ contentType: 'text/css',                          handler: sendFile },
//		xml:	{ contentType: 'text/xml',                          handler: sendFile },
//        swf:    { contentType: 'application/ x-shockwave-flash',    handler: sendFile }
	};

	function directoryIndex(fn) {
		var found = '';
		Config.directoryIndex.each(function(index) {
			var f = fn;
			f += '/';
			f += index;
			if (fs.exists(f)) {
				found = f;
				return false;
			}
		});
		return found;
	}
	function handleRequest() {
		req.script_path = req.uri;
		delete req.path_info;
		var parts = req.uri.substr(1).split('/');
        if (parts[0].length == 0) {
            parts[0] = 'main';
        }
        var action = parts[0] + '_action';
        if (global[action]) {
            global[action]();
            res.stop();
        }

		var fnPath = Config.documentRoot + req.uri;
		var fn = fs.realpath(fnPath);
		if (!fn) {
			fnPath = fs.realpath(Config.documentRoot + '/' + parts.shift());
			if (!fs.realpath(fnPath)) {
				notFound();
			}
			while (parts.length && fs.isDir(fnPath)) {
				var newPath = fnPath,
					part = parts.shift();
				newPath += '/';
				newPath += part;
				newPath = fs.realpath(newPath);
				if (!newPath) {
					parts.unshift(part);
					break;
				}
				fnPath = newPath;
			}
			fnPath = directoryIndex(fnPath);
			if (!fnPath) {
				notFound();
			}
			fn = fnPath;
			req.path_info = parts.join('/');
		}
		if (fs.isDir(fn)) {
			if (!req.uri.endsWith('/')) {
//				log('redirect ' + req.uri + ' ' + fn + ' ' + fn.substr(fn.length-1, 1));
				res.redirect(req.uri + '/');
			}
			var found = directoryIndex(fn);
			if (found) {
				fn = found;
			}
			else {
				fn += '/index.jst';
			}
		}
		if (!fs.isFile(fn)) {
			// extra path info
			notFound();
		}

		req.script_path = fn.replace(/index\..*$/, '').replace(fs.realpath(Config.documentRoot), '');
		res.status = 200;
        req.path = fn;
		parts = fn.split('.');
        var extension = parts.pop();
        var handler = dynamicHandlers[extension.toLowerCase()];
        if (handler) {
            res.contentType = handler.contentType;
            handler.handler(fn);
        }
        else {
//            if (!fs.exists(fn)) {
//                log('error: ' + fs.error());
//            }
            res.contentType = mimeTypes[extension] || 'text/plain';
//            res.sendHeaders();
            res.sendFile(fn);
            res.stop();
        }
	}

    // semaphore for locking around accept()
    var USE_FLOCK = true;
    var lock = USE_FLOCK ? function(lockfd) { fs.flock(lockfd, fs.LOCK_EX) } : function(lockfd) { fs.lockf(lockfd, fs.F_LOCK); }
    var unlock = USE_FLOCK ? function(lockfd) { fs.flock(lockfd, fs.LOCK_UN) } : function(lockfd) { fs.lockf(lockfd, fs.F_ULOCK); }

	return {
		requestHandler: null,   // called at start of each request
        endRequest: null,       // called at end of each request
		onStart: null,
		getCoffeeScript: function(fn) {
			return coffee_cache[fn];
		},
		run: function(serverSocket, pid) {
            // randomize the random number generator, just in case.
            var bits = pid % Config.numChildren;
            for (var b=0; b<bits; b++) {
                Math.random();
            }
            var logfile = global.logfile;
			HttpChild.onStart && HttpChild.onStart();
			// onStart is a better way for apps to initialize SQL
			if (Config.mysql) {
				SQL = new MySQL();
				SQL.connect();
			}
			var REQUESTS_PER_CHILD = Config.requestsPerChild;
			var requestHandler = HttpChild.requestHandler;
			var endRequest = HttpChild.endRequest;
			requestsHandled = 0;
			var lockfd = fs.open(Config.lockFile, fs.O_RDONLY);
			while (requestsHandled < REQUESTS_PER_CHILD) {
                lock(lockfd);
				var sock = net.accept(serverSocket);
                unlock(lockfd);
				var keepAlive = true;
				while (keepAlive) {
					if (++requestsHandled > REQUESTS_PER_CHILD) {
						keepAlive = false;
					}
                    var rstart = process.rusage();
					try {
						if (!req.init(sock)) {
							break;
						}
						keepAlive = res.init(sock, keepAlive, requestsHandled);
						// execute a pure JavaScript handler, if provided.
						if (requestHandler) {
							requestHandler();
						}
						handleRequest();
					}
					catch (e) {
						if (e !== 'RES.STOP') {
                            errorHandler(e);
//							Error.exceptionHandler(e);
						}
					}
                    endRequest && endRequest();
                    req.data = {};
                    res.data = {};
					res.flush();
                    res.reset();
					// this logfile.write() reduces # requests/sec by 5000!
                    var elapsed = ('' + (process.rusage().time - rstart.time)).substr(0,8);
					logfile.write(req.remote_addr + ' ' + req.method + ' ' + req.uri + ' completed in ' + elapsed + 's\n');
				}
				req.close();
				net.close(sock);
				v8.gc();
			}
			fs.close(lockfd);
			res.close();
		}
	};
})();
	
