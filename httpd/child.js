// httpd/child.js

HttpChild = (function() {
    var requestsHandled;

	function notFound() {
        global.notFound_action && global.notFound_action();
		res.reset();
		res.status = 404;
		res.write('<h1>Not Found</h1>');
		res.stop();
	}

	var jst_cache = {};
	function getCachedJst(fn) {
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
	
	var contentTypes = {
		sjs:	{ contentType: 'text/html',							handler: runSJS },
		less:	{ contentType: 'text/css',		                    handler: runLess },
		coffee:	{ contentType: 'text/html',		                    handler: runCoffee },
		jst:	{ contentType: 'text/html',                         handler: runJst },
		md:		{ contentType: 'text/html',                         handler: runMarkdown },
		ogg:	{ contentType: 'audio/ogg',                         handler: sendFile },
		mp3:	{ contentType: 'audio/mpeg3',                       handler: sendFile },
		png:	{ contentType: 'image/png',                         handler: sendFile },
		ico:	{ contentType: 'image/ico',                         handler: sendFile },
		gif:	{ contentType: 'image/gif',                         handler: sendFile },
		jpg:	{ contentType: 'image/jpeg'                  ,      handler: sendFile },
		jpeg:	{ contentType: 'image/jpeg',                        handler: sendFile },
		html:	{ contentType: 'text/html',                         handler: sendFile },
		js:		{ contentType: 'text/javascript',                   handler: sendFile },
		css:	{ contentType: 'text/css',                          handler: sendFile },
		xml:	{ contentType: 'text/xml',                          handler: sendFile },
        swf:    { contentType: 'application/ x-shockwave-flash',    handler: sendFile }
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
//            res.stop();
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
				log('redirect ' + req.uri + ' ' + fn + ' ' + fn.substr(fn.length-1, 1));
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

		req.script_path = fn.replace(/\index\..*$/, '').replace(fs.realpath(Config.documentRoot), '');
		res.status = 200;
        req.path = fn;
		parts = fn.split('.');
		if (parts.length > 1) {
			var extension = parts.pop();
			var handler = contentTypes[extension.toLowerCase()];
			if (handler) {
				res.contentType = handler.contentType;
				handler.handler(fn);
			}
		}
		var stat = fs.stat(fn);
		if (!stat) {
			log('error: ' + fs.error());
		}
		res.contentType = 'text/plain';
		res.sendHeaders();
		net.sendFile(res.sock, fn, 0, stat.size);
	}

    // semaphore for locking around accept()
    var USE_FLOCK = true;
    var lock = USE_FLOCK ? function(lockfd) { fs.flock(lockfd, fs.LOCK_EX) } : function(lockfd) { fs.lockf(lockfd, fs.F_LOCK); }
    var unlock = USE_FLOCK ? function(lockfd) { fs.flock(lockfd, fs.LOCK_UN) } : function(lockfd) { fs.lockf(lockfd, fs.F_ULOCK); }

	return {
		requestHandler: null,
		onStart: null,
		getCoffeeScript: function(fn) {
			return coffee_cache[fn];
		},
		run: function(serverSocket, pid) {
			HttpChild.onStart && HttpChild.onStart();
			// onStart is a better way for apps to initialize SQL
			if (Config.mysql) {
				SQL = new MySQL();
				SQL.connect();
			}
			var REQUESTS_PER_CHILD = Config.requestsPerChild;
			var requestHandler = HttpChild.requestHandler;
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
							Error.exceptionHandler(e);
						}
					}
                    req.data = {};
                    res.data = {};
					res.flush();
					// this logfile.write() reduces # requests/sec by 5000!
					logfile.write(req.remote_addr + ' ' + req.method + ' ' + req.uri + ' completed in ' + (new Date().getTime() - req.start) + '\n');
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
	
