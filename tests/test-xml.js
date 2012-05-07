var expat = require('builtin/expat');


var xml = require('XMLParser'),
    fs = require('fs'),
    console = require('console');

function main() {
    var content = fs.readFile('cd_catalog.xml');
    console.dir(xml.toObject(content));
}

