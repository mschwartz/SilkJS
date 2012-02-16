/* 
 * CommonJS require 1.1 implementation
 */

(function() {
	var fs = builtin.fs;
	var v8 = builtin.v8;
	function runScript(src, fn) {
		var script = v8.compileScript(src, fn);
		var exports = v8.runScript(script);
		v8.freeScript(script);
		return exports;
	}
	function locateFile(module) {
		if (module.substr(0,1) == '/' || module.substr(0,2) == './' || module.substr(0,3) == '../') {
			if (fs.isFile(module)) {
				return module;
				if (fs.isFile(module + '.js')) {
					return module + '.js';
				}
				else if (fs.isFile(module + '.coffee')) {
					return module + '.coffee';
				}
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
				if (fs.isFile(path)) {
					return path;
				}
				if (fs.isFile(path + '.js')) {
					return path + '.js';
				}
				else if (fs.isFile(path + '.coffee')) {
					return path + '.coffee';
				}
			}
		}
		throw 'Could not locate require file ' + module;
	}
	// coffeescript support
	var suffix = '.coffee',
		suffixLen = suffix.length;

	function loadFile(modulePath) {
		var contents = fs.readFile(modulePath);
		if (modulePath.indexOf(suffix, modulePath.length - suffixLen) !== -1) {
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
		var script = [
			'(function() {',
			'	var module = {',
			'		id: "' + module + '",',
			'		exports: {}',
			'	},',
			'	exports = module.exports;',
			loadFile(modulePath),
			'	return exports;',
			'}())'
		].join('\n');
		require.cache[modulePath] = runScript(script, modulePath);
		return require.cache[modulePath];
	};

	require.cache = {};
	require.path = [
		'modules',
		'/usr/share/silkjs/modules'
	];
}());
