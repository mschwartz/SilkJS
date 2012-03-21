/* 
 * SilkJS XMLHttpRequest implementation
 */

(function() {
	var xhrHelper = require('builtin/xhrHelper');

	var UNSENT = 0,
		OPENED = 1,
		HEADERS_RECEIVED = 2,
		LOADING = 3,
		DONE = 4;
        
    function isString(o) {
        return typeof v === 'string';
    }
    
	XMLHttpRequest = function() {
		this.extend({
			// private
			readyState: UNSENT,
			headers: {},
			responseHeaders: {},
			errorFlag: 0,
			// public
			status: 0,
			method: 'GET',
			url: '',
			username: undefined,
			password: undefined,
			onreadystatechange: function() {},
			statusText: '',
			responseText: null
		});
		return this;
	};
	XMLHttpRequest.prototype.extend({
		open: function(method, url, async, username, password) {
			this.extend({
				method: method,
				url: url,
				username: username,
				password: password,
				readyState: OPENED,
				status: 0,
				statusText: ''
			});
		},
		setRequestHeader: function(header, value) {
			this.headers[header] = value;
		},
		send: function(data) {
			var o = xhrHelper.request({
				method: this.method,
				username: this.username,
				password: this.password,
				url: this.url,
				data: data
			});
			if (isString(o)) {
				throw o;
			}
			this.responseText = o.responseText;
			this.status = o.status;
			this.readyState = DONE;
			this.onreadystatechange();
		},
		abort: function() {

		},
		getResponseHeader: function(header) {
			var lcHeader = header.toLowerCase();
			if (this.readyState == UNSENT || this.readyState == OPENED || this.errorFlag || lcHeader === 'set-cookie' || lcHeader === 'set-cookie2') {
				return null;
			}
			return this.responseHeaders[header] || null;
		},
		getAllResponseHeaders: function() {
			var headers = [];
			responseHeaders.each(function(value, key) {
				headers.push(key + ': ' + value);
			});
			return headers.join('\r\n');
		}
	});
}());
