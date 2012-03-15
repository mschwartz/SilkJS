/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
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
