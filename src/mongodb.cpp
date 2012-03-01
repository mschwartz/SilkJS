#include "SilkJS.h"
// HACK
#define MONGO_HAVE_STDINT
#include <mongo.h>

using namespace v8;

static JSVAL _mongodb_init(JSARGS args) {
    HandleScope scope;
	mongo *conn = new mongo();
	mongo_init(conn);
    return scope.Close(External::New(conn));
}

static JSVAL _mongodb_set_op_timeout(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    mongo *conn = (mongo *) wrap->Value();
	return scope.Close(Integer::New(mongo_set_op_timeout(conn, args[1]->IntegerValue())));
}

static JSVAL _mongodb_connect(JSARGS args) {
	HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    mongo *conn = (mongo *) wrap->Value();
	String::Utf8Value host(args[1]->ToString());
	mongo_connect(conn, *host, args[2]->IntegerValue());
	return scope.Close(Integer::New(conn->err));
}

static JSVAL _mongodb_destroy(JSARGS args) {
	HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    mongo *conn = (mongo *) wrap->Value();
	mongo_destroy(conn);
	delete conn;
	return Undefined();
}

static JSVAL _mongodb_replset_init(JSARGS args) {
    HandleScope scope;
	String::Utf8Value set_name(args[0]->ToString());
	mongo *conn = new mongo();
	mongo_replset_init(conn, *set_name);
    return scope.Close(External::New(conn));
}

static JSVAL _mongodb_replset_add_seed(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    mongo *conn = (mongo *) wrap->Value();
	String::Utf8Value set_name(args[1]->ToString());
	mongo_replset_add_seed(conn, *set_name, args[2]->IntegerValue());
    return Undefined();
}

static JSVAL _mongo_replset_connect(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    mongo *conn = (mongo *) wrap->Value();
	mongo_replset_connect(conn);
    return scope.Close(Integer::New(conn->err));
}


///////////////// BSON ///////////////////

static JSVAL _bson_init(JSARGS args) {
    HandleScope scope;
	bson *b = new bson();
	bson_init(b);
    return scope.Close(External::New(b));
}

static JSVAL _bson_append_string(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson *b = (bson *) wrap->Value();
	String::Utf8Value key(args[1]->ToString());
	String::Utf8Value value(args[2]->ToString());
    return scope.Close(Integer::New(bson_append_string(b, *key, *value)));
}

static JSVAL _bson_append_int(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson *b = (bson *) wrap->Value();
	String::Utf8Value key(args[1]->ToString());
    return scope.Close(Integer::New(bson_append_int(b, *key, args[2]->IntegerValue())));
}

static JSVAL _bson_append_new_oid(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson *b = (bson *) wrap->Value();
	String::Utf8Value key(args[1]->ToString());
    return scope.Close(Integer::New(bson_append_new_oid(b, *key)));
}

static JSVAL _bson_finish(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson *b = (bson *) wrap->Value();
    return scope.Close(Integer::New(bson_finish(b)));
}

static JSVAL _bson_destroy(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson *b = (bson *) wrap->Value();
	bson_destroy(b);
	delete b;
    return Undefined();
}

///////////////// QUERIES ///////////////////

static JSVAL _mongo_insert(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    mongo *conn = (mongo *) wrap->Value();
	String::Utf8Value db(args[1]->ToString());
	Local<External>wrap2 = Local<External>::Cast(args[2]);
	bson *b = (bson *) wrap2->Value();
    return scope.Close(Integer::New(mongo_insert(conn, *db, b)));
}


void init_mongodb_object() {
    HandleScope scope;

    Handle<ObjectTemplate>mongodb = ObjectTemplate::New();
	
	mongodb->Set(String::New("MONGO_MAJOR"), Integer::New(MONGO_MAJOR));
	mongodb->Set(String::New("MONGO_MINOR"), Integer::New(MONGO_MINOR));
	mongodb->Set(String::New("MONGO_PATCH"), Integer::New(MONGO_PATCH));

	mongodb->Set(String::New("MONGO_OK"), Integer::New(MONGO_OK));
	mongodb->Set(String::New("MONGO_ERROR"), Integer::New(MONGO_ERROR));

	mongodb->Set(String::New("MONGO_DEFAULT_PORT"), Integer::New(MONGO_DEFAULT_PORT));
	
	mongodb->Set(String::New("MONGO_CONN_SUCCESS"), Integer::New(MONGO_CONN_SUCCESS));
	mongodb->Set(String::New("MONGO_CONN_NO_SOCKET"), Integer::New(MONGO_CONN_NO_SOCKET));
	mongodb->Set(String::New("MONGO_CONN_FAIL"), Integer::New(MONGO_CONN_FAIL));
	mongodb->Set(String::New("MONGO_CONN_ADDR_FAIL"), Integer::New(MONGO_CONN_ADDR_FAIL));
	mongodb->Set(String::New("MONGO_CONN_NOT_MASTER"), Integer::New(MONGO_CONN_NOT_MASTER));
	mongodb->Set(String::New("MONGO_CONN_BAD_SET_NAME"), Integer::New(MONGO_CONN_BAD_SET_NAME));
	mongodb->Set(String::New("MONGO_CONN_NO_PRIMARY"), Integer::New(MONGO_CONN_NO_PRIMARY));
	mongodb->Set(String::New("MONGO_IO_ERROR"), Integer::New(MONGO_IO_ERROR));
	mongodb->Set(String::New("MONGO_READ_SIZE_ERROR"), Integer::New(MONGO_READ_SIZE_ERROR));
	mongodb->Set(String::New("MONGO_COMMAND_FAILED"), Integer::New(MONGO_COMMAND_FAILED));
	mongodb->Set(String::New("MONGO_BSON_INVALID"), Integer::New(MONGO_BSON_INVALID));
	mongodb->Set(String::New("MONGO_BSON_NOT_FINISHED"), Integer::New(MONGO_BSON_NOT_FINISHED));

	mongodb->Set(String::New("MONGO_CURSOR_EXHAUSTED"), Integer::New(MONGO_CURSOR_EXHAUSTED));
	mongodb->Set(String::New("MONGO_CURSOR_INVALID"), Integer::New(MONGO_CURSOR_INVALID));
	mongodb->Set(String::New("MONGO_CURSOR_PENDING"), Integer::New(MONGO_CURSOR_PENDING));
	mongodb->Set(String::New("MONGO_CURSOR_QUERY_FAIL"), Integer::New(MONGO_CURSOR_QUERY_FAIL));
	mongodb->Set(String::New("MONGO_CURSOR_BSON_ERROR"), Integer::New(MONGO_CURSOR_BSON_ERROR));
	
	mongodb->Set(String::New("MONGO_CURSOR_MUST_FREE"), Integer::New(MONGO_CURSOR_MUST_FREE));
	mongodb->Set(String::New("MONGO_CURSOR_QUERY_SENT"), Integer::New(MONGO_CURSOR_QUERY_SENT));
	
	mongodb->Set(String::New("MONGO_INDEX_UNIQUE"), Integer::New(MONGO_INDEX_UNIQUE));
	mongodb->Set(String::New("MONGO_INDEX_DROP_DUPS"), Integer::New(MONGO_INDEX_DROP_DUPS));
	mongodb->Set(String::New("MONGO_INDEX_BACKGROUND"), Integer::New(MONGO_INDEX_BACKGROUND));
	mongodb->Set(String::New("MONGO_INDEX_SPARSE"), Integer::New(MONGO_INDEX_SPARSE));
	
	mongodb->Set(String::New("MONGO_UPDATE_UPSERT"), Integer::New(MONGO_UPDATE_UPSERT));
	mongodb->Set(String::New("MONGO_UPDATE_MULTI"), Integer::New(MONGO_UPDATE_MULTI));
	mongodb->Set(String::New("MONGO_UPDATE_BASIC"), Integer::New(MONGO_UPDATE_BASIC));
	
	mongodb->Set(String::New("MONGO_TAILABLE"), Integer::New(MONGO_TAILABLE));
	mongodb->Set(String::New("MONGO_SLAVE_OK"), Integer::New(MONGO_SLAVE_OK));
	mongodb->Set(String::New("MONGO_NO_CURSOR_TIMEOUT"), Integer::New(MONGO_NO_CURSOR_TIMEOUT));
	mongodb->Set(String::New("MONGO_AWAIT_DATA"), Integer::New(MONGO_AWAIT_DATA));
	mongodb->Set(String::New("MONGO_EXHAUST"), Integer::New(MONGO_EXHAUST));
	mongodb->Set(String::New("MONGO_PARTIAL"), Integer::New(MONGO_PARTIAL));
	
	mongodb->Set(String::New("MONGO_OP_MSG"), Integer::New(MONGO_OP_MSG));
	mongodb->Set(String::New("MONGO_OP_UPDATE"), Integer::New(MONGO_OP_UPDATE));
	mongodb->Set(String::New("MONGO_OP_INSERT"), Integer::New(MONGO_OP_INSERT));
	mongodb->Set(String::New("MONGO_OP_QUERY"), Integer::New(MONGO_OP_QUERY));
	mongodb->Set(String::New("MONGO_OP_GET_MORE"), Integer::New(MONGO_OP_GET_MORE));
	mongodb->Set(String::New("MONGO_OP_DELETE"), Integer::New(MONGO_OP_DELETE));
	mongodb->Set(String::New("MONGO_OP_KILL_CURSORS"), Integer::New(MONGO_OP_KILL_CURSORS));



	
    builtinObject->Set(String::New("mongodb"), mongodb);
}

