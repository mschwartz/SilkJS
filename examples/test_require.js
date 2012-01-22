/* 
 * program to test require
 */

include('lib/require.js');
var console = require('builtin/console');

function main() {
    var inc = require('increment').increment;
    var a = 1;
    console.log(inc(a)); // 2

    var foo = require('test_module').foo;
    console.log(foo());
}