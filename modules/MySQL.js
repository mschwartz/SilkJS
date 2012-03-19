var mysql = require('builtin/mysql');

function isArray(v) {
    return toString.apply(v) === '[object Array]';
}

function addslashes(str) {
    return (str + '').replace(/([\\"'])/g, "\\$1").replace(/\0/g, "\\0");
}

var MySQL = function() {
    this.queryCount = 0;
    this.handle = null;
};

MySQL.prototype.extend({
    connect: function(host, user, passwd, db) {
        if (!this.handle) {
            host = host || Config.mysql.host;
            user = user || Config.mysql.user;
            passwd = passwd != undefined ? passwd : Config.mysql.passwd;
            db = db || Config.mysql.db;
            this.handle = mysql.connect(host, user, passwd, db);
        }
    },

    getDataRows: function(sql) {
        sql = isArray(sql) ? sql.join('\n') : sql;
        try {
            return mysql.getDataRows(this.handle, sql);
    //		return eval(mysql.getDataRowsJson(sql).replace(/\n/igm, '\\n'));
        }
        catch (e) {
            log('Exception ' + e);
            log(sql);
            throw new SQLException(this.queryCount + ' ' + e, sql);
        }
        this.queryCount++;
    },

    getDataRow: function(sql) {
        sql = isArray(sql) ? sql.join('\n') : sql;
        try {
            return mysql.getDataRow(this.handle, sql);
        }
        catch (e) {
            throw new SQLException(e, sql);
        }
    },

    getScalar: function(sql) {
        sql = isArray(sql) ? sql.join('\n') : sql;
        try {
            return mysql.getScalar(this.handle, sql);
        }
        catch (e) {
            throw new SQLException(e, sql);
        }
    },

    update: function(sql) {
        sql = isArray(sql) ? sql.join('\n') : sql;
        try {
            return mysql.update(this.handle, sql);
        }
        catch (e) {
            throw new SQLException(e, sql);
        }
    },

    insertId: function() {
        return mysql.getScalar(this.handle, 'SELECT LAST_INSERT_ID()');
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

