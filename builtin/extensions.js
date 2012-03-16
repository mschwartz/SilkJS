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

String.prototype.endsWith = function endsWith(c){
	if(this.charAt(this.length - 1) == c){
		return true;
	}
	else {
		return false;
	}
};

String.prototype.startsWith = function startsWith(c){
	if(this.charAt(0) == c){
		return true;
	}
	else {
		return false;
	}
};

String.prototype.replaceAll = function replaceAll(a, b){
	var s = this;
	while(s.indexOf(a) > -1){
		s = s.replace(a, b);
	}
	return s;
};

Object.prototype.extend = function() {
    var me = this;
    arguments.each(function(o) {
        for (var key in o) {
            me[key] = o[key];
        }
    });
};

Array.prototype.format = Object.prototype.format = function() {
    return builtin.print_r(this);
}

Array.prototype.dump = Object.prototype.dump = function(out) {
    out = out || println;
    out(this.format());
};
