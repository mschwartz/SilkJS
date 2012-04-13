/** @ignore */

"use strict";

var console = require('console'),
    Json = require('Json'),
    cURL = require('cURL');

var User = require('User').User;

function Connection(username, password) {
    this.username = username;
    this.password = password;
    this.url = 'https://' + username + ':' + password + '@api.github.com';
    var result = this.get('/users/'+username);
    if (this.status != 200) {
        throw result.message;
    }
}

function isString(v) {
    return typeof v === 'string';
}

function op(conn, method, path, params) {
    var config = {
        method: method,
        url: conn.url + path
    };
    if (params) {
        config.params = !isString(params) ? Json.encode(params) : params;
    }
    console.dir(config);
    var result = cURL(config);
    conn.status = result.status;
    if (result.responseText.length) {
        return Json.decode(result.responseText);
    }
    return conn.status;
}

Connection.prototype.extend({
    get: function(path, params) {
        return op(this, 'GET', path, params);
    },
    patch: function(path, params) {
        return op(this, 'PATCH', path, params);
    },
    getUser: function(username) {
        return new User(this, username);
    },
    getEmails: function() {
        return op(this, 'GET', '/user/emails');
    },

    /**
     * @function GitHub.addEmails
     *
     * ### Synopsis
     *
     * var emails = gh.addEmails(newEmails);
     *
     * Add email address(es).
     *
     * @param {array} newEmails - array of email addresses to add.
     * @returrn {array} emails - array of emails for user, after the additions.
     */
    addEmails: function(emails) {
        return op(this, 'POST', '/user/emails', emails);
    },

    /**
     * @function GitHub.deleteEmails
     *
     * ### Synopsis
     *
     * gh.DeleteEmails(newEmails);
     *
     * Delete email address(es).
     *
     * @param {array} newEmails - array of email addresses to delete.
     *
     * ### Note
     *
     * This method will throw a descriptive error {object} if GitHub reports an error.
     */
    deleteEmails: function(emails) {
        return op(this, 'DELETE', '/user/emails', emails);
        var response = cURL({
            method: 'DELETE',
            url: this.url + '/user/emails',
            params: Json.encode(emails)
        });
        this.status = response.status;
        if (this.status != 204) {
            throw Json.decode(response.responseText);
        }
    }
});

exports.Connection = Connection;
