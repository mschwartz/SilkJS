/* 
 */

(function() {
	SSH = function(aHost, aUser, aPassword) {
		this.connection = ssh2.connect(aHost, aUser, aPassword);
		if (!ssh2.alive(this.connection)) {
			throw ssh2.error(this.connection);
		}
	};
	SSH.prototype.alive = function() {
		return ssh2.alive(this.connection);
	};
	SSH.prototype.error = function() {
		return ssh2.error(this.connection);
	};
	SSH.prototype.exec = function(aCommand) {
		if (!ssh2.exec(this.connection, aCommand)) {
			return false;
		}
		return ssh2.response(this.connection);
	};
	SSH.prototype.exitCode = function() {
		return ssh2.exit_code(this.connection);
	};
	SSH.prototype.close = function() {
		ssh2.close(this.connection);
	}
}());
