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
        printf("%s = %s\n", a[0], a[1]);
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
 * var parser = expat.parser(thisObj, startFn, endFn);
 * 
 * Instantiate an XML parser.
 * 
 * The thisObj argument is the value of "this" passed to startFn and endFn.
 * 
 * The startFn function is called with an element name and attributes object when an open tag is encountered.
 * 
 * The endFn function is called with an element name when a close tag is encountered.
 * 
 * @param args
 * @return 
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
