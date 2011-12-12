// httpd/request.js

req = (function() {
	var stream = null;
	function readHeaders() {
		var headers = '';
		var newlineCount = 0;
		var c;
		while (newlineCount < 2) {
			c = http.readByte(stream);
			switch(c) {
				case 0:
				case -1:
					return null;
				case 13:
					continue;
				case 10:
					headers += '\n';
					newlineCount++;
					break;
				default:
					headers += String.fromCharCode(c);
					newlineCount = 0;
					break;
			}
		}
		return headers;
	}
	return {
		init: function(sock) {
			req.start = new Date().getTime();
			if (!stream) {
    			stream = http.openStream(sock);
            }
//			var raw = readHeaders(); // http.readHeaders(stream);
			var raw = http.readHeaders(stream);
			if (raw == null) {
				return false;
			}
			var lines = raw.split('\n');
			var first = lines.shift().split(/\s+/);
			var headers = {};
			forEach(lines, function(line) {
				var colon = line.indexOf(':');
				var key = line.substr(0, colon);
				var value = line.substr(colon+1).replace(/^\s+/, '');
				headers[key.toLowerCase()] = value;
			});
			req.headers = headers;
			var host = 'localhost';
			var port = Config.port;
			if (headers.host) {
				host = headers.host.split(':');
				port = host[1] || '80';
				host = host[0];
			}
			req.host = host;
			req.port = port;
			var uriParts = first[1].split('?');
			var data = {};
			if (uriParts[1]) {
				forEach(uriParts[1].split('&'), function(part) {
					part = part.split('=');
					data[part[0]] = decodeURIComponent(part[1].replace(/\+/g, ' '));
				});
			}
			req.method = first[0];
			req.uri = uriParts[0];
			req.proto = first[2] || 'http/0.9';
			req.remote_addr = net.remote_addr();
			
			// process cookies
			if (headers['cookie']) {
				forEach(headers['cookie'].split(/;\s*/), function(cookie) {
					var cookieParts = cookie.split('=');
					data[cookieParts[0]] = decodeURIComponent(cookieParts[1].replace(/\+/g, ' '));
				});
			}

			// process POST data
			var post = '';
			var mimeParts = [];
			var contentLength = headers['content-length'];
			if (contentLength) {
				var contentType = headers['content-type'];
				if (contentType && contentType.toLowerCase().indexOf('multipart/form-data') != -1) {
					var boundary = contentType.replace(/^.*?boundary=/i, '');
					post = http.readMime(stream, contentLength, boundary);
					mimeParts = post.split('--'+boundary);
					mimeParts.shift();
					mimeParts.pop();
					forEach(mimeParts, function(part) {
						var lines = part.split('\r\n');
						var disposition = lines[1].split(/;\s*/);
						var name = disposition[1].replace(/name="(.*)"/, '$1');
						if (disposition.length == 3) {
							// file upload
							data[name] = {
								filename: disposition[2].replace(/filename="(.*)"/, '$1'),
								contentType: lines[2].replace(/Content-Type:\s*/i, ''),
								size: parseInt(lines[3].replace(/Content-Length: /i, ''), 10),
								contentEncoding: lines[4].replace(/Content-Endcoding: /i, ''),
								content: lines[6]
							};
						}
						else {
							data[name] = lines[3];
						}
					});
				}
				else {
					post = http.readPost(stream, contentLength);
					if (headers['content-type'].match(/^application\/x-www-form-urlencoded/i)) {
						forEach(post.split('&'), function(part) {
							part = part.split('=');
							data[part[0]] = decodeURIComponent(part[1].replace(/\+/gm, ' '));
						});
					}
				}
			}
			req.data = data;
			
			return true;
		},
		getHeader: function(key) {
			return headers[key.toLowerCase()];
		},
		close: function() {
			if (stream) {
				http.closeStream(stream);
				stream = null;
			}
		}
		
	};
})();

