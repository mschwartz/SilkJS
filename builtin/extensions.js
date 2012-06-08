/*
 * @module extensions
 *
 * Provides handy extensions to builtin JS types
 */

Array.prototype.each = Object.prototype.each = Function.prototype.each = function(fun) {
	for (var key in this) {
		if (this.hasOwnProperty && this.hasOwnProperty(key)) {
			if (fun.call(this, this[key], key, this) === false) {
				return;
			}
		}
	}
};

String.prototype.endsWith = function(suffix) {
    return this.indexOf(suffix, this.length - suffix.length) !== -1;
};

String.prototype.trim = function() {
    return this.replace(/^\s+|\s+$/, '');
};

String.prototype.capitalize = function(limit) {
    if (limit === null) {
        limit = 1;
    }
    var head = this.substring(0, limit);
    var tail = this.substring(limit, this.length);
    return head.toUpperCase() + tail.toLowerCase();
};

String.prototype.endsWith = function endsWith(c) {
	if (this.charAt(this.length - 1) == c) {
		return true;
	}
	else {
		return false;
	}
};

String.prototype.startsWith = function startsWith(c) {
	if (this.charAt(0) == c) {
		return true;
	}
	else {
		return false;
	}
};

String.prototype.replaceAll = function replaceAll(a, b) {
	var s = this;
	while (s.indexOf(a) > -1) {
		s = s.replace(a, b);
	}
	return s;
};

Object.prototype.extend = Function.prototype.extend = function() {
    var me = this;
    arguments.each(function(o) {
        for (var key in o) {
            var g = o.__lookupGetter__(key), s = o.__lookupSetter__(key);
            if (g || s) {
                if (g) {
                    me.__defineGetter__(key, g);
                }
                if (s) {
                    me.__defineSetter__(key, s);
                }
            }
            else {
                me[key] = o[key];
            }
        }
    });
    return this;
};

//Array.prototype.format = Object.prototype.format = function(depth) {
//    return builtin.print_r(this, depth);
//};
//
//Array.prototype.dump = Object.prototype.dump = function(out, depth) {
//    out = out || println;
//    out(this.format(depth));
//};
