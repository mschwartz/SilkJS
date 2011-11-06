/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


String.prototype.endsWith = function(suffix) {
    return this.indexOf(suffix, this.length - suffix.length) !== -1;
};

Function.prototype.forEach = function(object, block, context, protos) {
	if (object) {
		var key;
		for (key in object) {
			//(!!protos || object.hasOwnProperty(key)) && block.call(context || object, object[key], key, object);
			if (!!protos || (object.hasOwnProperty && object.hasOwnProperty(key))) {
				if (block.call(context || object, object[key], key, object) === false) {
					break;
				}
			}
		}
	}
};

forEach = function(object, block, context, deep) {
	function isNumber(v) {
		return typeof v === 'number' && isFinite(v);
	}
	
	context = context || object;
	if (object) {
		if (typeof block != "function") {
			throw new TypeError();
		}
		var resolve = Object;
		if (object instanceof Function) {
			// functions have a "length" property
			resolve = Function;

		}
		else if (object.forEach instanceof Function) {
			// the object implements a custom forEach method so use that
			return object.forEach(block, context);

		}
		else if (typeof object == "string") {
			// the object is a string
			resolve = String;

		}
		else if (object.length && isNumber(object.length)) {
			// the object is array-like
			resolve = Array;
		}
		return resolve.forEach(object, block, context, deep);
	}
	return undefined;
};

if (!Array.prototype.forEach) {
	Array.prototype.forEach = function(fun /* , thisp */) {
		var len = this.length;
		if (typeof fun != "function")
			throw new TypeError();

		var thisp = arguments[1];
		for (var i = 0; i < len; i++) {
			if (i in this) {
				if (fun.call(thisp, this[i], i, this) === false) {
					return;
				}
			}
		}
	};
}
