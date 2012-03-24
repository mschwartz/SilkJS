/** @ignore */

/**
 * Created by JetBrains WebStorm.
 * User: mschwartz
 * Date: 1/22/12
 * Time: 12:35 PM
 * Sample math module, per http://wiki.commonjs.org/wiki/Modules/1.1
 */

exports.add = function() {
    var sum = 0, i = 0, args = arguments, l = args.length;
    while (i < l) {
        sum += args[i++];
    }
    return sum;
};