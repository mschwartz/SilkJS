class Logfile
    construct: ->
        @logfile = require 'builtin/logfile'

    start: ->
        { fork, sleep } = require 'builtin/process'
        if fork()
            sleep 5
            @logfile.flush()

    write: (s) ->
        @logfile.write(s)

