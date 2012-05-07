/** @ignore */

"use strict";

var cairo = require('builtin/cairo');

var namedColors = [
//  name                     RRGGBBAA
    "transparent",          'ffffff00',
    "aliceblue",            'f0f8ffff',
    "antiquewhite",         'faebd7ff',
    "aqua",                 '00ffffff',
    "aquamarine",           '7fffd4ff',
    "azure",                'f0ffffff',
    "beige",                'f5f5dcff',
    "bisque",               'ffe4c4ff',
    "black",                '000000ff',
    "blanchedalmond",       'ffebcdff',
    "blue",                 '0000ffff',
    "blueviolet",           '8a2be2ff',
    "brown",                'a52a2aff',
    "burlywood",            'deb887ff',
    "cadetblue",            '5f9ea0ff',
    "chartreuse",           '7fff00ff',
    "chocolate",            'd2691eff',
    "coral",                'ff7f50ff',
    "cornflowerblue",       '6495edff',
    "cornsilk",             'fff8dcff',
    "crimson",              'dc143cff',
    "cyan",                 '00ffffff',
    "darkblue",             '00008bff',
    "darkcyan",             '008b8bff',
    "darkgoldenrod",        'b8860bff',
    "darkgray",             'a9a9a9ff',
    "darkgreen",            '006400ff',
    "darkgrey",             'a9a9a9ff',
    "darkkhaki",            'bdb76bff',
    "darkmagenta",          '8b008bff',
    "darkolivegreen",       '556b2fff',
    "darkorange",           'ff8c00ff',
    "darkorchid",           '9932ccff',
    "darkred",              '8b0000ff',
    "darksalmon",           'e9967aff',
    "darkseagreen",         '8fbc8fff',
    "darkslateblue",        '483d8bff',
    "darkslategray",        '2f4f4fff',
    "darkslategrey",        '2f4f4fff',
    "darkturquoise",        '00ced1ff',
    "darkviolet",           '9400d3ff',
    "deeppink",             'ff1493ff',
    "deepskyblue",          '00bfffff',
    "dimgray",              '696969ff',
    "dimgrey",              '696969ff',
    "dodgerblue",           '1e90ffff',
    "firebrick",            'b22222ff',
    "floralwhite",          'fffaf0ff',
    "forestgreen",          '228b22ff',
    "fuchsia",              'ff00ffff',
    "gainsboro",            'dcdcdcff',
    "ghostwhite",           'f8f8ffff',
    "gold",                 'ffd700ff',
    "goldenrod",            'daa520ff',
    "gray",                 '808080ff',
    "green",                '008000ff',
    "greenyellow",          'adff2fff',
    "grey",                 '808080ff',
    "honeydew",             'f0fff0ff',
    "hotpink",              'ff69b4ff',
    "indianred",            'cd5c5cff',
    "indigo",               '4b0082ff',
    "ivory",                'fffff0ff',
    "khaki",                'f0e68cff',
    "lavender",             'e6e6faff',
    "lavenderblush",        'fff0f5ff',
    "lawngreen",            '7cfc00ff',
    "lemonchiffon",         'fffacdff',
    "lightblue",            'add8e6ff',
    "lightcoral",           'f08080ff',
    "lightcyan",            'e0ffffff',
    "lightgoldenrodyellow", 'fafad2ff',
    "lightgray",            'd3d3d3ff',
    "lightgreen",           '90ee90ff',
    "lightgrey",            'd3d3d3ff',
    "lightpink",            'ffb6c1ff',
    "lightsalmon",          'ffa07aff',
    "lightseagreen",        '20b2aaff',
    "lightskyblue",         '87cefaff',
    "lightslategray",       '778899ff',
    "lightslategrey",       '778899ff',
    "lightsteelblue",       'b0c4deff',
    "lightyellow",          'ffffe0ff',
    "lime",                 '00ff00ff',
    "limegreen",            '32cd32ff',
    "linen",                'faf0e6ff',
    "magenta",              'ff00ffff',
    "maroon",               '800000ff',
    "mediumaquamarine",     '66cdaaff',
    "mediumblue",           '0000cdff',
    "mediumorchid",         'ba55d3ff',
    "mediumpurple",         '9370dbff',
    "mediumseagreen",       '3cb371ff',
    "mediumslateblue",      '7b68eeff',
    "mediumspringgreen",    '00fa9aff',
    "mediumturquoise",      '48d1ccff',
    "mediumvioletred",      'c71585ff',
    "midnightblue",         '191970ff',
    "mintcream",            'f5fffaff',
    "mistyrose",            'ffe4e1ff',
    "moccasin",             'ffe4b5ff',
    "navajowhite",          'ffdeadff',
    "navy",                 '000080ff',
    "oldlace",              'fdf5e6ff',
    "olive",                '808000ff',
    "olivedrab",            '6b8e23ff',
    "orange",               'ffa500ff',
    "orangered",            'ff4500ff',
    "orchid",               'da70d6ff',
    "palegoldenrod",        'eee8aaff',
    "palegreen",            '98fb98ff',
    "paleturquoise",        'afeeeeff',
    "palevioletred",        'db7093ff',
    "papayawhip",           'ffefd5ff',
    "peachpuff",            'ffdab9ff',
    "peru",                 'cd853fff',
    "pink",                 'ffc0cbff',
    "plum",                 'dda0ddff',
    "powderblue",           'b0e0e6ff',
    "purple",               '800080ff',
    "red",                  'ff0000ff',
    "rosybrown",            'bc8f8fff',
    "royalblue",            '4169e1ff',
    "saddlebrown",          '8b4513ff',
    "salmon",               'fa8072ff',
    "sandybrown",           'f4a460ff',
    "seagreen",             '2e8b57ff',
    "seashell",             'fff5eeff',
    "sienna",               'a0522dff',
    "silver",               'c0c0c0ff',
    "skyblue",              '87ceebff',
    "slateblue",            '6a5acdff',
    "slategray",            '708090ff',
    "slategrey",            '708090ff',
    "snow",                 'fffafaff',
    "springgreen",          '00ff7fff',
    "steelblue",            '4682b4ff',
    "tan",                  'd2b48cff',
    "teal",                 '008080ff',
    "thistle",              'd8bfd8ff',
    "tomato",               'ff6347ff',
    "turquoise",            '40e0d0ff',
    "violet",               'ee82eeff',
    "wheat",                'f5deb3ff',
    "white",                'ffffffff',
    "whitesmoke",           'f5f5f5ff',
    "yellow",               'ffff00ff',
    "yellowgreen",          '9acd32ff'
];

function parseHex(s) {
    var h = parseInt(s, 16);
    if (isNaN(h) || h < 0 || h > 255) {
        throw 'Invalid hex digits';
    }
    return h;
}
function parseDec(s) {
    var d = parseInt(s, 10);
    if (isNaN(d) || d < 0 || d > 255) {
        throw 'Invalid decimal digits';
    }
    return d;
}

(function() {
    var clen = namedColors.length;
    for (var i=1; i<clen; i+=2) {
        var c = namedColors[i];
        namedColors[i] = {
            r: parseHex(c.substr(0,2)),
            g: parseHex(c.substr(2,2)),
            b: parseHex(c.substr(4,2)),
            a: parseHex(c.substr(6,2))
        }
    }
}());


function parseColorHex(s) {
    if (s[0] !== '#') {
        return false;
    }
    try {
        switch (s.length) {
            case 4:             // #RGB
                return {
                    r: parseHex(s.substr(1,1)),
                    g: parseHex(s.substr(2,1)),
                    b: parseHex(s.substr(3,1)),
                    a: 255
                };
            case 7:             // #RRGGBB
                return {
                    r: parseHex(s.substr(1,2)),
                    g: parseHex(s.substr(3,2)),
                    b: parseHex(s.substr(5,2)),
                    a: 255
                };
            default:
                return false;
        }
    }
    catch (e) {
        return false;
    }
}

function parseColorRgb(s) {
    if (s.substr(0, 4) !== 'rgb(') {
        return false;
    }
    var colors = s.substr(4).replace(')', '').split(/\s+,\s+/);
    try {
        return {
            r: parseDec(colors[0]),
            g: parseDec(colors[1]),
            b: parseDec(colors[2]),
            a: 255
        };
    }
    catch (e) {
        return false;
    }
}

function parseColorRgba(s) {
    if (s.substr(0, 5) !== 'rgba(') {
        return false;
    }
    var colors = s.substr(5).replace(')', '').split(/\s+,\s+/);
    try {
        return {
            r: parseDec(colors[0]),
            g: parseDec(colors[1]),
            b: parseDec(colors[2]),
            a: parseInt(parseFloat(colors[3])*255, 10)
        };
    }
    catch (e) {
        return false;
    }
}

function parseColorName(s) {
    var ndx = namedColors.indexOf(s);
    if (ndx == -1) {
        return false;
    }
    return namedColors[ndx+1];
}

// parse color from string that looks like:
// #RGB
// #RRGGBB
// rgb(r,g,b)
// rgba(r,g,b,a)
// named color
function parseColor(s) {
    return parseColorHex(s) || parseColorRgb(s) || parseColorRgba(s) || parseColorName(s) || { r: 0, g: 0, b: 0, a: 255 };
}




function CanvasGradient(context, pattern) {
    this._context = context;
    this._pattern = pattern;
}
CanvasGradient.prototype.extend({
    addColorStop: function(offset, color) {
        color = parseColor(color);
        cairo.pattern_add_color_stop_rgba(this._pattern, offset, color.r, color.g, color.b, color.a);
    },
    destroy: function() {
        cairo.pattern_destroy(this._pattern);
    }
});

exports.extend({
    CanvasGradient: CanvasGradient,
    parseColor: parseColor
});

