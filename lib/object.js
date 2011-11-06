/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

Object.prototype.apply = function(src) {
	for (var i in src) {
		this[i] = src[i];
	}
};

Object.prototype.dump = function() {
	return print_r(this);
};

