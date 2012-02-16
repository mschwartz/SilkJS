Config =
    documentRoot: 'docroot'
    port: 7070
    lockFile: '/tmp/silkf.lock'
    numChildren: 250
    requestsPerChild: 100000

exports = Config if exports
