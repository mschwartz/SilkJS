/*
 * CommonJS require 1.1 implementation
 */

(function() {
	var fs = builtin.fs;
	var v8 = builtin.v8;
	var console = builtin.console;

	function runScript(src, fn) {
//		try {
			var script = v8.compileScript(src, fn);
			var exports = v8.runScript(script);
			v8.freeScript(script);
			return exports;
//		}
//		catch (e) {
//			console.log(e);
//			if (e.stack) {
//				console.log(e.stack);
//			}
//			throw new Error('require failed due to compile errors');
//			console.log(builtin.print_r(e));
//			builtin.process.exit(1);
//		}
	}
	function locateFile(module) {
		function tryFile(path) {
			var tryPath = fs.realpath(path);
            if (tryPath) {
                if (fs.isFile(tryPath)) {
                    return tryPath;
                }
                else if (fs.isDir(tryPath)) {
                    if (!tryPath.endsWith('/')) {
                        tryPath += '/';
                    }
                    tryPath += 'index.js';
                }
                if (fs.isFile(tryPath)) {
                    return tryPath;
                }
            }
			tryPath = fs.realpath(require.fsPath + path);
			if (tryPath) {
                if (fs.isFile(tryPath)) {
                    return tryPath;
                }
                else if (fs.isDir(tryPath)) {
                    if (!tryPath.endsWith('/')) {
                        tryPath += '/';
                    }
                    tryPath += 'index.js';
                }
                if (fs.isFile(tryPath)) {
                    return tryPath;
                }
            }
			return false;
		}
		if (module.substr(0,1) == '/' || module.substr(0,2) == './' || module.substr(0,3) == '../') {
            var foundFile = tryFile(module) || tryFile(module + '.js') || tryFile(module + '.coffee') || tryFile(module + '.so');
            if (foundFile) {
                return foundFile;
            }
		}
		else {
			var paths = require.path;
			for (var i=0, len=paths.length; i<len; i++) {
				var path = paths[i];
				if (path.substr(path.length-1, 1) != '/') {
					path += '/';
				}
				path += module;
				var found = tryFile(path) || tryFile(path+'.js') || tryFile(path + '.coffee') || tryFile(path + '.so');
				if (found) {
					return found;
				}
			}
		}
		throw 'Could not locate require file ' + module;
	}
	// coffeescript support
	var coffeeRegEx = /\.coffee$/,
        soRegEx = /\.so/;

	function loadFile(modulePath) {
		var contents = fs.readFile(modulePath);
		if (coffeeRegEx.test(modulePath)) {
			contents = CoffeeScript.compile(contents, { bare: true });
		}
		return contents;
	}
	require = function(module) {
		if (module.substr(0, 8) == 'builtin/') {
			var m = builtin[module.substr(8)];
			return m;
		}
		var modulePath = locateFile(module);
		if (require.cache[modulePath]) {
			return require.cache[modulePath];
		}
        if (soRegEx.test(modulePath)) {
            require.cache[modulePath] = loadDll(modulePath);
            return require.cache[modulePath];
        }
		var content = loadFile(modulePath);
		require.dirStack.push(require.fsPath);
		var fsPath = modulePath.split('/');
		fsPath.pop();
		require.fsPath = fsPath.join('/') + '/';
		var exports = require.cache[modulePath] = {};
		var script = [
			'(function() {',
			'	var exports = {}', //  module.exports;',
			'	var module = {',
			'		id: "' + module + '",',
			'		url: "' + modulePath + '"',
			'	};',
			content,
			'	return exports;',
			'}())'
		].join('\n');
		// prepare function arguments
		var context = {};
		var _module = { id: module, uri: modulePath };
		require.cache[modulePath] = runScript(script, modulePath);
//		content += '\nreturn exports;';
//		var f = new Function("require", "exports", "module", content);
//		require.cache[modulePath] = f.call({}, require, exports, { id: module, uri: modulePath, exports: exports });
		require.fsPath = require.dirStack.pop();
		return require.cache[modulePath];
	};

    require.isRequiredFile = function(fn) {
        return require.cache[fn] ? true : false;
    },
    
	require.main = this;
	require.dirStack = [];
	require.fsPath = '';
	require.cache = {};
	require.path = [
		'./',
		'modules',
        '/usr/local/silkjs',
        '/usr/local/silkjs/contrib',
        '/usr/local/silkjs/modules',
		'/usr/share/silkjs/modules'
	];
}());
