/**
 * Concatenates the values of a variable into an easily readable string
 * by Matt Hackett [scriptnode.com]
 * @param {Object} x The variable to debug
 * @param {Number} max The maximum number of recursions allowed (keep low, around 5 for HTML elements to prevent errors) [default: 10]
 * @param {String} sep The separator to use between [default: a single space ' ']
 * @param {Number} l The current level deep (amount of recursion). Do not use this parameter: it's for the function's own use
 */
builtin.print_r = function(x, max, sep, l) {

	l = l || 0;
	max = max || 10;
	sep = sep || ' ';

	if (l > max) {
		return "[WARNING: Too much recursion]\n";
	}

	var
		i,
		r = '',
		t = typeof x,
		tab = '';

	if (x === null) {
		r += "(null)\n";
	} else if (t == 'object') {

		l++;

		for (i = 0; i < l; i++) {
			tab += sep;
		}

		if (x && x.length) {
			t = 'array';
		}

		r += '(' + t + ") :\n";

		for (i in x) {
			if (typeof x[i] == 'function' && i in Object.prototype) {
				continue;
			}
			try {
				r += tab + '[' + i + '] : ' + builtin.print_r(x[i], max, sep, (l + 1));
			} catch(e) {
				return "[ERROR: " + e + "]\n";
			}
		}

	} else {
		if (t == 'string') {
			if (x == '') {
				x = '(empty)';
			}
		}

		if (t == 'function') {
			if (x.toString().length > 64) {
				r += 'function() { ... }';
			}
			else {
				r += x.toString();
			}
		}
		else {
			r += '(' + t + ') ';
			r += x;
		}
		r +=  "\n";
	}

	return r;

}
