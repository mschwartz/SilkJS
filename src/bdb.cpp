/** @ignore */
/*
 * Berkley Database (BDB) Interface
 */

////////////////  IN PROGRESS - NOT READY FOR USE /////////////////////////


#include "SilkJS.h"
#include <db.h>



void init_bdb_object() {
    HandleScope scope;

    Handle<ObjectTemplate>bdb = ObjectTemplate::New();

	bdb->Set(String::New("VERSION_MAJOR"), Integer::New(DB_VERSION_MAJOR));
	bdb->Set(String::New("VERSION_MINOR"), Integer::New(DB_VERSION_MINOR));
	bdb->Set(String::New("VERSION_PATCH"), Integer::New(DB_VERSION_PATCH));
	bdb->Set(String::New("VERSION_STRING"), String::New(DB_VERSION_STRING));
	
	bdb->Set(String::New("DB_AUTO_COMMIT"), Integer::New(DB_AUTO_COMMIT));
	bdb->Set(String::New("DB_CREATE"), Integer::New(DB_EXCL));
	bdb->Set(String::New("DB_MULTIVERSION"), Integer::New(DB_MULTIVERSION));
	bdb->Set(String::New("DB_NOMMAP"), Integer::New(DB_NOMMAP));
	bdb->Set(String::New("DB_RDONLY"), Integer::New(DB_RDONLY));
	bdb->Set(String::New("DB_READ_UNCOMMITTED"), Integer::New(DB_READ_UNCOMMITTED));
	bdb->Set(String::New("DB_THREAD"), Integer::New(DB_THREAD));
	bdb->Set(String::New("DB_TRUNCATE"), Integer::New(DB_TRUNCATE));
	
	
	builtinObject->Set(String::New("bdb"), bdb);
}
