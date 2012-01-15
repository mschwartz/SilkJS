/* 
 * program to test require
 */

include('lib/require.js');
include('lib/print_r.js');
var console = require('builtin/console');
println(print_r(console));
var foo = require('test_module').foo;

function main() {
	console.log(foo());
}
