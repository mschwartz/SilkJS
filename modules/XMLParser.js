var expat = require('builtin/expat'),
    console = require('console');

function XmlToObject(xml) {
    this.obj = this.currentObject = {};
    this.ancestors = [];
    var parser = expat.parser(this, this.startElement, this.endElement, this.text);
    expat.parse(parser, xml);
    expat.destroy(parser);
    return this.obj;
}

XmlToObject.prototype.extend({
    startElement: function(name, attrs) {
        if (!(name in this.currentObject)) {
            this.currentObject[name] = attrs;
        }
        else if (!(this.currentObject[name] instanceof Array)) {
            var newArray = [this.currentObject[name]];
            newArray.push(attrs);
            this.currentObject[name] = newArray;
        }
        else {
            this.currentObject[name].push(attrs);
        }
        this.ancestors.push(this.currentObject);
        if (this.currentObject[name] instanceof Array) {
            this.currentObject = this.currentObject[name][this.currentObject[name].length-1];
        }
        else {
            this.currentObject = this.currentObject[name];
        }
    },
    text: function(data) {
        data = data.trim();
        if (!data.length) {
            return;
        }
        this.currentObject['$t'] = (this.currentObject['$t'] || '') + data;
    },
    endElement: function(name) {
        var ancestor = this.ancestors.pop();
        if ((Object.keys(this.currentObject).length == 1) && ('$t' in this.currentObject)) {
            if (ancestor[name] instanceof Array) {
                ancestor[name].push(ancestor[name].pop()['$t']);
            } else {
                ancestor[name] = this.currentObject['$t'];
            }
        }
        this.currentObject = ancestor;
    }
});

exports.extend({
    toObject: function(xml) {
        return new XmlToObject(xml);
    },
    toJson: function(xml) {
        return JSON.stringify(new XmlToObject(xml));
    }
});
