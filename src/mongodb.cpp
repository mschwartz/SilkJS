#include "SilkJS.h"
#include <client/dbclient.h>

using namespace mongo;

#ifndef OSX
/**
 * var conn = mongodb.init(host);
 * @param {string} host - string of form hostname[:port]
 */
static JSVAL mongodb_init(JSARGS args) {
    HandleScope scope;
	String::Utf8Value host(args[0]);
	DBClientConnection *c = new DBClientConnection(*host);
	try {
		c->connect();
	}
	catch (DBException &e) {
		return ThrowException(String::New(e.what()));
	}
    return scope.Close(External::New(c));
}


#endif

void init_mongodb_object() {
    HandleScope scope;

    Handle<ObjectTemplate>mongodb = ObjectTemplate::New();
    builtinObject->Set(String::New("mongodb"), mongodb);
}

