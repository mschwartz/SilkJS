/**
 * @module Json
 *
 * ### Synopsis
 *
 * var Json = require('Json');
 *
 * ### Notes
 *
 * If a global Server singleton exists, its endRequest method is called; otherwise res.stop() is called.  The Server.endRequest() method might do a bit more cleanup than res.stop allows - like writing session data to disk, unlocking semaphores, etc.
 */
var Json = function() {
    return {
        /**
         * @function Json.format
         *
         * ### Synopsis
         *
         * var s = Json.format(obj);
         * var s = Json.format(obj, spaces);
         *
         * Encode an object as JSON and return it as a formatted string, suitable for printing.
         *
         * @param {Object} obj - object to format.
         * @param {int} spaces - number of spaces to indent while formatting.
         */
        format: function(o, spaces) {
            spaces = spaces || 4;
            return JSON.stringify(o, null, spaces);
        },

        /**
         * @function Json.encode
         *
         * ### Synopsis
         *
         * var s = Json.encode(obj);
         *
         * Encode an object using NATIVE JSON object for speed
         *
         * @param {Object} obj - Object to be encoded as a string
         * @returns {string} s - Object encoded as a string
         */
        encode: function(o) {
            return JSON.stringify(o);
        },
        
        /**
         * @function Json.decode
         *
         * ### Synopsis
         *
         * var obj = Json.decode(s);
         *
         * Decode a string using NATIVE JSON object for speed
         *
         * @param {string} s - String to be decoded
         * @returns {Object} obj - Object decoded from string
         */
        decode: function(s) {
            return JSON.parse(s);
        },
        
        /**
         * @function Json.successString
         *
         * ### Synopsis
         *
         * var s = Json.successString(obj);
         *
         * Generate a JSON encoded success string from an Object
         *
         * @param {Object} obj - Object/Response to be sent to client
         * @returns {string} s = Wrapped Object converted to JSON with success indication
         */
        successString: function(obj) {
            obj.success = true;
            return Json.encode(obj);
        },
        
        /**
         * @function Json.success
         *
         * ### Synopsis
         *
         * Json.success(obj);
         *
         * Send success response to client.  The obj argument has success: true added to it, and it is sent to the client.  If no object is passed, then { success: true } is sent.
         *
         * @param {Object} obj - object to send to the client
         */
        success: function(obj) {
            res.reset();
            obj = obj || {};
            obj.success = true;
            if (req.data.callback) {
                res.contentType = 'text/javascript';
                res.write([req.data.callback, '(', Json.encode(obj), ')'].join(''));
            }
            else {
                var contentType = req.getHeader('content-type') || '';
                var ret = Json.encode(obj);
                if (contentType.indexOf('multipart/form-data') != -1) {
                    // it's something like a post through an invisible iframe, so we wrap the reponse in  textarea tags
                    res.write('<textarea>' + ret + '</textarea>');
                }
                else {
                    res.contentType = 'application/json';
                    res.write(ret);
                }
            }
            if (global.Server && global.Server.endRequest()) {
                Server.endRequest();
            }
            else {
                res.stop();
            }
        },

        /**
         * @function  Json.send
         *
         * ### Synopsis
         *
         * Json.send(json_string);
         *
         * Send an already JSON encoded string and end the request.
         *
         * @param {string} json_string - String to be sent
         */
        send: function(json) {
            res.write(json);
            if (global.Server && global.Server.endRequest()) {
                Server.endRequest();
            }
            else {
                res.stop();
            }
        },
        
        /**
        * @function Json.failure
        *
        * ### Synopsis
        *
        * Json.failure(message);
        *
        * Send an error/failure response to the client.  The content has the form { success: false, message: 'text of failure description' },
        *
        * @param {string} message - message to send
        */
        failure: function(msg) {
            var responseObj = {
                success: false,
                message: msg
            };
            var contentType = req.getHeader('content-type') || '';
            if (contentType && contentType.indexOf('multipart/form-data') !== -1) {
                res.write('<textarea>' + Json.encode(responseObj) + '</textarea>');
            }
            else {
                res.write(Json.encode(responseObj));
            }

            if (global.Server && global.Server.endRequest()) {
                Server.endRequest();
            }
            else {
                res.stop();
            }
        },

        /**
         * @function Json.exception
         *
         * ### Synopsis
         *
         * Json.exception(msg);
         *
         * Similar to Json.failure, except this differentiates between failures and try/catch type exceptions.  The message is typically the cause of the exception with stack trace.
         *
         * The object sent to the client looks like { success: false, excsption: 'message' }
         *
         * @param {string} msg - text to send as exception.
         */
        exception : function(msg) {
            res.write(Json.encode({
                success   : false,
                exception : msg
            }));
            if (global.Server && global.Server.endRequest()) {
                Server.endRequest();
            }
            else {
                res.stop();
            }
        }
    };
}();

if (exports) {
    exports = Json;
}
