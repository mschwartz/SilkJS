/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

include('lib/forEach.js');
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
	exec('cp SilkJS /usr/local/bin');
	exec('chmod 755 /usr/local/bin/SilkJS');

	println('-- copying examples to /usr/share/SilkJS/');
	exec('rm -rf /usr/share/SilkJS');
	exec('mkdir /usr/share/SilkJS');
	exec('cp -r examples/ /usr/share/SilkJS');
	println('-- copying httpd to /usr/share/silkJS/');
	exec('cp -r httpd/ /usr/share/SilkJS');
	println('-- copying SilkJS JavaScript library files to /usr/share/SilkJS/');
	exec('cp -r lib/ /usr/share/SilkJS');
	
	println('-- copying examples to /usr/local/bin');
	forEach(examples, function(example) {
		exec('cp examples/'+example+'  /usr/local/bin');
		exec('chmod 755 /usr/local/bin/'+example);
	});
	
	println('-- copying httpd/main.js to /usr/local/bin/httpd-silk.js');
	exec('cp httpd/main.js /usr/local/bin/httpd-silk.js');
	exec('chmod 755 /usr/local/bin/httpd-silk.js');
}