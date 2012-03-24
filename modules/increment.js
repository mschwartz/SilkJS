/** @ignore */

/**
 * Created by JetBrains WebStorm.
 * User: mschwartz
 * Date: 1/22/12
 * Time: 12:36 PM
 * Sample increment module, per http://wiki.commonjs.org/wiki/Modules/1.1
 */

var add = require('math').add;
exports.increment = function(val) {
    return add(val, 1);
};
