/** @ignore */

/**
 * @fileoverview
 *
 * <p>PHPJS - implementations of PHP core functions in JavaScript.</p>
 */

function array_chunk (input, size, preserve_keys) {
    // http://kevin.vanzonneveld.net
    // +   original by: Carlos R. L. Rodrigues (http://www.jsfromhell.com)
    // +   improved by: Brett Zamir (http://brett-zamir.me)
    // %        note 1: Important note: Per the ECMAScript specification, objects may not always iterate in a predictable order
    // *     example 1: array_chunk(['Kevin', 'van', 'Zonneveld'], 2);
    // *     returns 1: [['Kevin', 'van'], ['Zonneveld']]
    // *     example 2: array_chunk(['Kevin', 'van', 'Zonneveld'], 2, true);
    // *     returns 2: [{0:'Kevin', 1:'van'}, {2: 'Zonneveld'}]
    // *     example 3: array_chunk({1:'Kevin', 2:'van', 3:'Zonneveld'}, 2);
    // *     returns 3: [['Kevin', 'van'], ['Zonneveld']]
    // *     example 4: array_chunk({1:'Kevin', 2:'van', 3:'Zonneveld'}, 2, true);
    // *     returns 4: [{1: 'Kevin', 2: 'van'}, {3: 'Zonneveld'}]

    var x, p = '', i = 0, c = -1, l = input.length || 0, n = [];

    if (size < 1) {
        return null;
    }

    if (Object.prototype.toString.call(input) === '[object Array]') {
        if (preserve_keys) {
            while (i < l) {
                (x = i % size) ? n[c][i] = input[i] : n[++c] = {}, n[c][i] = input[i];
                i++;
            }
        }
        else {
            while (i < l) {
                (x = i % size) ? n[c][x] = input[i] : n[++c] = [input[i]];
                i++;
            }
        }
    }
    else {
        if (preserve_keys) {
            for (p in input) {
                if (input.hasOwnProperty(p)) {
                    (x = i % size) ? n[c][p] = input[p] : n[++c] = {}, n[c][p] = input[p];
                    i++;
                }
            }
        }
        else {
            for (p in input) {
                if (input.hasOwnProperty(p)) {
                    (x = i % size) ? n[c][x] = input[p] : n[++c] = [input[p]];
                    i++;
                }
            }
        }
    }
    return n;
}

function array_intersect () {
    // Returns the entries of arr1 that have values which are present in all the other arguments  
    // 
    // version: 1006.1915
    // discuss at: http://phpjs.org/functions/array_intersect
    // +   original by: Brett Zamir (http://brett-zamir.me)
    // %        note 1: These only output associative arrays (would need to be
    // %        note 1: all numeric and counting from zero to be numeric)
    // *     example 1: $array1 = {'a' : 'green', 0:'red', 1: 'blue'};
    // *     example 1: $array2 = {'b' : 'green', 0:'yellow', 1:'red'};
    // *     example 1: $array3 = ['green', 'red'];
    // *     example 1: $result = array_intersect($array1, $array2, $array3);
    // *     returns 1: {0: 'red', a: 'green'}
    var arr1 = arguments[0], retArr = {};
    var k1 = '', arr = {}, i = 0, k = '';
    
        arr1keys:
        for (k1 in arr1) {
            arrs:
            for (i=1; i < arguments.length; i++) {
                arr = arguments[i];
                for (k in arr) {
                    if (arr[k] === arr1[k1]) {
                        if (i === arguments.length-1) {
                            retArr[k1] = arr1[k1];
                        }
                        // If the innermost loop always leads at least once to an equal value, continue the loop until done
                        continue arrs;
                    }
                }
                // If it reaches here, it wasn't found in at least one array, so try next value
                continue arr1keys;
            }
        }
 
    return retArr;
}

function array_key_exists ( key, search ) {
    // Checks if the given key or index exists in the array  
    // 
    // version: 1004.2314
    // discuss at: http://phpjs.org/functions/array_key_exists
    // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   improved by: Felix Geisendoerfer (http://www.debuggable.com/felix)
    // *     example 1: array_key_exists('kevin', {'kevin': 'van Zonneveld'});
    // *     returns 1: true
    // input sanitation
    if (!search || (search.constructor !== Array && search.constructor !== Object)){
        return false;
    }
 
    return key in search;
}

function array_keys (input, search_value, argStrict) {
    // http://kevin.vanzonneveld.net
    // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +      input by: Brett Zamir (http://brett-zamir.me)
    // +   bugfixed by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // *     example 1: array_keys( {firstname: 'Kevin', surname: 'van Zonneveld'} );
    // *     returns 1: {0: 'firstname', 1: 'surname'}
    
    var tmp_arr = {}, strict = !!argStrict, include = true, cnt = 0;
    var key = '';
    
    for (key in input) {
        include = true;
        if (search_value != undefined) {
            if (strict && input[key] !== search_value){
                include = false;
            } else if (input[key] != search_value){
                include = false;
            }
        }
        
        if (include) {
            tmp_arr[cnt] = key;
            cnt++;
        }
    }
    
    return tmp_arr;
}

function array_unique (inputArr) {
    // http://kevin.vanzonneveld.net
    // +   original by: Carlos R. L. Rodrigues (http://www.jsfromhell.com)
    // +      input by: duncan
    // +   bugfixed by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   bugfixed by: Nate
    // +      input by: Brett Zamir (http://brett-zamir.me)
    // +   bugfixed by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   improved by: Michael Grier
    // +   bugfixed by: Brett Zamir (http://brett-zamir.me)
    // %          note 1: The second argument, sort_flags is not implemented;
    // %          note 1: also should be sorted (asort?) first according to docs
    // *     example 1: array_unique(['Kevin','Kevin','van','Zonneveld','Kevin']);
    // *     returns 1: {0: 'Kevin', 2: 'van', 3: 'Zonneveld'}
    // *     example 2: array_unique({'a': 'green', 0: 'red', 'b': 'green', 1: 'blue', 2: 'red'});
    // *     returns 2: {a: 'green', 0: 'red', 1: 'blue'}

    var key = '', tmp_arr2 = {}, val = '';

    var __array_search = function (needle, haystack) {
        var fkey = '';
        for (fkey in haystack) {
            if (haystack.hasOwnProperty(fkey)) {
                if ((haystack[fkey] + '') === (needle + '')) {
                    return fkey;
                }
            }
        }
        return false;
    };

    for (key in inputArr) {
        if (inputArr.hasOwnProperty(key)) {
            val = inputArr[key];
            if (false === __array_search(val, tmp_arr2)) {
                tmp_arr2[key] = val;
            }
        }
    }

    return tmp_arr2;
}

function basename (path, suffix) {
    // Returns the filename component of the path  
    // 
    // version: 1004.2314
    // discuss at: http://phpjs.org/functions/basename
    // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   improved by: Ash Searle (http://hexmen.com/blog/)
    // +   improved by: Lincoln Ramsay
    // +   improved by: djmix
    // *     example 1: basename('/www/site/home.htm', '.htm');
    // *     returns 1: 'home'
    // *     example 2: basename('ecra.php?p=1');
    // *     returns 2: 'ecra.php?p=1'
    var b = path.replace(/^.*[\/\\]/g, '');
    
    if (typeof(suffix) == 'string' && b.substr(b.length-suffix.length) == suffix) {
        b = b.substr(0, b.length-suffix.length);
    }
    
    return b;
}

/**
 * count(o)
 *
 * Simulates PHP's count() function
 * 
 * @param o {mixed} object, array, string, etc.
 * @returns {int} size
 *
 * size is number of members of an object, length of a string, length of an array, or 1 for others
 */

function count(o) {
    if (Util.isArray(o) || Util.isString(o)) {
        return o.length;
    }
    else if (Util.isObject(o)) {
        var n = 0;
        forEach(o, function(oo, key) {
            if (o.hasOwnProperty(key)) {
                n++;
            }
        });
        return n;
    }
    else {
        return 1;
    }
}

function dirname (path) {
    // Returns the directory name component of the path  
    // 
    // version: 1004.2314
    // discuss at: http://phpjs.org/functions/dirname
    // +   original by: Ozh
    // +   improved by: XoraX (http://www.xorax.info)
    // *     example 1: dirname('/etc/passwd');
    // *     returns 1: '/etc'
    // *     example 2: dirname('c:/Temp/x');
    // *     returns 2: 'c:/Temp'
    // *     example 3: dirname('/dir/test/');
    // *     returns 3: '/dir'
    
    return path.replace(/\\/g,'/').replace(/\/[^\/]*\/?$/, '');
}

function echo() {
    var arg = '', argc = arguments.length, argv = arguments, i = 0;
    for (i=0; i<argc; i++) {
        arg = argv[i];
        res.write(arg);
    }
}

function empty (mixed_var) {
    // !No description available for empty. @php.js developers: Please update the function summary text file.
    // 
    // version: 1004.2314
    // discuss at: http://phpjs.org/functions/empty
    // +   original by: Philippe Baumann
    // +      input by: Onno Marsman
    // +   bugfixed by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +      input by: LH
    // +   improved by: Onno Marsman
    // +   improved by: Francesco
    // +   improved by: Marc Jansen
    // +   input by: Stoyan Kyosev (http://www.svest.org/)
    // *     example 1: empty(null);
    // *     returns 1: true
    // *     example 2: empty(undefined);
    // *     returns 2: true
    // *     example 3: empty([]);
    // *     returns 3: true
    // *     example 4: empty({});
    // *     returns 4: true
    // *     example 5: empty({'aFunc' : function () { alert('humpty'); } });
    // *     returns 5: false
    
    var key;
    
    if (mixed_var === "" ||
        mixed_var === 0 ||
        mixed_var === "0" ||
        mixed_var === null ||
        mixed_var === false ||
        typeof mixed_var === 'undefined'
        ){
        return true;
    }
 
    if (typeof mixed_var == 'object') {
        for (key in mixed_var) {
            if (mixed_var.hasOwnProperty(key)) {
                return false;
            }
        }
        return true;
    }
 
    return false;
}

function file_exists(path) {
    var fs = require('fs');
    return fs.exists(path);
}

function is_null (mixed_var) {
    // Returns true if variable is null  
    // 
    // version: 1004.2314
    // discuss at: http://phpjs.org/functions/is_null
    // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // *     example 1: is_null('23');
    // *     returns 1: false
    // *     example 2: is_null(null);
    // *     returns 2: true
    return ( mixed_var === null );
}

function is_numeric (mixed_var) {
    // Returns true if value is a number or a numeric string  
    // 
    // version: 1004.2314
    // discuss at: http://phpjs.org/functions/is_numeric
    // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   improved by: David
    // +   improved by: taith
    // +   bugfixed by: Tim de Koning
    // +   bugfixed by: WebDevHobo (http://webdevhobo.blogspot.com/)
    // +   bugfixed by: Brett Zamir (http://brett-zamir.me)
    // *     example 1: is_numeric(186.31);
    // *     returns 1: true
    // *     example 2: is_numeric('Kevin van Zonneveld');
    // *     returns 2: false
    // *     example 3: is_numeric('+186.31e2');
    // *     returns 3: true
    // *     example 4: is_numeric('');
    // *     returns 4: false
    // *     example 4: is_numeric([]);
    // *     returns 4: false
    return (typeof(mixed_var) === 'number' || typeof(mixed_var) === 'string') && mixed_var !== '' && !isNaN(mixed_var);
}

function isset () {
    // !No description available for isset. @php.js developers: Please update the function summary text file.
    // 
    // version: 1004.2314
    // discuss at: http://phpjs.org/functions/isset
    // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   improved by: FremyCompany
    // +   improved by: Onno Marsman
    // *     example 1: isset( undefined, true);
    // *     returns 1: false
    // *     example 2: isset( 'Kevin van Zonneveld' );
    // *     returns 2: true
    
    var a=arguments, l=a.length, i=0;
    
    if (l===0) {
        error('Empty isset');
    }
    
    while (i!==l) {
        if (typeof(a[i])=='undefined' || a[i]===null) { 
            return false; 
        } else { 
            i++; 
        }
    }
    return true;
}

function lcfirst (str) {
    // http://kevin.vanzonneveld.net
    // +   original by: Brett Zamir (http://brett-zamir.me)
    // *     example 1: lcfirst('Kevin Van Zonneveld');
    // *     returns 1: 'kevin Van Zonneveld'

    str += '';
    var f = str.charAt(0).toLowerCase();
    return f + str.substr(1);
}

function ltrim ( str, charlist ) {
    // http://kevin.vanzonneveld.net
    // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +      input by: Erkekjetter
    // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   bugfixed by: Onno Marsman
    // *     example 1: ltrim('    Kevin van Zonneveld    ');
    // *     returns 1: 'Kevin van Zonneveld    '

    charlist = !charlist ? ' \\s\u00A0' : (charlist+'').replace(/([\[\]\(\)\.\?\/\*\{\}\+\$\^\:])/g, '$1');
    var re = new RegExp('^[' + charlist + ']+', 'g');
    return (str+'').replace(re, '');
}

function number_format(number, decimals, dec_point, thousands_sep) {
    // http://kevin.vanzonneveld.net
    // +   original by: Jonas Raoni Soares Silva (http://www.jsfromhell.com)
    // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +     bugfix by: Michael White (http://getsprink.com)
    // +     bugfix by: Benjamin Lupton
    // +     bugfix by: Allan Jensen (http://www.winternet.no)
    // +    revised by: Jonas Raoni Soares Silva (http://www.jsfromhell.com)
    // +     bugfix by: Howard Yeend
    // +    revised by: Luke Smith (http://lucassmith.name)
    // +     bugfix by: Diogo Resende
    // +     bugfix by: Rival
    // +      input by: Kheang Hok Chin (http://www.distantia.ca/)
    // +   improved by: davook
    // +   improved by: Brett Zamir (http://brett-zamir.me)
    // +      input by: Jay Klehr
    // +   improved by: Brett Zamir (http://brett-zamir.me)
    // +      input by: Amir Habibi (http://www.residence-mixte.com/)
    // +     bugfix by: Brett Zamir (http://brett-zamir.me)
    // +   improved by: Theriault
    // *     example 1: number_format(1234.56);
    // *     returns 1: '1,235'
    // *     example 2: number_format(1234.56, 2, ',', ' ');
    // *     returns 2: '1 234,56'
    // *     example 3: number_format(1234.5678, 2, '.', '');
    // *     returns 3: '1234.57'
    // *     example 4: number_format(67, 2, ',', '.');
    // *     returns 4: '67,00'
    // *     example 5: number_format(1000);
    // *     returns 5: '1,000'
    // *     example 6: number_format(67.311, 2);
    // *     returns 6: '67.31'
    // *     example 7: number_format(1000.55, 1);
    // *     returns 7: '1,000.6'
    // *     example 8: number_format(67000, 5, ',', '.');
    // *     returns 8: '67.000,00000'
    // *     example 9: number_format(0.9, 0);
    // *     returns 9: '1'
    // *    example 10: number_format('1.20', 2);
    // *    returns 10: '1.20'
    // *    example 11: number_format('1.20', 4);
    // *    returns 11: '1.2000'
    // *    example 12: number_format('1.2000', 3);
    // *    returns 12: '1.200'
    var n = !isFinite(+number) ? 0 : +number, 
    prec = !isFinite(+decimals) ? 0 : Math.abs(decimals),
    sep = (typeof thousands_sep === 'undefined') ? ',' : thousands_sep,
    dec = (typeof dec_point === 'undefined') ? '.' : dec_point,
    s = '',
    toFixedFix = function (n, prec) {
        var k = Math.pow(10, prec);
        return '' + Math.round(n * k) / k;
    };
    // Fix for IE parseFloat(0.55).toFixed(0) = 0;
    s = (prec ? toFixedFix(n, prec) : '' + Math.round(n)).split('.');
    if (s[0].length > 3) {
        s[0] = s[0].replace(/\B(?=(?:\d{3})+(?!\d))/g, sep);
    }
    if ((s[1] || '').length < prec) {
        s[1] = s[1] || '';
        s[1] += new Array(prec - s[1].length + 1).join('0');
    }
    return s.join(dec);
}

function pathinfo (path, options) {
    // http://kevin.vanzonneveld.net
    // +   original by: Nate
    // +    revised by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +    improved by: Brett Zamir (http://brett-zamir.me)
    // %        note 1: Inspired by actual PHP source: php5-5.2.6/ext/standard/string.c line #1559
    // %        note 1: The way the bitwise arguments are handled allows for greater flexibility
    // %        note 1: & compatability. We might even standardize this code and use a similar approach for
    // %        note 1: other bitwise PHP functions
    // %        note 2: php.js tries very hard to stay away from a core.js file with global dependencies, because we like
    // %        note 2: that you can just take a couple of functions and be on your way.
    // %        note 2: But by way we implemented this function, if you want you can still declare the PATHINFO_*
    // %        note 2: yourself, and then you can use: pathinfo('/www/index.html', PATHINFO_BASENAME | PATHINFO_EXTENSION);
    // %        note 2: which makes it fully compliant with PHP syntax.
    // -    depends on: dirname
    // -    depends on: basename
    // *     example 1: pathinfo('/www/htdocs/index.html', 1);
    // *     returns 1: '/www/htdocs'
    // *     example 2: pathinfo('/www/htdocs/index.html', 'PATHINFO_BASENAME');
    // *     returns 2: 'index.html'
    // *     example 3: pathinfo('/www/htdocs/index.html', 'PATHINFO_EXTENSION');
    // *     returns 3: 'html'
    // *     example 4: pathinfo('/www/htdocs/index.html', 'PATHINFO_FILENAME');
    // *     returns 4: 'index'
    // *     example 5: pathinfo('/www/htdocs/index.html', 2 | 4);
    // *     returns 5: {basename: 'index.html', extension: 'html'}
    // *     example 6: pathinfo('/www/htdocs/index.html', 'PATHINFO_ALL');
    // *     returns 6: {dirname: '/www/htdocs', basename: 'index.html', extension: 'html', filename: 'index'}
    // *     example 7: pathinfo('/www/htdocs/index.html');
    // *     returns 7: {dirname: '/www/htdocs', basename: 'index.html', extension: 'html', filename: 'index'}

    // Working vars
    var opt = '', optName = '', optTemp = 0, tmp_arr = {}, cnt = 0, i=0;
    var have_basename = false, have_extension = false, have_filename = false;

    // Input defaulting & sanitation
    if (!path) {
        return false;
    }
    if (!options) {
        options = 'PATHINFO_ALL';
    }

    // Initialize binary arguments. Both the string & integer (constant) input is
    // allowed
    var OPTS = {
        'PATHINFO_DIRNAME': 1,
        'PATHINFO_BASENAME': 2,
        'PATHINFO_EXTENSION': 4,
        'PATHINFO_FILENAME': 8,
        'PATHINFO_ALL': 0
    };
    // PATHINFO_ALL sums up all previously defined PATHINFOs (could just pre-calculate)
    for (optName in OPTS) {
        OPTS.PATHINFO_ALL = OPTS.PATHINFO_ALL | OPTS[optName];
    }
    if (typeof options !== 'number') { // Allow for a single string or an array of string flags
        options = [].concat(options);
        for (i=0; i < options.length; i++) {
            // Resolve string input to bitwise e.g. 'PATHINFO_EXTENSION' becomes 4
            if (OPTS[options[i]]) {
                optTemp = optTemp | OPTS[options[i]];
            }
        }
        options = optTemp;
    }

    // Internal Functions
    var __getExt = function (path) {
        var str  = path+'';
        var dotP = str.lastIndexOf('.')+1;
        return str.substr(dotP);
    };


    // Gather path infos
    if (options & OPTS.PATHINFO_DIRNAME) {
        tmp_arr.dirname = this.dirname(path);
    }

    if (options & OPTS.PATHINFO_BASENAME) {
        if (false === have_basename) {
            have_basename = this.basename(path);
        }
        tmp_arr.basename = have_basename;
    }

    if (options & OPTS.PATHINFO_EXTENSION) {
        if (false === have_basename) {
            have_basename = this.basename(path);
        }
        if (false === have_extension) {
            have_extension = __getExt(have_basename);
        }
        tmp_arr.extension = have_extension;
    }

    if (options & OPTS.PATHINFO_FILENAME) {
        if (false === have_basename) {
            have_basename = this.basename(path);
        }
        if (false === have_extension) {
            have_extension = __getExt(have_basename);
        }
        if (false === have_filename) {
            have_filename  = have_basename.substr(0, (have_basename.length - have_extension.length)-1);
        }

        tmp_arr.filename = have_filename;
    }


    // If array contains only 1 element: return string
    cnt = 0;
    for (opt in tmp_arr){
        cnt++;
    }
    if (cnt == 1) {
        return tmp_arr[opt];
    }

    // Return full-blown array
    return tmp_arr;
}

function rtrim ( str, charlist ) {
    // http://kevin.vanzonneveld.net
    // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +      input by: Erkekjetter
    // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   bugfixed by: Onno Marsman
    // +   input by: rem
    // +   bugfixed by: Brett Zamir (http://brett-zamir.me)
    // *     example 1: rtrim('    Kevin van Zonneveld    ');
    // *     returns 1: '    Kevin van Zonneveld'

    charlist = !charlist ? ' \\s\u00A0' : (charlist+'').replace(/([\[\]\(\)\.\?\/\*\{\}\+\$\^\:])/g, '\\$1');
    var re = new RegExp('[' + charlist + ']+$', 'g');
    return (str+'').replace(re, '');
}

function serialize (mixed_value) {
    // http://kevin.vanzonneveld.net
    // +   original by: Arpad Ray (mailto:arpad@php.net)
    // +   improved by: Dino
    // +   bugfixed by: Andrej Pavlovic
    // +   bugfixed by: Garagoth
    // +      input by: DtTvB (http://dt.in.th/2008-09-16.string-length-in-bytes.html)
    // +   bugfixed by: Russell Walker (http://www.nbill.co.uk/)
    // +   bugfixed by: Jamie Beck (http://www.terabit.ca/)
    // +      input by: Martin (http://www.erlenwiese.de/)
    // +   bugfixed by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // -    depends on: utf8_encode
    // %          note: We feel the main purpose of this function should be to ease the transport of data between php & js
    // %          note: Aiming for PHP-compatibility, we have to translate objects to arrays
    // *     example 1: serialize(['Kevin', 'van', 'Zonneveld']);
    // *     returns 1: 'a:3:{i:0;s:5:"Kevin";i:1;s:3:"van";i:2;s:9:"Zonneveld";}'
    // *     example 2: serialize({firstName: 'Kevin', midName: 'van', surName: 'Zonneveld'});
    // *     returns 2: 'a:3:{s:9:"firstName";s:5:"Kevin";s:7:"midName";s:3:"van";s:7:"surName";s:9:"Zonneveld";}'

    var _getType = function (inp) {
        var type = typeof inp, match;
        var key;
        if (type == 'object' && !inp) {
            return 'null';
        }
        if (type == "object") {
            if (!inp.constructor) {
                return 'object';
            }
            var cons = inp.constructor.toString();
            match = cons.match(/(\w+)\(/);
            if (match) {
                cons = match[1].toLowerCase();
            }
            var types = ["boolean", "number", "string", "array"];
            for (key in types) {
                if (cons == types[key]) {
                    type = types[key];
                    break;
                }
            }
        }
        return type;
    };
    var type = _getType(mixed_value);
    var val, ktype = '';
    
    switch (type) {
        case "function":
            val = ""; 
            break;
        case "boolean":
            val = "b:" + (mixed_value ? "1" : "0");
            break;
        case "number":
            val = (Math.round(mixed_value) == mixed_value ? "i" : "d") + ":" + mixed_value;
            break;
        case "string":
            mixed_value = this.utf8_encode(mixed_value);
            val = "s:" + encodeURIComponent(mixed_value).replace(/%../g, 'x').length + ":\"" + mixed_value + "\"";
            break;
        case "array":
        case "object":
            val = "a";
            /*
            if (type == "object") {
                var objname = mixed_value.constructor.toString().match(/(\w+)\(\)/);
                if (objname == undefined) {
                    return;
                }
                objname[1] = this.serialize(objname[1]);
                val = "O" + objname[1].substring(1, objname[1].length - 1);
            }
            */
            var count = 0;
            var vals = "";
            var okey;
            var key;
            for (key in mixed_value) {
                ktype = _getType(mixed_value[key]);
                if (ktype == "function") { 
                    continue; 
                }
                
                okey = (key.match(/^[0-9]+$/) ? parseInt(key, 10) : key);
                vals += this.serialize(okey) +
                this.serialize(mixed_value[key]);
                count++;
            }
            val += ":" + count + ":{" + vals + "}";
            break;
        case "undefined": // Fall-through
        default: // if the JS object has a property which contains a null value, the string cannot be unserialized by PHP
            val = "N";
            break;
    }
    if (type != "object" && type != "array") {
        val += ";";
    }
    return val;
}

function sprintf ( ) {
    // Return a formatted string  
    // 
    // version: 1004.2314
    // discuss at: http://phpjs.org/functions/sprintf
    // +   original by: Ash Searle (http://hexmen.com/blog/)
    // + namespaced by: Michael White (http://getsprink.com)
    // +    tweaked by: Jack
    // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +      input by: Paulo Ricardo F. Santos
    // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +      input by: Brett Zamir (http://brett-zamir.me)
    // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // *     example 1: sprintf("%01.2f", 123.1);
    // *     returns 1: 123.10
    // *     example 2: sprintf("[%10s]", 'monkey');
    // *     returns 2: '[    monkey]'
    // *     example 3: sprintf("[%'#10s]", 'monkey');
    // *     returns 3: '[####monkey]'
    var regex = /%%|%(\d+\$)?([-+\'#0 ]*)(\*\d+\$|\*|\d+)?(\.(\*\d+\$|\*|\d+))?([scboxXuidfegEG])/g;
    var a = arguments, i = 0, format = a[i++];
 
    // pad()
    var pad = function (str, len, chr, leftJustify) {
        if (!chr) {
            chr = ' ';
        }
        var padding = (str.length >= len) ? '' : Array(1 + len - str.length >>> 0).join(chr);
        return leftJustify ? str + padding : padding + str;
    };
 
    // justify()
    var justify = function (value, prefix, leftJustify, minWidth, zeroPad, customPadChar) {
        var diff = minWidth - value.length;
        if (diff > 0) {
            if (leftJustify || !zeroPad) {
                value = pad(value, minWidth, customPadChar, leftJustify);
            } else {
                value = value.slice(0, prefix.length) + pad('', diff, '0', true) + value.slice(prefix.length);
            }
        }
        return value;
    };
 
    // formatBaseX()
    var formatBaseX = function (value, base, prefix, leftJustify, minWidth, precision, zeroPad) {
        // Note: casts negative numbers to positive ones
        var number = value >>> 0;
        prefix = prefix && number && {
            '2': '0b', 
            '8': '0', 
            '16': '0x'
        }
        [base] || '';
        value = prefix + pad(number.toString(base), precision || 0, '0', false);
        return justify(value, prefix, leftJustify, minWidth, zeroPad);
    };
 
    // formatString()
    var formatString = function (value, leftJustify, minWidth, precision, zeroPad, customPadChar) {
        if (precision != null) {
            value = value.slice(0, precision);
        }
        return justify(value, '', leftJustify, minWidth, zeroPad, customPadChar);
    };
 
    // doFormat()
    var doFormat = function (substring, valueIndex, flags, minWidth, _, precision, type) {
        var number;
        var prefix;
        var method;
        var textTransform;
        var value;
 
        if (substring == '%%') {
            return '%';
        }
 
        // parse flags
        var leftJustify = false, positivePrefix = '', zeroPad = false, prefixBaseX = false, customPadChar = ' ';
        var flagsl = flags.length;
        for (var j = 0; flags && j < flagsl; j++) {
            switch (flags.charAt(j)) {
                case ' ':
                    positivePrefix = ' ';
                    break;
                case '+':
                    positivePrefix = '+';
                    break;
                case '-':
                    leftJustify = true;
                    break;
                case "'":
                    customPadChar = flags.charAt(j+1);
                    break;
                case '0':
                    zeroPad = true;
                    break;
                case '#':
                    prefixBaseX = true;
                    break;
            }
        }
 
        // parameters may be null, undefined, empty-string or real valued
        // we want to ignore null, undefined and empty-string values
        if (!minWidth) {
            minWidth = 0;
        } else if (minWidth == '*') {
            minWidth = +a[i++];
        } else if (minWidth.charAt(0) == '*') {
            minWidth = +a[minWidth.slice(1, -1)];
        } else {
            minWidth = +minWidth;
        }
 
        // Note: undocumented perl feature:
        if (minWidth < 0) {
            minWidth = -minWidth;
            leftJustify = true;
        }
 
        if (!isFinite(minWidth)) {
            error('sprintf: (minimum-)width must be finite');
        }
 
        if (!precision) {
            precision = 'fFeE'.indexOf(type) > -1 ? 6 : (type == 'd') ? 0 : undefined;
        } else if (precision == '*') {
            precision = +a[i++];
        } else if (precision.charAt(0) == '*') {
            precision = +a[precision.slice(1, -1)];
        } else {
            precision = +precision;
        }
 
        // grab value using valueIndex if required?
        value = valueIndex ? a[valueIndex.slice(0, -1)] : a[i++];
 
        switch (type) {
            case 's':
                return formatString(String(value), leftJustify, minWidth, precision, zeroPad, customPadChar);
            case 'c':
                return formatString(String.fromCharCode(+value), leftJustify, minWidth, precision, zeroPad);
            case 'b':
                return formatBaseX(value, 2, prefixBaseX, leftJustify, minWidth, precision, zeroPad);
            case 'o':
                return formatBaseX(value, 8, prefixBaseX, leftJustify, minWidth, precision, zeroPad);
            case 'x':
                return formatBaseX(value, 16, prefixBaseX, leftJustify, minWidth, precision, zeroPad);
            case 'X':
                return formatBaseX(value, 16, prefixBaseX, leftJustify, minWidth, precision, zeroPad).toUpperCase();
            case 'u':
                return formatBaseX(value, 10, prefixBaseX, leftJustify, minWidth, precision, zeroPad);
            case 'i':
            case 'd':
                number = parseInt(+value, 10);
                prefix = number < 0 ? '-' : positivePrefix;
                value = prefix + pad(String(Math.abs(number)), precision, '0', false);
                return justify(value, prefix, leftJustify, minWidth, zeroPad);
            case 'e':
            case 'E':
            case 'f':
            case 'F':
            case 'g':
            case 'G':
                number = +value;
                prefix = number < 0 ? '-' : positivePrefix;
                method = ['toExponential', 'toFixed', 'toPrecision']['efg'.indexOf(type.toLowerCase())];
                textTransform = ['toString', 'toUpperCase']['eEfFgG'.indexOf(type) % 2];
                value = prefix + Math.abs(number)[method](precision);
                return justify(value, prefix, leftJustify, minWidth, zeroPad)[textTransform]();
            default:
                return substring;
        }
    };
 
    return format.replace(regex, doFormat);
}

function str_pad (input, pad_length, pad_string, pad_type) {
    // http://kevin.vanzonneveld.net
    // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // + namespaced by: Michael White (http://getsprink.com)
    // +      input by: Marco van Oort
    // +   bugfixed by: Brett Zamir (http://brett-zamir.me)
    // *     example 1: str_pad('Kevin van Zonneveld', 30, '-=', 'STR_PAD_LEFT');
    // *     returns 1: '-=-=-=-=-=-Kevin van Zonneveld'
    // *     example 2: str_pad('Kevin van Zonneveld', 30, '-', 'STR_PAD_BOTH');
    // *     returns 2: '------Kevin van Zonneveld-----'

    var half = '', pad_to_go;

    var str_pad_repeater = function (s, len) {
        var collect = '', i;

        while (collect.length < len) {
            collect += s;
        }
        collect = collect.substr(0,len);

        return collect;
    };

    input += '';
    pad_string = pad_string !== undefined ? pad_string : ' ';
    
    if (pad_type != 'STR_PAD_LEFT' && pad_type != 'STR_PAD_RIGHT' && pad_type != 'STR_PAD_BOTH') {
        pad_type = 'STR_PAD_RIGHT';
    }
    if ((pad_to_go = pad_length - input.length) > 0) {
        if (pad_type == 'STR_PAD_LEFT') {
            input = str_pad_repeater(pad_string, pad_to_go) + input;
        }
        else if (pad_type == 'STR_PAD_RIGHT') {
            input = input + str_pad_repeater(pad_string, pad_to_go);
        }
        else if (pad_type == 'STR_PAD_BOTH') {
            half = str_pad_repeater(pad_string, Math.ceil(pad_to_go/2));
            input = half + input + half;
            input = input.substr(0, pad_length);
        }
    }

    return input;
}

function str_repeat(input, multiplier) {
    return new Array(multiplier+1).join(input);
}

function str_replace (search, replace, subject, count) {
    // http://kevin.vanzonneveld.net
    // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   improved by: Gabriel Paderni
    // +   improved by: Philip Peterson
    // +   improved by: Simon Willison (http://simonwillison.net)
    // +    revised by: Jonas Raoni Soares Silva (http://www.jsfromhell.com)
    // +   bugfixed by: Anton Ongson
    // +      input by: Onno Marsman
    // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +    tweaked by: Onno Marsman
    // +      input by: Brett Zamir (http://brett-zamir.me)
    // +   bugfixed by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   input by: Oleg Eremeev
    // +   improved by: Brett Zamir (http://brett-zamir.me)
    // +   bugfixed by: Oleg Eremeev
    // %          note 1: The count parameter must be passed as a string in order
    // %          note 1:  to find a global variable in which the result will be given
    // *     example 1: str_replace(' ', '.', 'Kevin van Zonneveld');
    // *     returns 1: 'Kevin.van.Zonneveld'
    // *     example 2: str_replace(['{name}', 'l'], ['hello', 'm'], '{name}, lars');
    // *     returns 2: 'hemmo, mars'

    var i = 0, j = 0, temp = '', repl = '', sl = 0, fl = 0,
    f = [].concat(search),
    r = [].concat(replace),
    s = subject,
    ra = r instanceof Array, sa = s instanceof Array;
    s = [].concat(s);
    if (count) {
        this.window[count] = 0;
    }

    for (i=0, sl=s.length; i < sl; i++) {
        if (s[i] === '') {
            continue;
        }
        for (j=0, fl=f.length; j < fl; j++) {
            temp = s[i]+'';
            repl = ra ? (r[j] !== undefined ? r[j] : '') : r[0];
            s[i] = (temp).split(f[j]).join(repl);
            if (count && s[i] !== temp) {
                this.window[count] += (temp.length-s[i].length)/f[j].length;
            }
        }
    }
    return sa ? s : s[0];
}

function stristr (haystack, needle, bool) {
    // Finds first occurrence of a string within another, case insensitive  
    // 
    // version: 1004.2314
    // discuss at: http://phpjs.org/functions/stristr
    // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   bugfxied by: Onno Marsman
    // *     example 1: stristr('Kevin van Zonneveld', 'Van');
    // *     returns 1: 'van Zonneveld'
    // *     example 2: stristr('Kevin van Zonneveld', 'VAN', true);
    // *     returns 2: 'Kevin '
    var pos = 0;
 
    haystack += '';
    pos = haystack.toLowerCase().indexOf( (needle+'').toLowerCase() );
    if (pos == -1){
        return false;
    } else{
        if (bool) {
            return haystack.substr( 0, pos );
        } else{
            return haystack.slice( pos );
        }
    }
}

function strrpos (haystack, needle, offset) {
    // http://kevin.vanzonneveld.net
    // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   bugfixed by: Onno Marsman
    // +   input by: saulius
    // +   bugfixed by: Brett Zamir (http://brett-zamir.me)
    // *     example 1: strrpos('Kevin van Zonneveld', 'e');
    // *     returns 1: 16
    // *     example 2: strrpos('somepage.com', '.', false);
    // *     returns 2: 8
    // *     example 3: strrpos('baa', 'a', 3);
    // *     returns 3: false
    // *     example 4: strrpos('baa', 'a', 2);
    // *     returns 4: 2

    var i = -1;
    if (offset) {
        i = (haystack+'').slice(offset).lastIndexOf(needle); // strrpos' offset indicates starting point of range till end,
        // while lastIndexOf's optional 2nd argument indicates ending point of range from the beginning
        if (i !== -1) {
            i += offset;
        }
    }
    else {
        i = (haystack+'').lastIndexOf(needle);
    }
    return i >= 0 ? i : false;
}

function strtotime (str, now) {
    // http://kevin.vanzonneveld.net
    // +   original by: Caio Ariede (http://caioariede.com)
    // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +      input by: David
    // +   improved by: Caio Ariede (http://caioariede.com)
    // +   improved by: Brett Zamir (http://brett-zamir.me)
    // +   bugfixed by: Wagner B. Soares
    // +   bugfixed by: Artur Tchernychev
    // %        note 1: Examples all have a fixed timestamp to prevent tests to fail because of variable time(zones)
    // *     example 1: strtotime('+1 day', 1129633200);
    // *     returns 1: 1129719600
    // *     example 2: strtotime('+1 week 2 days 4 hours 2 seconds', 1129633200);
    // *     returns 2: 1130425202
    // *     example 3: strtotime('last month', 1129633200);
    // *     returns 3: 1127041200
    // *     example 4: strtotime('2009-05-04 08:30:00');
    // *     returns 4: 1241418600
    var i, l, match, s, parse = '';

    str = str.replace(/\s{2,}|^\s|\s$/g, ' '); // unecessary spaces
    str = str.replace(/[\t\r\n]/g, ''); // unecessary chars
    if (str === 'now') {
        return now === null || isNaN(now) ? new Date().getTime() / 1000 | 0 : now | 0;
    } else if (!isNaN(parse = Date.parse(str))) {
        return parse / 1000 | 0;
    } else if (now) {
        now = new Date(now * 1000); // Accept PHP-style seconds
    } else {
        now = new Date();
    }

    str = str.toLowerCase();

    var __is = {
        day: {
            'sun': 0,
            'mon': 1,
            'tue': 2,
            'wed': 3,
            'thu': 4,
            'fri': 5,
            'sat': 6
        },
        mon: [
        'jan',
        'feb',
        'mar',
        'apr',
        'may',
        'jun',
        'jul',
        'aug',
        'sep',
        'oct',
        'nov',
        'dec'
        ]
    };

    var process = function (m) {
        var ago = (m[2] && m[2] === 'ago');
        var num = (num = m[0] === 'last' ? -1 : 1) * (ago ? -1 : 1);

        switch (m[0]) {
            case 'last':
            case 'next':
                switch (m[1].substring(0, 3)) {
                    case 'yea':
                        now.setFullYear(now.getFullYear() + num);
                        break;
                    case 'mon':
                        now.setMonth(now.getMonth() + num);
                        break;
                    case 'wee':
                        now.setDate(now.getDate() + (num * 7));
                        break;
                    case 'day':
                        now.setDate(now.getDate() + num);
                        break;
                    case 'hou':
                        now.setHours(now.getHours() + num);
                        break;
                    case 'min':
                        now.setMinutes(now.getMinutes() + num);
                        break;
                    case 'sec':
                        now.setSeconds(now.getSeconds() + num);
                        break;
                    default:
                        var day = __is.day[m[1].substring(0, 3)];
                        if (typeof day !== 'undefined') {
                            var diff = day - now.getDay();
                            if (diff === 0) {
                                diff = 7 * num;
                            } else if (diff > 0) {
                                if (m[0] === 'last') {
                                    diff -= 7;
                                }
                            } else {
                                if (m[0] === 'next') {
                                    diff += 7;
                                }
                            }
                            now.setDate(now.getDate() + diff);
                        }
                }
                break;

            default:
                if (/\d+/.test(m[0])) {
                    num *= parseInt(m[0], 10);

                    switch (m[1].substring(0, 3)) {
                        case 'yea':
                            now.setFullYear(now.getFullYear() + num);
                            break;
                        case 'mon':
                            now.setMonth(now.getMonth() + num);
                            break;
                        case 'wee':
                            now.setDate(now.getDate() + (num * 7));
                            break;
                        case 'day':
                            now.setDate(now.getDate() + num);
                            break;
                        case 'hou':
                            now.setHours(now.getHours() + num);
                            break;
                        case 'min':
                            now.setMinutes(now.getMinutes() + num);
                            break;
                        case 'sec':
                            now.setSeconds(now.getSeconds() + num);
                            break;
                    }
                } else {
                    return false;
                }
                break;
        }
        return true;
    };

    match = str.match(/^(\d{2,4}-\d{2}-\d{2})(?:\s(\d{1,2}:\d{2}(:\d{2})?)?(?:\.(\d+))?)?$/);
    if (match !== null) {
        if (!match[2]) {
            match[2] = '00:00:00';
        } else if (!match[3]) {
            match[2] += ':00';
        }

        s = match[1].split(/-/g);

        s[1] = __is.mon[s[1] - 1] || s[1];
        s[0] = +s[0];

        s[0] = (s[0] >= 0 && s[0] <= 69) ? '20' + (s[0] < 10 ? '0' + s[0] : s[0] + '') : (s[0] >= 70 && s[0] <= 99) ? '19' + s[0] : s[0] + '';
        return parseInt(this.strtotime(s[2] + ' ' + s[1] + ' ' + s[0] + ' ' + match[2]) + (match[4] ? match[4] / 1000 : ''), 10);
    }

    var regex = '([+-]?\\d+\\s' + '(years?|months?|weeks?|days?|hours?|min|minutes?|sec|seconds?' + '|sun\\.?|sunday|mon\\.?|monday|tue\\.?|tuesday|wed\\.?|wednesday' + '|thu\\.?|thursday|fri\\.?|friday|sat\\.?|saturday)' + '|(last|next)\\s' + '(years?|months?|weeks?|days?|hours?|min|minutes?|sec|seconds?' + '|sun\\.?|sunday|mon\\.?|monday|tue\\.?|tuesday|wed\\.?|wednesday' + '|thu\\.?|thursday|fri\\.?|friday|sat\\.?|saturday))' + '(\\sago)?';

    match = str.match(new RegExp(regex, 'gi')); // Brett: seems should be case insensitive per docs, so added 'i'
    if (match === null) {
        return false;
    }

    for (i = 0, l = match.length; i < l; i++) {
        if (!process(match[i].split(' '))) {
            return false;
        }
    }

    return now.getTime() / 1000 | 0;
}

function substr_replace (str, replace, start, length) {
    // http://kevin.vanzonneveld.net
    // +   original by: Brett Zamir (http://brett-zamir.me)
    // *     example 1: substr_replace('ABCDEFGH:/MNRPQR/', 'bob', 0);
    // *     returns 1: 'bob'
    // *     example 2: $var = 'ABCDEFGH:/MNRPQR/';
    // *     example 2: substr_replace($var, 'bob', 0, $var.length);
    // *     returns 2: 'bob'
    // *     example 3: substr_replace('ABCDEFGH:/MNRPQR/', 'bob', 0, 0);
    // *     returns 3: 'bobABCDEFGH:/MNRPQR/'
    // *     example 4: substr_replace('ABCDEFGH:/MNRPQR/', 'bob', 10, -1);
    // *     returns 4: 'ABCDEFGH:/bob/'
    // *     example 5: substr_replace('ABCDEFGH:/MNRPQR/', 'bob', -7, -1);
    // *     returns 5: 'ABCDEFGH:/bob/'
    // *     example 6: 'substr_replace('ABCDEFGH:/MNRPQR/', '', 10, -1)'
    // *     returns 6: 'ABCDEFGH://'

    if (start < 0) { // start position in str
        start = start + str.length;
    }
    length = length !== undefined ? length : str.length;
    if (length < 0) {
        length = length + str.length - start;
    }
    return str.slice(0, start)+replace.substr(0, length)+replace.slice(length)+str.slice(start+length);
}

function trim (str, charlist) {
    // http://kevin.vanzonneveld.net
    // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   improved by: mdsjack (http://www.mdsjack.bo.it)
    // +   improved by: Alexander Ermolaev (http://snippets.dzone.com/user/AlexanderErmolaev)
    // +      input by: Erkekjetter
    // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +      input by: DxGx
    // +   improved by: Steven Levithan (http://blog.stevenlevithan.com)
    // +    tweaked by: Jack
    // +   bugfixed by: Onno Marsman
    // *     example 1: trim('    Kevin van Zonneveld    ');
    // *     returns 1: 'Kevin van Zonneveld'
    // *     example 2: trim('Hello World', 'Hdle');
    // *     returns 2: 'o Wor'
    // *     example 3: trim(16, 1);
    // *     returns 3: 6

    var whitespace, l = 0, i = 0;
    str += '';
    
    if (!charlist) {
        // default list
        whitespace = " \n\r\t\f\x0b\xa0\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200a\u200b\u2028\u2029\u3000";
    } else {
        // preg_quote custom list
        charlist += '';
        whitespace = charlist.replace(/([\[\]\(\)\.\?\/\*\{\}\+\$\^\:])/g, '$1');
    }
    
    l = str.length;
    for (i = 0; i < l; i++) {
        if (whitespace.indexOf(str.charAt(i)) === -1) {
            str = str.substring(i);
            break;
        }
    }
    
    l = str.length;
    for (i = l - 1; i >= 0; i--) {
        if (whitespace.indexOf(str.charAt(i)) === -1) {
            str = str.substring(0, i + 1);
            break;
        }
    }
    
    return whitespace.indexOf(str.charAt(0)) === -1 ? str : '';
}

function ucwords(str) {
    // http://kevin.vanzonneveld.net
    // +   original by: Jonas Raoni Soares Silva (http://www.jsfromhell.com)
    // +   improved by: Waldo Malqui Silva
    // +   bugfixed by: Onno Marsman
    // +   improved by: Robin
    // *     example 1: ucwords('kevin van zonneveld');
    // *     returns 1: 'Kevin Van Zonneveld'
    // *     example 2: ucwords('HELLO WORLD');
    // *     returns 2: 'HELLO WORLD'

    return (str + '').replace(/^(.)|\s(.)/g, function ($1) {
        return $1.toUpperCase();
    });
}

function unserialize (data) {
    // http://kevin.vanzonneveld.net
    // +     original by: Arpad Ray (mailto:arpad@php.net)
    // +     improved by: Pedro Tainha (http://www.pedrotainha.com)
    // +     bugfixed by: dptr1988
    // +      revised by: d3x
    // +     improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +        input by: Brett Zamir (http://brett-zamir.me)
    // +     improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +     improved by: Chris
    // +     improved by: James
    // +        input by: Martin (http://www.erlenwiese.de/)
    // +     bugfixed by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +     improved by: Le Torbi
    // +     input by: kilops
    // +     bugfixed by: Brett Zamir (http://brett-zamir.me)
    // -      depends on: utf8_decode
    // %            note: We feel the main purpose of this function should be to ease the transport of data between php & js
    // %            note: Aiming for PHP-compatibility, we have to translate objects to arrays
    // *       example 1: unserialize('a:3:{i:0;s:5:"Kevin";i:1;s:3:"van";i:2;s:9:"Zonneveld";}');
    // *       returns 1: ['Kevin', 'van', 'Zonneveld']
    // *       example 2: unserialize('a:3:{s:9:"firstName";s:5:"Kevin";s:7:"midName";s:3:"van";s:7:"surName";s:9:"Zonneveld";}');
    // *       returns 2: {firstName: 'Kevin', midName: 'van', surName: 'Zonneveld'}

    var that = this;
    var utf8Overhead = function(chr) {
        // http://phpjs.org/functions/unserialize:571#comment_95906
        var code = chr.charCodeAt(0);
        if (code < 0x0080) {
            return 0;
        }
        if (code < 0x0800) {
            return 1;
        }
        return 2;
    };


    var error = function (type, msg, filename, line){
        error (that.window[type](msg, filename, line));
    };
    var read_until = function (data, offset, stopchr){
        var buf = [];
        var chr = data.slice(offset, offset + 1);
        var i = 2;
        while (chr != stopchr) {
            if ((i+offset) > data.length) {
                error('Error', 'Invalid');
            }
            buf.push(chr);
            chr = data.slice(offset + (i - 1),offset + i);
            i += 1;
        }
        return [buf.length, buf.join('')];
    };
    var read_chrs = function (data, offset, length){
        var buf;

        buf = [];
        for (var i = 0;i < length;i++){
            var chr = data.slice(offset + (i - 1),offset + i);
            buf.push(chr);
            length -= utf8Overhead(chr); 
        }
        return [buf.length, buf.join('')];
    };
    var _unserialize = function (data, offset){
        var readdata;
        var readData;
        var chrs = 0;
        var ccount;
        var stringlength;
        var keyandchrs;
        var keys;

        if (!offset) {
            offset = 0;
        }
        var dtype = (data.slice(offset, offset + 1)).toLowerCase();

        var dataoffset = offset + 2;
        var typeconvert = function(x) {
            return x;
        };

        switch (dtype){
            case 'i':
                typeconvert = function (x) {
                    return parseInt(x, 10);
                };
                readData = read_until(data, dataoffset, ';');
                chrs = readData[0];
                readdata = readData[1];
                dataoffset += chrs + 1;
                break;
            case 'b':
                typeconvert = function (x) {
                    return parseInt(x, 10) !== 0;
                };
                readData = read_until(data, dataoffset, ';');
                chrs = readData[0];
                readdata = readData[1];
                dataoffset += chrs + 1;
                break;
            case 'd':
                typeconvert = function (x) {
                    return parseFloat(x);
                };
                readData = read_until(data, dataoffset, ';');
                chrs = readData[0];
                readdata = readData[1];
                dataoffset += chrs + 1;
                break;
            case 'n':
                readdata = null;
                break;
            case 's':
                ccount = read_until(data, dataoffset, ':');
                chrs = ccount[0];
                stringlength = ccount[1];
                dataoffset += chrs + 2;

                readData = read_chrs(data, dataoffset+1, parseInt(stringlength, 10));
                chrs = readData[0];
                readdata = readData[1];
                dataoffset += chrs + 2;
                if (chrs != parseInt(stringlength, 10) && chrs != readdata.length){
                    error('SyntaxError', 'String length mismatch');
                }

                // Length was calculated on an utf-8 encoded string
                // so wait with decoding
                readdata = that.utf8_decode(readdata);
                break;
            case 'a':
                readdata = {};

                keyandchrs = read_until(data, dataoffset, ':');
                chrs = keyandchrs[0];
                keys = keyandchrs[1];
                dataoffset += chrs + 2;

                for (var i = 0; i < parseInt(keys, 10); i++){
                    var kprops = _unserialize(data, dataoffset);
                    var kchrs = kprops[1];
                    var key = kprops[2];
                    dataoffset += kchrs;

                    var vprops = _unserialize(data, dataoffset);
                    var vchrs = vprops[1];
                    var value = vprops[2];
                    dataoffset += vchrs;

                    readdata[key] = value;
                }

                dataoffset += 1;
                break;
            default:
                error('SyntaxError', 'Unknown / Unhandled data type(s): ' + dtype);
                break;
        }
        return [dtype, dataoffset - offset, typeconvert(readdata)];
    };
    
    return _unserialize((data+''), 0)[2];
}

function urlencode (str) {
    // http://kevin.vanzonneveld.net
    // +   original by: Philip Peterson
    // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +      input by: AJ
    // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   improved by: Brett Zamir (http://brett-zamir.me)
    // +   bugfixed by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +      input by: travc
    // +      input by: Brett Zamir (http://brett-zamir.me)
    // +   bugfixed by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   improved by: Lars Fischer
    // +      input by: Ratheous
    // +      reimplemented by: Brett Zamir (http://brett-zamir.me)
    // +   bugfixed by: Joris
    // +      reimplemented by: Brett Zamir (http://brett-zamir.me)
    // %          note 1: This reflects PHP 5.3/6.0+ behavior
    // %        note 2: Please be aware that this function expects to encode into UTF-8 encoded strings, as found on
    // %        note 2: pages served as UTF-8
    // *     example 1: urlencode('Kevin van Zonneveld!');
    // *     returns 1: 'Kevin+van+Zonneveld%21'
    // *     example 2: urlencode('http://kevin.vanzonneveld.net/');
    // *     returns 2: 'http%3A%2F%2Fkevin.vanzonneveld.net%2F'
    // *     example 3: urlencode('http://www.google.nl/search?q=php.js&ie=utf-8&oe=utf-8&aq=t&rls=com.ubuntu:en-US:unofficial&client=firefox-a');
    // *     returns 3: 'http%3A%2F%2Fwww.google.nl%2Fsearch%3Fq%3Dphp.js%26ie%3Dutf-8%26oe%3Dutf-8%26aq%3Dt%26rls%3Dcom.ubuntu%3Aen-US%3Aunofficial%26client%3Dfirefox-a'

    str = (str+'').toString();
    
    // Tilde should be allowed unescaped in future versions of PHP (as reflected below), but if you want to reflect current
    // PHP behavior, you would need to add ".replace(/~/g, '%7E');" to the following.
    return encodeURIComponent(str).replace(/!/g, '%21').replace(/'/g, '%27').replace(/\(/g, '%28').replace(/\)/g, '%29').replace(/\*/g, '%2A').replace(/%20/g, '+');
}

function uuid() {
    function S4() {
        return (((1+Math.random())*0x10000)|0).toString(16).substring(1);
    }
    return (S4()+S4()+"-"+S4()+"-"+S4()+"-"+S4()+"-"+S4()+S4()+S4());
}

function file_put_contents(fn, data) {
    var fs = require('fs');
    fs.writeFile(fn, data);
}

function file_get_contents(fn) {
    var fs = require('fs');
    return fs.readFile(fn);
}

exports && exports.extend({
    array_chunk: array_chunk,
    array_intersect: array_intersect,
    array_key_exists: array_key_exists,
    array_keys: array_keys,
    array_unique: array_unique,
    basename: basename,
    count: count,
    dirname: dirname,
    echo: echo,
    empty: empty,
    file_exists: file_exists,
    file_get_contents: file_get_contents,
    file_put_contents: file_put_contents,
    is_null: is_null,
    is_numeric: is_numeric,
    isset: isset,
    lcfirst: lcfirst,
    ltrim: ltrim,
    number_format: number_format,
    pathinfo: pathinfo,
    rtrim: rtrim,
    serialize: serialize,
    sprintf: sprintf,
    str_pad: str_pad,
    str_repeat: str_repeat,
    str_replace: str_replace,
    stristr: stristr,
    strrpos: strrpos,
    strtotime: strtotime,
    substr_replace: substr_replace,
    trim: trim,
    ucwords: ucwords,
    unserialize: unserialize,
    urlencode: urlencode,
    uuid: uuid
});

