function main() {
    x = 20;
    var script = v8.compileScript('x = {}; x.bar = 10; x.foo = function() { return 10; }; x', 'test');
//    var script = v8.compileScript(context, '10;', 'test');
    for (var xxx=0; xxx<100; xxx++) {
        var res = v8.runScript(script);
        log(res.foo());
        log(x);
    }
    v8.freeScript(script);
}

