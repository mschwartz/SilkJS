/**
 * @module modules/MySQL
 * 
 * ### Synopsis
 * 
 * MySQL interface built on top of the builtin/mysql module.
 * 
 * ### Description
 * 
 * General purpose MySQL interface.  The Schema ORM is built on top of this interface.
 * 
 * ### Usage
 * 
 * var MySQL = require('MySQL');
 * 
 * ### Note
 * 
 * Throughout this documentation, an SQL variable that is an instantiated MySQL object is referred to.  This is the "vision" for how to use this class.
 * 
 * ### See Also
 * 
 * builtin/mysql
 * modules/Schema
 */
var mysql = require('builtin/mysql');

function isArray(v) {
    return toString.apply(v) === '[object Array]';
}

function addslashes(str) {
    return (str + '').replace(/([\\"'])/g, "\\$1").replace(/\0/g, "\\0");
}

/**
 * @constructor MySQL
 * 
 * ### Synopsis
 * 
 * var SQL = new MySQL();
 *
 * MySQL class constructor.
 *  
 * ### Description
 * 
 * The MySQL class represents a connection to a MySQL server and database.  
 * 
 * You may create multiple MySQL instances connected to different servers and/or databases.
 * 
 * ### Example
 * 
 * ```
 * var SQL = new MySQL();
 * SQL.connect(host, user, password, db);
 * console.dir(SQL.getDataRows('SELECT * FROM table'));
 * ```
 * 
 */
var MySQL = function() {
    this.queryCount = 0;
    this.handle = null;
};

MySQL.prototype.extend({
	/**
	 * @function SQL.connect
	 * 
	 * ### Synopsis
	 * 
	 * SQL.connect(host, user, passwd, db);
	 * 
	 * Connect an SQL instance to a server and database.
	 * 
	 * @param {string} host - host name of MySQL server.
	 * @param {string} user - MySQL username.
	 * @param {string} passwd - MySQL password for the given user;
	 * @param {string} db - name of database to use.
	 */
    connect: function(host, user, passwd, db) {
        if (!this.handle) {
            host = host || Config.mysql.host;
            user = user || Config.mysql.user;
            passwd = passwd != undefined ? passwd : Config.mysql.passwd;
            db = db || Config.mysql.db;
            this.handle = mysql.connect(host, user, passwd, db);
        }
    },

	/**
	 * @function SQL.getDataRows
	 * 
	 * ### Synopsis
	 * 
	 * var rows = SQL.getDataRows(query);
	 * 
	 * Query the database for an array of rows.
	 * 
	 * ### Description
	 * 
	 * An array of rows is returned if the query succeeds and there are one or more rows matching the query.
	 * 
	 * Each row in the array is an object/hash, with key/value pairs.  The key is the column name in the table, the value is the value from the column for the selected row.
	 * 
	 * The query may be a string or an array.  If it is an array, this function will join that array with newline.  See examples below.
	 * 
	 * @param {string|array} query - the query to perform (a SELECT)
	 * @returns {array|false} rows - array (possibly empty) of result set, or false if error.
	 * 
	 * ### Examples
	 * 
	 * _Query as string_
	 * ```
	 * console.dir(SQL.getDataRows('SELECT * FROM table'));
	 * ```
	 * 
	 * _Query as array_
	 * ```
	 * console.dir(SQL.getDataRows([
	 *		'SELECT',
	 *		'	*',
	 *		'FROM',
	 *		'	table'
	 * ]));
	 * ```
	 */
    getDataRows: function(sql) {
        sql = isArray(sql) ? sql.join('\n') : sql;
        try {
            return mysql.getDataRows(this.handle, sql);
//        	return eval(mysql.getDataRowsJson(sql).replace(/\n/igm, '\\n'));
        }
        catch (e) {
//            log('Exception ' + e);
//            log(sql);
            throw new SQLException(this.queryCount + ' ' + e, sql);
        }
        this.queryCount++;
    },

	/**
	 * @function SQL.getDataRows
	 * 
	 * ### Synopsis
	 * 
	 * var row = SQL.getDataRow(query);
	 * 
	 * Query the database for a single record.
	 * 
	 * ### Description
	 * 
	 * An object is returned if the query succeeds and there are one or more rows matching the query.
	 * 
	 * The object is a hash with key/value pairs.  The key is the column name in the table, the value is the value from the column for the selected row.
	 * 
	 * The query may be a string or an array.  If it is an array, this function will join that array with newline.  See examples below.
	 * 
	 * @param {string|array} query - the query to perform (a SELECT)
	 * @return {object|false} row - object representing the row queried, or false if error.
	 * 
	 * ### Examples
	 * 
	 * _Query as string_
	 * ```
	 * console.dir(SQL.getDataRow('SELECT * FROM table'));
	 * ```
	 * 
	 * _Query as array_
	 * ```
	 * console.dir(SQL.getDataRow([
	 *		'SELECT',
	 *		'	*',
	 *		'FROM',
	 *		'	table'
	 * ]));
	 * ```
	 */
    getDataRow: function(sql) {
        sql = isArray(sql) ? sql.join('\n') : sql;
        try {
            return mysql.getDataRow(this.handle, sql);
        }
        catch (e) {
            throw new SQLException(e, sql);
        }
    },

	/**
	 * @function SQL.getScalar
	 * 
	 * ### Synopsis
	 * 
	 * var value = SQL.getScalar(query);
	 * 
	 * Query the database for a single column value from a single record.
	 * 
	 * ### Description
	 * 
	 * This method is handy for <code>SELECT COUNT(*) FROM table</code> type queries.  
	 * 
	 * The query may be a string or an array.  If it is an array, this function will join that array with newline.  See examples below.
	 * 
	 * @param {string|array} query - the query to perform (a SELECT)
	 * @return {int|string|false} value - the value of the query, or false if error.
	 * 
	 * ### Examples
	 * 
	 * _Query as string_
	 * ```
	 * console.dir(SQL.getScalar('SELECT * FROM table'));
	 * ```
	 * 
	 * _Query as array_
	 * ```
	 * console.dir(SQL.getScalar([
	 *		'SELECT',
	 *		'	*',
	 *		'FROM',
	 *		'	table'
	 * ]));
	 * ```
	 */
    getScalar: function(sql) {
        sql = isArray(sql) ? sql.join('\n') : sql;
        try {
            return mysql.getScalar(this.handle, sql);
        }
        catch (e) {
            throw new SQLException(e, sql);
        }
    },


	/**
	 * @function SQL.update
	 * 
	 * ### Synopsis
	 * 
	 * var affectedRows = SQL.update(query);
	 * 
	 * Perform an update query (e.g. not a select type query) and return the number of affected rows.
	 * 
	 * The query may be a string or an array.  If it is an array, this function will join that array with newline.  See examples below.
	 * 
	 * @param {string|array} query - the query to perform (an update/delete/alter table/etc.)
	 * @returns {int} affectedRows - the number of rows in the table affected by the update query.
	 */
    update: function(sql) {
        sql = isArray(sql) ? sql.join('\n') : sql;
        try {
            return mysql.update(this.handle, sql);
        }
        catch (e) {
            throw new SQLException(e, sql);
        }
    },

	/**
	 * @function SQL.insertId
	 * 
	 * ### Synopsis
	 * 
	 * var id = SQL.insertId();
	 * 
	 * Retrieves the ID generated for an AUTO_INCREMENT column by the previous query (usually INSERT or REPLACE).
	 * 
	 * @returns {int} the ID.
	 */
    insertId: function() {
        try {
	        return mysql.getScalar(this.handle, 'SELECT LAST_INSERT_ID()');
        }
        catch (e) {
            throw new SQLException(e, sql);
        }
			
    },

    startTransaction: function() {
        try {
	        return mysql.update(this.handle, 'START TRANSACTION');
        }
        catch (e) {
            throw new SQLException(e, sql);
        }
    },
    
    endTransaction: function() {
        try {
	        return mysql.update(this.handle, 'END TRANSACTION');
        }
        catch (e) {
            throw new SQLException(e, sql);
        }
    },
    
    commit: function() {
         try {
	       return mysql.update(this.handle, 'COMMIT');
        }
        catch (e) {
            throw new SQLException(e, sql);
        }
    },
    
    rollback: function() {
        try {
	        return mysql.update(this.handle, 'ROLLBACK');
        }
        catch (e) {
            throw new SQLException(e, sql);
        }
    },
    
    quote: function(s) {
        if (isArray(s)) {
            var ret = [];
            s.each(function(e) {
                ret.push(MySQL.prototype.quote(e));
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
                addslashes(s),
                "'"
            ].join('');
        }
    },

    close: function() {
        mysql.close(this.handle);
        this.handle = null;
    }
});

if (exports) {
    exports.MySQL = MySQL;
}

