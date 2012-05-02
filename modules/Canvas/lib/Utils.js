/** @ignore */

var valid_lineCap = [ 'butt', 'round', 'square' ];
var valid_lineJoin = [ 'bevel', 'round', 'miter' ];
var valid_textAlign = [ 'start', 'end', 'left', 'right', 'center' ];
var valid_textBaseline = [ 'top', 'hanging', 'middle', 'alphabetic', 'ideographic', 'bottom' ];

var cache = {};

var weights = 'normal|bold|bolder|lighter|[1-9]00', 
    styles = 'normal|italic|oblique', 
    units = 'px|pt|pc|in|cm|mm|%', 
    string = '\'([^\']+)\'|"([^"]+)"|[\\w-]+';

var fontre = new RegExp('^ *'
    + '(?:(' + weights + ') *)?'
    + '(?:(' + styles + ') *)?'
    + '([\\d\\.]+)(' + units + ') *'
    + '((?:' + string + ')( *, *(?:' + string + '))*)'
);
 
 var parseFont = exports.parseFont = function(str) {
    var font = {}, 
    captures = fontre.exec(str);
    
    // Invalid
    if (!captures) {
        return false;
    }
    
    // Cached
    if (cache[str]) {
        return cache[str];
    }
    
    // Populate font object
    font.weight = captures[1] || 'normal';
    font.style = captures[2] || 'normal';
    font.size = parseFloat(captures[3]);
    font.unit = captures[4];
    font.family = captures[5].replace(/["']/g, '');
    
    // TODO: dpi
    // TODO: remaining unit conversion
    switch (font.unit) {
        case 'pt':
            font.size /= .75;
            break;
        case 'in':
            font.size *= 96;
            break;
        case 'mm':
            font.size *= 96.0 / 25.4;
            break;
        case 'cm':
            font.size *= 96.0 / 2.54;
            break;
    }
    
    return cache[str] = font;
};

exports.parseFont = parseFont;
