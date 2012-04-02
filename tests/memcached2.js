/* 
 * Test JavaScript Memcached module/class.
 */

var Memcached = require('Memcached'),
	console = require('console');
	
function main() {
	var mc = new Memcached('localhost');
	mc.set('foo', {
		a: 'string',
		b: 10,
		c: [ 1,2,3 ],
		d: { x: 'aaa', y: 'bbb'}
	});
	console.dir(mc.get('foo'));
}
