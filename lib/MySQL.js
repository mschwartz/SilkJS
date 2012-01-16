Util = global.Util || {
	isArray: function(v) {
		return toString.apply(v) === '[object Array]';
	}
};
MySQL = function() {
	this.queryCount = 0;
	this.connected = false;
};

MySQL.prototype.connect = function(host, user, passwd, db) {
	if (!this.connected) {
		host = host || Config.mysql.host;
		user = user || Config.mysql.user;
		passwd = passwd != undefined ? passwd : Config.mysql.passwd;
		db = db || Config.mysql.db;
		this.handle = mysql.connect(host, user, passwd, db);
	}
	this.connected = true;
};

MySQL.prototype.getDataRows = function(sql) {
	sql = Util.isArray(sql) ? sql.join('\n') : sql;
	try {
		return mysql.getDataRows(sql);
//		return eval(mysql.getDataRowsJson(sql).replace(/\n/igm, '\\n'));
	}
	catch (e) {
		log('Exception ' + e);
		log(sql);
		throw new SQLException(this.queryCount + ' ' + e, sql);
	}
	this.queryCount++;
};

MySQL.prototype.getDataRow = function(sql) {
	sql = Util.isArray(sql) ? sql.join('\n') : sql;
	try {
		return mysql.getDataRow(sql);
	}
	catch (e) {
		throw new SQLException(e, sql);
	}
};

MySQL.prototype.getScalar = function(sql) {
	sql = Util.isArray(sql) ? sql.join('\n') : sql;
	try {
		return mysql.getScalar(sql);
	}
	catch (e) {
		throw new SQLException(e, sql);
	}
};

MySQL.prototype.update = function(sql) {
	sql = Util.isArray(sql) ? sql.join('\n') : sql;
	try {
		return mysql.update(sql);
	}
	catch (e) {
		throw new SQLException(e, sql);
	}
};

MySQL.prototype.insertId = function() {
	return mysql.getScalar('SELECT LAST_INSERT_ID()');
};

MySQL.prototype.quote = function(s) {
	if (Util.isArray(s)) {
		var ret = [];
		forEach(s, function(e) {
			ret.push(SQL.quote(e));
		});
		return ret;
	}
	else if (s === null || s === undefined) {
		return 'NULL';
	}
	else if (s === true || s == 'yes') {
		return "'1'";
	}
	else if (s === false || s == 'no') {
		return "'0'";
	}
	else {
		s = s == undefined ? '' : s;
		return [
			"'",
			Util.addslashes(s),
			"'"
		].join('');
	}
};
 
MySQL.prototype.close = function() {
	mysql.close();
};
