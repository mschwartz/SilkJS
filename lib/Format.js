/**
 * @fileoverview
 *
 * <p>Namespace to hold assorted number and string format functions.</p>
 */

/**
 * @namespace
 *
 * <p>Namespace to hold assorted number and string format functions.</p>
 */

Format = (function() {
    /** @scope Format */
    return {
        comma   : ',',
        decimal : '.',
        /**
         * Replaces newline characters with HTML line break
         *
         * Function.nl2br('foo\nbar\r\nHello'); = 'foo<br/>bar<br/>Hello'
         *
         * @param str {String} String to parse
         * @returns {String} The parsed string
         */
        nl2br : function(str) {
            return str.replace(/\r?\n/g, '<br/>');
        },

        /**
         * Parses a number into a formatted string.
         *
         * Format.number(123456.7890, '0');        = '123457'
         * Format.number(123456.7890, '0.00');     = '123456.79'
         * Format.number(123456.7890, '0.0000');   = '123456.7890'
         * Format.number(123456.7890, '0,000');    = '123,457'
         * Format.number(123456.7890, '0,000.00'); = '123,456.79'
         * Format.number(123456.7890, '0,0.00');   = '123,456.79'
         *
         * @param num {Number} The number to format
         * @param string {String} The format string
         * @returns {String} The parsed string
         */
        number : function(num, string) {
            if (!string) {
                return num;
            }

            if (isFinite(num)) {
                num = parseFloat(num);
            }

            if (isNaN(num)) {
                return '';
            }

            var comma    = Format.comma,
                dec      = Format.decimal,
                neg      = num < 0 ? '-' : '',
                i        = 0,
                parr     = [],
                hasComma = string.indexOf(',') != -1,
                psplit   = string.replace(/[^\d\.]/g, '').split('.'),
                fixed    = psplit.length === 2 ? psplit[1].length : 0,
                numStr, cnum, j, m, n;

            num    = Math.abs(num).toFixed(fixed);
            numStr = num.toString();
            psplit = numStr.split(dec);

            if (hasComma) {
                cnum = psplit[0];
                j    = cnum.length;
                m    = Math.floor(j / 3);
                n    = cnum.length % 3 || 3;

                for (; i < j; i += n) {
                    if (i !== 0) {
                        n = 3;
                    }

                    parr[parr.length] = cnum.substr(i, n);
                    m -= 1;
                }

                numStr = parr.join(comma);

                if (psplit[1]) {
                    numStr += dec + psplit[1];
                }
            } else if (psplit[1]) {
                numStr = psplit[0] + dec + psplit[1];
            }

            return neg + string.replace(/[\d,?\.?]+/, numStr);
        },

        /**
         * Rounds the number to the required precision.
         *
         * Format.round(123456.7890, 2); = 123456.79
         * Format.round(123456.7819, 3); = 123456.782
         * Format.round(123456.78, 3);   = 123456.78
         *
         * @param num {Number} The number to round.
         * @param precision {Number} The precision to round to.
         * @returns {Number} The rounded number.
         */
        round : function(num, precision) {
            precision = Math.pow(10, precision);

            return Math.round(num * precision) / precision;
        },

        /**
         * Format a Number into a currency.
         *
         * Format.currency(1234.7, '$', 2);       = '$1,234.70'
         * Format.currency(1234.7, '$', 0, true); = '1,235$'
         *
         * @param num {Number} The number to parse
         * @param sign {String} The currency sign, example '$'
         * @param precision {Number} The decimal precision
         * @param end {Boolean} True to put the sign at the end
         * @returns {String} The currency string
         */
        currency: function(num, sign, precision, end) {
            var neg    = num < 0 ? '-' : '',
                format = ',0',
                i      = 0;

            if (precision > 0) {
                format += '.';

                for (; i < precision; i++) {
                    format += '0';
                }
            }

            num = this.number(num, format);

            return neg + (end ? num + sign : sign + num);
        }
    };
})();