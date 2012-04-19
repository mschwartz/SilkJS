/** @ignore */

/**
 * @fileoverview
 * <p>This class provides interfaces to Halma's built-in onStart, onStop, and onCodeUpdate functionality, as well
 * as endRequest and abortRequest methods.</p>
 */

/**
 * @namespace
 * 
 * <p>This class provides interfaces to Halma's built-in onStart, onStop, and onCodeUpdate functionality, as well
 * as endRequest and abortRequest methods.</p>
 * 
 * <p>This class provides interfaces to Halma's built-in onStart and onStop functionality </p>
 * 
 * <p>For onStart and onStop, arrays of functions are maintained.  An included (repository) class/file
 * can add onStart/onStop handlers by calling Server.addOnStart() and Server.addOnStop().  This is
 * useful, for example, for MsUserManager to load all the user records from the DB when Helma starts
 * up, and storing all the user records back to the DB when Helma exits.</p>
 * 
 */
var Server = (function() {
    var started = false;
    var onStartFuncs = [];
    var onStopFuncs = [];
    var onCodeUpdateFuncs = [];
	
    /** @scope Server */
    return {
        /**
		 * Add a function to be called when Helma first starts.
		 *
		 * @param {mixed} config Config object containing name and func key/values
		 * @returns {void} nothing
		 */
        addOnStart: function(config) {
            config = Util.isFunction(config) ? {
                name: 'unnamed', 
                func: config
            } : config;
            if (started) {
                config.func();
            }
            else {
                onStartFuncs.push(config);
            }
        },
		
        /**
		 * Call all registered onStart functions.
		 *
		 * @returns {void} nothing
		 */
        onStart: function() {
            var onstart_time = Util.currentTimeMillis();
            onStartFuncs.each(function(config) {
                console.log('Starting ' + config.name);
                var start_time = Util.currentTimeMillis();
                var ret = config.func();
                var elapsed = Util.currentTimeMillis() - start_time;
                ret && console.log(['--> ', ret, ' in ', elapsed, 'ms'].join(''));
            });
            var onstart_elapsed = Util.currentTimeMillis() - onstart_time;
            started = true;
        },
		
        /**
		 * Add a function to be called when Helma shuts down.
		 *
		 * @param {function} function to be called when Helma shuts down.
		 * @returns {void} nothing
		 */
        addOnStop: function(func) {
            onStopFuncs.push(func);
        },
		
        /**
		 * Call all registered onStop functions.
		 *
		 * @returns {void} nothing
		 */
        onStop: function() {
            var onstop_time = Util.currentTimeMillis();
            onStopFuncs.each(function(func) {
                func();
            });
            var onstop_elapsed = Util.currentTimeMillis() - onstop_time;
        },
		
        /**
		 * Add a function to be called when Helma compiles/recompiles a .js file in the code
		 * repository.
		 *
		 * @param {function} function to be called when Helma compiles the code.
		 * @returns {void} nothing
		 */
        addOnCodeUpdate: function(func) {
            onCodeUpdateFuncs.push(func);
        },
		
        /**
		 * Call all registered onCodeUpdate functions.
		 *
		 * @returns {void} nothing
		 */
        onCodeUpdate: function() {
            onCodeUpdateFuncs.each(function(func) {
                func();
            });
        },
		
        /**
		 * <p>Successfully end the current request.</p>
		 *
		 * <p>NOTE: all requests should call this upon successful completion to assure
		 * any SQL transactions are committed and any dangling Sempahore locks are unlocked.</p>
		 */
        endRequest: function() {
            // Cache.unlockAll();
            res.stop();
        },

        /**
		 * <p>Abort the current request.</p>
		 *
		 * <p>NOTE: all requests should call this upon error to assure
		 * any SQL transactions are rolled back and any dangling Sempahore locks are unlocked.</p>
		 */
        abortRequest: function() {
            // Cache.unlockAll();
            res.stop();
        }
    }
})();

if (exports) {
    exports = Server;
}