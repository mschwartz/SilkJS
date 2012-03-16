(function() {
	var fs = builtin.fs,
		v8 = builtin.v8;
	
	function locateFile(fn) {
		if (fs.isFile(fn)) {
            return fn;
        }
        if (fs.isFile(fn + '.js')) {
            return fn + '.js';
        }
        else if (fs.isFile(fn + '.coffee')) {
            return fn + '.coffee';
        }
		if (fn.substr(0,1) == '/' || fn.substr(0,2) == './' || fn.substr(0,3) == '../') {
			throw 'Could not locate include file ' + fn;
		}
		var paths = include.path;
		for (var i=0, len=paths.length; i<len; i++) {
			var path = paths[i];
			if (path.substr(path.length-1, 1) != '/') {
				path += '/';
			}
			path += fn;
			if (fs.isFile(path)) {
				return path;
			}
			if (fs.isFile(path + '.js')) {
				return path + '.js';
			}
			else if (fs.isFile(fn + '.coffee')) {
				return fn + '.coffee';
			}
		}
		throw Error('Could not locate include file ' + fn);
	}
	var suffix = '.coffee',
		suffixLen = suffix.length;
	function includeFile(fn) {
		fn = locateFile(fn);
		var contents = fs.readFile(fn);
		if (fn.indexOf(suffix, fn.length - suffixLen) !== -1) {
			contents = CoffeeScript.compile(contents, { bare: true });
// uncomment these lines to generate a listing of the translated CoffeeScript source
//			var lines = contents.split('\n');
//			var lineNum = 0;
//			var source = '';
//			lines.each(function(line) {
//				source += (++lineNum) + ': ' + line + '\n';
//			});
//
//			log('\n'+source)
		}
		var script = v8.compileScript(contents, fn);
		v8.runScript(script);
		v8.freeScript(script);
	}
	include = function() {
		var argLen = arguments.length;
		for (var i=0; i<argLen; i++) {
			includeFile(arguments[i]);
		}
	};
	include.path = [
		'./',
		'./coffeescript',
		'/usr/share/silkjs',
		'/usr/share/silkjs/coffeescript'
	];
}());
