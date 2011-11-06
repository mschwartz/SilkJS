/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
String.prototype.endsWith = function(suffix) {
    return this.indexOf(suffix, this.length - suffix.length) !== -1;
};



