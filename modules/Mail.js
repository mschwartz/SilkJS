/**
 * @class Mail
 * 
 * ### Synopsis
 *
 * var Mail = require('Mail').Mail;
 * var message = new Mail();
 *
 * Interface for sending email messages from SilkJS programs.
 *
 * This class provides support for encoding and sending multipart email messages.  You can use this class to send HTML email, email with attachments, etc.
 *
 * ### Example
 *
 * ```
 * var message = new Mail();
 * message.setFrom(from);
 * message.addTo(to);
 * message.setSubject(subject);
 * message.setHtml('<h1>Test Message</h1>');
 * message.addAttachment('foo', 'text/plain', fs.readFile64('/tmp/foo'));
 * message.send();
 * ```
 *
 * ### NOTE
 *
 * This class relies on the command-line sendmail program.
 *
 * It is located in /usr/sbin/sendmail on OSX, but must be installed on Linux using your package management software (e.g. apt, yum, rpm, etc.)
 *
 *
 */

"use strict";

var uuid = require('phpjs').uuid,
    fs = require('fs'),
    process = require('builtin/process'),
    console = require('console');

/**
 * @consructor Mail
 *
 * ### synopsis
 *
 * var message = new Mail();
 *
 * Create instance of mail message.
 *
 * The message is not sent until you call message.send().  Before you send the message, you will typically set at least the to, subject, and text parts of the message.
 */
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
    /**
     * @function Mail.setFrom
     *
     * ### Synopsis
     *
     * message.setFrom(from);       // e.g. from = 'me@example.com'
     *
     * Set from: field for message.
     *
     * @param {string} from - from: email address for message
     */
    setFrom: function(from) {
        this.from = from;
    },

    /**
     * @function Mail.setTo
     *
     * ### Synopsis
     *
     * message.setTo(to);           // e.g. to = 'you@example.com'
     *
     * Set to: field for message (e.g. recipient's email address).
     *
     * Note that an email message may have multiple recipients.  This function clears the recipient list and sets it to the one email address you provide.
     *
     * See Mail.addTo() for a method to add additional recpient email addresses to the to: field.
     *
     * @param {string} to - initial to: email address for message
     */
    setTo: function(to) {
        this.to = [to];
    },

    /**
     * @function Mail.addTo
     *
     * ### Synopsis
     *
     * message.addTo(to);           // e.g. to = 'you@example.com'
     *
     * Add recipient to the  to: field for message (e.g. recipient's email address).
     *
     * Note that an email message may have multiple recipients.  This function adds the recipient to the to: list.
     *
     * See Mail.setTo() for a method to reset the to: field to an initial single recipient
     *
     * @param {string} to - to email address to add to message
     */
    addTo: function(to) {
        this.to.push(to);
    },

    /**
     * @function Mail.setCc
     *
     * ### Synopsis
     *
     * message.setCc(cc);         // e.g. cc = 'you@example.com'
     *
     * Set cc: field for message (e.g. recipient's email address).
     *
     * Note that an email message may have multiple recipients.  This function clears the cc list and sets it to the one email address you provide.
     *
     * See Mail.addCc() for a method to add additional recipient email addresses to the cc: field.
     *
     * @param {string} cc - email address to set cc: field to for message
     */
    setCc: function(cc) {
        this.cc = [cc];
    },

    /**
     * @function Mail.addCc
     *
     * ### Synopsis
     *
     * message.addCc(cc);           // e.g. cc = 'you@example.com'
     *
     * Add recipient to the  cc: field for message (e.g. recipient's email address).
     *
     * Note that an email message may have multiple recipients.  This function adds the recipient to the cc: list.
     *
     * See Mail.setCc() for a method to reset the cc: field to an initial single recipient
     *
     * @param {string} cc - to email address to add to message
     */
    addCc: function(cc) {
        this.cc.push(cc);
    },

    /**
     * @function Mail.setBcc
     *
     * ### Synopsis
     *
     * message.setBcc(bcc);         // e.g. bcc = 'you@example.com'
     *
     * Set bcc: field for message (e.g. recipient's email address).
     *
     * Note that an email message may have multiple recipients.  This function clears the bcc list and sets it to the one email address you provide.
     *
     * See Mail.addBcc() for a method to add additional recipient email addresses to the bcc: field.
     *
     * @param {string} bcc - email address to set bcc: field to for message
     */
    setBcc: function(bcc) {
        this.bcc = [bcc];
    },

    /**
     * @function Mail.addBcc
     *
     * ### Synopsis
     *
     * message.addBcc(bcc);         // e.g. bcc = 'you@example.com'
     *
     * Add recipient to the bcc: field for message (e.g. recipient's email address).
     *
     * Note that an email message may have multiple recipients.  This function adds the recipient to the bcc: list.
     *
     * See Mail.setBcc() for a method to reset the bcc: field to an initial single recipient
     *
     * @param {string} bcc - bcc email address to add to message
     */
    addBcc: function(bcc) {
        this.bcc.push(bcc);
    },

    /**
     * @function Mail.setSubject
     *
     * ### Synopsis
     *
     * message.setSubject(subject);
     *
     * Set the subject: field for the email message.
     *
     * An email message can have only one subject: field.
     *
     * @param {string} subject - value to set subject: field to for message.
     */
    setSubject: function(subject) {
        this.subject = subject;
    },

    /**
     * @function Mail.setText
     *
     * ### Synopsis
     *
     * message.setText(text);
     *
     * This function sets the text part of the email message.
     *
     * ### Discussion
     *
     * Email messages do not have to be multipart MIME - they can be plain text.
     *
     * For a MIME message, typically containing at least an HTML body, you should set a text body as well.  This is so people who have email clients that do not support HTML will be able to display a plain text version of the message body.
     *
     * For a plain text message, you will only need to set the text part.   Or if you do not set an HTML part, the recipient will see the plain text version, but will also be able to manage any attachments you add to the message.
     *
     * See Message.setHtml() for a method to set the HTML part, and Message.addAttachment() for adding attachment parts.
     *
     * @param {string} text - plain text email message body; the content of the message you want to send.
     */
    setText: function(text) {
        this.text = text;
    },

    /**
     * @function Mail.setHtml
     *
     * ### Synopsis
     *
     * message.setHtml(html);
     *
     * This function sets the HTML part of the email message.
     *
     * ### Discussion
     *
     * Email messages do not have to be multipart MIME - they can be plain text.
     *
     * For a MIME message, typically containing at least an HTML body, you should set a text body as well.  This is so people who have email clients that do not support HTML will be able to display a plain text version of the message body.
     *
     * For a plain text message, you will only need to set the text part.   Or if you do not set an HTML part, the recipient will see the plain text version, but will also be able to manage any attachments you add to the message.
     *
     * See Message.setText() for a method to set the text part, and Message.addAttachment() for adding attachment parts.
     *
     * @param {string} html - HTML email message body; the content of the message you want to send.
     */
    setHtml: function(html) {
        this.html = html;
    },

    /**
     * @function Mail.addAttachment
     *
     * ### Synopsis
     *
     * message.addAttachment(filename, contentType, body);
     *
     * Add an attachment to an email message.
     *
     * ### Discussion
     *
     * Email messages may contain multiple attachment parts.  Use this function to add one attachment at a time to your email message.
     *
     * While it is possible to send binary attachments in binary form, JavaScript does not have a built-in binary type.  As with many of the SilkJS libraries, this class deals with binary as base64 encoded strings; thus your attachments must be base64 encoded.
     *
     * @param filename - name of the attachment
     * @param contentType - MIME content-type of the body (e.g. image/gif)
     * @param {string} body - base64 encoded body of the attachment.
     */
    addAttachment: function(filename, contentType, body) {
        this.attachments.push({ filename: filename, contentType: contentType, body: body });
    },

    /**
     * @function Mail.send
     *
     * ### Synopsis
     *
     * message.send();
     *
     * Sends the email message.
     *
     * This function actually formats the multipart MIME email message, writes it to a file in /tmp, and invokes your system's sendmail program to send the message.
     *
     * You should have set the to:, subject:, and text of the message, at least, before calling this function.'
     *
     * ### NOTE
     *
     * This function does not provide any indication of whether the email was actually sent, if the sendmail binary could be found on the system, etc.
     */
    send: function() {
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
            if (this.text) {
                putln('--' + boundary);
                putln('Content-Type: text/plain');
                putln('Content-Transfer-Encoding: quoted-printable');
                putln('');
                putln(this.text);
                putln('');
            }
            if (this.html) {
                putln('--' + boundary);
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
    }
});

exports.Mail = Mail;
