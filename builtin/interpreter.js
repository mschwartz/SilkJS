var ReadLine = require('ReadLine'),
    console = require('console');
        
function main() {
    var stdin = new ReadLine('silkjs');
    stdin.prompt('SilkJS> ');
    while (1) {
        try {
            var line = stdin.gets();
            console.log(eval(line));
        }
        catch (e) {
            if (e === 'SIGQUIT') {
                break;
            }
            else if (e === 'SIGTERM') {
                break;
            }
            console.dir(e);
        }
    }
}
