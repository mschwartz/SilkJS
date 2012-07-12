/**
 * @fileoverview
 *
 * <p>Namespace to hold assorted core utility functions.</p>
 */

/**
 * @namespace
 *
 * <p>Namespace to hold assorted core utility functions.</p>
 */

Util = (function() {
	/** @scope Util */
	return {
		/**
		 * Clones an object, array, etc.
		 * We sneakily call Json.decode(Json.encode(o)).
		 *
		 * @param o {object/array} the thing to be cloned
		 * @returns {object/array} the cloned thing
		 */
		clone: function(o) {
			return Json.decode(Json.encode(o));
		},

		/**
		 * Generate something like a UUID
		 *
		 * @returns {string} 38 character UUID-like string
		 */
		uuid: function() {
			function S4() {
				return (((1+Math.random())*0x10000)|0).toString(16).substring(1);
			}
			return (S4()+S4()+"-"+S4()+"-"+S4()+"-"+S4()+"-"+S4()+S4()+S4());
		},

		/**
		 * <p>
		 * Returns true if the passed value is empty.
		 * </p>
		 * <p>
		 * The value is deemed to be empty if it is<div class="mdetail-params">
		 * <ul>
		 * <li>null</li>
		 * <li>undefined</li>
		 * <li>an empty array</li>
		 * <li>a zero length string (Unless the <tt>allowBlank</tt> parameter
		 * is <tt>true</tt>)</li>
		 * </ul>
		 * </div>
		 *
		 * @param {Mixed} value The value to test
		 * @param {Boolean} allowBlank (optional) true to allow empty strings (defaults to false)
		 * @return {Boolean} true if value is empty
		 */
		isEmpty: function(v, allowBlank) {
			return v === null || v === undefined || ((Util.isArray(v) && !v.length)) || (!allowBlank ? v === '' : false);
		},

		/**
		 * Returns true if the passed value is a JavaScript array, otherwise
		 * false.
		 *
		 * @param {Mixed} value The value to test
		 * @return {Boolean}
		 */
		isArray: function(v) {
			return toString.apply(v) === '[object Array]';
		},

		/**
		 * Returns true if the passed object is a JavaScript date object,
		 * otherwise false.
		 *
		 * @param {Object} object The object to test
		 * @return {Boolean}
		 */
		isDate: function(v) {
			return toString.apply(v) === '[object Date]';
		},

		/**
		 * Returns true if the passed value is a JavaScript Object, otherwise
		 * false.
		 *
		 * @param {Mixed} value The value to test
		 * @return {Boolean}
		 */
		isObject: function(v) {
			return !!v && Object.prototype.toString.call(v) === '[object Object]';
		},

		/**
		 * Returns true if the passed value is a JavaScript 'primitive', a
		 * string, number or boolean.
		 *
		 * @param {Mixed}
			*            value The value to test
		 * @return {Boolean}
		 */
		isPrimitive: function(v) {
			return Util.isString(v) || Util.isNumber(v) || Util.isBoolean(v);
		},

		/**
		 * Returns true if the passed value is a JavaScript Function, otherwise
		 * false.
		 *
		 * @param {Mixed}
			*            value The value to test
		 * @return {Boolean}
		 */
		isFunction: function(v) {
			return toString.apply(v) === '[object Function]';
		},

		/**
		 * Returns true if the passed value is a number. Returns false for
		 * non-finite numbers.
		 *
		 * @param {Mixed}
			*            value The value to test
		 * @return {Boolean}
		 */
		isNumber: function(v) {
			return typeof v === 'number' && isFinite(v);
		},

		/**
		 * Returns true if the passed value is a string.
		 *
		 * @param {Mixed}
			*            value The value to test
		 * @return {Boolean}
		 */
		isString: function(v) {
			return typeof v === 'string';
		},

		/**
		 * Returns true if the passed value is a boolean.
		 *
		 * @param {Mixed}
			*            value The value to test
		 * @return {Boolean}
		 */
		isBoolean: function(v) {
			return typeof v === 'boolean';
		},

		/**
		 * Returns true if the passed value is not undefined.
		 *
		 * @param {Mixed}
			*            value The value to test
		 * @return {Boolean}
		 */
		isDefined: function(v) {
			return typeof v !== 'undefined';
		},

		/**
		 * creates a random string (numbers and chars)
		 * @param len length of key
		 * @param mode determines which letters to use. null or 0 = all letters;
		 *      1 = skip 0, 1, l and o which can easily be mixed with numbers;
		 *      2 = use numbers only
		 * @returns random string
		 */
		randomString: function(len, mode) {
			if (mode == 2) {
				var x = Math.random() * Math.pow(10,len);
				return Math.floor(x);
			}
			var keystr = '';
			for (var i=0; i<len; i++) {
				var x = Math.floor((Math.random() * 36));
				if (mode == 1) {
					// skip 0,1
					x = (x<2) ? x + 2 : x;
					// don't use the letters l (charCode 21+87) and o (24+87)
					x = (x==21) ? 22 : x;
					x = (x==24) ? 25 : x;
				}
				if (x<10) {
					keystr += String(x);
				}    else    {
					keystr += String.fromCharCode(x+87);
				}
			}
			return keystr;
		},

		// rename this to applyIf
		applyIf:  function(dst, src) {
			for (var key in src) {
				if (dst[key] === undefined) {
					dst[key] = src[key];
				}
			}
			return dst;
		},
		apply: function(dst, src) {
			for (var key in src) {
				dst[key] = src[key];
			}
			return dst;
		},

		inet_ntoa:  function(ip) {
			ip = parseInt(ip);
			var quads = [];
			for (var i = 0; i < 4; i++) {
				quads.push(ip % 256);
				ip >>= 8;
			}
			return quads[3] + '.' + quads[2] + '.' + quads[1] + '.' + quads[0];
		},

		inet_aton:  function(ip) {
			var quads = ip.split('.');
			return (parseInt(quads[0]) << 24) | (parseInt(quads[1]) << 16) | (parseInt(quads[2]) << 8) | parseInt(quads[3]);
		},

		currentTime:  function() {
			var date = new Date();
			return parseInt(date.getTime() / 1000, 10);
		},

		currentTimeMillis:  function(fraction) {
			var now = new Date().getTime();
			return fraction ? now/1000 : now;
		},

		today: function() {
			var date = new Date();
			date.setHours(0);
			date.setMinutes(0);
			date.setSeconds(0);
			return date.getTime()/1000;
		},

		dateOnlyString:  function(timestamp) {
			timestamp = parseInt(timestamp);
			var date = new Date(timestamp * 1000);
			var today = Util.today();
			var tomorrow = today + 86400;
			var dayafter = tomorrow + 86400;
			var yesterday = today - 86400;
			if (timestamp >= tomorrow && timestamp <= dayafter) {
				return "Tomorrow";
			}
			if (timestamp >= today && timestamp <= tomorrow) {
				return "Today";
			}
			if (timestamp >= yesterday && timestamp <= today) {
				return "Yesterday";
			}
			return date.format('MM/dd/yyyy');
		},

		timeOnlyString:  function(timestamp) {
			timestamp = parseInt(timestamp);
			var date = new Date(timestamp * 1000);
			return date.format('h:mm:ss a z');
		},

		dateString:  function(timestamp) {
			timestamp = parseInt(timestamp);
			var today = Util.today();
			var yesterday = today - 86400;
			var date = new Date(timestamp * 1000);
			if (timestamp >= today) {
				return "Today " + date.format('h:m:s a');
			}
			if (timestamp >= yesterday) {
				return "Yesterday " + date.format('h:m:s a');
			}
			return date.format('MM/dd/yyyy h:m:s a');
		},

		dateStringLocale:  function(timestamp) {
			return new Date(timestamp * 1000).toLocaleString();
		},

		stripslashes:  function(str) {
			return (str + '').replace(/\0/g, '0').replace(/\\([\\'"])/g, '$1');
		},

		addslashes:  function(str) {
			return (str + '').replace(/([\\"'])/g, "\\$1").replace(/\0/g, "\\0");
		},

		base64Decode: function(input) {
			var keyStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
			var output = "";
			var chr1, chr2, chr3, enc1, enc2, enc3, enc4;
			var i = 0;

			do {
				enc1 = keyStr.indexOf(input.charAt(i++));
				enc2 = keyStr.indexOf(input.charAt(i++));
				enc3 = keyStr.indexOf(input.charAt(i++));
				enc4 = keyStr.indexOf(input.charAt(i++));

				chr1 = (enc1 << 2) | (enc2 >> 4);
				chr2 = ((enc2 & 15) << 4) | (enc3 >> 2);
				chr3 = ((enc3 & 3) << 6) | enc4;

				output = output + String.fromCharCode(chr1);

				if (enc3 != 64) { output = output + String.fromCharCode(chr2); }
				if (enc4 != 64) { output = output + String.fromCharCode(chr3); }
			} while (i < input.length);

			return output; // Util.utf8Encode(output);
		},

		md5: function (string) {
			string = ''+string;

			function RotateLeft(lValue, iShiftBits) {
				return (lValue<<iShiftBits) | (lValue>>>(32-iShiftBits));
			}

			function AddUnsigned(lX,lY) {
				var lX4,lY4,lX8,lY8,lResult;
				lX8 = (lX & 0x80000000);
				lY8 = (lY & 0x80000000);
				lX4 = (lX & 0x40000000);
				lY4 = (lY & 0x40000000);
				lResult = (lX & 0x3FFFFFFF)+(lY & 0x3FFFFFFF);
				if (lX4 & lY4) {
					return (lResult ^ 0x80000000 ^ lX8 ^ lY8);
				}
				if (lX4 | lY4) {
					if (lResult & 0x40000000) {
						return (lResult ^ 0xC0000000 ^ lX8 ^ lY8);
					} else {
						return (lResult ^ 0x40000000 ^ lX8 ^ lY8);
					}
				} else {
					return (lResult ^ lX8 ^ lY8);
				}
			}

			function F(x,y,z) { return (x & y) | ((~x) & z); }
			function G(x,y,z) { return (x & z) | (y & (~z)); }
			function H(x,y,z) { return (x ^ y ^ z); }
			function I(x,y,z) { return (y ^ (x | (~z))); }

			function FF(a,b,c,d,x,s,ac) {
				a = AddUnsigned(a, AddUnsigned(AddUnsigned(F(b, c, d), x), ac));
				return AddUnsigned(RotateLeft(a, s), b);
			};

			function GG(a,b,c,d,x,s,ac) {
				a = AddUnsigned(a, AddUnsigned(AddUnsigned(G(b, c, d), x), ac));
				return AddUnsigned(RotateLeft(a, s), b);
			};

			function HH(a,b,c,d,x,s,ac) {
				a = AddUnsigned(a, AddUnsigned(AddUnsigned(H(b, c, d), x), ac));
				return AddUnsigned(RotateLeft(a, s), b);
			};

			function II(a,b,c,d,x,s,ac) {
				a = AddUnsigned(a, AddUnsigned(AddUnsigned(I(b, c, d), x), ac));
				return AddUnsigned(RotateLeft(a, s), b);
			};

			function ConvertToWordArray(string) {
				var lWordCount;
				var lMessageLength = string.length;
				var lNumberOfWords_temp1=lMessageLength + 8;
				var lNumberOfWords_temp2=(lNumberOfWords_temp1-(lNumberOfWords_temp1 % 64))/64;
				var lNumberOfWords = (lNumberOfWords_temp2+1)*16;
				var lWordArray=Array(lNumberOfWords-1);
				var lBytePosition = 0;
				var lByteCount = 0;
				while ( lByteCount < lMessageLength ) {
					lWordCount = (lByteCount-(lByteCount % 4))/4;
					lBytePosition = (lByteCount % 4)*8;
					lWordArray[lWordCount] = (lWordArray[lWordCount] | (string.charCodeAt(lByteCount)<<lBytePosition));
					lByteCount++;
				}
				lWordCount = (lByteCount-(lByteCount % 4))/4;
				lBytePosition = (lByteCount % 4)*8;
				lWordArray[lWordCount] = lWordArray[lWordCount] | (0x80<<lBytePosition);
				lWordArray[lNumberOfWords-2] = lMessageLength<<3;
				lWordArray[lNumberOfWords-1] = lMessageLength>>>29;
				return lWordArray;
			};
			
			function Utf8Encode(string) {
				string = string.replace(/\r\n/g,"\n");
				var utftext = "";

				for (var n = 0; n < string.length; n++) {

					var c = string.charCodeAt(n);

					if (c < 128) {
						utftext += String.fromCharCode(c);
					}
					else if((c > 127) && (c < 2048)) {
						utftext += String.fromCharCode((c >> 6) | 192);
						utftext += String.fromCharCode((c & 63) | 128);
					}
					else {
						utftext += String.fromCharCode((c >> 12) | 224);
						utftext += String.fromCharCode(((c >> 6) & 63) | 128);
						utftext += String.fromCharCode((c & 63) | 128);
					}

				}

				return utftext;
			};
			

			function WordToHex(lValue) {
				var WordToHexValue="",WordToHexValue_temp="",lByte,lCount;
				for (lCount = 0;lCount<=3;lCount++) {
					lByte = (lValue>>>(lCount*8)) & 255;
					WordToHexValue_temp = "0" + lByte.toString(16);
					WordToHexValue = WordToHexValue + WordToHexValue_temp.substr(WordToHexValue_temp.length-2,2);
				}
				return WordToHexValue;
			};

			var x=Array();
			var k,AA,BB,CC,DD,a,b,c,d;
			var S11=7, S12=12, S13=17, S14=22;
			var S21=5, S22=9 , S23=14, S24=20;
			var S31=4, S32=11, S33=16, S34=23;
			var S41=6, S42=10, S43=15, S44=21;

			string = Utf8Encode(string);

			x = ConvertToWordArray(string);

			a = 0x67452301; b = 0xEFCDAB89; c = 0x98BADCFE; d = 0x10325476;

			for (k=0;k<x.length;k+=16) {
				AA=a; BB=b; CC=c; DD=d;
				a=FF(a,b,c,d,x[k+0], S11,0xD76AA478);
				d=FF(d,a,b,c,x[k+1], S12,0xE8C7B756);
				c=FF(c,d,a,b,x[k+2], S13,0x242070DB);
				b=FF(b,c,d,a,x[k+3], S14,0xC1BDCEEE);
				a=FF(a,b,c,d,x[k+4], S11,0xF57C0FAF);
				d=FF(d,a,b,c,x[k+5], S12,0x4787C62A);
				c=FF(c,d,a,b,x[k+6], S13,0xA8304613);
				b=FF(b,c,d,a,x[k+7], S14,0xFD469501);
				a=FF(a,b,c,d,x[k+8], S11,0x698098D8);
				d=FF(d,a,b,c,x[k+9], S12,0x8B44F7AF);
				c=FF(c,d,a,b,x[k+10],S13,0xFFFF5BB1);
				b=FF(b,c,d,a,x[k+11],S14,0x895CD7BE);
				a=FF(a,b,c,d,x[k+12],S11,0x6B901122);
				d=FF(d,a,b,c,x[k+13],S12,0xFD987193);
				c=FF(c,d,a,b,x[k+14],S13,0xA679438E);
				b=FF(b,c,d,a,x[k+15],S14,0x49B40821);
				a=GG(a,b,c,d,x[k+1], S21,0xF61E2562);
				d=GG(d,a,b,c,x[k+6], S22,0xC040B340);
				c=GG(c,d,a,b,x[k+11],S23,0x265E5A51);
				b=GG(b,c,d,a,x[k+0], S24,0xE9B6C7AA);
				a=GG(a,b,c,d,x[k+5], S21,0xD62F105D);
				d=GG(d,a,b,c,x[k+10],S22,0x2441453);
				c=GG(c,d,a,b,x[k+15],S23,0xD8A1E681);
				b=GG(b,c,d,a,x[k+4], S24,0xE7D3FBC8);
				a=GG(a,b,c,d,x[k+9], S21,0x21E1CDE6);
				d=GG(d,a,b,c,x[k+14],S22,0xC33707D6);
				c=GG(c,d,a,b,x[k+3], S23,0xF4D50D87);
				b=GG(b,c,d,a,x[k+8], S24,0x455A14ED);
				a=GG(a,b,c,d,x[k+13],S21,0xA9E3E905);
				d=GG(d,a,b,c,x[k+2], S22,0xFCEFA3F8);
				c=GG(c,d,a,b,x[k+7], S23,0x676F02D9);
				b=GG(b,c,d,a,x[k+12],S24,0x8D2A4C8A);
				a=HH(a,b,c,d,x[k+5], S31,0xFFFA3942);
				d=HH(d,a,b,c,x[k+8], S32,0x8771F681);
				c=HH(c,d,a,b,x[k+11],S33,0x6D9D6122);
				b=HH(b,c,d,a,x[k+14],S34,0xFDE5380C);
				a=HH(a,b,c,d,x[k+1], S31,0xA4BEEA44);
				d=HH(d,a,b,c,x[k+4], S32,0x4BDECFA9);
				c=HH(c,d,a,b,x[k+7], S33,0xF6BB4B60);
				b=HH(b,c,d,a,x[k+10],S34,0xBEBFBC70);
				a=HH(a,b,c,d,x[k+13],S31,0x289B7EC6);
				d=HH(d,a,b,c,x[k+0], S32,0xEAA127FA);
				c=HH(c,d,a,b,x[k+3], S33,0xD4EF3085);
				b=HH(b,c,d,a,x[k+6], S34,0x4881D05);
				a=HH(a,b,c,d,x[k+9], S31,0xD9D4D039);
				d=HH(d,a,b,c,x[k+12],S32,0xE6DB99E5);
				c=HH(c,d,a,b,x[k+15],S33,0x1FA27CF8);
				b=HH(b,c,d,a,x[k+2], S34,0xC4AC5665);
				a=II(a,b,c,d,x[k+0], S41,0xF4292244);
				d=II(d,a,b,c,x[k+7], S42,0x432AFF97);
				c=II(c,d,a,b,x[k+14],S43,0xAB9423A7);
				b=II(b,c,d,a,x[k+5], S44,0xFC93A039);
				a=II(a,b,c,d,x[k+12],S41,0x655B59C3);
				d=II(d,a,b,c,x[k+3], S42,0x8F0CCC92);
				c=II(c,d,a,b,x[k+10],S43,0xFFEFF47D);
				b=II(b,c,d,a,x[k+1], S44,0x85845DD1);
				a=II(a,b,c,d,x[k+8], S41,0x6FA87E4F);
				d=II(d,a,b,c,x[k+15],S42,0xFE2CE6E0);
				c=II(c,d,a,b,x[k+6], S43,0xA3014314);
				b=II(b,c,d,a,x[k+13],S44,0x4E0811A1);
				a=II(a,b,c,d,x[k+4], S41,0xF7537E82);
				d=II(d,a,b,c,x[k+11],S42,0xBD3AF235);
				c=II(c,d,a,b,x[k+2], S43,0x2AD7D2BB);
				b=II(b,c,d,a,x[k+9], S44,0xEB86D391);
				a=AddUnsigned(a,AA);
				b=AddUnsigned(b,BB);
				c=AddUnsigned(c,CC);
				d=AddUnsigned(d,DD);
			}

			var temp = WordToHex(a)+WordToHex(b)+WordToHex(c)+WordToHex(d);

			return temp.toLowerCase();
		},
		/**
		 * Concatenates the values of a variable into an easily readable string
		 * by Matt Hackett [scriptnode.com]
		 * @param {Object} x The variable to debug
		 * @param {Number} max The maximum number of recursions allowed (keep low, around 5 for HTML elements to prevent errors) [default: 10]
		 * @param {String} sep The separator to use between [default: a single space ' ']
		 * @param {Number} l The current level deep (amount of recursion). Do not use this parameter: it's for the function's own use
		 */
		print_r: function(x, max, sep, l) {

			l = l || 0;
			max = max || 8;
			sep = sep || ' ';

			if (l > max) {
				return "[WARNING: Too much recursion]\n";
			}

			var
				i,
				r = '',
				t = typeof x,
				tab = '';

			//	r += t + '\n';
			if (x === null) {
				r += "(null)\n";
			} else if (t === 'function') {
				r += '(function)\n';
				x = 'function';
			} else if (t === 'object') {

				l++;

				for (i = 0; i < l; i++) {
					tab += sep;
				}

				if (x && x.length) {
					t = 'array';
				}

				r += '(' + t + ") :\n";

				for (i in x) {
					if (i === 'class') continue;
					try {
						r += tab + '[' + i + '] : ' + Util.print_r(x[i], max, sep, (l + 1));
					} catch(e) {
						return "[ERROR: " + e + "]\n";
					}
				}

			} else {

				if (t == 'string') {
					if (x == '') {
						x = '(empty)';
					}
					else {
						x = x.replace(/</igm, '&lt;');
					}
				}

				r += '(' + t + ') ' + x + "\n";

			}

			return r;

		},

		// taken from phpjs.org		
		htmlspecialchars_decode: function(string, quote_style) {
			// Convert special HTML entities back to characters  
			// 
			// version: 1004.2314
			// discuss at: http://phpjs.org/functions/htmlspecialchars_decode
			// +   original by: Mirek Slugen
			// +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
			// +   bugfixed by: Mateusz "loonquawl" Zalega
			// +      input by: ReverseSyntax
			// +      input by: Slawomir Kaniecki
			// +      input by: Scott Cariss
			// +      input by: Francois
			// +   bugfixed by: Onno Marsman
			// +    revised by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
			// +   bugfixed by: Brett Zamir (http://brett-zamir.me)
			// +      input by: Ratheous
			// +      input by: Mailfaker (http://www.weedem.fr/)
			// +      reimplemented by: Brett Zamir (http://brett-zamir.me)
			// +    bugfixed by: Brett Zamir (http://brett-zamir.me)
			// *     example 1: htmlspecialchars_decode("<p>this -&gt; &quot;</p>", 'ENT_NOQUOTES');
			// *     returns 1: '<p>this -> &quot;</p>'
			// *     example 2: htmlspecialchars_decode("&amp;quot;");
			// *     returns 2: '&quot;'
			var optTemp = 0, i = 0, noquotes= false;
			if (typeof quote_style === 'undefined') {
				quote_style = 2;
			}
			string = string.toString().replace(/&lt;/g, '<').replace(/&gt;/g, '>');
			var OPTS = {
				'ENT_NOQUOTES': 0,
				'ENT_HTML_QUOTE_SINGLE' : 1,
				'ENT_HTML_QUOTE_DOUBLE' : 2,
				'ENT_COMPAT': 2,
				'ENT_QUOTES': 3,
				'ENT_IGNORE' : 4
			};
			if (quote_style === 0) {
				noquotes = true;
			}
			if (typeof quote_style !== 'number') { // Allow for a single string or an array of string flags
				quote_style = [].concat(quote_style);
				for (i=0; i < quote_style.length; i++) {
					// Resolve string input to bitwise e.g. 'PATHINFO_EXTENSION' becomes 4
					if (OPTS[quote_style[i]] === 0) {
						noquotes = true;
					}
					else if (OPTS[quote_style[i]]) {
						optTemp = optTemp | OPTS[quote_style[i]];
					}
				}
				quote_style = optTemp;
			}
			if (quote_style & OPTS.ENT_HTML_QUOTE_SINGLE) {
				string = string.replace(/&#0*39;/g, "'"); // PHP doesn't currently escape if more than one 0, but it should
				// string = string.replace(/&apos;|&#x0*27;/g, "'"); // This would also be useful here, but not a part of PHP
			}
			if (!noquotes) {
				string = string.replace(/&quot;/g, '"');
			}
			// Put this in last place to avoid escape being double-decoded
			string = string.replace(/&amp;/g, '&');

			return string;
		}
	}
})();
//defineLibraryScope('Util');


//function print(s) {
//	global.res ? res.write(s) : console.log(s);
////	res.write(s);
//}
//function println(s) {
//	global.res ? res.writeln(s) : console.log(s + '\n');
//}
//function printbr(s) {
//	res.writeln(s+'<br/>');
//}
//defineLibraryScope('print');
//defineLibraryScope('println');
//defineLibraryScope('printbr');

function printo(o) {
	println('<pre style="text-align:left; border:1px solid #000; background:#eee; font:normal 11px/16px "Courier New",courier; color:#000;">' + Util.print_r(o).replace(/</g, '&lt;') + '</pre>');
}
//defineLibraryScope('printo');

function d(o) {
	return Util.print_r(o);
}

function pre(o, maxRecursion) {
	maxRecursion = maxRecursion || 64;
	return '<pre style="text-align:left; border:1px solid #000; background:#eee; font:normal 11px/16px "Courier New",courier; color:#000;">' + Util.print_r(o, maxRecursion) + '</pre>';
}
printA = pre;

// global.console = function() {
	// return {
		// log: function(s) {
			// app.debug(s);
		// },
		// dir: function(o) {
			// app.debug(Util.print_r(o));
		// }
	// }
// }();
//defineLibraryScope('console');

function file_put_contents(fn, data) {
	var f = new helma.File(fn);
	f.remove();
	f.open();
	f.write(data);
	f.close();
}

function file_get_contents(fn) {
	var f = new helma.File(fn);
	var ret = f.readAll();
	f.close();
	return ret;
}

