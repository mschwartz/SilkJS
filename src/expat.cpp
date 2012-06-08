/**
 * @namespace builtin/expat
 *
 * ### Synopsis
 *
 * var expat = require('builtin.expat');
 *
 * Low level interface to libexpat.
 *
 */
#include "SilkJS.h"
#include <expat.h>

struct p {
    XML_Parser parser;
    Persistent<Object> thisObj;
    Persistent<Function> start;
    Persistent<Function> end;
    Persistent<Function> charHandler;
};

void start(void *data, const char *el, const XML_Char **attr) {
    p *ptr = (p *)data;
    JSOBJ o = Object::New();
    for (const XML_Char **a = attr; *a; a += 2) {
        // printf("%s = %s\n", a[0], a[1]);
        o->Set(String::New(a[0]), String::New(a[1]));
    }
    Handle<Value>argv[2] = { String::New(el), o };
    ptr->start->Call(ptr->thisObj, 2, argv);
}
void end(void *data, const char *el) {
    p *ptr = (p *)data;
    Handle<Value>argv[1] = { String::New(el) };
    ptr->end->Call(ptr->thisObj, 1, argv);
}
void charHandler(void *data, const char *txt, int txtlen) {
    p *ptr = (p *)data;
    Handle<Value>argv[1] = { String::New(txt, txtlen) };
    ptr->charHandler->Call(ptr->thisObj, 1, argv);
}

/**
 * @function expat.parser
 * 
 * ### Synopsis
 * 
 * var parser = expat.parser(thisObj, startFn, endFn, textFn);
 * 
 * Instantiate an XML parser.
 * 
 * The thisObj argument is the value of "this" passed to startFn and endFn.
 * 
 * The startFn function is called with an element name and attributes object when an open tag is encountered.
 * 
 * The endFn function is called with an element name when a close tag is encountered.
 *
 * The textFn function is called with the value between the tags.
 *
 * ### Callback Signatures
 *
 * startFn(name, attr); // {string} name, {object} attr
 * endFn(name); // {string} name
 * textFn(text); // {string} text
 *
 * @param {object} thisObj - object to be passed as "this" to startFn, endFn, and textFn
 * @param {function} startFn - function to be called when a start tag is encountered during parsing.
 * @param {function} endFn - function to be called when an end tag is encountered during parsing.
 * @param {function} textFn - function to be called with the text (value) between start/end tags during parsing.
 *
 * @return {object} parser - opaque handle to an expat parser.
 */
static JSVAL parser(JSARGS args) {
    p *ptr = new p;
    ptr->thisObj = Persistent<Object>::New(args[0]->ToObject());
    ptr->parser = XML_ParserCreate(NULL);
    XML_SetUserData(ptr->parser, (void *) ptr);
    XML_SetElementHandler(ptr->parser, start, end);
    XML_SetCharacterDataHandler(ptr->parser, charHandler);
    ptr->start = Persistent<Function>::New(Local<Function>::Cast(args[1]));
    ptr->end = Persistent<Function>::New(Local<Function>::Cast(args[2]));
    ptr->charHandler = Persistent<Function>::New(Local<Function>::Cast(args[3]));
    return External::New(ptr);
}


/**
 * @function expat.destroy
 * 
 * ### Synopsis
 * 
 * expat.destroy(parser);
 * 
 * Destroy (free memory and resources used by) a expat parser.
 * 
 * @param {object} parser - opaque handle to a parser, as returned by expat.parser(). 
 */
static JSVAL destroy(JSARGS args) {
    p *ptr = (p *)JSEXTERN(args[0]);
    ptr->end.Dispose();
    ptr->end.Clear();
    ptr->start.Dispose();
    ptr->start.Clear();
    ptr->thisObj.Dispose();
    ptr->thisObj.Clear();
    delete ptr;
    return Undefined();
}

/**
 * @function expat.parse
 * 
 * ### Synopsis
 * 
 * var success = expat.parse(parser, xml);
 * 
 * Parse XML using this parser.
 * 
 * @param {object} parser - opaque handle to parser to use.
 * @param {string} xml - xml document to parse
 * @return {int} success - 1 if document was successfully parsed.
 * 
 * ### Note
 * 
 * When you created the expat parser, you provided callback functions to handle processing the parsed XML file.
 */

static JSVAL parse(JSARGS args) {
    p *ptr = (p *)JSEXTERN(args[0]);
    String::Utf8Value xml(args[1]->ToString());
    int res = XML_Parse(ptr->parser, *xml, strlen(*xml), 1);
    return Integer::New(res);
}

void init_expat_object() {
    Handle<ObjectTemplate>expat = ObjectTemplate::New();

    expat->Set(String::New("parser"), FunctionTemplate::New(parser));
    expat->Set(String::New("destroy"), FunctionTemplate::New(destroy));
    expat->Set(String::New("parse"), FunctionTemplate::New(parse));
    
    builtinObject->Set(String::New("expat"), expat);
}
