/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

process = require('builtin/process');

var examples = [
	'systat.js'
];

function exec(cmd) {
	println('- ' + cmd);
	var ret = process.exec(cmd);
	if (ret.length) {
		println(ret);
		exit(0);
	}
	
}
function main() {
	var uid = process.getuid();
	if (uid !== 0) {
		print('Must be run as root or via sudo\n');
		process.exit(0);
	}
	println('-- copying SilkJS /usr/local/bin');
	exec('cp silkjs /usr/local/bin');
	exec('chmod 755 /usr/local/bin/silkjs');

	println('-- copying examples to /usr/share/silkjs/');
	exec('rm -rf /usr/share/silkjs');
	exec('mkdir /usr/share/silkjs');
	exec('cp -r examples /usr/share/silkjs');
	println('-- copying httpd to /usr/share/silkjs/');
	exec('cp -r httpd /usr/share/silkjs');
	println('-- copying SilkJS CORE JavaScript library files to /usr/share/silkjs/');
	exec('cp -r builtin /usr/share/silkjs');
	println('-- copying SilkJS JavaScript library files to /usr/share/silkjs/');
	exec('cp -r lib /usr/share/silkjs');
	println('-- copying SilkJS JavaScript module files to /usr/share/silkjs/');
	exec('cp -r modules /usr/share/silkjs');

	println('-- copying examples to /usr/local/bin');
	examples.each(function(example) {
		exec('cp examples/'+example+'  /usr/local/bin');
		exec('chmod 755 /usr/local/bin/'+example);
	});
	
	println('-- copying httpd/main.js to /usr/local/bin/httpd-silk.js');
	exec('cp httpd/main.js /usr/local/bin/httpd-silk.js');
	exec('chmod 755 /usr/local/bin/httpd-silk.js');
}
