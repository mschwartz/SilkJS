/**
 * @class Memcached
 * 
 * ### Synopsis
 * 
 * var Memcached = require('Memcached');
 * 
 * JavaScript wrapper around the builtin/memcached module.
 * 
 * ### Example
 * ```
 * var mc = new Memcached('localhost');
 * mc.add('foo, 'bar');
 * console.dir(mc.get('foo'));
 * ```
 */

var memcached = require('builtin/memcached'),
	Json = require('Json');

function isArray(v) {
    return toString.apply(v) === '[object Array]';
}

function isString(v) {
    return typeof v === 'string';
}

function exec(handle, method, key, o, expires) {
	expires = expires || 0;
	var flags = 0;	// not a string
	if (isString(o)) {
		flags = 1;
	}
	else {
		o = Json.encode(o);
	}
	return memcached[method](handle, key, o, expires, flags);
}

/**
 * @constructor Memcached
 * 
 * ### Synopsis
 * 
 * var mc = new Memcached(hosts);
 * 
 * Create a new Memcached connection instance.
 * 
 * @param {string|array) hosts - host names, either an array or comma separated list in a string.
 * 
 * ### Note
 * 
 * This function throws an error if a connection could not be made.
 */
function Memcached(hosts) {
	if (isArray(hosts)) {
		hosts = hosts.join(',');
	}
	this.handle = memcached.connect(hosts);
	if (!this.handle) {
		error('Could not connect to memcached');
	}
}

Memcached.prototype.extend({
	
	/**
	 * @function mc.set
	 * 
	 * ### Synopsis
	 * 
	 * mc.set(key, value);
	 * mc.set(key, value, expires);
	 * 
	 * Store information in memcached indexed by key.
	 * 
	 * If an object identified by key already exists on the server, it wil be replaced.
	 * 
	 * @param {string} key - key of data to set in memcached.
	 * @param {string|object} value - value to set in memcached.
	 * @param {int} expires - time to live (seconds) of item in cache.
	 * 
	 * ### Notes
	 * 
	 * This function throws an error if the action was unsuccessful.
	 * 
	 * If the value to store is an object, it is JSON encoded automatically.
	 */
	set: function(key, o, expires) {
		var rc = exec(this.handle, 'set', key, o, expires);
		if (rc) {
			error(memcached.error(this.handle, rc));
		}
	},
	
	/**
	 * @function mc.add
	 * 
	 * ### Synopsis
	 * 
	 * mc.add(key, value);
	 * mc.add(key, value, expires);
	 * 
	 * Store information in memcached indexed by key.
	 * 
	 * If an object identified by key already exists on the server, an error occurs.
	 * 
	 * @param {string} key - key of data to set in memcached.
	 * @param {string|object} value - value to set in memcached.
	 * @param {int} expires - time to live (seconds) of item in cache.
	 * 
	 * ### Notes
	 * 
	 * This function throws an error if the action was unsuccessful.
	 * 
	 * If the value to store is an object, it is JSON encoded automatically.
	 */
	add: function(key, o, expires) {
		var rc = exec(this.handle, 'add', key, o, expires);
		if (rc) {
			error(memcached.error(this.handle, rc));
		}
	},
	
	/**
	 * @function mc.replace
	 * 
	 * ### Synopsis
	 * 
	 * mc.replace(key, value);
	 * mc.replace(key, value, expires);
	 * 
	 * Store information in memcached indexed by key.
	 * 
	 * If an object identified by key already exists on the server, it is replaced. Otherwise an error occurs.
	 * 
	 * @param {string} key - key of data to set in memcached.
	 * @param {string|object} value - value to set in memcached.
	 * @param {int} expires - time to live (seconds) of item in cache.
	 * 
	 * ### Notes
	 * 
	 * This function throws an error if the action was unsuccessful.
	 * 
	 * If the value to store is an object, it is JSON encoded automatically.
	 */
	replace: function(key, o, expires) {
		var rc = exec(this.handle, 'replace', key, o, expires);
		if (rc) {
			error(memcached.error(this.handle, rc));
		}
	},
	
	/**
	 * @function mc.prepend
	 * 
	 * ### Synopsis
	 * 
	 * mc.prepend(key, value);
	 * mc.add(key, value, expires);
	 * 
	 * Prepends the given value string to the value of an existing item.
	 * 
	 * If an object identified by key does not exist, an error occurs.
	 * 
	 * @param {string} key - key of data to set in memcached.
	 * @param {string|object} value - value to set in memcached.
	 * @param {int} expires - time to live (seconds) of item in cache.
	 * 
	 * ### Notes
	 * 
	 * This function throws an error if the action was unsuccessful.
	 * 
	 * If the value to store is an object, it is JSON encoded automatically.
	 */
	prepend: function(key, o, expires) {
		var rc = exec(this.handle, 'prepend', key, o, expires);
		if (rc) {
			error(memcached.error(this.handle, rc));
		}
	},
	
	/**
	 * @function mc.append
	 * 
	 * ### Synopsis
	 * 
	 * mc.append(key, value);
	 * mc.append(key, value, expires);
	 * 
	 * Store information in memcached indexed by key.
	 * 
	 * If an object identified by key already exists on the server, an error occurs.
	 * 
	 * @param {string} key - key of data to set in memcached.
	 * @param {string|object} value - value to set in memcached.
	 * @param {int} expires - time to live (seconds) of item in cache.
	 * 
	 * ### Notes
	 * 
	 * appends the given value string to the value of an existing item.
	 * 
	 * If an object identified by key does not exist, an error occurs.
	 */
	append: function(key, o, expires) {
		var rc = exec(this.handle, 'append', key, o, expires);
		if (rc) {
			error(memcached.error(this.handle, rc));
		}
	},

	/**
	 * @function mc.get
	 * 
	 * ### Synopsis
	 * 
	 * var o = mc.get(handle, key);
	 * 
	 * Get a value from memcached by key.
	 * 
	 * @param {string} key - key of data to get from memcached
	 * @return {object} o - if the item stored is an object, the object is returned; if the item stored is a string, the string is returned; or false if an error occurred.
	 */
	get: function(key) {
		var o = memcached.get(this.handle, key);
		if (o.rc !== 0) {
			return false;
		}
		return o.flags === 0 ? Json.decode(o.value) : o.value;
	},
	
	/**
	 * @function mc.mget
	 * 
	 * ### Synopsis:
	 * 
	 * var o = mc.get(handle, array_of_keys);
	 * 
	 * Get multiple values, identified by an array of keys, from memcached.
	 * 
	 * The returned object is a hash of returned values, indexed by the key.  
	 * 
	 * For each of these keys, the value is an object in the form described at the top of this page.
	 * 
	 * @param {array} keys - array of keys of data to get from memcached
	 * @return {object} o - hash of returned objects; index/hash/key is the key, value is an object or string; or false if an error occurred.
	 */
	mget: function(keys) {
		var res = memcached.mget(this.handle, keys);
		var o = {};
		res.each(function(value, key) {
			if (value.rc !== 0) {
				o[key] = value.flags === 0 ? Json.decode(value.value) : value.value;
			}
		});
		return res;
	},

	/**
	 * @function mc.remove
	 * 
	 * ### Synopsis
	 * 
	 * var rc = mc.remove(handle, key);
	 * 
	 * Remove an item from memcached by key.
	 * 
	 * @param {string} key - key of data to set in memcached.
	 * @return {int} rc - result code; 0 if no error, otherwise the error code.
	 */
	remove: function(key) {
		return memcached.remove(this.handle, key);
	},
	
	/**
	 * @function mc.close
	 * 
	 * ### Synopsis
	 * 
	 * mc.close();
	 * 
	 * Close an opened memcached handle and free all resources used for it.
	 */
	close: function() {
		memcached.close(this.handle);
	}
});

exports = Memcached;
