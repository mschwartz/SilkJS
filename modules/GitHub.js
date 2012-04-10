/** @ignore */

var cURL = require('cURL'),
    Json = require('Json'),
    console = require('console');

function GitHub(username, password) {
    this.username = username;
    this.password = password;
    this.url = 'https://' + username + ':' + password + '@api.github.com';
    var response = cURL({
        url: this.url + '/users/'+username
    });
    var result = Json.decode(response.responseText);
    
    if (response.status !== 200) {
        throw result.message;
    }
    this.status = response.status;
    this.user = result;
}

GitHub.prototype.extend({
    getUser: function(username) {
        username = username || this.username;
        var response = cURL({
            url: this.url + '/users/'+username
        });
        this.status = response.status;
        var user = Json.decode(response.responseText);
        if (username === this.username && response.status === 200) {
            this.user = user;
        }
        return user;
    },
    editUser: function(o) {
        var response = cURL({
            method: 'PATCH',
            url: this.url + '/user',
            params: Json.encode(o)
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },
    getEmails: function() {
        var response = cURL({
            url: this.url + '/user/emails'
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },
    addEmails: function(emails) {
        var response = cURL({
            method: 'POST',
            url: this.url + '/user/emails',
            params: Json.encode(emails)
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },
    deleteEmails: function(emails) {
        var response = cURL({
            method: 'DELETE',
            url: this.url + '/user/emails',
            params: Json.encode(emails)
        });
        this.status = response.status;
        if (this.status == 204) {
            return "{}";
        }
        return Json.decode(response.responseText);
    },
    listFollowers: function(username) {
        username = username || this.username;
        var response = cURL({
            url: this.url + '/users/'+username+'/followers'
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },
    listFollowing: function(username) {
        username = username || this.username;
        var response = cURL({
            url: this.url + '/users/'+username+'/following'
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },
    amFollowing: function(username) {
        username = username || this.username;
        var response = cURL({
            url: this.url + '/user/following/'+username
        });
        this.status = response.status;
        return this.status === 204;
    },
    follow: function(username) {
        var response = cURL({
            method: 'PUT',
            url: this.url + '/user/following/'+username,
            params: ' ' // hack - forces a content-length: 1 header, required by github api
        });
        this.status = response.status;
        console.log(this.status);
        console.dir(response.responseText);
        return this.status == 204 ? true : Json.decode(response.responseText);
    },
    unfollow: function(username) {
        var response = cURL({
            method: 'DELETE',
            url: this.url + '/user/following/'+username
        });
        this.status = response.status;
        return this.status == 204 ? true : Json.decode(response.responseText);
    },
    
    listKeys: function() {
        var response = cURL({
            url: this.url + '/user/keys'
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },
    
    getKey: function(id) {
        var response = cURL({
            url: this.url + '/user/keys/'+id
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },
    
    /**
     * @function GitHub.createKey
     * 
     * ### Synopsis
     * 
     * var response = GitHub.createKey(title, key)
     */
    createKey: function(title, key) {
        var response = cURL({
            method: 'POST',
            url: this.url + '/user/keys',
            params: Json.encode({ title: title, key: key })
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },
    
    /**
     * @function GitHub.updateKey
     * 
     * ### Synopsis
     * 
     * var response = GitHub.updateKey(id, title, key)
     */
    updateKey: function(id, title, key) {
        var response = cURL({
            method: 'PATCH',
            url: this.url + '/user/keys/'+id,
            params: Json.encode({ title: title, key: key })
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },
    
    /**
     * @function GitHub.deleteKey
     * 
     * ### Synopsis
     * 
     * var response = GitHub.deleteKey(id)
     */
    deleteKey: function(id) {
        var response = cURL({
            method: 'delete',
            url: this.url + '/user/keys/'+id
        });
        this.status = response.status;
        return this.status == 204 ? true : Json.decode(response.responseText);
    },

    /**
     * @function GitHub.listRepos
     *
     * ### Synopsis
     *
     * var repos = gh.listRepos();
     * var repos = gh.listRepos(user);
     * var repos = gh.listRepos(user, type);
     *
     * List repositories for the specified user.  The type variable may be "all", "owner", "member" or "public" (default).
     *
     * @param {string} user - username of user to get repositories for.  If ommitted, it is the current authenticated user.
     * @param {string} type - type of repositories to get.
     * @returns {array} repos - array of repo objects.
     */
    listRepos: function(user, type) {
        user = user || this.username;
        var url = this.url + '/users/' + user + '/repos';
        if (type) {
            url += '?type='+type;
        }
        var response = cURL({
            url: url
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },

    listCollaborators: function(name) {
        var parts = name.split('/');
        var user;
        if (parts.length > 1) {
            name = parts[1];
            user = parts[0];
        }
        else {
            user = this.username;
        }

        var url = this.url + '/repos/' + user + '/' + name + '/collaborators';
        var response = cURL({
            url: url
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    }

});

exports.GitHub = GitHub;
