#include include.js
#include require.js

require.path.unshift('./lib/');

{ fork, sleep, exit, wait }  = require 'builtin/process'
console = require 'builtin/console'
Config = require 'Config'

String::beginsWith = (str) -> if @match(new RegExp "^#{str}") then true else false
String::endsWith = (str) -> if @match(new RegExp "#{str}$") then true else false

########################################################################################################################

class Logfile
    { fork, sleep } = require 'builtin/process'

    construct: ->
        @logfile = require 'builtin/logfile'

    start: ->
        if !fork()
            while true
                sleep 5
                @logfile.flush()
        return

    write: (s) ->
        @logfile.write(s)

########################################################################################################################

class Semaphore
    { open, close, exists, flock, LOCK_EX, LOCK_UN, O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC } = require 'builtin/fs'

    constructor: (@lockFile) ->
        if !exists(@lockFile)
            @fd = open @lockFile, O_WRONLY|O_CREAT|O_TRUNC, 0644
        else
            @fd = open @lockFile, O_RDONLY

    lock: ->
        flock @fd, LOCK_EX

    unlock: ->
        flock @fd, LOCK_UN

    close: ->
        close @fd

########################################################################################################################

class ServerSocket
    { listen, accept } = require 'builtin/net'

    constructor: (port) ->
        @socket = listen port

    accept: ->
        accept @socket

########################################################################################################################

class HttpRequest
    http = require 'builtin/http'
    net = require 'builtin/net'

    stream = null;

    init: (sock) ->
        @start = new Date().getTime()
        stream = http.openStream sock unless stream
        raw = http.readHeaders stream
        return false unless raw

        lines = raw.split '\n'
        first = lines.shift().split /\s+/

        # create @headers from key: value pairs (request header lines)
        headers = {}
        for line in lines
            colon = line.indexOf ':'
            key = line.substr 0,colon
            value = line.substr(colon+1).replace /&\s+/, ''
            headers[key.toLowerCase()] = value
        @headers = headers;

        # process Host: header
        host = 'localhost'
        port = Config.port
        if headers.host
            host = headers.host.split ':'
            port = host[1] || 80
            host = host[0]
        @host = host;
        @port = port

        # process query_string
        uriParts = first[1].split '?'
        data = {}
        if uriParts[1]
            for part in uriParts[1].split '&'
                [key,value] = part.split '&'
                data[key] = decodeUriComponent value.replace(/\+/g, '')

        @method = first[0]
        @uri = uriParts[0]
        @proto = first[2] || 'http/0.9'
        @remote_addr = net.remote_addr()

        # process cookies
        if headers.cookie
            for cookie in headers.cookie.split /;\s*/
                [key,value] = cookie.split '='
                data[key] = decodeUriComponent value.replace(/+/g, ' ')

        # process POST data
        post = ''
        mimeParts = []
        contentLength = headers['content-length']
        if (contentLength)
            contentType = headers['content-type']
            if contentType?.toLwerCase().indexOf('multipart/form-data') != -1
                boundary = contentType.replace /^.*?boundary=/i, ''
                post = http.readMime stream, contentLength, boundary
                mimeParts = post.split '--'+boundary
                mimeParts.shift()
                mimeParts.pop()
                for part in mimeParts
                    lines = part.split '\r\n'
                    disposition = lines[1].split /;\s*/
                    name = disposition[1].replace /name="(.*)"/, '$1'
                    if disposition.length == 3
                        data[name] =
                            filename: disposition[2].replace /filename="(.*)"/, '$1'
                            contentType: lines[2].replace /Content-Type:\s*/i, ''
                            size: parseInt(lines[3].replace(/Content-Length:\s*/i, ''), 10)
                            contentEncoding: lines[4].replace /Content-Encoding:\s*/i, ''
                            content: lines[6]
                    else
                        data[name] = lines[3]
            else
                post = http.readPost stream, contentLength
                if headers['content-type'].match(/^application\/x-www-form-urlencoded/i)
                    for part in post.split '&'
                        [key,value] = part.split '='
                        data[key] = decodeURIComponent value.replace(/\+/gm, ' ')
        @data = data
        true

    getHeader: (key) ->
        headers[key.toLowerCase()]

    close: ->
        http.closeStream stream if stream
        stream = null

########################################################################################################################

class HttpResponse
    buffer = require 'builtin/buffer'
    net = require 'builtin/net'
    fs = require 'builtin/fs'

    responseCodeText =
    	100: 'Continue'
    	101: 'Switching Protocols'
    	200: 'OK'
    	201: 'Created'
    	202: 'Accepted'
    	203: 'Non-Authoritative Information'
    	204: 'No Content'
    	205: 'Reset Content'
    	206: 'Partial Content'
    	300: 'Multiple Choices'
    	301: 'Moved Permanently'
    	302: 'Found'
    	303: 'See Other'
    	304: 'Not Modified'
    	305: 'Use Proxy'
    	307: 'Temporary Redirect'
    	400: 'Bad Request'
    	401: 'Unauthorized'
    	402: 'Payment Required' # note RFC says reserved for future use
    	403: 'Forbidden'
    	404: 'Not Found'
    	405: 'Method Not Allowed'
    	406: 'Not Acceptable'
    	407: 'Proxy Authentication Required'
    	408: 'Request Timeout'
    	409: 'Conflict'
    	410: 'Gone'
    	411: 'Length Required'
    	412: 'Precondition Failed'
    	413: 'Request Entity Too Large'
    	414: 'Request-URI Too Long'
    	415: 'Unsupported Media Type'
    	416: 'Request Range Not Satisfiable'
    	417: 'Expectation Failed'
    	500: 'Internal Server Error'
    	501: 'Not Implemented'
    	502: 'Bad Gateway'
    	503: 'Service Unavailable'
    	504: 'Gateway Timeout'
    	505: 'HTTP Version Not Supported'


    constructor: ->
        @buf = buffer.create()

    init: (@clientSocket, keepAlive, requestsHandled) ->
        buffer.reset @buf
        @status = 200
        @contentLength = 0
        @contentType = 'text/html'
        @cookies = {}
        @headers =
            Server: 'SILK JS CoffeeScript Server'
        @data = {}
        @headersSent = false

        ka = if req.headers then req.headers.connection else false
        if ka && keepAlive
            @headers.Connection = 'Keep-Alive'
            @headers['keep-alive'] = 'timeout: 5; max=' + (Config.requestsPerChild - requestsHandled)
        else
            keepAlive = false
            @headers.Connection = 'close'

        return keepAlive

    stop: ->
        throw 'RES.STOP'

    reset: ->
        buffer.reset(@buf)

    setCookie: (key, value, expires, path, domain) ->
        cookie =
            value: value
        if expires
            cookie.expires = if Util.isDate(expires) then expires.toGMTString() else expires
        cookie.path = path if path?
        cookie.domain = domain if domain?
        @cookies[key] = cookie

    setHeader: (key, value) ->
        @headers[key] = value

    sendHeaders: ->
        return if headersSent
        out = '';
        out += req.proto + ' ' + @status + ' ' +  responseCodeText[@status] + '\r\n';
        out += 'Date: ' + new Date().toGMTString() + '\r\n';
        for key,value of @headers
            out += key + ': ' + value + '\r\n'
        for key,cookie of @cookies
            out += 'Set-Cookie: ' + key + '=' +encodeURIComponent(cookie.value);
            out += '; Expires='+cookie.expires unless cookie.expires
            out += '; Path='+encodeURIComponent(cookie.path) unless cookie.path
            out += '; Domain='+encodeURIComponent(cookie.domain) unless cookie.domain
            out += '\r\n';
        out += 'Content-Type: ' + @contentType + '\r\n';
        out += 'Content-Length: ' + @contentLength + '\r\n\r\n';
        try
            net.cork @clientSocket, true
            net.write @clientSocket, out, out.length
        catch e
            throw new SilkException e
        headersSent = true

    write: (s) ->
        try
            buffer.write @buf, s, s.length
        catch e
            throw new SilkException e

    writeln: (s) ->
        try
            buffer.write @buf, s+'\n', s.length+1
        catch e
            throw new SilkException e

    write64: (s) ->
        try
            buffer.write64 @buf, s, s.length
        catch e
            throw new SilkException e

    sendFile: (fn) ->
        @reset();	# so extra stuff sent with res.write() isn't sent'
        modified = fs.fileModified fn
        size = fs.fileSize fn
        @headers['last-modified'] = new Date(modified*1000).toGMTString()
        ifModifiedSince = req.headers['if-modified-since']
        if ifModifiedSince
            ifModifiedSince = Date.parse(ifModifiedSince) / 1000
            if modified <= ifModifiedSince
                @status = 304
                @stop()
        @contentLength = size
        try
            @sendHeaders();
            net.sendFile(@clientSocket, fn, 0, size) # (FileSystem.readfile64(fn));
        catch e
            throw new SilkException e
        @stop();

    flush: ->
        len = buffer.size @buf
        if len
            @contentLength = len
            @sendHeaders()
            net.writeBuffer @clientSocket, @buf
        else
            @sendHeaders()
        net.cork @clientSocket, false

    redirect: (uri) ->
        @status = 302
        base = 'http://' + req.host
        base += ':' + req.port unless req.port == 80
        @setHeader 'Location', base + uri
        @stop()

    close: ->
        buffer.destroy @buf

########################################################################################################################

class HttpChild
    v8 = require 'builtin/v8'
    Config = require 'Config'
    net = require 'builtin/net'
    { fork, exit, sleep, wait, getpid, error } = require 'builtin/process'
    { stat, readFile, close, realpath, isDir, isFile, exists } = require 'builtin/fs'

    notFound = () ->
        res.reset()
        res.result = 404
        global.notFound_action() if global.notFound_action
        res.write '<h1>Not Found</h1>'
        res.stop()

    @requestHandler: null

    jst_cache = {}
    getCachedJst = (fn) ->
        jst = jst_cache[fn]
        mtime = stat(fn).mtime
        if !jst || mtime < jst.mtime
            source = readFile fn
            jst =
                mtime: mtime
                parsed: Jst.parse source
            jst_cache[fn] = jst;
        return jst.parsed

    includeJst = (fn) ->
        jst = getCachedJst Config.documentRoot + fn
        return Jst.includeParsed jst,
            include: includeJst

    runJst = (fn) ->
        jst = getCachedJst fn
        out = Jst.executeParsed jst
            include: includeJst
        res.contentLength = out.length
        res.write out
        res.stop()

    coffee_cache = {}
    getCachedCoffeeScript = (fn) ->
        coffee = coffee_cache[fn]
        mtime = stat(fn).mtime
        if !coffee || mtime > coffee.mtime
            v8.freeScript coffee.script if coffee
            source = readFile fn
            compiled = CoffeeScript.compile source
            script = v8.compileScript compiled
            coffee =
                mtime: mtime
                script: script
            coffee_cache[fn] = coffee
        return coffee.script

    runCoffee = (fn) ->
        coffee = getCachedCoffee fn
        out = v8.runScript coffee
        res.stop()

    runMarkdown = (fn) ->
        content = readFile fn
        converter = new Showdown.converter()
        html = converter.makeHtml content
        res.write html
        res.stop()

    sendFile = (fn) ->
        res.sendFile fn

    contentTypes =
        coffee: { contentType: 'text/html',       handler: runCoffee }
        jst:	{ contentType: 'text/html',       handler: runJst }
        md:		{ contentType: 'text/html',       handler: runMarkdown }
        ogg:	{ contentType: 'audio/ogg',       handler: sendFile }
        mp3:	{ contentType: 'audio/mpeg3',     handler: sendFile }
        png:	{ contentType: 'image/png',       handler: sendFile }
        ico:	{ contentType: 'image/ico',       handler: sendFile }
        gif:	{ contentType: 'image/gif',       handler: sendFile }
        jpg:	{ contentType: 'image/jpeg',      handler: sendFile }
        jpeg:	{ contentType: 'image/jpeg',      handler: sendFile }
        html:	{ contentType: 'text/html',       handler: sendFile }
        js:		{ contentType: 'text/javascript', handler: sendFile }
        css:	{ contentType: 'text/css',        handler: sendFile }
        xml:	{ contentType: 'text/xml',        handler: sendFile }

    handleRequest = () ->
        parts = req.uri.substr(1).split '/'
        parts[0] = 'main' unless parts[0].length
        action = parts[0] + '_action'
        global[action]() if global[action]
        fnPath = Config.documentRoot + req.uri
        fn = realpath fnPath
        notFound() unless fn
        if isDir fn
            if !req.uri.endsWith '/'
                log 'redirect ' + req.uri + ' ' + fn + ' ' + fn.substr(fn.length-1, 1)
                res.redirect req.uri + '/'
            found = ''
            for index in Config.directoryIndex
                f = fn
                f += '/'
                f += index
                if exists f
                    found = f
                    break
            if found
                fn = found
            else if isFile fn + '/index.jst'
                fn += '/index.jst'
            else if isFile fn + '/index.coffee'
                fn += '/index.coffee'
            else if isFile fn + '/index.md'
                fn += '/index.md'

        notFound() unless isFile fn

        res.status = 200
        req.path = fn

        parts = fn.split '.'
        if parts.length > 1
            extension = parts.pop().toLowerCase()
            handler = contentTypes[extension]
            if handler
                res.contentType = handler.contentType
                handler.handler fn

        info = stat fn
        notFound() unless info

        res.contentType = 'text/plain'
        res.sendHeaders()
        net.sendFile @clientSocket, fn, 0, info.size

    constructor: (@serverSocket) ->
        global.req = new HttpRequest
        global.res = new HttpResponse

    HttpChild::pids = {}

    spawn: (@serverSocket) ->
        pid = fork()
        if pid == -1
            throw error()
        else if pid != 0
            HttpChild::pids[pid] = this;
            return pid
        @pid = getpid()
        @run();

    run: ->
        serverSocket = @serverSocket
        requestHandler = @requestHandler

        semaphore = new Semaphore Config.lockFile
        REQUESTS_PER_CHILD = Config.requestsPerChild || 100
        requestsHandled = 0;
        while requestsHandled < REQUESTS_PER_CHILD
            semaphore.lock();
            clientSocket = serverSocket.accept()
            semaphore.unlock()
            keepAlive = true
            while keepAlive
                keepAlive = false if ++requestsHandled > REQUESTS_PER_CHILD
                try
                    break unless req.init clientSocket
                    keepAlive = res.init clientSocket, keepAlive, requestsHandled
                    if requestHandler?
                        requestHandler()
                    handleRequest()
                catch e
                    Error.exceptionHandler e unless e == 'RES.STOP'
                res.flush()
#                console.log req.remote_addr + ' ' + req.method + ' ' + req.uri + ' completed in ' + (new Date().getTime() - req.start) + '\n'
                # logfile.write req.remote_addr + ' ' + req.method + ' ' + req.uri + ' completed in ' + (new Date().getTime() - req.start) + '\n'
            req.close()
            net.close clientSocket
            v8.gc()
        semaphore.close()
        res.close()
        return

    wait: ->
        while true
            o = wait()
            if HttpChild::pids?[o.pid]?
                delete HttpChild::pids[o.pid]
                this.spawn()
        return

########################################################################################################################

main = (files...) ->
    for file of files
        if file.endsWith '.js' or file.endsWith '.coffee'
            include file

    serverSocket = new ServerSocket(Config.port)
    child = new HttpChild serverSocket

    child.spawn serverSocket for i in [ 1 .. Config.numChildren ]
    child.wait()
