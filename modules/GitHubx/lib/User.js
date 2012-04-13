/** @ignore */

exports.User = function(conn, username) {
    this.conn = conn;
    username = username || conn.username;
    this.extend(conn.get('/users/'+username));
};
exports.User.extend({
    edit: function(o) {
        this.extend(this.conn.patch('/user', o));
    }
});

