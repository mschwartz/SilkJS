/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

var memcached = require('builtin/memcached'),
	console = require('console');

function main() {
	var handle = memcached.connect('localhost');
	if (!handle) {
		console.log('connect failed');
		return 0;
	}
	console.log('connected');
	memcached.add(handle, 'foo', 'bar');
	
	console.dir(memcached.get(handle, 'foo'));
}
