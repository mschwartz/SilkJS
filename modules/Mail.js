/**
 * @class Mail
 * 
 * ### Synopsis
 * 
 * Interface for sending email messages from SilkJS programs.
 */

"use strict";

var popen = require('popen').popen,
    uuid = require('phpjs').uuid,
    fs = require('fs'),
    process = require('builtin/process'),
    console = require('console');

function Mail() {
    this.attachments = [];
    this.to = [];
    this.cc = [];
    this.bcc = [];
    this.subject = 'No Subject';
    this.text = null;
    this.html = null;
    this.attachments = [];
}

Mail.prototype.extend({
    setFrom: function(from) {
        this.from = from;
    },
    setTo: function(to) {
        this.to = [to];
    },
    addTo: function(to) {
        this.to.push(to);
    },
    setCc: function(cc) {
        this.cc = [cc];
    },
    addCc: function(cc) {
        this.cc.push(cc);
    },
    setBcc: function(bcc) {
        this.bcc = [bcc];
    },
    addBcc: function(bcc) {
        this.bcc.push(bcc);
    },
    setSubject: function(subject) {
        this.subject = subject;
    },
    setText: function(text) {
        this.text = text;
    },
    setHtml: function(html) {
        this.html = html;
    },
    addAttachment: function(filename, contentType, body) {
        this.attachments.push({ filename: filename, contentType: contentType, body: body });
    },
    send: function() {
//        var sendmail = new popen('sendmail -t', 'w');
        var out = [];
        function putln(s) {
            out.push(s);
        }
        putln('From: ' + this.from);
        putln('To: ' + this.to.join(','));
        if (this.cc.length) {
            putln('CC: ' + this.cc.join(','));
        }
        if (this.bcc.length) {
            putln('BCC: ' + this.bcc.join(','));
        }
        putln('Subject: ' + this.subject);
        putln('Date: ' + new Date().toGMTString());
        if (this.text && !this.html && !this.attachments.length) {
            putln('Content-type: text/plain');
            putln('');
            putln(this.text);
        }
        else {
            var boundary = 'silk_mail_'+uuid();
            putln('MIME-Version: 1.0');
            if (this.attachments.length) {
                putln('Content-Type: multipart/mixed; boundary="'+boundary+'"');
            }
            else {
                putln('Content-Type: multipart/alternative; boundary="' + boundary + '"');
            }
            putln('');
    //        putln('This is a message with multiple parts in MIME format.');
            if (this.text) {
                putln('--' + boundary);
    //            putln('MIME-version: 1.0');
                putln('Content-Type: text/plain');
                putln('Content-Transfer-Encoding: quoted-printable');
                putln('');
                putln(this.text);
                putln('');
            }
            if (this.html) {
                putln('--' + boundary);
    //            putln('MIME-version: 1.0');
                putln('Content-Type: text/html');
                putln('Content-Transfer-Encoding: quoted-printable');
                putln('');
                putln(this.html);
                putln('');
            }
            if (this.attachments.length) {
                this.attachments.each(function(attachment) {
                    putln('--' + boundary);
                    putln('Content-Type: ' + attachment.contentType + '; name="'+attachment.filename + '"');
                    putln('Content-Disposition: attachment; filename="' + attachment.filename + '"');
                    putln('Content-Transfer-Encoding: base64');
                    putln('');
                    putln(attachment.body);
                    putln('');
                });
            }
            putln('--' + boundary + '--');
            putln('');
        }
        fs.writeFile('/tmp/'+boundary, out.join('\r\n'));
        process.exec('sendmail -t </tmp/'+boundary);
        fs.unlink('/tmp/'+boundary);
//        sendmail.close();
    }
});

exports.Mail = Mail;
