var MySQL = require('MySQL').MySQL;

var db = new MySQL();
db.connect('localhost', 'bar', 'baz', 'boo');

