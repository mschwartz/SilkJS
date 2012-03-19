var o = {
	key1: 'value1',
	key2: 'value2',
	key3: [ 1,2,3],
	key4: { a: '1', b: 2, c: 3 }
};

var print_r = builtin.print_r;

function main() {
	println(print_r(o));
}

