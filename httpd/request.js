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
			lines.each(function(line) {
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
            req.queryParams = {};
			if (uriParts[1]) {
				uriParts[1].split('&').each(function(part) {
					part = part.split('=');
                    try {
						req.queryParams[part[0]] = data[part[0]] = decodeURIComponent(part[1].replace(/\+/g, ' '));
                    }
                    catch (e) {
//                        console.dir(uriParts);
//                        console.dir(e);
//                        console.log(e.stack);
//                        throw e;
                    }
				});
			}
			req.method = first[0];
			req.uri = uriParts[0];
			req.proto = first[2] || 'http/0.9';
			req.remote_addr = net.remote_addr();
			
			// process cookies
            req.cookies = {};
			if (headers['cookie']) {
				try {
					headers['cookie'].split(/;\s*/).each(function(cookie) {
						var cookieParts = cookie.split('=');
						req.cookies[cookieParts[0]] = data[cookieParts[0]] = decodeURIComponent(cookieParts[1].replace(/\+/g, ' '));
					});
				}
				catch (e) {
					
				}
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
					mimeParts.each(function(part) {
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
                            lines.shift(); lines.shift(); lines.shift();
                            if (lines[lines.length-1] === '') {
                                lines.pop();
                            }
							data[name] = lines.join('\n');
						}
					});
				}
				else {
					post = http.readPost(stream, contentLength);
					if (post) {
						if (headers['content-type'] && headers['content-type'].match(/^application\/x-www-form-urlencoded/i)) {
							post.split('&').each(function(part) {
								part = part.split('=');
								data[part[0]] = decodeURIComponent(part[1].replace(/\+/gm, ' '));
							});
						}
						else {
							data.post = post;
						}
					}
					else {
						data.post = '';
					}
				}
			}
			req.data = data;
			
			return true;
		},
		getHeader: function(key) {
			return req.headers[key.toLowerCase()];
		},
		close: function() {
			if (stream) {
				http.closeStream(stream);
				stream = null;
			}
		}
		
	};
})();

