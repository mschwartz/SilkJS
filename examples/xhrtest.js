/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

include('lib/forEach.js');
include('lib/Util.js');
include('lib/print_r.js');
include('lib/Exceptions.js');
include('lib/xhr.js');

function main() {
	res = {
		write: print
	}
	var xhr = new XMLHttpRequest();
	xhr.open('GET', 'https://google.com');
	xhr.send();
	log(Util.print_r(xhr));
//	print('here');
//	print(xhr.responseText);
//	print(print_r(xhr.responseText));
}