#include "SilkJS.h"

#include <mongo/client/dbclient.h>
#include <mongo/bson/bson.h>
#include <mongo/scripting/v8_wrapper.h>
#include <mongo/scripting/v8_db.h>
#include <mongo/scripting/v8_utils.h>

#include <iostream>
#include <map>
#include <sstream>

using namespace std;
using namespace mongo;

static BSONObj v8ToMongo( v8::Handle<v8::Object> o , int depth );

#if false
static Handle<Value> NamedReadOnlySet( Local<v8::String> property, Local<Value> value, const AccessorInfo& info ) {
	cout << "cannot write to read-only object" << endl;
	return value;
}

static Handle<Boolean> NamedReadOnlyDelete( Local<v8::String> property, const AccessorInfo& info ) {
	cout << "cannot delete from read-only object" << endl;
	return Boolean::New( false );
}

static Handle<Value> IndexedReadOnlySet( uint32_t index, Local<Value> value, const AccessorInfo& info ) {
	cout << "cannot write to read-only array" << endl;
	return value;
}

static Handle<Boolean> IndexedReadOnlyDelete( uint32_t index, const AccessorInfo& info ) {
	cout << "cannot delete from read-only array" << endl;
	return Boolean::New( false );
}


//static v8::Function * getNamedCons( const char * name ) {
//	return v8::Function::Cast( *(v8::Context::GetCurrent()->Global()->Get( v8::String::New( name ) ) ) );
//}

//static v8::Function * getObjectIdCons() {
//	return getNamedCons( "ObjectId" );
//}

static Local< v8::Value > newFunction( const char *code ) {
	stringstream codeSS;
	codeSS << "____MontoToV8_newFunction_temp = " << code;
	string codeStr = codeSS.str();
	Local< Script > compiled = Script::New( v8::String::New( codeStr.c_str() ) );
	Local< Value > ret = compiled->Run();
	return ret;
}

static Local< v8::Value > newId( const OID &id ) {
	v8::Function * idCons = getObjectIdCons();
	v8::Handle<v8::Value> argv[1];
	argv[0] = v8::String::New( id.str().c_str() );
	return idCons->NewInstance( 1 , argv );
}

static Local<v8::Object> mongoToV8( const BSONObj& m , bool array, bool readOnly ) {

	Local<v8::Object> o;

	// handle DBRef. needs to come first. isn't it? (metagoto)
	static string ref = "$ref";
	if ( ref == m.firstElement().fieldName() ) {
		const BSONElement& id = m["$id"];
		if (!id.eoo()) { // there's no check on $id exitence in sm implementation. risky ?
			v8::Function* dbRef = getNamedCons( "DBRef" );
			o = dbRef->NewInstance();
		}
	}

	Local< v8::ObjectTemplate > readOnlyObjects;
	// Hoping template construction is fast...
	Local< v8::ObjectTemplate > internalFieldObjects = v8::ObjectTemplate::New();
	internalFieldObjects->SetInternalFieldCount( 1 );

	if ( !o.IsEmpty() ) {
		readOnly = false;
	}
	else if ( array ) {
		// NOTE Looks like it's impossible to add interceptors to v8 arrays.
		readOnly = false;
		o = v8::Array::New();
	}
	else if ( !readOnly ) {
		o = v8::Object::New();
	}
	else {
		// NOTE Our readOnly implemention relies on undocumented ObjectTemplate
		// functionality that may be fragile, but it still seems like the best option
		// for now -- fwiw, the v8 docs are pretty sparse.  I've determined experimentally
		// that when property handlers are set for an object template, they will attach
		// to objects previously created by that template.  To get this to work, though,
		// it is necessary to initialize the template's property handlers before
		// creating objects from the template (as I have in the following few lines
		// of code).
		// NOTE In my first attempt, I configured the permanent property handlers before
		// constructiong the object and replaced the Set() calls below with ForceSet().
		// However, it turns out that ForceSet() only bypasses handlers for named
		// properties and not for indexed properties.
		readOnlyObjects = v8::ObjectTemplate::New();
		// NOTE This internal field will store type info for special db types.  For
		// regular objects the field is unnecessary - for simplicity I'm creating just
		// one readOnlyObjects template for objects where the field is & isn't necessary,
		// assuming that the overhead of an internal field is slight.
		readOnlyObjects->SetInternalFieldCount( 1 );
		readOnlyObjects->SetNamedPropertyHandler( 0 );
		readOnlyObjects->SetIndexedPropertyHandler( 0 );
		o = readOnlyObjects->NewInstance();
	}

	mongo::BSONObj sub;

	for ( BSONObjIterator i(m); i.more(); ) {
		const BSONElement& f = i.next();

		Local<Value> v;

		switch ( f.type() ) {

		case mongo::Code:
			o->Set( v8::String::New( f.fieldName() ), newFunction( f.valuestr() ) );
			break;

		case CodeWScope:
			if ( f.codeWScopeObject().isEmpty() )
				log() << "warning: CodeWScope doesn't transfer to db.eval" << endl;
			o->Set( v8::String::New( f.fieldName() ), newFunction( f.codeWScopeCode() ) );
			break;

		case mongo::String:
			o->Set( v8::String::New( f.fieldName() ) , v8::String::New( f.valuestr() ) );
			break;

		case mongo::jstOID: {
			v8::Function * idCons = getObjectIdCons();
			v8::Handle<v8::Value> argv[1];
			argv[0] = v8::String::New( f.__oid().str().c_str() );
			o->Set( v8::String::New( f.fieldName() ) ,
					idCons->NewInstance( 1 , argv ) );
			break;
		}

		case mongo::NumberDouble:
		case mongo::NumberInt:
			o->Set( v8::String::New( f.fieldName() ) , v8::Number::New( f.number() ) );
			break;

		case mongo::Array:
		case mongo::Object:
			sub = f.embeddedObject();
			o->Set( v8::String::New( f.fieldName() ) , mongoToV8( sub , f.type() == mongo::Array, readOnly ) );
			break;

		case mongo::Date:
			o->Set( v8::String::New( f.fieldName() ) , v8::Date::New( f.date() ) );
			break;

		case mongo::Bool:
			o->Set( v8::String::New( f.fieldName() ) , v8::Boolean::New( f.boolean() ) );
			break;

		case mongo::jstNULL:
		case mongo::Undefined: // duplicate sm behavior
			o->Set( v8::String::New( f.fieldName() ) , v8::Null() );
			break;

		case mongo::RegEx: {
			v8::Function * regex = getNamedCons( "RegExp" );

			v8::Handle<v8::Value> argv[2];
			argv[0] = v8::String::New( f.regex() );
			argv[1] = v8::String::New( f.regexFlags() );

			o->Set( v8::String::New( f.fieldName() ) , regex->NewInstance( 2 , argv ) );
			break;
		}

		case mongo::BinData: {
			Local<v8::Object> b = readOnly ? readOnlyObjects->NewInstance() : internalFieldObjects->NewInstance();

			int len;
			const char *data = f.binData( len );

			v8::Function* binData = getNamedCons( "BinData" );
			v8::Handle<v8::Value> argv[3];
			argv[0] = v8::Number::New( len );
			argv[1] = v8::Number::New( f.binDataType() );
			argv[2] = v8::String::New( data, len );
			o->Set( v8::String::New( f.fieldName() ), binData->NewInstance(3, argv) );
			break;
		}

		case mongo::Timestamp: {
			Local<v8::Object> sub = readOnly ? readOnlyObjects->NewInstance() : internalFieldObjects->NewInstance();

			sub->Set( v8::String::New( "t" ) , v8::Number::New( f.timestampTime() ) );
			sub->Set( v8::String::New( "i" ) , v8::Number::New( f.timestampInc() ) );
			sub->SetInternalField( 0, v8::Uint32::New( f.type() ) );

			o->Set( v8::String::New( f.fieldName() ) , sub );
			break;
		}

		case mongo::NumberLong: {
			Local<v8::Object> sub = readOnly ? readOnlyObjects->NewInstance() : internalFieldObjects->NewInstance();
			unsigned long long val = f.numberLong();
			v8::Function* numberLong = getNamedCons( "NumberLong" );
			if ( (long long)val == (long long)(double)(long long)(val) ) {
				v8::Handle<v8::Value> argv[1];
				argv[0] = v8::Number::New( (double)(long long)( val ) );
				o->Set( v8::String::New( f.fieldName() ), numberLong->NewInstance( 1, argv ) );
			}
			else {
				v8::Handle<v8::Value> argv[3];
				argv[0] = v8::Number::New( (double)(long long)(val) );
				argv[1] = v8::Integer::New( val >> 32 );
				argv[2] = v8::Integer::New( (unsigned long)(val & 0x00000000ffffffff) );
				o->Set( v8::String::New( f.fieldName() ), numberLong->NewInstance(3, argv) );
			}
			break;
		}

		case mongo::MinKey: {
			Local<v8::Object> sub = readOnly ? readOnlyObjects->NewInstance() : internalFieldObjects->NewInstance();
			sub->Set( v8::String::New( "$MinKey" ), v8::Boolean::New( true ) );
			sub->SetInternalField( 0, v8::Uint32::New( f.type() ) );
			o->Set( v8::String::New( f.fieldName() ) , sub );
			break;
		}

		case mongo::MaxKey: {
			Local<v8::Object> sub = readOnly ? readOnlyObjects->NewInstance() : internalFieldObjects->NewInstance();
			sub->Set( v8::String::New( "$MaxKey" ), v8::Boolean::New( true ) );
			sub->SetInternalField( 0, v8::Uint32::New( f.type() ) );
			o->Set( v8::String::New( f.fieldName() ) , sub );
			break;
		}

		case mongo::DBRef: {
			v8::Function* dbPointer = getNamedCons( "DBPointer" );
			v8::Handle<v8::Value> argv[2];
			argv[0] = v8::String::New( f.dbrefNS() );
			argv[1] = newId( f.dbrefOID() );
			o->Set( v8::String::New( f.fieldName() ), dbPointer->NewInstance(2, argv) );
			break;
		}

		default:
			cout << "can't handle type: ";
			cout  << f.type() << " ";
			cout  << f.toString();
			cout  << endl;
			break;
		}

	}

	if ( readOnly ) {
		readOnlyObjects->SetNamedPropertyHandler( 0, NamedReadOnlySet, 0, NamedReadOnlyDelete );
		readOnlyObjects->SetIndexedPropertyHandler( 0, IndexedReadOnlySet, 0, IndexedReadOnlyDelete );
	}

	return o;
}


static std::string toSTLString( const Handle<v8::Value> & o ) {
	v8::String::Utf8Value str(o);
	const char * foo = *str;
	std::string s(foo);
	return s;
}

static std::string toSTLString( const v8::TryCatch * try_catch ) {

	stringstream ss;

	//while ( try_catch ){ // disabled for v8 bleeding edge

	v8::String::Utf8Value exception(try_catch->Exception());
	Handle<v8::Message> message = try_catch->Message();

	if (message.IsEmpty()) {
		ss << *exception << endl;
	}
	else {

		v8::String::Utf8Value filename(message->GetScriptResourceName());
		int linenum = message->GetLineNumber();
		ss << *filename << ":" << linenum << " " << *exception << endl;

		v8::String::Utf8Value sourceline(message->GetSourceLine());
		ss << *sourceline << endl;

		int start = message->GetStartColumn();
		for (int i = 0; i < start; i++)
			ss << " ";

		int end = message->GetEndColumn();
		for (int i = start; i < end; i++)
			ss << "^";

		ss << endl;
	}

	//try_catch = try_catch->next_;
	//}

	return ss.str();
}

static void v8ToMongoElement( BSONObjBuilder & b , v8::Handle<v8::String> name , const string sname , v8::Handle<v8::Value> value , int depth =0 ) {

	if ( value->IsString() ) {
		b.append( sname , toSTLString( value ).c_str() );
		return;
	}

	if ( value->IsFunction() ) {
		b.appendCode( sname , toSTLString( value ) );
		return;
	}

	if ( value->IsNumber() ) {
		if ( value->IsInt32() )
			b.append( sname, int( value->ToInt32()->Value() ) );
		else
			b.append( sname , value->ToNumber()->Value() );
		return;
	}

	if ( value->IsArray() ) {
		BSONObj sub = v8ToMongo( value->ToObject() , depth );
		b.appendArray( sname , sub );
		return;
	}

	if ( value->IsDate() ) {
		b.appendDate( sname , Date_t( (unsigned long long)(v8::Date::Cast( *value )->NumberValue())) );
		return;
	}

	if ( value->IsExternal() )
		return;

	if ( value->IsObject() ) {
		// The user could potentially modify the fields of these special objects,
		// wreaking havoc when we attempt to reinterpret them.  Not doing any validation
		// for now...
		Local< v8::Object > obj = value->ToObject();
		if ( obj->InternalFieldCount() && obj->GetInternalField( 0 )->IsNumber() ) {
			switch( obj->GetInternalField( 0 )->ToInt32()->Value() ) { // NOTE Uint32's Value() gave me a linking error, so going with this instead
			case Timestamp:
				b.appendTimestamp( sname,
								   Date_t( (unsigned long long)(obj->Get( v8::String::New( "t" ) )->ToNumber()->Value() )),
								   obj->Get( v8::String::New( "i" ) )->ToInt32()->Value() );
				return;
			case MinKey:
				b.appendMinKey( sname );
				return;
			case MaxKey:
				b.appendMaxKey( sname );
				return;
			default:
				assert( "invalid internal field" == 0 );
			}
		}
		string s = toSTLString( value );
		if ( s.size() && s[0] == '/' ) {
			s = s.substr( 1 );
			string r = s.substr( 0 , s.rfind( "/" ) );
			string o = s.substr( s.rfind( "/" ) + 1 );
			b.appendRegex( sname , r , o );
		}
		else if ( value->ToObject()->GetPrototype()->IsObject() &&
				  value->ToObject()->GetPrototype()->ToObject()->HasRealNamedProperty( v8::String::New( "isObjectId" ) ) ) {
			OID oid;
			oid.init( toSTLString( value ) );
			b.appendOID( sname , &oid );
		}
		else if ( !value->ToObject()->GetHiddenValue( v8::String::New( "__NumberLong" ) ).IsEmpty() ) {
			// TODO might be nice to potentially speed this up with an indexed internal
			// field, but I don't yet know how to use an ObjectTemplate with a
			// constructor.
			v8::Handle< v8::Object > it = value->ToObject();
			long long val;
			if ( !it->Has( v8::String::New( "top" ) ) ) {
				val = (long long)( it->Get( v8::String::New( "floatApprox" ) )->NumberValue() );
			}
			else {
				val = (long long)
					  ( (unsigned long long)( it->Get( v8::String::New( "top" ) )->ToInt32()->Value() ) << 32 ) +
					  (unsigned)( it->Get( v8::String::New( "bottom" ) )->ToInt32()->Value() );
			}

			b.append( sname, val );
		}
		else if ( !value->ToObject()->GetHiddenValue( v8::String::New( "__DBPointer" ) ).IsEmpty() ) {
			OID oid;
			oid.init( toSTLString( value->ToObject()->Get( v8::String::New( "id" ) ) ) );
			string ns = toSTLString( value->ToObject()->Get( v8::String::New( "ns" ) ) );
			b.appendDBRef( sname, ns, oid );
		}
		else if ( !value->ToObject()->GetHiddenValue( v8::String::New( "__BinData" ) ).IsEmpty() ) {
			int len = obj->Get( v8::String::New( "len" ) )->ToInt32()->Value();
			v8::String::Utf8Value data( obj->Get( v8::String::New( "data" ) ) );
			const char *dataArray = *data;
			assert( data.length() == len );
			b.appendBinData( sname,
							 len,
							 mongo::BinDataType( obj->Get( v8::String::New( "type" ) )->ToInt32()->Value() ),
							 dataArray );
		}
		else {
			BSONObj sub = v8ToMongo( value->ToObject() , depth );
			b.append( sname , sub );
		}
		return;
	}

	if ( value->IsBoolean() ) {
		b.appendBool( sname , value->ToBoolean()->Value() );
		return;
	}

	else if ( value->IsUndefined() ) {
		b.appendUndefined( sname );
		return;
	}

	else if ( value->IsNull() ) {
		b.appendNull( sname );
		return;
	}

	cout << "don't know how to convert to mongo field [" << sname; //  << "]\t" << value << endl;
}


static BSONObj v8ToMongo( v8::Handle<v8::Object> o , int depth ) {
	BSONObjBuilder b;

	if ( depth == 0 ) {
		v8::Handle<v8::String> idName = v8::String::New( "_id" );
		if ( o->HasRealNamedProperty( idName ) ) {
			v8ToMongoElement( b , idName , "_id" , o->Get( idName ) );
		}
	}

	Local<v8::Array> names = o->GetPropertyNames();
	for ( unsigned int i=0; i<names->Length(); i++ ) {
		v8::Local<v8::String> name = names->Get(v8::Integer::New(i) )->ToString();

		if ( o->GetPrototype()->IsObject() &&
				o->GetPrototype()->ToObject()->HasRealNamedProperty( name ) )
			continue;

		v8::Local<v8::Value> value = o->Get( name );

		const string sname = toSTLString( name );
		if ( depth == 0 && sname == "_id" )
			continue;

		v8ToMongoElement( b , name , sname , value , depth + 1 );
	}
	return b.obj();
}
#endif

namespace mongo {
	void disableV8Interrupt()  {

	}

	void enableV8Interrupt() {
	}
	
	const char * (*ScriptEngine::_checkInterruptCallback)() = 0;

}

static DBClientBase * getConnection( const Arguments& args ) {
	Local<External> c = External::Cast( *(args.This()->GetInternalField( 0 )) );
	DBClientBase * conn = (DBClientBase*)(c->Value());
	assert( conn );
	return conn;
}

static void destroyConnection( Persistent<Value> self, void* parameter) {
	delete static_cast<DBClientBase*>(parameter);
	self.Dispose();
	self.Clear();
}


// ---- real methods

static void destroyCursor( Persistent<Value> self, void* parameter) {
	delete static_cast<mongo::DBClientCursor*>(parameter);
	self.Dispose();
	self.Clear();
}

static Handle<Value> mongoConsExternal(const Arguments& args) {

	char host[255];

	if ( args.Length() > 0 && args[0]->IsString() ) {
		assert( args[0]->ToString()->Utf8Length() < 250 );
		args[0]->ToString()->WriteAscii( host );
	}
	else {
		strcpy( host , "127.0.0.1" );
	}

	string errmsg;
	ConnectionString cs = ConnectionString::parse( host , errmsg );
	if ( ! cs.isValid() )
		return v8::ThrowException( v8::String::New( errmsg.c_str() ) );


	DBClientWithCommands * conn;
	{
		V8Unlock ul;
		conn = cs.connect( errmsg );
	}
	if ( ! conn )
		return v8::ThrowException( v8::String::New( errmsg.c_str() ) );

	Persistent<v8::Object> self = Persistent<v8::Object>::New( args.Holder() );
	self.MakeWeak( conn , destroyConnection );

	{
		V8Unlock ul;
		ScriptEngine::runConnectCallback( *conn );
	}

	args.This()->SetInternalField( 0 , External::New( conn ) );
	args.This()->Set( v8::String::New( "slaveOk" ) , Boolean::New( false ) );
	args.This()->Set( v8::String::New( "host" ) , v8::String::New( host ) );

	return v8::Undefined();
}

static Handle<Value> mongoConsLocal(const Arguments& args) {

	if ( args.Length() > 0 )
		return v8::ThrowException( v8::String::New( "local Mongo constructor takes no args" ) );

	DBClientBase * conn;
	{
		V8Unlock ul;
		conn = createDirectClient();
	}

	Persistent<v8::Object> self = Persistent<v8::Object>::New( args.This() );
	self.MakeWeak( conn , destroyConnection );

	// NOTE I don't believe the conn object will ever be freed.
	args.This()->SetInternalField( 0 , External::New( conn ) );
	args.This()->Set( v8::String::New( "slaveOk" ) , Boolean::New( false ) );
	args.This()->Set( v8::String::New( "host" ) , v8::String::New( "EMBEDDED" ) );

	return v8::Undefined();
}


template < v8::Handle< v8::Value > ( *f ) ( const v8::Arguments& ) >
v8::Local< v8::FunctionTemplate > _newV8Function() {
	return v8::FunctionTemplate::New( v8Callback< f > );
}


static v8::Handle<v8::FunctionTemplate> _getMongoFunctionTemplate( bool local ) {
	v8::Local<v8::FunctionTemplate> mongo;
	if ( local ) {
		mongo = _newV8Function< mongoConsLocal >();
	}
	else {
		mongo = _newV8Function< mongoConsExternal >();
	}
	mongo->InstanceTemplate()->SetInternalFieldCount( 1 );

	v8::Local<v8::Template> proto = mongo->PrototypeTemplate();

	proto->Set( v8::String::New( "find" ) , newV8Function< mongoFind >() );
	proto->Set( v8::String::New( "insert" ) , newV8Function< mongoInsert >() );
	proto->Set( v8::String::New( "remove" ) , newV8Function< mongoRemove >() );
	proto->Set( v8::String::New( "update" ) , newV8Function< mongoUpdate >() );

	Local<FunctionTemplate> ic = newV8Function< internalCursorCons >();
	ic->InstanceTemplate()->SetInternalFieldCount( 1 );
	ic->PrototypeTemplate()->Set( v8::String::New("next") , newV8Function< internalCursorNext >() );
	ic->PrototypeTemplate()->Set( v8::String::New("hasNext") , newV8Function< internalCursorHasNext >() );
	ic->PrototypeTemplate()->Set( v8::String::New("objsLeftInBatch") , newV8Function< internalCursorObjsLeftInBatch >() );
	proto->Set( v8::String::New( "internalCursor" ) , ic );



	return mongo;
}


void init_mongodb_object() {
    HandleScope scope;
	Handle<v8::FunctionTemplate> t = _getMongoFunctionTemplate(false);
}


#if false

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
// see http://api.mongodb.org/c/0.4/api/bson_8h.html

static JSVAL _bson_size(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson *b = (bson *) wrap->Value();
	return scope.Close(Integer::New(bson_size(b)));
}

static JSVAL _bson_print(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson *b = (bson *) wrap->Value();
	bson_print(b);
	return Undefined();
}

static JSVAL _bson_data(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson *b = (bson *) wrap->Value();
    return scope.Close(External::New((void *)bson_data(b)));
}

static JSVAL _bson_print_raw(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    char *b = (char *) wrap->Value();
	bson_print_raw(b, args[1]->IntegerValue());
	return Undefined();
}

static JSVAL _bson_find(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
    Local<External>wrap2 = Local<External>::Cast(args[1]);
    bson *b = (bson *) wrap2->Value();
	String::Utf8Value name(args[2]->ToString());
    return scope.Close(Integer::New(bson_find(i, b, *name)));
}

static JSVAL _bson_iterator_init(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson *b = (bson *) wrap->Value();
	bson_iterator *i = new bson_iterator;
	bson_iterator_init(i, b);
    return scope.Close(External::New(i));
}

static JSVAL _bson_iterator_more(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(Boolean::New(bson_iterator_more(i)));
}

static JSVAL _bson_iterator_next(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(Integer::New(bson_iterator_next(i)));
}

static JSVAL _bson_iterator_type(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(Integer::New(bson_iterator_type(i)));
}

static JSVAL _bson_iterator_key(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(String::New(bson_iterator_key(i)));
}

static JSVAL _bson_iterator_value(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(String::New(bson_iterator_value(i)));
}

static JSVAL _bson_iterator_double(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(Number::New(bson_iterator_double(i)));
}

static JSVAL _bson_iterator_int(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(Integer::New(bson_iterator_int(i)));
}

static JSVAL _bson_iterator_long(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(Integer::New(bson_iterator_long(i)));
}

static JSVAL _bson_iterator_timestamp(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(Integer::New((int)bson_iterator_timestamp(i)));
}

static JSVAL _bson_iterator_bool(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(Boolean::New(bson_iterator_bool(i)));
}

static JSVAL _bson_iterator_double_raw(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(Number::New(bson_iterator_double_raw(i)));
}

static JSVAL _bson_iterator_int_raw(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(Integer::New(bson_iterator_int_raw(i)));
}

static JSVAL _bson_iterator_long_raw(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(Integer::New(bson_iterator_long_raw(i)));
}

static JSVAL _bson_iterator_bool_raw(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(Boolean::New(bson_iterator_bool_raw(i)));
}

static JSVAL _bson_iterator_oid(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(External::New(bson_iterator_oid(i)));
}

static JSVAL _bson_iterator_string(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(String::New(bson_iterator_string(i)));
}

static JSVAL _bson_iterator_string_len(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(String::New(bson_iterator_string_len(i, args[1]->IntegerValue())));
}

static JSVAL _bson_iterator_code(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
	return scope.Close(String::New(bson_iterator_code(i)));
}

static JSVAL _bson_code_scope(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    bson_iterator *i = (bson_iterator *) wrap->Value();
    Local<External>wrap2 = Local<External>::Cast(args[1]);
    bson *b = (bson *) wrap2->Value();
	bson_iterator_code_scope(i, b);
	return Undefined();
}









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

///////////////// CURSOR ///////////////////

static JSVAL _mongo_cursor_init(JSARGS args) {
    HandleScope scope;
	mongo_cursor *cursor = new mongo_cursor;
    Local<External>wrap = Local<External>::Cast(args[0]);
    mongo *conn = (mongo *) wrap->Value();
	String::Utf8Value name_space(args[1]->ToString());
	return scope.Close(External::New(cursor, conn, *name_space));
}

static JSVAL _mongo_cursor_next(JSARGS args) {
    HandleScope scope;
    Local<External>wrap = Local<External>::Cast(args[0]);
    mongo_cursor *cursor = (mongo_cursor *) wrap->Value();
	return scope.Close(Integer::New(mongo_cursor_next(cursor)));
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

	// BSON defines
	mongodb->Set(String::New("BSON_OK"), Integer::New(BSON_OK));
	mongodb->Set(String::New("BSON_ERROR"), Integer::New(BSON_ERROR));
	
	// bson_validity_t
	mongodb->Set(String::New("BSON_VALID"), Integer::New(BSON_VALID));
	mongodb->Set(String::New("BSON_NOT_UTF8"), Integer::New(BSON_NOT_UTF8));
	mongodb->Set(String::New("BSON_FIELD_HAS_DOT"), Integer::New(BSON_FIELD_HAS_DOT));
	mongodb->Set(String::New("BSON_FIELD_INIT_DOLLAR"), Integer::New(BSON_FIELD_INIT_DOLLAR));
	mongodb->Set(String::New("BSON_ALREADY_FINISHED"), Integer::New(BSON_ALREADY_FINISHED));

	// bson_binary_subtype_t
	mongodb->Set(String::New("BSON_BIN_BINARY"), Integer::New(BSON_BIN_BINARY));
	mongodb->Set(String::New("BSON_BIN_FUNC"), Integer::New(BSON_BIN_FUNC));
	mongodb->Set(String::New("BSON_BIN_BINARY_OLD"), Integer::New(BSON_BIN_BINARY_OLD));
	mongodb->Set(String::New("BSON_BIN_UUID"), Integer::New(BSON_BIN_UUID));
	mongodb->Set(String::New("BSON_BIN_MD5"), Integer::New(BSON_BIN_MD5));
	mongodb->Set(String::New("BSON_BIN_USER"), Integer::New(BSON_BIN_USER));

	// bson_type
	mongodb->Set(String::New("BSON_EOO"), Integer::New(BSON_EOO));
	mongodb->Set(String::New("BSON_DOUBLE"), Integer::New(BSON_DOUBLE));
	mongodb->Set(String::New("BSON_STRING"), Integer::New(BSON_STRING));
	mongodb->Set(String::New("BSON_OBJECT"), Integer::New(BSON_OBJECT));
	mongodb->Set(String::New("BSON_ARRAY"), Integer::New(BSON_ARRAY));
	mongodb->Set(String::New("BSON_BINDATA"), Integer::New(BSON_BINDATA));
	mongodb->Set(String::New("BSON_UNDEFINED"), Integer::New(BSON_UNDEFINED));
	mongodb->Set(String::New("BSON_OID"), Integer::New(BSON_OID));
	mongodb->Set(String::New("BSON_BOOL"), Integer::New(BSON_BOOL));
	mongodb->Set(String::New("BSON_DATE"), Integer::New(BSON_DATE));
	mongodb->Set(String::New("BSON_NULL"), Integer::New(BSON_NULL));
	mongodb->Set(String::New("BSON_REGEX"), Integer::New(BSON_REGEX));
	mongodb->Set(String::New("BSON_DBREF"), Integer::New(BSON_DBREF));
	mongodb->Set(String::New("BSON_CODE"), Integer::New(BSON_CODE));
	mongodb->Set(String::New("BSON_SYMBOL"), Integer::New(BSON_SYMBOL));
	mongodb->Set(String::New("BSON_CODEWSCOPE"), Integer::New(BSON_CODEWSCOPE));
	mongodb->Set(String::New("BSON_INT"), Integer::New(BSON_INT));
	mongodb->Set(String::New("BSON_TIMESTAMP"), Integer::New(BSON_TIMESTAMP));
	mongodb->Set(String::New("BSON_LONG"), Integer::New(BSON_LONG));


	
    builtinObject->Set(String::New("mongodb"), mongodb);
}

#endif