/**
 * Created by .
 * User: mschwartz
 * Date: 7/13/11
 * Time: 3:17 PM
 * To change this template use File | Settings | File Templates.
 */

function myTest() {
	res.contentType = 'text/html';
	for (var i=0; i<10; i++) {
		res.write('hello, world\n');
	}
	res.stop();
}

function mysqlTest() {
	var start = new Date().getTime();
	var rows = SQL.getDataRows('select * from STATS_1011');
	var elapsed = new Date().getTime() - start;
	res.write("Elapsed = " + elapsed);
	res.stop();
}
HttpChild.requestHandler = function() {
	switch (req.uri) {
		case '/test':
			myTest();
		case '/ls':
			var out = process.exec('ls -l');
			res.write('<pre>'+out+'</pre>');
			res.stop();
		case '/mysql':
			mysqlTest();
	}
}