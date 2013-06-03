/**
 * @class GitHub
 *
 * ### Synopsis
 *
 * Interface to GitHub.
 *
 * This class uses the GitHub v3 API, documented here: http://developer.github.com/v3/.
 */
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
    this.page = 1;
    this.per_page = 100;
    this.nextUrl = null;
    this.url = 'https://' + username + ':' + password + '@api.github.com';
    var response = cURL({
        url: this.url + '/users/'+username
    });
    var result = Json.decode(response.responseText);

    if (response.status !== 200) {
        throw result;
    }
    this.status = response.status;
    this.user = result;
}

GitHub.prototype.extend({
    /* private */ _repoName: function(repo) {
        return (repo.indexOf('/') !== -1) ? repo : (this.username + '/' + repo);
    },

    /* private */
    _get: function(url) {
        var response = cURL({
            url: url
        });
        this.status = response.status;
        return Json.decode(response.responseText);
    },

    _getList: function(url) {
        this.nextUrl = url;
        var response = cURL({
            url: url + '?page='+this.page+'&per_page='+this.per_page
        });
        this.status = response.status;
        if (this.status !== 200) {
            this.nextUrl = 0;
            return false;
        }
        var list = Json.decode(response.responseText);
        this.nextPage = this.page+1;
        return list;
    },
    next: function() {
        if (!this.nextUrl) {
            return false;
        }
        this.page = this.nextPage;
        var ret =  this._getList(this.nextUrl);
        this.page = 1;
        return ret;
    },

    _getBoolean: function(url) {
        var response = cURL({
            url: url
        });
        this.status = response.status;
        return this.status === 204 ? true : Json.decode(response.responseText);
    },

    _delete: function(url) {
        var response = cURL({
            method: 'DELETE',
            url: url
        });
        this.status = response.status;
        return this.status === 204 ? true : Json.decode(response.responseText);
    },

    _post: function(url, params) {
        var config = {
            url: url,
            method: 'POST'
        };
        if (params) {
            config.params = Json.encode(params);
        }
        var response = cURL(config);
        this.status = response.status;
        return Json.decode(response.responseText);
    },

    _patch: function(url, params) {
        var config = {
            url: url,
            method: 'PATCH'
        };
        if (params) {
            config.params = Json.encode(params);
        }
        var response = cURL(config);
        this.status = response.status;
        return Json.decode(response.responseText);
    },

    _put: function(url, params) {
        var config = {
            url: url,
            method: 'PUT'
        };
        if (params) {
            config.params = Json.encode(params);
        }
        else {
            config.params = ' ';    // hack - forces a content-length: 1 header, requried by github api
        }
        var response = cURL(config);
        this.status = response.status;
        return this.status === 204 ? true : Json.decode(response.responseText);
    },



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
        var user = this._get(this.url + '/users/'+username);
        if (username === this.username && this.status === 200) {
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
        return this._patch(this.url + '/user', o);
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
        return this._getList(this.url + '/user/emails');
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
        return this._post(this.url + '/user/emails', emails);
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
        return this._delete(this.url + '/user/emails', emails);
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
        return this._getList(this.url + '/users/'+username+'/followers');
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
        return this._getList(this.url + '/users/'+username+'/following');
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
        return this._getBoolean(this.url + '/user/following/'+username);
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
        return this._put(this.url + '/user/following/'+username);
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
     * @return {boolean} success - true if authenticated user is no longer following the specified user.
     *
     */
    unfollow: function(username) {
        return this._delete(this.url + '/user/following/'+username);
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
        return this._getList(this.url + '/user/keys');
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
        return this._get(this.url + '/user/keys/'+id);
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
        return this._post(this.url + '/user/keys', { title: title, key: key });
    },

    /**
     * @function GitHub.updateKey
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
        return this._patch(this.url + '/user/keys/'+id, { title: title, key: key });
    },

    /**
     * @function GitHub.deleteKey
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
        return this._delete(this.url + '/user/keys/'+id);
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
        return this._getList(this.url + '/orgs/' + org + '/repos');
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
        return this._post(url, config);
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
    getRepository: function(repo) {
        var parts = repo.split('/');
        var user;
        if (parts.length > 1) {
            repo = parts[1];
            user = parts[0];
        }
        else {
            user = this.username;
        }
        return this._get(this.url + '/repos/' + user + '/' + repo);
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
        return this._patch(this.url + '/repos/' + user + '/' + name, config);
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
        return this._getList(this.url + '/repos/' + user + '/' + name + '/contributors');
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
        return this._getList(this.url + '/repos/' + user + '/' + name + '/languages');
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
        return this._getList(this.url + '/repos/' + user + '/' + name + '/teams');
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
        return this._getList(this.url + '/repos/' + user + '/' + name + '/tags');
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
        return this._getList(this.url + '/repos/' + user + '/' + name + '/branches');
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
        return this._getList(this.url + '/repos/' + user + '/' + name + '/collaborators');
    },

    /**
     * @function GitHub.isCollaborator
     *
     * var isCollaborator = gh.isCollaborator(repo, user);
     *
     * Determine if a user is a collaborator for a repo.
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {string} oUser - name of a user to check.
     * @return {boolean} isCollaborator - true if user is a collaborator.
     */
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
        return this._getBoolean(this.url + '/repos/' + user + '/' + repo + '/collaborators/' + oUser);
    },

    /**
     * @function GitHub.addCollaborator
     *
     * var success = gh.addCollaborator(repo, user);
     *
     * Add a user as a collaborator for a repo.
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {string} oUser - name of a user to add.
     * @return {boolean} success - true if user added as a collaborator.
     */
    addCollaborator: function(repo, oUser) {
        var parts = repo.split('/');
        var user;
        if (parts.length > 1) {
            repo = parts[1];
            user = parts[0];
        }
        else {
            user = this.username;
        }
        return this._put(this.url + '/repos/' + user + '/' + repo + '/collaborators/' + oUser);
    },

    /**
     * @function GitHub.removeCollaborator
     *
     * var success = gh.removeCollaborator(repo, user);
     *
     * Remove a user as a collaborator for a repo.
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {string} oUser - name of a user to remove.
     * @return {boolean} success - true if user removed as a collaborator.
     */
    removeCollaborator: function(repo, oUser) {
        return this._delete(this.url + '/repos/' + this._repoName(repo) + '/collaborators/' + oUser);
    },

    /**
     * @funciton GitHub.listCommits
     *
     * ### Synopsis
     *
     * var commits = gh.listCommits(repo);
     * var commits = gh.listCommits(repo, params);
     *
     * List commits on a repository.
     *
     * The params object has the form:
     *
     * + {string} sha - optional, SHA or branch to start listingj commits from.
     * + {string} path - optional, Only commits containing this file path will be returned.
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {object} params - params object, per above description.
     * @return {array} commits - array of objects describing the commits.
     */
    listCommits: function(repo, params) {
        return this._getList(this.url + '/repos/' + this._repoName(repo) + '/commits');
    },

    /**
     * @function GitHub.getCommit
     *
     * ### Synopsis
     *
     * var commit = gh.getCommit(repo, sha);
     *
     * Get a single commmit.
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {string} sha - sha string that identifies the commit
     * @return {object} commit - info about the commit
     */
    getCommit: function(repo, sha) {
        return this._get(this.url + '/repos/' + this._repoName(repo) + '/commits/' + sha);
    },

    /**
     * @function GitHub.listComments
     *
     * ### Synopsis
     *
     * var comments = gh.listComments(repo);        // get comments for repository
     * var comments = gh.listComments(repo, sha);   // get comments for a single commit
     *
     * List comments for a repository or a single commit
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {string} sha - sha string that identifies the commit
     * @return {Object} comments - array of comments
     */
    listComments: function(repo, sha) {
        var url = this.url + '/repos/' + this._repoName(repo);
        if (sha) {
            url += '/commits/' + sha;
        }
        url += '/comments';
        return this._getList(url);
    },

    /**
     * @function GitHub.createCommitComment
     *
     * ### Synopsis
     *
     * var comment = gh.createCommitComment(repo, sha, config);
     *
     * Create a commit comment
     *
     * The config parameter is an object with the following members:
     *
     * + {string} body - required, body of the comment
     * + {string} commit_id - required, SHA of the commit to comment on
     * + {number} line - required, line number in the file to comment on
     * + {string} path - required, relative path of file to comment on
     * + {number} position - required - line index in the diff to comment on
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {string} sha - sha string that identifies the commit
     * @param {object} config - see description above.
     * @return {object} comment - resulting comment information
     */
    createCommitComment: function(repo, sha, comment) {
        return this._post(this.url + '/repos/' + this._repoName(repo) + '/commits/' + sha + '/comments', comment);
    },

    /**
     * @function GitHub.getCommitComment
     *
     * ### Synopsis
     *
     * var comment = gh.getCommitComment(repo, id);
     *
     * Get a single commit comment
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {string} id - sha string that identifies the commit comment
     * @return {object} comment - resulting comment information
     */
    getCommitComment: function(repo, id) {
        return this._get(this.url + '/repos/' + this._repoName(repo) + '/comments/' + id);
    },

    /**
     * @function GitHub.updateCommitComment
     *
     * ### Synopsis
     *
     * var comment = gh.updateCommitComment(repo, id, body);
     *
     * Get a single commit comment
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {string} id - sha string that identifies the commit comment
     * @param {string} body - new text for the comment
     * @return {object} comment - resulting comment information
     */
    updateCommitComment: function(repo, id, body) {
        return this._patch(this.url + '/repos/' + this._repoName(repo) + '/comments/' + id, { body: body });
    },

    /**
     * @function GitHub.compareCommits
     *
     * ### Synopsis
     *
     * var info = gh.compareCommits(repo, base, head);
     *
     * Compare two commits
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {string} base - SHA of base commit
     * @param {string} head - SHA of head commit
     * @return {object} info - comparison information about the two commits
     */
    compareCommits: function(repo, base, head) {
        return this._get(this.url + '/repos/' + this._repoName(repo) + '/compare/' + base + '...' + head);
    },

    /**
     * @function GitHub.deleteCommitComment
     *
     * ### Synopsis
     *
     * var success = gh.deleteCommitComment(repo, id);
     *
     * Delete a commit comment.
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {string} id - sha string that identifies the commit comment
     * @return {boolean} success - true if comment deleted
     *
     * ### Notes
     * If an error occurs, an exception is thrown.
     */
    deleteCommitComment: function(repo, id) {
        return this._delete(this.url + '/repos/' + this._repoName(repo) + '/comments/' + id);
    },

    // downloads api

    /**
     * @function GitHub.listDownloads
     *
     * ### Synopsis
     *
     * var downloads = gh.listDownloads(repo);
     *
     * List downloads for a repository
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @return {array} downloads - array of objects describing each download item
     */
    listDownloads: function(repo) {
        return this._getList(this.url + '/repos/' + this._repoName(repo) + '/downloads');
    },

    /**
     * @function GitHub.getDownload
     *
     * ### Synopsis
     *
     * var download = gh.getDownload(repo, id);
     *
     * Get a single download
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {string} id - id of the download to get
     * @return {object} downloads - object describing each download item
     */
    getDownload: function(repo, id) {
        return this._get(this.url + '/repos/' + this._repoName(repo) + '/downloads/' + id);
    },
    
    /**
     * @function GitHub.createDownload
     * 
     * ### Synopsis
     * 
     * var success = gh.createDownload(repo, path);
     * var success = gh.createDownload(repo, path, description);
     * var success = gh.createDownload(repo, path, description, contentType);
     * 
     * Create a download for a repo.
     * 
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {string} path - path to file to upload (e.g. the download file to create).
     * @param {string} description - description of the download.
     * @param {string} contentType - content-type of file (MIME type)
     * @return {boolean} success - true if download was created
     */
    createDownload: function(repo, path, description, contentType) {
        var url = this.url + '/repos/' + this._repoName(repo) + '/downloads';
        // create the resource
        if (!fs.exists(path)) {
            throw 'File ' + path + ' does not exist';
        }
        var params = {
            name:  path.split('/').pop(),
            size: fs.fileSize(path)
        };
        if (description) {
            params.description = description;
        }
        if (contentType) {
            params.content_type = contentType;
        }
        //console.log('Creating GitHub resource...')
        var response = cURL({
            method: 'POST',
            url: url,
            params: Json.encode(params)
        });
        var o = Json.decode(response.responseText);
        if (response.status != 201) {
            throw o;
        }
        // upload to S3
        //console.log('Uploading to S3...');
        var config = {
            method: 'POST',
            url: o.s3_url,
            form: [
                { name: 'key', value: o.path },
                { name: 'acl', value: o.acl },
                { name: 'success_action_status', value: 201 },
                { name: 'Filename', value: o.name },
                { name: 'AWSAccessKeyId', value: o.accesskeyid },
                { name: 'Policy', value: o.policy },
                { name: 'Signature', value: o.signature },
                { name: 'Content-Type', value: o.mime_type },
                { name: 'file', value: path, fileUpload: true }
            ]
        };
        response = cURL(config);
        this.status = response.status;
        return response.status == 201;
    },
    
    /**
     * @function GitHub.deleteDownload
     * 
     * ### Synopsis
     * 
     * var success = gh.deleteDownload(repo, id);
     * 
     * Delete a download.
     * 
     * The id parameter can be determined by looking at the id emmber of the object for the download returned by GitHub.listDownloads().
     * 
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {string} id - id of download to delete.
     * @return {boolean} success - true if the download was deleted.
     */
    deleteDownload: function(repo, id) {
        return this._delete(this.url + '/repos/' + this._repoName(repo) + '/downloads/' + id);
    },

    /**
     * @function GitHub.listForks
     *
     * ### Synopsis
     *
     * var forks = gh.listForks(repo);
     * var forks = gh.listForks(repo, sort);
     *
     * List forks of a repo.
     *
     * The sort parameter may be one of the following:
     *
     * + 'newest'
     * + 'oldest'
     * + 'watchers'
     *
     * Default value for sort is 'newest'
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {string} sort - one of the above values
     * @return {Array} forks - array of objects describing each of the forks for the repo
     */
    listForks: function(repo,sort) {
        var url = this.url + '/repos/' + this._repoName(repo) + '/forks';
        if (sort && sort.length) {
            url += '?sort='+sort;
        }
        return this._getList(url);
    },

    /**
     * @function GitHub.createFork
     *
     * ### Synopsis
     *
     * var success = gh.createFork(repo);
     * var success = gh.createFork(repo, org);
     *
     * Create a fork for the authenticated user.
     *
     * If the org parameter is supplied, it is an organization login; the repository will be forked into this organization.
     *
     * <p>
     * Forking a repository happens asynchronously.  Therefore, you may have to wait a short period before accessing the
     * git objects.  If this takes longer than 5 minutes, contact GitHub customer support.
     * </p>
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {string} org - organization to fork into.
     * @return {boolean} success - true if fork request was accepted.
     */
    createFork: function(repo, org) {
        var url = this.url + '/repos/' + this._repoName(repo) + '/forks';
        if (sort && sort.length) {
            url += '?org='+org;
        }
        return this._post(url);
    },

    /**
     * @function GitHub.listRepoDeployKeys
     *
     * ### Synopsis
     *
     * var keys = gh.listRepoDeployKeys(repo);
     *
     * List Repo Deploy Keys.
     *
     * ### See Also:
     *
     * http://help.github.com/deploy-keys/
     * http://developer.github.com/v3/repos/keys/
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @return {Array} keys - array of repo deploy keys.
     */
    listRepoDeployKeys: function(repo) {
        return this._getList(this.url + '/repos/' + this._repoName(repo) + '/keys');
    },

    /**
     * @function GitHub.getRepoDeployKey
     *
     * ### Synopsis
     *
     * var key = gh.getRepoDeployKey(repo, id);
     *
     * Get a specific repo deploy key by id.
     *
     * ### See Also:
     *
     * http://help.github.com/deploy-keys/
     * http://developer.github.com/v3/repos/keys/
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {int} id - id of key to return.
     * @return {Object} key - object containing information for a key.
     */
    getRepoDeployKey: function(repo, id) {
        return this._get(this.url + '/repos/' + this._repoName(repo) + '/keys/' + id);
    },

    /**
     * @function GitHub.createRepoDeployKey
     *
     * ### Synopsis
     *
     * var key = gh.createRepoDeployKey(repo, title, key);
     *
     * This function creates a repo deploy key with the given title and SSH-type key (e.g. DSA or RSA string).
     *
     * ### See Also:
     *
     * http://help.github.com/deploy-keys/
     * http://developer.github.com/v3/repos/keys/
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {string} title - title for the key.
     * @param {string} key - the RSA or DSA key to add.
     * @return {Object} key - information about the created key.
     */
    createRepoDeployKey: function(repo, title, key) {
        return this._post(this.url + '/repos/' + this._repoName(repo) + '/keys', { title: title, key: key });
    },

    /**
     * @function GitHub.editRepoDeployKey
     *
     * ### Synopsis
     *
     * var key = gh.editRepoDeployKey(repo, id, title, key);
     *
     * This function updates a repo deploy key with the given title and SSH-type key (e.g. DSA or RSA string).
     *
     * ### See Also:
     *
     * http://help.github.com/deploy-keys/
     * http://developer.github.com/v3/repos/keys/
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {int} id - id of key to update.
     * @param {string} title - title for the key.
     * @param {string} key - the RSA or DSA key to add.
     * @return {Object} key - information about the created key, or false if an error occurred.
     */
    editRepoDeployKey: function(repo, id, title, key) {
        return this._patch(this.url + '/repos/' + this._repoName(repo) + '/keys/' + id, { title: title, key: key});
    },

    /**
     * @function GitHub.deleteRepoDeployKey
     *
     * ### Synopsis
     *
     * var success = gh.getRepoDeployKey(repo, id);
     *
     * Delete a specific repo deploy key by id.
     *
     * ### See Also:
     *
     * http://help.github.com/deploy-keys/
     * http://developer.github.com/v3/repos/keys/
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {int} id - id of key to delete.
     * @return {boolean} success - true if key was deleted.
     */
    deleteRepoDeployKey: function(repo, id) {
        return this._delete(this.url + '/repos/' + this._repoName(repo) + '/keys/' + id);
    },

    /**
     * @function GitHub.listWatchers
     *
     * ### Synopsis
     *
     * var watchers = gh.listWatchers(repo);
     *
     * Lists watchers of a repository.
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @return {Array} watchers - array of watchers information
     */
    listWatchers: function(repo) {
        return this._getList(this.url + '/repos/' + this._repoName(repo) + '/watchers');
    },

    /**
     * @function GitHub.listWatching
     *
     * ### Synopsis
     *
     * var repos = gh.listWatching(user);
     *
     * List repos being watched by a user.
     *
     * @param {string} user - GitHub user name.
     * @return {Array} repos - array of repo information.
     */
    listWatched: function(user) {
        return this._getList(this.url + (user ? ('/users/' + user + '/watched') : '/user/watched'));
    },

    /**
     * @function GitHub.amWatching
     *
     * ### Synopsis
     *
     * var amWatching = gh.amWatching(repo);
     *
     * Check if current user is watching a repo.
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @return {boolean} amWatching - true if user is watching the repo.
     */
    amWatching: function(repo) {
        return this._getBoolean(this.url + '/user/watched/' + this._repoName(repo));
    },

    /**
     * @function GitHub.watch
     *
     * ### Synopsis
     *
     * var success = gh.watch(repo);
     *
     * Watch a repo.
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @return {boolean} success - true if watch request was successful.
     */
    watch: function(repo) {
        return this._put(this.url + '/user/watched/' + this._repoName(repo));
    },

    /**
     * @function GitHub.stopWatching
     *
     * ### Synopsis
     *
     * var success = gh.stopWatching(repo);
     *
     * Stop watching a repo.
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @return {boolean} success - true if unwatch request was successful.
     */
    stopWatching: function(repo) {
        return this._delete(this.url + '/user/watched/' + this._repoName(repo));
    },

    /**
     * @function GitHub.getBlob
     *
     * ### Synopsis
     *
     * var blob = gh.getBlob(repo, sha);
     *
     * Get a blob.
     *
     * The returned blob has the following members:
     *
     * + {string} content: content of the blob
     * + {string} encoding: encoding of the blob (e.g. "utf-8")
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {string} sha - sha of the blob to get
     * @return {Object} blob - object of the form described above.
     */
    getBlob: function(repo, sha) {
        return this._get(this.url + '/repos/' + this._repoName(repo) + '/git/blobs/' + sha);
    },

    /**
     * @function GitHub.createBlob
     *
     * ### Synopsis
     *
     * var sha = gh.createBlob(repo, obj);
     *
     * Create a blob.
     *
     * The obj parameter describes the blob to be created, and has the following members:
     *
     * + {string} content: content of the blob
     * + {string} encoding: encoding of the blob (e.g. "utf-8")
     *
     * @param {string} repo - name of repo, e.g. mschwartz/SilkJS or SilkJS if the authenticated user is mschwartz.
     * @param {object} obj - description of blob, see above.
     * @return {string} sha - the sha of the created blob, or false if an error occurred.
     */
    createBlob: function(repo, o) {
        return this._post(this.url + '/repos/' + this._repoName(repo) + 'git/blobs', o);
    },

    /*
     * @function GitHub.listIssues
     *
     * ### Synopsis
     *
     * var issues = gh.listIssues(config);
     *
     * List issues for current user.
     *
     * Config object has the following format
     */
    listIssues: function(config) {

    },

    getTree: function(repo, sha, recursive) {
        recursive = recursive ? '?recursive=1' : '';
        var url = this.url + '/repos/' + this._repoName(repo) + '/git/trees/' + sha + recursive;
        return this._get(url);
    }
});

exports.GitHub = GitHub;
