println('loading module test2.mod.js');

function a() {
	println('a');
}

// will not be available to caller of require()
function b() {
	println('private b');
}

function c() {
	b();	// is available privately to me
	println('c');
}

// export these 
exports.a = a;
exports.c = c;

