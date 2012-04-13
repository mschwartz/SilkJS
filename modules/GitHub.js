/** @ignore */

var cURL = require('cURL'),
    Json = require('Json'),
    console = require('console');

/**
 * @constructor GitHub
 *
 * ### Synopsis
 *
 * var gh = new GitHub(username, password);
 *
 * Create a new GitHub connection.
 *
 * ### Details
 *
 * Once connected, you may access the methods and member variables of the gh instance/object.  The API maintains the following member variables:
 *
 * + {string} gh.username - username of the authenticated user.
 * + {string} gh.password - password of the authenticated user.
 * + {object} gh.user - current information about the authenticated user.
 * + {int} gh.status - HTTP status code of the last transaction with GitHub API.
 *
 * @param {string} username - GitHub username for authentication.
 * @param {string} password - GitHub password for authentication.
 *
 * ### Notes
 *
 * If the authentication fails or some other error is reported by GitHub, the error message is thrown.
 */
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
    /**
     * @function GitHub.getUser
     *
     * ### Synopsis
     *
     * var user = gh.getUser();
     * var user = gh.getUser(username);
     *
     * Get information about a GitHub user, or the currently authenticated user.
     *
     * @param {string} username - GitHub user to get information for.
     * @return {object} user - information about the user.
     *
     */
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

    /**
     * @function GitHub.editUser
     *
     * ### Synopsis
     *
     * var result = gh.editUser(userInfo);
     *
     * Update the authenticated user.
     *
     * The userInfo object has the following form:
     *
     * + {string} name - optional
     * + {string} email - optional
     * + {string} blog - optional
     * + {string} company - optional
     * + {string} location - optional
     * + {boolean} hireable - optional
     * + {string} bio - optional
     *
     * @param {object} userInfo - Object containing information to update for the authenticated user.
     * @return {object} result - Object containing the information about the authenticated user.
     */
    editUser: function(o) {
        var response = cURL({
            method: 'PATCH',
            url: this.url + '/user',
            params: Json.encode(o)
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },

    /**
     * @function GitHub.getEmails
     *
     * ### Synopsis
     *
     * var emails = gh.getEmails();
     *
     * Get email addresses for the authenticated user.
     *
     * @return {array} emails - array of email addresses for the user.
     */
    getEmails: function() {
        var response = cURL({
            url: this.url + '/user/emails'
        });
        this.status = response.status;
        return Json.decode(response.responseText);
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
        var response = cURL({
            method: 'POST',
            url: this.url + '/user/emails',
            params: Json.encode(emails)
        });
        this.status = response.status;
        return Json.decode(response.responseText);
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
        var response = cURL({
            method: 'DELETE',
            url: this.url + '/user/emails',
            params: Json.encode(emails)
        });
        this.status = response.status;
        if (this.status != 204) {
            throw Json.decode(response.responseText);
        }
    },

    /**
     * @function GitHub.listFollowers
     *
     * ### Synopsis
     *
     * var followers = gh.listFollowers();
     * var followers = gh.listFollowers(username);
     *
     * List followers of the specified GitHub user, or the currently authenticated user.
     *
     * @param {string} username - GitHub username to get followers for.
     * @return {array} followers - array of objects describing each user that is following the specified user.
     */
    listFollowers: function(username) {
        username = username || this.username;
        var response = cURL({
            url: this.url + '/users/'+username+'/followers'
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },

    /**
     * @function GitHub.listFollowing
     *
     * ### Synopsis
     *
     * var following = gh.listFollowing();
     * var following = gh.listFollowing(username);
     *
     * List users that the specified GitHub user, or the currently authenticated user, is following.
     *
     * @param {string} username - GitHub username to get followers for.
     * @return {array} following - array of objects describing each user that is being following by the specified user.
     */
    listFollowing: function(username) {
        username = username || this.username;
        var response = cURL({
            url: this.url + '/users/'+username+'/following'
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },

    /**
     * @function GitHub.amFollowing
     *
     * ### Synopsis
     *
     * var amFollowing = gh.amFollowing(username);
     *
     * Check if authenticated user is following another user.
     *
     * @param {string} username - GitHub username to check if following.
     * @return {boolean} amFollowing - true if authenticated user is following the specfied user.
     */
    amFollowing: function(username) {
        username = username || this.username;
        var response = cURL({
            url: this.url + '/user/following/'+username
        });
        this.status = response.status;
        return this.status === 204;
    },

    /**
     * @function GitHub.follow
     *
     * ### Synopsis
     *
     * var success = gh.follow(username);
     *
     * Follow a user.
     *
     * @param {string} username - GitHub username to follow.
     * @return {boolean} success - true if authenticaed user is following the specified user.
     *
     */
    follow: function(username) {
        var response = cURL({
            method: 'PUT',
            url: this.url + '/user/following/'+username,
            params: ' ' // hack - forces a content-length: 1 header, required by github api
        });
        this.status = response.status;
        console.log(this.status);
        console.dir(response.responseText);
        return this.status == 204;
    },

    /**
     * @function GitHub.unfollow
     *
     * ### Synopsis
     *
     * var success = gh.unfollow(username);
     *
     * Stop following a user.
     *
     * @param {string} username - GitHub username to stop following.
     * @return {boolean} success - true if authenticaed user is no longer following the specified user.
     *
     */
    unfollow: function(username) {
        var response = cURL({
            method: 'DELETE',
            url: this.url + '/user/following/'+username
        });
        this.status = response.status;
        return this.status == 204;
    },

    /**
     * @function GitHub.listKeys
     *
     * ### Synopsis
     *
     * var keys = gh.listKeys();
     *
     * List public keys for the authenticated user.
     *
     * @return {array} keys - array of objects describing each of the publich keys for the authenticated user.
     */
    listKeys: function() {
        var response = cURL({
            url: this.url + '/user/keys'
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },

    /**
     * @function GitHub.getKey
     *
     * ### Synopsis
     *
     * var key = gh.getKey(id);
     *
     * Get a single public key for the authenticated user.
     *
     * @param {string} id - id of the key to retrieve.
     *
     * ### Notes
     *
     * The GitHub.listKeys method returns an array of objects, one per key.  Each object contains the id of the key.
     */
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
     * var response = gh.createKey(title, key);
     *
     * Create a public key.
     *
     * @param {string} title - The title for the key.
     * @param {string} key - The text of the RSA/DSA/etc. key to add.
     * @return {object} response - The key that was added, or an object with descriptive information about why the key could not be added.
     *
     * ### Discussion
     *
     * The GitHub WWW site allows you to upload RSA/DSA/etc. keys to allow git:repoURI authentication.  You will generate the key (file) on your workstation and upload it to GitHub using this method.  The title is a string that identifies the key, e.g. "my workstation."
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
     * @function gh.updateKey
     * 
     * ### Synopsis
     * 
     * var response = GitHub.updateKey(id, title, key);
     *
     * Update an existing public key.
     *
     * @param {string} id - id of the key to update.
     * @param {string} title - The title for the key.
     * @param {string} key - The text of the RSA/DSA/etc. key to add.
     * @return {object} response - The key that was updated, or an object with descriptive information about why the key could not be updated.
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
     * @function gh.deleteKey
     * 
     * ### Synopsis
     * 
     * var success = GitHub.deleteKey(id);
     *
     * Delete a public key.
     *
     * @param {string} id - id of the key to delete.
     * @returns {boolean} success - true if the key was deleted.
     */
    deleteKey: function(id) {
        var response = cURL({
            method: 'delete',
            url: this.url + '/user/keys/'+id
        });
        this.status = response.status;
        return this.status == 204;
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

    /**
     * @function GitHub.listOrgRepos
     *
     * ### Synopsis
     *
     * var repos = gh.listOrgRepos(org);
     *
     * List repositories for the specified organization.
     *
     * @param {string} org - name of organization
     * @return {array} repos - array of repo objects.
     */
    listOrgRepos: function(org) {
        var response = cURL({
            url: this.url + '/orgs/' + org + '/repos'
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },

    /**
     * @funciton GitHub.createRepository
     *
     * ### Synopsis
     *
     * var repo = gh.createRepository(config);
     * var repo = gh.createRepository(config, org);
     *
     * Create a new repository for the authenticated user.  The second form creates a new repository in the specified organization; the authenticated user must be a member of that organization.
     *
     * The config object has the following members:
     *
     * + {string} name - required, name of repository.
     * + {string} description - optional
     * + {string} homepage - optional
     * + {boolean} private - optional, true to create a private repository, false to create a public one.  Private repositories require a paid GitHub account.  Default is false.
     * + {boolean} has_issues - optional, true to enable issues for the repository, false to diaable them.  Default is true.
     * + {boolean} has_wiki - optional, true to enable the wiki for this repository, false to disable it.  Default is true.
     * + {boolean} has_downloads - optional, true to enable downloads for this repository, false to disable them.  Default is true.
     * + {int} team_id - the id of the team that will be granted access to this repository.  This is only valid when creating a repo in an organization.
     *
     * @param {object} config - object describing the attributes of the repository to be created.  See notes above.
     * @return {object} repo - object describing the created repository.
     */
    createRepository: function(config, org) {
        var url = this.url + (org ? ('/orgs/' + org + '/repos') : '/user/repos');
        console.dir(url);
        var response = cURL({
            method: 'POST',
            url: url,
            params: Json.encode(config)
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },

    /**
     * @function GitHub.getRepository
     *
     * ### Synopsis
     *
     * var info = gh.getRepository(repo);
     *
     * Get information about a repository.
     *
     * @param {string} repo - name of repository to get information about (e.g. mschwartz/SilkJS or SilkJS)
     *
     */
    getRepository: function(name) {
        var parts = name.split('/');
        var user;
        if (parts.length > 1) {
            name = parts[1];
            user = parts[0];
        }
        else {
            user = this.username;
        }
        var response = cURL({
            url: this.url + '/repos/' + user + '/' + name
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },

    /**
     * @function GitHub.editRepository
     *
     * ### Synopsis
     *
     * var repoInfo = gh.editRepository(name, config);
     *
     * Update a repository's settings.
     *
     * The config object has the following members:
     *
     * + {string} name - required, name of repository.
     * + {string} description - optional
     * + {string} homepage - optional
     * + {boolean} private - optional, true to create a private repository, false to create a public one.  Private repositories require a paid GitHub account.  Default is false.
     * + {boolean} has_issues - optional, true to enable issues for the repository, false to diaable them.  Default is true.
     * + {boolean} has_wiki - optional, true to enable the wiki for this repository, false to disable it.  Default is true.
     * + {boolean} has_downloads - optional, true to enable downloads for this repository, false to disable them.  Default is true.
     * + {int} team_id - the id of the team that will be granted access to this repository.  This is only valid when creating a repo in an organization.
     *
     * @param {string} name - name of repository to update, e.g. mschwartz/SilkJS or just SilkJS
     * @param {object} config - object describing the attributes of the repository to be updated.  See notes above.
     * @return {object} repo - object describing the created repository.
     */
    editRepository: function(name, config) {
        var parts = name.split('/');
        var user;
        if (parts.length > 1) {
            name = parts[1];
            user = parts[0];
        }
        else {
            user = this.username;
        }

        var url = this.url + '/repos/' + user + '/' + name;
        var response = cURL({
            method: 'PATCH',
            url: url,
            params: Json.encode(config)
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },

    /**
     * @function GitHub.listContributors
     *
     * ### Synopsis
     *
     * var contributors = gh.listcontributors(repo);
     *
     * Get list of contributors for a repository.
     *
     * @param {string} repo - name of repo, e.g. "mschwartz/SilkJS" or "SilkJS" if the authenticated user is "mschwartz"
     * @return {array} contributors - array of objects describing each contributor.
     */
    listContributors: function(name) {
        var parts = name.split('/');
        var user;
        if (parts.length > 1) {
            name = parts[1];
            user = parts[0];
        }
        else {
            user = this.username;
        }

        var url = this.url + '/repos/' + user + '/' + name + '/contributors';
        var response = cURL({
            url: url
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },

    /**
     * @function GitHub.listLanguages
     *
     * ### Synopsis
     *
     * var languages = gh.listLanguages(repo);
     *
     * Get list of languages of a repository (programming languages).
     *
     * @param {string} repo - name of repo, e.g. "mschwartz/SilkJS" or "SilkJS" if the authenticated user is "mschwartz"
     * @return {object} collaborators - object/hash; key is language (e.g. "C"), value is (lines of code?)
     */
    listLanguages: function(name) {
        var parts = name.split('/');
        var user;
        if (parts.length > 1) {
            name = parts[1];
            user = parts[0];
        }
        else {
            user = this.username;
        }

        var url = this.url + '/repos/' + user + '/' + name + '/languages';
        var response = cURL({
            url: url
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },

    /**
     * @function GitHub.listTeams
     *
     * ### Synopsis
     *
     * var teams = gh.listTeams(repo);
     *
     * Get list of teams for a repository.
     *
     * @param {string} repo - name of repo, e.g. "mschwartz/SilkJS" or "SilkJS" if the authenticated user is "mschwartz"
     * @return {array} collaborators - array of objects describing each team.
     */
    listTeams: function (name) {
        var parts = name.split('/');
        var user;
        if (parts.length > 1) {
            name = parts[1];
            user = parts[0];
        }
        else {
            user = this.username;
        }

        var url = this.url + '/repos/' + user + '/' + name + '/teams';
        var response = cURL({
            url: url
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },

    /**
     * @function GitHub.listTags
     *
     * ### Synopsis
     *
     * var tags = gh.listTags(repo);
     *
     * Get list of tags for a repository.
     *
     * @param {string} repo - name of repo, e.g. "mschwartz/SilkJS" or "SilkJS" if the authenticated user is "mschwartz"
     * @return {array} tags - array of objects describing each tag.
     */
    listTags: function (name) {
        var parts = name.split('/');
        var user;
        if (parts.length > 1) {
            name = parts[1];
            user = parts[0];
        }
        else {
            user = this.username;
        }

        var url = this.url + '/repos/' + user + '/' + name + '/tags';
        var response = cURL({
            url: url
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },

    /**
     * @function GitHub.listBranches
     *
     * ### Synopsis
     *
     * var branches = gh.listBranches(repo);
     *
     * Get list of branches for a repository.
     *
     * @param {string} repo - name of repo, e.g. "mschwartz/SilkJS" or "SilkJS" if the authenticated user is "mschwartz"
     * @return {array} tags - array of objects describing each branch.
     */
    listBranches: function (name) {
        var parts = name.split('/');
        var user;
        if (parts.length > 1) {
            name = parts[1];
            user = parts[0];
        }
        else {
            user = this.username;
        }

        var url = this.url + '/repos/' + user + '/' + name + '/branches';
        var response = cURL({
            url: url
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },

    /**
     * @function GitHub.listCollaborators
     *
     * ### Synopsis
     *
     * var collaborators = gh.listCollaborators(repo);
     *
     * Get list of collaborators for a repository.
     *
     * @param {string} repo - name of repo, e.g. "mschwartz/SilkJS" or "SilkJS" if the authenticated user is "mschwartz"
     * @return {array} collaborators - array of objects describing each collaborator.
     */
    listCollaborators: function (name) {
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
    },

    isCollaborator: function(repo, oUser) {
        var parts = repo.split('/');
        var user;
        if (parts.length > 1) {
            repo = parts[1];
            user = parts[0];
        }
        else {
            user = this.username;
        }

        var url = this.url + '/repos/' + user + '/' + repo + '/collaborators/' + oUser;
        var response = cURL({
            url: url
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    }

});

exports.GitHub = GitHub;
