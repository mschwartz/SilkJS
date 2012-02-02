#include "SilkJS.h"
#include <sqlite3.h>

static JSVAL sqlite_open(JSARGS args) {
	HandleScope scope;
	String::Utf8Value filename(args[0]->ToString());
	sqlite3 *db;
	if (sqlite3_open(*filename, &db)) {
		return scope.Close(String::New(sqlite3_errmsg(db)));
	}
	return scope.Close(External::New(db));
}

static JSVAL sqlite_open16(JSARGS args) {
	HandleScope scope;
	String::Utf8Value filename(args[0]->ToString());
	sqlite3 *db;
	if (sqlite3_open16(*filename, &db)) {
		return scope.Close(String::New(sqlite3_errmsg(db)));
	}
	return scope.Close(External::New(db));
}

static JSVAL sqlite_open_v2(JSARGS args) {
	HandleScope scope;
	String::Utf8Value filename(args[0]->ToString());
	int flags = args[1]->IntegerValue()	;
	String::Utf8Value zVfs(args[2]->ToString());
	sqlite3 *db;
	if (sqlite3_open_v2(*filename, &db, flags, *zVfs)) {
		return scope.Close(String::New(sqlite3_errmsg(db)));
	}
	return scope.Close(External::New(db));
}

static JSVAL sqlite_extended_result_codes(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3 *db = (sqlite3 *)wrap->Value();
	int onoff = args[1]->IntegerValue()	;
	return scope.Close(Integer::New(sqlite3_extended_result_codes(db, onoff)));
}

static JSVAL sqlite_errcode(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3 *db = (sqlite3 *)wrap->Value();
	return scope.Close(Integer::New(sqlite3_errcode(db)));
}

static JSVAL sqlite_extended_errcode(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3 *db = (sqlite3 *)wrap->Value();
	return scope.Close(Integer::New(sqlite3_extended_errcode(db)));
}

static JSVAL sqlite_errmsg(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3 *db = (sqlite3 *)wrap->Value();
	return scope.Close(String::New(sqlite3_errmsg(db)));
}

static JSVAL sqlite_errmsg16(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3 *db = (sqlite3 *)wrap->Value();
	return scope.Close(String::New((char *)sqlite3_errmsg16(db)));
}

static JSVAL sqlite_prepare(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3 *db = (sqlite3 *)wrap->Value();
	String::Utf8Value zSql(args[1]->ToString());
	int nByte = args[2]->IntegerValue()	;
	sqlite3_stmt *stmt;
	if (!sqlite3_prepare(db, *zSql, nByte, &stmt, NULL)) {
		return scope.Close(String::New(sqlite3_errmsg(db)));
	}
	return scope.Close(External::New(stmt));
}

static JSVAL sqlite_prepare_v2(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3 *db = (sqlite3 *)wrap->Value();
	String::Utf8Value zSql(args[1]->ToString());
	int nByte = args[2]->IntegerValue()	;
	sqlite3_stmt *stmt;
	if (!sqlite3_prepare_v2(db, *zSql, nByte, &stmt, NULL)) {
		return scope.Close(String::New(sqlite3_errmsg(db)));
	}
	return scope.Close(External::New(stmt));
}

static JSVAL sqlite_prepare16(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3 *db = (sqlite3 *)wrap->Value();
	String::Utf8Value zSql(args[1]->ToString());
	int nByte = args[2]->IntegerValue()	;
	sqlite3_stmt *stmt;
	if (!sqlite3_prepare16(db, *zSql, nByte, &stmt, NULL)) {
		return scope.Close(String::New(sqlite3_errmsg(db)));
	}
	return scope.Close(External::New(stmt));
}

static JSVAL sqlite_prepare16_v2(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3 *db = (sqlite3 *)wrap->Value();
	String::Utf8Value zSql(args[1]->ToString());
	int nByte = args[2]->IntegerValue()	;
	sqlite3_stmt *stmt;
	if (!sqlite3_prepare16_v2(db, *zSql, nByte, &stmt, NULL)) {
		return scope.Close(String::New(sqlite3_errmsg(db)));
	}
	return scope.Close(External::New(stmt));
}

static JSVAL sqlite_close(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3 *db = (sqlite3 *)wrap->Value();
	return scope.Close(Integer::New(sqlite3_close(db)));
}

static JSVAL sqlite_finalize(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3_stmt *stmt = (sqlite3_stmt *)wrap->Value();
	return scope.Close(Integer::New(sqlite3_finalize(stmt)));
}

static JSVAL sqlite_reset(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3_stmt *stmt = (sqlite3_stmt *)wrap->Value();
	return scope.Close(Integer::New(sqlite3_reset(stmt)));
}

static JSVAL sqlite_step(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3_stmt *stmt = (sqlite3_stmt *)wrap->Value();
	return scope.Close(Integer::New(sqlite3_step(stmt)));
}

static JSVAL sqlite_column_count(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3_stmt *stmt = (sqlite3_stmt *)wrap->Value();
	return scope.Close(Integer::New(sqlite3_column_count(stmt)));
}

static JSVAL sqlite_data_count(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3_stmt *stmt = (sqlite3_stmt *)wrap->Value();
	return scope.Close(Integer::New(sqlite3_data_count(stmt)));
}

static JSVAL sqlite_column_type(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3_stmt *stmt = (sqlite3_stmt *)wrap->Value();
	int iCol = args[1]->IntegerValue();
	return scope.Close(Integer::New(sqlite3_column_type(stmt, iCol)));
}

static JSVAL sqlite_column_bytes(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3_stmt *stmt = (sqlite3_stmt *)wrap->Value();
	int iCol = args[1]->IntegerValue();
	return scope.Close(Integer::New(sqlite3_column_bytes(stmt, iCol)));
}

static JSVAL sqlite_column_bytes16(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3_stmt *stmt = (sqlite3_stmt *)wrap->Value();
	int iCol = args[1]->IntegerValue();
	return scope.Close(Integer::New(sqlite3_column_bytes16(stmt, iCol)));
}

static JSVAL sqlite_column_blob(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3_stmt *stmt = (sqlite3_stmt *)wrap->Value();
	int iCol = args[1]->IntegerValue();
	return scope.Close(String::New((char *)sqlite3_column_blob(stmt, iCol)));
}

// returns a binary blob as base64 encoded string
static JSVAL sqlite_column_blob64(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3_stmt *stmt = (sqlite3_stmt *)wrap->Value();
	int iCol = args[1]->IntegerValue();
	const void *blob = sqlite3_column_blob(stmt, iCol);
	return scope.Close(String::New(Base64Encode((unsigned char const*) blob, sqlite3_column_bytes(stmt, iCol)).c_str()));
}

static JSVAL sqlite_column_double(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3_stmt *stmt = (sqlite3_stmt *)wrap->Value();
	int iCol = args[1]->IntegerValue();
	return scope.Close(Number::New(sqlite3_column_double(stmt, iCol)));
}

static JSVAL sqlite_column_int(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3_stmt *stmt = (sqlite3_stmt *)wrap->Value();
	int iCol = args[1]->IntegerValue();
	return scope.Close(Integer::New(sqlite3_column_int(stmt, iCol)));
}

static JSVAL sqlite_column_int64(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3_stmt *stmt = (sqlite3_stmt *)wrap->Value();
	int iCol = args[1]->IntegerValue();
	return scope.Close(Integer::New(sqlite3_column_int64(stmt, iCol)));
}

static JSVAL sqlite_column_text(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3_stmt *stmt = (sqlite3_stmt *)wrap->Value();
	int iCol = args[1]->IntegerValue();
	return scope.Close(String::New((const char *)sqlite3_column_text(stmt, iCol)));
}

static JSVAL sqlite_column_text16(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3_stmt *stmt = (sqlite3_stmt *)wrap->Value();
	int iCol = args[1]->IntegerValue();
	return scope.Close(String::New((char *)sqlite3_column_text16(stmt, iCol)));
}

static JSVAL sqlite_column_value(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	sqlite3_stmt *stmt = (sqlite3_stmt *)wrap->Value();
	int iCol = args[1]->IntegerValue();
	return scope.Close(External::New(sqlite3_column_value(stmt, iCol)));
}

void init_sqlite3_object() {
	HandleScope scope;

	JSOBJT o = ObjectTemplate::New();
	
	// open flags
	o->Set(String::New("OPEN_READONLY"), Integer::New(SQLITE_OPEN_READONLY));
	o->Set(String::New("OPEN_READWRITENLY"), Integer::New(SQLITE_OPEN_READWRITE));
	o->Set(String::New("OPEN_CREATE"), Integer::New(SQLITE_OPEN_CREATE));
	o->Set(String::New("SQLITE_OPEN_DELETEONCLOSE"), Integer::New(SQLITE_OPEN_DELETEONCLOSE));
	o->Set(String::New("SQLITE_OPEN_EXCLUSIVE"), Integer::New(SQLITE_OPEN_EXCLUSIVE));
#ifdef SQLITE_OPEN_AUTOPROXY
	o->Set(String::New("SQLITE_OPEN_AUTOPROXY"), Integer::New(SQLITE_OPEN_AUTOPROXY));
#endif
#ifdef SQLITE_OPEN_URI
	o->Set(String::New("SQLITE_OPEN_URI"), Integer::New(SQLITE_OPEN_URI));
#endif
	o->Set(String::New("SQLITE_OPEN_MAIN_DB"), Integer::New(SQLITE_OPEN_MAIN_DB));
	o->Set(String::New("SQLITE_OPEN_TEMP_DB"), Integer::New(SQLITE_OPEN_TEMP_DB));
	o->Set(String::New("SQLITE_OPEN_TRANSIENT_DB"), Integer::New(SQLITE_OPEN_TRANSIENT_DB));
	o->Set(String::New("SQLITE_OPEN_MAIN_JOURNAL"), Integer::New(SQLITE_OPEN_MAIN_JOURNAL));
	o->Set(String::New("SQLITE_OPEN_TEMP_JOURNAL"), Integer::New(SQLITE_OPEN_TEMP_JOURNAL));
	o->Set(String::New("SQLITE_OPEN_SUBJOURNAL"), Integer::New(SQLITE_OPEN_SUBJOURNAL));
	o->Set(String::New("SQLITE_OPEN_MASTER_JOURNAL"), Integer::New(SQLITE_OPEN_MASTER_JOURNAL));
	o->Set(String::New("OPEN_NOMUTEX"), Integer::New(SQLITE_OPEN_NOMUTEX));
	o->Set(String::New("OPEN_FULLMUTEX"), Integer::New(SQLITE_OPEN_FULLMUTEX));
	o->Set(String::New("OPEN_SHAREDCACHE"), Integer::New(SQLITE_OPEN_SHAREDCACHE));
	o->Set(String::New("OPEN_PRIVATECACHE"), Integer::New(SQLITE_OPEN_PRIVATECACHE));
#ifdef SQLITE_OPEN_WAL
	o->Set(String::New("OPEN_WAL"), Integer::New(SQLITE_OPEN_WAL));
#endif
	
	// Result Codes
	o->Set(String::New("OK"), Integer::New(SQLITE_OK));
	o->Set(String::New("ERROR"), Integer::New(SQLITE_ERROR));
	o->Set(String::New("IGNORE"), Integer::New(SQLITE_ERROR));
	
	o->Set(String::New("INTERNAL"), Integer::New(SQLITE_INTERNAL));
	o->Set(String::New("PERM"), Integer::New(SQLITE_PERM));
	o->Set(String::New("ABORT"), Integer::New(SQLITE_ABORT));
	o->Set(String::New("BUSY"), Integer::New(SQLITE_BUSY));
	o->Set(String::New("LOCKED"), Integer::New(SQLITE_LOCKED));
	o->Set(String::New("NOMEM"), Integer::New(SQLITE_NOMEM));
	o->Set(String::New("READONLY"), Integer::New(SQLITE_READONLY));
	o->Set(String::New("INTERRUPT"), Integer::New(SQLITE_INTERRUPT));
	o->Set(String::New("IOERR"), Integer::New(SQLITE_IOERR));
	o->Set(String::New("CORRUPT"), Integer::New(SQLITE_CORRUPT));
	o->Set(String::New("NOTFOUND"), Integer::New(SQLITE_NOTFOUND));
	o->Set(String::New("FULL"), Integer::New(SQLITE_FULL));
	o->Set(String::New("CANTOPEN"), Integer::New(SQLITE_CANTOPEN));
	o->Set(String::New("PROTOCOL"), Integer::New(SQLITE_PROTOCOL));
	o->Set(String::New("EMPTY"), Integer::New(SQLITE_EMPTY));
	o->Set(String::New("SCHEMA"), Integer::New(SQLITE_SCHEMA));
	o->Set(String::New("TOOBIG"), Integer::New(SQLITE_TOOBIG));
	o->Set(String::New("CONSTRAINT"), Integer::New(SQLITE_CONSTRAINT));
	o->Set(String::New("MISMATCH"), Integer::New(SQLITE_MISMATCH));
	o->Set(String::New("MISUSE"), Integer::New(SQLITE_MISUSE));
	o->Set(String::New("NOLFS"), Integer::New(SQLITE_NOLFS));
	o->Set(String::New("AUTH"), Integer::New(SQLITE_AUTH));
	o->Set(String::New("FORMAT"), Integer::New(SQLITE_FORMAT));
	o->Set(String::New("RANGE"), Integer::New(SQLITE_RANGE));
	o->Set(String::New("NOTADB"), Integer::New(SQLITE_NOTADB));
	o->Set(String::New("ROW"), Integer::New(SQLITE_ROW));
	o->Set(String::New("DONE"), Integer::New(SQLITE_DONE));
	
	// Extended Result Codes
	o->Set(String::New("IOERR_READ"), Integer::New(SQLITE_IOERR_READ));
	o->Set(String::New("IOERR_SHORT_READ"), Integer::New(SQLITE_IOERR_SHORT_READ));
	o->Set(String::New("IOERR_WRITE"), Integer::New(SQLITE_IOERR_WRITE));
	o->Set(String::New("IOERR_FSYNC"), Integer::New(SQLITE_IOERR_FSYNC));
	o->Set(String::New("IOERR_DIR_FSYNC"), Integer::New(SQLITE_IOERR_DIR_FSYNC));
	o->Set(String::New("IOERR_TRUNCATE"), Integer::New(SQLITE_IOERR_TRUNCATE));
	o->Set(String::New("IOERR_FSTAT"), Integer::New(SQLITE_IOERR_FSTAT));
	o->Set(String::New("IOERR_UNLOCK"), Integer::New(SQLITE_IOERR_UNLOCK));
	o->Set(String::New("IOERR_RDLOCK"), Integer::New(SQLITE_IOERR_RDLOCK));
	o->Set(String::New("IOERR_DELETE"), Integer::New(SQLITE_IOERR_DELETE));
	o->Set(String::New("IOERR_BLOCKED"), Integer::New(SQLITE_IOERR_BLOCKED));
	o->Set(String::New("IOERR_NOMEM"), Integer::New(SQLITE_IOERR_NOMEM));
	o->Set(String::New("IOERR_ACCESS"), Integer::New(SQLITE_IOERR_ACCESS));
	o->Set(String::New("IOERR_CHECKRESERVEDLOCK"), Integer::New(SQLITE_IOERR_CHECKRESERVEDLOCK));
	o->Set(String::New("IOERR_LOCK"), Integer::New(SQLITE_IOERR_LOCK));
	o->Set(String::New("IOERR_CLOSE"), Integer::New(SQLITE_IOERR_CLOSE));
	o->Set(String::New("IOERR_DIR_CLOSE"), Integer::New(SQLITE_IOERR_DIR_CLOSE));
#ifdef SQLITE_IOERR_SHMOPEN
	o->Set(String::New("IOERR_SHMOPEN"), Integer::New(SQLITE_IOERR_SHMOPEN));
#endif
#ifdef SQLITE_IOERR_SHMSIZE
	o->Set(String::New("IOERR_SHMSIZE"), Integer::New(SQLITE_IOERR_SHMSIZE));
#endif
#ifdef SQLITE_IOERR_SHMLOCK
	o->Set(String::New("IOERR_SHMLOCK"), Integer::New(SQLITE_IOERR_SHMLOCK));
#endif
#ifdef IOERR_SHMMAP
	o->Set(String::New("IOERR_SHMMAP"), Integer::New(SQLITE_IOERR_SHMMAP));
#endif
#ifdef IOERR_SEEK
	o->Set(String::New("IOERR_SEEK"), Integer::New(SQLITE_IOERR_SEEK));
#endif
	o->Set(String::New("LOCKED_SHAREDCACHE"), Integer::New(SQLITE_LOCKED_SHAREDCACHE));
#ifdef SQLITE_BUSY_RECOVERY
	o->Set(String::New("BUSY_RECOVERY"), Integer::New(SQLITE_BUSY_RECOVERY));
#endif
#ifdef SQLITE_CANTOPEN_NOTEMPDIR
	o->Set(String::New("CANTOPEN_NOTEMPDIR"), Integer::New(SQLITE_CANTOPEN_NOTEMPDIR));
#endif
#ifdef SQLITE_CORRUPT_VTAB
	o->Set(String::New("CORRUPT_VTAB"), Integer::New(SQLITE_CORRUPT_VTAB));
#endif
#ifdef SQLITE_READONLY_RECOVERY
	o->Set(String::New("READONLY_RECOVERY"), Integer::New(SQLITE_READONLY_RECOVERY));
#endif
#ifdef READONLY_CANTLOCK
	o->Set(String::New("READONLY_CANTLOCK"), Integer::New(SQLITE_READONLY_CANTLOCK));
#endif

	// fundamental data types
	o->Set(String::New("INTEGER"), Integer::New(SQLITE_INTEGER));
	o->Set(String::New("FLOAT"), Integer::New(SQLITE_FLOAT));
	o->Set(String::New("BLOB"), Integer::New(SQLITE_BLOB));
	o->Set(String::New("NULL"), Integer::New(SQLITE_NULL));
	o->Set(String::New("TEXT"), Integer::New(SQLITE_TEXT));

	// functions
	o->Set(String::New("open"), FunctionTemplate::New(sqlite_open));
	o->Set(String::New("open16"), FunctionTemplate::New(sqlite_open16));
	o->Set(String::New("open_v2"), FunctionTemplate::New(sqlite_open_v2));
	o->Set(String::New("extended_result_codes"), FunctionTemplate::New(sqlite_extended_result_codes));
	o->Set(String::New("errcode"), FunctionTemplate::New(sqlite_errcode));
	o->Set(String::New("extended_errcode"), FunctionTemplate::New(sqlite_extended_errcode));
	o->Set(String::New("errmsg"), FunctionTemplate::New(sqlite_errmsg));
	o->Set(String::New("errmsg16"), FunctionTemplate::New(sqlite_errmsg16));
	o->Set(String::New("prepare"), FunctionTemplate::New(sqlite_prepare));
	o->Set(String::New("prepare_v2"), FunctionTemplate::New(sqlite_prepare_v2));
	o->Set(String::New("prepare16"), FunctionTemplate::New(sqlite_prepare16));
	o->Set(String::New("prepare16_v2"), FunctionTemplate::New(sqlite_prepare16_v2));
	o->Set(String::New("close"), FunctionTemplate::New(sqlite_close));
	o->Set(String::New("finalize"), FunctionTemplate::New(sqlite_finalize));
	o->Set(String::New("reset"), FunctionTemplate::New(sqlite_reset));
	o->Set(String::New("step"), FunctionTemplate::New(sqlite_step));
	o->Set(String::New("column_count"), FunctionTemplate::New(sqlite_column_count));
	o->Set(String::New("data_count"), FunctionTemplate::New(sqlite_data_count));
	o->Set(String::New("column_type"), FunctionTemplate::New(sqlite_column_type));
	o->Set(String::New("column_bytes"), FunctionTemplate::New(sqlite_column_bytes));
	o->Set(String::New("column_bytes16"), FunctionTemplate::New(sqlite_column_bytes16));
	o->Set(String::New("column_blob"), FunctionTemplate::New(sqlite_column_blob));
	o->Set(String::New("column_blob64"), FunctionTemplate::New(sqlite_column_blob64));
	o->Set(String::New("column_double"), FunctionTemplate::New(sqlite_column_double));
	o->Set(String::New("column_int"), FunctionTemplate::New(sqlite_column_int));
	o->Set(String::New("column_int64"), FunctionTemplate::New(sqlite_column_int64));
	o->Set(String::New("column_text"), FunctionTemplate::New(sqlite_column_text));
	o->Set(String::New("column_text16"), FunctionTemplate::New(sqlite_column_text16));
	o->Set(String::New("column_value"), FunctionTemplate::New(sqlite_column_value));
	
	builtinObject->Set(String::New("sqlite3"), o);
}
