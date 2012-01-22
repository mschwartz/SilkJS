/* 
 * Program to test XMLHttpRequest implementation
 */

include('lib/require.js');

include('lib/print_r.js');
include('lib/xhr.js');

function main() {
    var console = require('builtin/console');
	var xhr = new XMLHttpRequest();
	xhr.open('GET', 'https://google.com');
	xhr.send();
	console.log(print_r(xhr));
}
