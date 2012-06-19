#include "SilkJS.h"
#ifdef __APPLE__
#include <mysql.h>
#else
#include <mysql/mysql.h>
#endif

struct mstate {
    MYSQL *handle;
    char *currentDb;
    char *host;
    char *user;
    char *passwd;

    mstate(MYSQL *h, const char *d) {
        this->handle = h;
        this->currentDb = strdup(d);
        this->host = this->user = this->passwd = NULL;
    }

    ~mstate() {
        if (this->currentDb) {
            delete[] this->currentDb;
        }
        if (this->passwd) {
            delete [] this->passwd;
        }
        if (this->user) {
            delete [] this->user;
        }
        if (this->host) {
            delete [] this->host;
        }
    }

    void setCurrentDb(const char *d) {
        if (this->currentDb) {
            delete[] this->currentDb;
        }
        this->currentDb = strdup(d);
    }
};

static inline const char *currentDb (Handle<Value>v) {
    if (v->IsNull()) {
        ThrowException(String::New("Handle is NULL"));
        return NULL;
    }
    mstate *m = (mstate *) JSEXTERN(v);
    return m->currentDb;
}

static inline MYSQL* HANDLE (Handle<Value>v) {
    if (v->IsNull()) {
        ThrowException(String::New("Handle is NULL"));
        return NULL;
    }
    mstate *m = (mstate *) JSEXTERN(v);
    if (!mysql_ping(m->handle)) {
        return m->handle;
    }
    // reconnect
    mysql_close(m->handle);
    m->handle = mysql_init(NULL);
    m->handle = mysql_real_connect(m->handle, m->host, m->user, m->passwd, m->currentDb, 3306, NULL, CLIENT_IGNORE_SIGPIPE | CLIENT_FOUND_ROWS);
    if (!m->handle) {
        ThrowException(String::New("Could not reconnect to MySQL server"));
        return NULL;
    }
    return m->handle;
}

static inline void deleteHandle (Handle<Value>v) {
    if (v->IsNull()) {
        ThrowException(String::New("Handle is NULL"));
        return;
    }
    mstate *m = (mstate *) JSEXTERN(v);
    delete m;
}

static inline void setCurrentDb (Handle<Value>v, const char *s) {
    if (v->IsNull()) {
        ThrowException(String::New("Handle is NULL"));
        return;
    }
    mstate *m = (mstate *) JSEXTERN(v);
    m->setCurrentDb(s);
}

static inline MYSQL_RES *RESULT_SET (Handle<Value> v) {
    if (v->IsNull()) {
        ThrowException(String::New("Handle is NULL"));
        return NULL;
    }
    return (MYSQL_RES *) JSEXTERN(v);
}

static JSVAL affected_rows (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    return Integer::New(mysql_affected_rows(handle));
}

static JSVAL autocommit (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    bool flag = args[1]->BooleanValue();
    return Boolean::New(mysql_autocommit(handle, flag));
}

static JSVAL change_user (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    String::Utf8Value user(args[1]->ToString());
    String::Utf8Value password(args[2]->ToString());
    String::Utf8Value db(args[3]->ToString());
    return Boolean::New(mysql_change_user(handle, *user, *password, *db));
}

static JSVAL character_set_name (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    return String::New(mysql_character_set_name(handle));
}

//static JSVAL close(JSARGS args) {
//  HandleScope scope;
//  MYSQL *handle = (MYSQL *) args[0]->IntegerValue();
//  mysql_close(handle);
//  return Undefined();
//}

static JSVAL commit (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    return Boolean::New(mysql_commit(handle));
}

static JSVAL data_seek (JSARGS args) {
    MYSQL_RES *res = RESULT_SET(args[0]);
    mysql_data_seek(res, args[1]->IntegerValue());
    return Undefined();
}

#undef errno

static JSVAL errno (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    return Integer::New(mysql_errno(handle));
}

static JSVAL error (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    const char *error = mysql_error(handle);
    return String::New(error);
}

//  char *name;                 /* Name of column */
//  char *org_name;             /* Original column name, if an alias */
//  char *table;                /* Table of column if column was a field */
//  char *org_table;            /* Org table name, if table was an alias */
//  char *db;                   /* Database for table */
//  char *catalog;        /* Catalog for table */
//  char *def;                  /* Default value (set by mysql_list_fields) */
//  unsigned long length;       /* Width of column (create length) */
//  unsigned long max_length;   /* Max width for selected set */
//  unsigned int name_length;
//  unsigned int org_name_length;
//  unsigned int table_length;
//  unsigned int org_table_length;
//  unsigned int db_length;
//  unsigned int catalog_length;
//  unsigned int def_length;
//  unsigned int flags;         /* Div flags */
//  unsigned int decimals;      /* Number of decimals in field */
//  unsigned int charsetnr;     /* Character set */
//  enum enum_field_types type; /* Type of field. See mysql_com.h for types */
//  void *extension;

static void MakeField (MYSQL_FIELD *f, JSOBJ o) {
    o->Set(String::New("name"), String::New(f->name));
    o->Set(String::New("org_name"), String::New(f->org_name));
    o->Set(String::New("table"), String::New(f->table));
    o->Set(String::New("org_table"), String::New(f->org_table));
    o->Set(String::New("db"), String::New(f->db));
    o->Set(String::New("catalog"), String::New(f->catalog));
    o->Set(String::New("def"), String::New(f->def));
    o->Set(String::New("length"), Integer::New(f->length));
    o->Set(String::New("max_length"), Integer::New(f->max_length));
    o->Set(String::New("name_length"), Integer::New(f->name_length));
    o->Set(String::New("org_name_length"), Integer::New(f->org_name_length));
    o->Set(String::New("table_length"), Integer::New(f->table_length));
    o->Set(String::New("db_length"), Integer::New(f->db_length));
    o->Set(String::New("catalog_length"), Integer::New(f->catalog_length));
    o->Set(String::New("def_length"), Integer::New(f->def_length));
    o->Set(String::New("flags"), Integer::New(f->flags));
    o->Set(String::New("decimals"), Integer::New(f->decimals));
    o->Set(String::New("charsetnr"), Integer::New(f->charsetnr));
    switch (f->type) {
        case MYSQL_TYPE_DECIMAL:
            o->Set(String::New("type"), String::New("DECIMAL"));
            break;
        case MYSQL_TYPE_TINY:
            o->Set(String::New("type"), String::New("TINY"));
            break;
        case MYSQL_TYPE_SHORT:
            o->Set(String::New("type"), String::New("SHORT"));
            break;
        case MYSQL_TYPE_LONG:
            o->Set(String::New("type"), String::New("LONG"));
            break;
        case MYSQL_TYPE_FLOAT:
            o->Set(String::New("type"), String::New("FLOAT"));
            break;
        case MYSQL_TYPE_DOUBLE:
            o->Set(String::New("type"), String::New("DOUBLE"));
            break;
        case MYSQL_TYPE_NULL:
            o->Set(String::New("type"), String::New("NULL"));
            break;
        case MYSQL_TYPE_TIMESTAMP:
            o->Set(String::New("type"), String::New("TIMESTAMP"));
            break;
        case MYSQL_TYPE_LONGLONG:
            o->Set(String::New("type"), String::New("LONGLONG"));
            break;
        case MYSQL_TYPE_INT24:
            o->Set(String::New("type"), String::New("INT24"));
            break;
        case MYSQL_TYPE_DATE:
            o->Set(String::New("type"), String::New("DATE"));
            break;
        case MYSQL_TYPE_TIME:
            o->Set(String::New("type"), String::New("TIME"));
            break;
        case MYSQL_TYPE_DATETIME:
            o->Set(String::New("type"), String::New("DATETIME"));
            break;
        case MYSQL_TYPE_YEAR:
            o->Set(String::New("type"), String::New("YEAR"));
            break;
        case MYSQL_TYPE_NEWDATE:
            o->Set(String::New("type"), String::New("NEWDATE"));
            break;
        case MYSQL_TYPE_VARCHAR:
            o->Set(String::New("type"), String::New("VARCHAR"));
            break;
        case MYSQL_TYPE_BIT:
            o->Set(String::New("type"), String::New("BIT"));
            break;
        case MYSQL_TYPE_NEWDECIMAL:
            o->Set(String::New("type"), String::New("NEWDECIMAL"));
            break;
        case MYSQL_TYPE_ENUM:
            o->Set(String::New("type"), String::New("ENUM"));
            break;
        case MYSQL_TYPE_SET:
            o->Set(String::New("type"), String::New("SET"));
            break;
        case MYSQL_TYPE_TINY_BLOB:
            o->Set(String::New("type"), String::New("TINY_BLOB"));
            break;
        case MYSQL_TYPE_MEDIUM_BLOB:
            o->Set(String::New("type"), String::New("MEDIUM_BLOB"));
            break;
        case MYSQL_TYPE_LONG_BLOB:
            o->Set(String::New("type"), String::New("LONG_BLOB"));
            break;
        case MYSQL_TYPE_BLOB:
            o->Set(String::New("type"), String::New("BLOB"));
            break;
        case MYSQL_TYPE_VAR_STRING:
            o->Set(String::New("type"), String::New("VAR_STRING"));
            break;
        case MYSQL_TYPE_STRING:
            o->Set(String::New("type"), String::New("STRING"));
            break;
        case MYSQL_TYPE_GEOMETRY:
            o->Set(String::New("type"), String::New("GEOMETRY"));
            break;
        default:
            o->Set(String::New("type"), String::New("UNKNOWN"));
            break;
    }
}

static JSVAL fetch_field (JSARGS args) {
    MYSQL_RES *res = RESULT_SET(args[0]);
    MYSQL_FIELD *f = mysql_fetch_field(res);
    JSOBJ o = Object::New();
    MakeField(f, o);
    return o;
}

static JSVAL fetch_field_direct (JSARGS args) {
    MYSQL_RES *res = RESULT_SET(args[0]);
    MYSQL_FIELD *f = mysql_fetch_field_direct(res, args[1]->IntegerValue());
    JSOBJ o = Object::New();
    MakeField(f, o);
    return o;
}

static JSVAL fetch_fields (JSARGS args) {
    MYSQL_RES *result = RESULT_SET(args[0]);
    unsigned int num_fields = mysql_num_fields(result);
    MYSQL_FIELD *fields = mysql_fetch_fields(result);
    JSARRAY a = Array::New();
    for (unsigned int i = 0; i < num_fields; i++) {
        JSOBJ o = Object::New();
        MakeField(&fields[i], o);
        a->Set(i, o);
    }
    return a;
}

static JSVAL fetch_lengths (JSARGS args) {
    MYSQL_RES *result = RESULT_SET(args[0]);
    unsigned int num_fields = mysql_num_fields(result);
    unsigned long *lengths = mysql_fetch_lengths(result);
    JSARRAY a = Array::New();
    ;
    for (unsigned int i = 0; i < num_fields; i++) {
        a->Set(i, Integer::New(lengths[i]));
    }
    return a;
}

static JSVAL fetch_row (JSARGS args) {
    MYSQL_RES *result = RESULT_SET(args[0]);
    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row) {
        return Null();
    }
    unsigned int num_fields = mysql_num_fields(result);
    unsigned long *lengths = mysql_fetch_lengths(result);
    MYSQL_FIELD *fields = mysql_fetch_fields(result);
    JSOBJ o = Object::New();
    for (unsigned int i = 0; i < num_fields; i++) {
        if (row[i] == NULL) {
            o->Set(String::New(fields[i].name), Null());
        }
        else {
            switch (fields[i].type) {
                case MYSQL_TYPE_TINY:
                    o->Set(String::New(fields[i].name), Integer::New(atol(row[i])));
                    break;
                case MYSQL_TYPE_SHORT:
                    o->Set(String::New(fields[i].name), Integer::New(atol(row[i])));
                    break;
                case MYSQL_TYPE_FLOAT:
                    o->Set(String::New(fields[i].name), Number::New(atof(row[i])));
                    break;
                case MYSQL_TYPE_DOUBLE:
                    o->Set(String::New(fields[i].name), Number::New(atof(row[i])));
                    break;
                case MYSQL_TYPE_NULL:
                    o->Set(String::New(fields[i].name), Null());
                    break;
                case MYSQL_TYPE_TIMESTAMP:
                    o->Set(String::New(fields[i].name), Integer::New(atol(row[i])));
                    break;
                case MYSQL_TYPE_LONGLONG:
                case MYSQL_TYPE_LONG:
                case MYSQL_TYPE_INT24:
                    o->Set(String::New(fields[i].name), Integer::New(atol(row[i])));
                    break;
                default:
                    o->Set(String::New(fields[i].name), String::New(row[i], lengths[i]));
                    break;
            }
        }
    }
    return o;
}

static JSVAL field_count (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    return Integer::New(mysql_field_count(handle));
}

static JSVAL field_seek (JSARGS args) {
    MYSQL_RES *result = RESULT_SET(args[0]);
    MYSQL_FIELD_OFFSET offset = (MYSQL_FIELD_OFFSET) args[1]->IntegerValue();
    return Integer::New(mysql_field_seek(result, offset));
}

static JSVAL field_tell (JSARGS args) {
    MYSQL_RES *result = RESULT_SET(args[0]);
    return Integer::New(mysql_field_tell(result));
}

static JSVAL free_result (JSARGS args) {
    MYSQL_RES *result = RESULT_SET(args[0]);
    mysql_free_result(result);
    return Undefined();
}

static JSVAL get_character_set_info (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    MY_CHARSET_INFO cs;
    mysql_get_character_set_info(handle, &cs);
    JSOBJ o = Object::New();
    o->Set(String::New("number"), Integer::New(cs.number));
    o->Set(String::New("name"), String::New(cs.name));
    o->Set(String::New("csname"), String::New(cs.csname));
    o->Set(String::New("comment"), String::New(cs.comment));
    o->Set(String::New("dir"), String::New(cs.dir));
    o->Set(String::New("mbminlen"), Integer::New(cs.mbminlen));
    o->Set(String::New("mbmaxlen"), Integer::New(cs.mbmaxlen));
    return o;
}

static JSVAL get_client_info (JSARGS args) {
    return String::New(mysql_get_client_info());
}

static JSVAL get_client_version (JSARGS args) {
    return Integer::New(mysql_get_client_version());
}

static JSVAL get_host_info (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    return String::New(mysql_get_host_info(handle));
}

static JSVAL get_proto_info (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    return Integer::New(mysql_get_proto_info(handle));
}

static JSVAL get_server_info (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    const char *s = mysql_get_server_info(handle);
    return String::New(s);
}

static JSVAL get_server_version (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    return Integer::New(mysql_get_server_version(handle));
}

static JSVAL info (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    const char *info = mysql_info(handle);
    if (info) {
        return String::New(info);
    }
    else {
        return Null();
    }
}

static JSVAL insert_id (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    return Integer::New(mysql_insert_id(handle));
}

static JSVAL kill (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);

    return Integer::New(mysql_kill(handle, args[1]->IntegerValue()));
}

static JSVAL list_dbs (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    const char *wild = NULL;
    if (args.Length() > 1) {
        String::Utf8Value pat(args[1]->ToString());
        wild = *pat;
    }
    return Integer::New((unsigned long) mysql_list_dbs(handle, wild));
}

static JSVAL list_fields (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    const char *table = NULL;
    const char *wild = NULL;
    if (args.Length() > 1) {
        String::Utf8Value tbl(args[1]->ToString());
        table = *tbl;
    }
    if (args.Length() > 2) {
        String::Utf8Value pat(args[1]->ToString());
        wild = *pat;
    }
    return Integer::New((unsigned long) mysql_list_fields(handle, table, wild));
}

static JSVAL list_processes (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    return Integer::New((unsigned long) mysql_list_processes(handle));
}

static JSVAL list_tables (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    const char *wild = NULL;
    if (args.Length() > 1) {
        String::Utf8Value pat(args[1]->ToString());
        wild = *pat;
    }
    return Integer::New((unsigned long) mysql_list_tables(handle, wild));
}

static JSVAL query (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    String::Utf8Value sql(args[1]->ToString());
    return Integer::New((unsigned long) mysql_query(handle, *sql));
}

static JSVAL store_result (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    MYSQL_RES *result = mysql_store_result(handle);
    return Integer::New((unsigned long) result);
}

static JSVAL init (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
#ifdef EMBEDDED_MYSQL
    if (handle) {
        mysql_options(handle, MYSQL_READ_DEFAULT_GROUP, "libmysqld_client");
        mysql_options(handle, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);
    }
#endif
    return Integer::New((unsigned long) handle);
}

static JSVAL real_connect (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    String::Utf8Value host(args[1]->ToString());
    String::Utf8Value user(args[2]->ToString());
    String::Utf8Value password(args[3]->ToString());
    String::Utf8Value db(args[4]->ToString());


    //  mysql_options(handle, MYSQL_READ_DEFAULT_GROUP, "libmysqld_client");
    //  mysql_options(handle, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);
    return Integer::New((unsigned long) mysql_real_connect(handle, *host, *user, *password, *db, 0, NULL, CLIENT_IGNORE_SIGPIPE | CLIENT_FOUND_ROWS));
    //  return scope.Close(Integer::New((unsigned long)mysql_real_connect(handle, NULL, NULL, NULL, *db, 0,NULL,0)));
}

//static JSVAL TEMPLATE(/JSARGS args) {
//  HandleScope scope;
//  MYSQL *handle = (MYSQL *) args[0]->IntegerValue();
//  return scope.Close(String::New(mysql_character_set_name(handle)));
//}

static JSVAL getDataRowsJson (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    String::Utf8Value sql(args[1]);
    mysql_ping(handle);
    //  printf("%d %s\n", mysql_ping(handle), mysql_error(handle));
    int failure = mysql_query(handle, *sql);
    if (failure) {
        return False();
    }
    MYSQL_RES *result = mysql_store_result(handle);
    if (!result) {
        return False();
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_FIELD *fields = mysql_fetch_fields(result);

    int types[num_fields];
    char *names[num_fields];
    for (int n = 0; n < num_fields; n++) {
        //      names[n] = String::New(fields[n].name);
        types[n] = fields[n].type;
        names[n] = fields[n].name;
    }
    string json = "[";
    ;
    unsigned long rowNdx = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        if (rowNdx++) {
            json += ",";
        }
        json += "{";
        for (int i = 0; i < num_fields; i++) {
            if (i) {
                json += ",";
            }
            if (row[i] == NULL) {
                json += "\"";
                json += names[i];
                json += "\":null";
            }
            else {
                switch (types[i]) {
                    case MYSQL_TYPE_NULL:
                        json += "\"";
                        json += names[i];
                        json += "\":null";
                        break;
                    case MYSQL_TYPE_TINY:
                    case MYSQL_TYPE_SHORT:
                    case MYSQL_TYPE_LONG:
                    case MYSQL_TYPE_FLOAT:
                    case MYSQL_TYPE_DOUBLE:
                    case MYSQL_TYPE_TIMESTAMP:
                    case MYSQL_TYPE_LONGLONG:
                    case MYSQL_TYPE_INT24:
                        json += "\"";
                        json += names[i];
                        json += "\":";
                        json += row[i];
                        break;
                    default:
                        json += "\"";
                        json += names[i];
                        json += "\":\"";
                        json += row[i];
                        json += "\"";
                        //                      break;
                }
            }
        }
        json += "}";
        //  printf("%s\n", json.c_str());
    }
    json += "]";
    mysql_free_result(result);
    //  for (int i=0; i<num_fields; i++) {
    //      delete[] names[i];
    //  }
    //  printf("%s\n", json.c_str());
    //  printf("%s\n", json.GetBuffer());
    return String::New(json.c_str(), json.size());
}

JSVAL getDataRows (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    String::Utf8Value sql(args[1]->ToString());
    mysql_ping(handle);
    int failure = mysql_query(handle, *sql);
    if (failure) {
        return ThrowException(String::New(mysql_error(handle)));
    }
    MYSQL_RES *result = mysql_use_result(handle);
    if (!result) {
        return False();
    }
    unsigned int num_fields = mysql_num_fields(result);
    MYSQL_FIELD *fields = mysql_fetch_fields(result);

    Handle<Array>a = Array::New(mysql_num_rows(result));
    Local<String> names[num_fields];
    int types[num_fields];
    for (unsigned int n = 0; n < num_fields; n++) {
        names[n] = String::New(fields[n].name);
        types[n] = fields[n].type;
    }
    unsigned long rowNdx = 0;
    unsigned int i;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        unsigned long *lengths = mysql_fetch_lengths(result);
        JSOBJ o = Object::New();
        for (i = 0; i < num_fields; i++) {
            if (row[i] == NULL) {
                o->Set(names[i], Null());
            }
            else {
                switch (types[i]) {
                    case MYSQL_TYPE_NULL:
                        o->Set(names[i], Null());
                        break;
                    case MYSQL_TYPE_TINY:
                    case MYSQL_TYPE_SHORT:
                    case MYSQL_TYPE_LONG:
                    case MYSQL_TYPE_TIMESTAMP:
                    case MYSQL_TYPE_LONGLONG:
                    case MYSQL_TYPE_INT24:
                        o->Set(names[i], Number::New(atol(row[i])));
                        break;
                    case MYSQL_TYPE_FLOAT:
                    case MYSQL_TYPE_DOUBLE:
                        o->Set(names[i], Number::New(atof(row[i])));
                        break;
                    default:
                        o->Set(names[i], String::New(row[i], lengths[i]));
                        break;
                }
            }
        }
        a->Set(rowNdx++, o);
    }
    mysql_free_result(result);
    return a;
}

JSVAL getDataRow (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    String::Utf8Value sql(args[1]->ToString());
    mysql_ping(handle);
    int failure = mysql_query(handle, *sql);
    if (failure) {
        return ThrowException(String::New(mysql_error(handle)));
    }
    MYSQL_RES *result = mysql_store_result(handle);
    if (!result) {
        return False();
    }
    JSOBJ o = Object::New();
    unsigned int num_fields = mysql_num_fields(result);
    MYSQL_FIELD *fields = mysql_fetch_fields(result);
    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row) {
        return False();
    }
    unsigned long *lengths = mysql_fetch_lengths(result);
    for (unsigned int i = 0; i < num_fields; i++) {
        if (row[i] == NULL) {
            o->Set(String::New(fields[i].name), Null());
        }
        else {
            switch (fields[i].type) {
                case MYSQL_TYPE_TINY:
                case MYSQL_TYPE_SHORT:
                case MYSQL_TYPE_LONG:
                case MYSQL_TYPE_TIMESTAMP:
                case MYSQL_TYPE_LONGLONG:
                case MYSQL_TYPE_INT24:
                    o->Set(String::New(fields[i].name), Number::New(atol(row[i])));
                    break;
                case MYSQL_TYPE_FLOAT:
                    o->Set(String::New(fields[i].name), Number::New(atof(row[i])));
                    break;
                case MYSQL_TYPE_DOUBLE:
                    o->Set(String::New(fields[i].name), Number::New(atof(row[i])));
                    break;
                case MYSQL_TYPE_NULL:
                    o->Set(String::New(fields[i].name), Null());
                    break;
                default:
                    o->Set(String::New(fields[i].name), String::New(row[i], lengths[i]));
                    break;
            }
        }
    }
    mysql_free_result(result);
    return o;
}

JSVAL getScalar (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    String::Utf8Value sql(args[1]->ToString());

    int failure = mysql_query(handle, *sql);
    if (failure) {
        return ThrowException(Exception::Error(String::New(mysql_error(handle))));
    }
    MYSQL_RES *result = mysql_store_result(handle);
    if (!result) {
        return False();
    }
    unsigned long *lengths = mysql_fetch_lengths(result);
    MYSQL_FIELD *fields = mysql_fetch_fields(result);
    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row) {
        return False();
    }

    if (row[0] == NULL) {
        return Null();
    }
    else {
        switch (fields[0].type) {
            case MYSQL_TYPE_TINY:
            case MYSQL_TYPE_SHORT:
            case MYSQL_TYPE_LONG:
            case MYSQL_TYPE_TIMESTAMP:
            case MYSQL_TYPE_LONGLONG:
            case MYSQL_TYPE_INT24:
                return Number::New(atol(row[0]));
            case MYSQL_TYPE_FLOAT:
                return Number::New(atof(row[0]));
            case MYSQL_TYPE_DOUBLE:
                return Number::New(atof(row[0]));
            case MYSQL_TYPE_NULL:
                return Null();
            default:
                return String::New(row[0], lengths[0]);
        }
    }
}

JSVAL update (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    String::Utf8Value query(args[1]->ToString());
    int failure = mysql_query(handle, *query);
    if (failure) {
        return ThrowException(Exception::Error(String::New(mysql_error(handle))));
    }
    return Integer::New(mysql_affected_rows(handle));
}

JSVAL connect (JSARGS args) {
    String::AsciiValue host(args[0]->ToString());
    String::AsciiValue user(args[1]->ToString());
    String::AsciiValue passwd(args[2]->ToString());
    String::AsciiValue db(args[3]->ToString());

    int port = 3306;
    if (args.Length() > 4) {
        port = args[4]->IntegerValue();
    }
    MYSQL *handle = mysql_init(NULL);
    my_bool reconnect = 1;
    mysql_options(handle, MYSQL_OPT_RECONNECT, &reconnect);

    //      handle = mysql_real_connect(handle, "localhost", "mschwartz", "", "sim", 3306, NULL, 0);
    handle = mysql_real_connect(handle, *host, *user, *passwd, *db, port, NULL, CLIENT_IGNORE_SIGPIPE | CLIENT_FOUND_ROWS);
    if (!handle) {
        return ThrowException(Exception::Error(String::New("MySQL connection failed")));
    }
    mysql_options(handle, MYSQL_OPT_RECONNECT, &reconnect);
    mysql_ping(handle);
    mstate *m = new mstate(handle, *db);
    m->host = strdup(*host);
    m->user = strdup(*user);
    m->passwd = strdup(*passwd);
    return External::New(m);
}

JSVAL select_db (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    String::AsciiValue db(args[1]->ToString());
    if (strcmp(*db, currentDb(args[0]))) {
        if (mysql_select_db(handle, *db)) {
            return ThrowException(Exception::Error(String::New(mysql_error(handle))));
        }
        setCurrentDb(args[0], *db);
    }
    return Undefined();
}

JSVAL close (JSARGS args) {
    MYSQL *handle = HANDLE(args[0]);
    mysql_close(handle);
    deleteHandle(args[0]);
    return Undefined();
}

void init_mysql_object () {
    JSOBJT o = ObjectTemplate::New();
    o->Set(String::New("connect"), FunctionTemplate::New(connect));
    o->Set(String::New("selectDb"), FunctionTemplate::New(select_db));
    o->Set(String::New("getDataRows"), FunctionTemplate::New(getDataRows));
    o->Set(String::New("getDataRowsJson"), FunctionTemplate::New(getDataRowsJson));
    o->Set(String::New("getDataRow"), FunctionTemplate::New(getDataRow));
    o->Set(String::New("getScalar"), FunctionTemplate::New(getScalar));
    o->Set(String::New("update"), FunctionTemplate::New(update));

    o->Set(String::New("affected_rows"), FunctionTemplate::New(affected_rows));
    o->Set(String::New("autocommit"), FunctionTemplate::New(autocommit));
    o->Set(String::New("change_user"), FunctionTemplate::New(change_user));
    o->Set(String::New("character_set_name"), FunctionTemplate::New(character_set_name));
    o->Set(String::New("close"), FunctionTemplate::New(close));
    o->Set(String::New("commit"), FunctionTemplate::New(commit));
    o->Set(String::New("data_seek"), FunctionTemplate::New(data_seek));
    o->Set(String::New("errno"), FunctionTemplate::New(errno));
    o->Set(String::New("error"), FunctionTemplate::New(error));
    o->Set(String::New("fetch_field"), FunctionTemplate::New(fetch_field));
    o->Set(String::New("fetch_field_direct"), FunctionTemplate::New(fetch_field_direct));
    o->Set(String::New("fetch_fields"), FunctionTemplate::New(fetch_fields));
    o->Set(String::New("fetch_lengths"), FunctionTemplate::New(fetch_lengths));
    o->Set(String::New("fetch_row"), FunctionTemplate::New(fetch_row));
    o->Set(String::New("field_count"), FunctionTemplate::New(field_count));
    o->Set(String::New("field_seek"), FunctionTemplate::New(field_seek));
    o->Set(String::New("field_tell"), FunctionTemplate::New(field_tell));
    o->Set(String::New("free_result"), FunctionTemplate::New(free_result));
    o->Set(String::New("get_character_set_info"), FunctionTemplate::New(get_character_set_info));
    o->Set(String::New("get_client_info"), FunctionTemplate::New(get_client_info));
    o->Set(String::New("get_client_version"), FunctionTemplate::New(get_client_version));
    o->Set(String::New("get_host_info"), FunctionTemplate::New(get_host_info));
    o->Set(String::New("get_proto_info"), FunctionTemplate::New(get_proto_info));
    o->Set(String::New("get_server_info"), FunctionTemplate::New(get_server_info));
    o->Set(String::New("get_server_version"), FunctionTemplate::New(get_server_version));
    o->Set(String::New("info"), FunctionTemplate::New(info));
    o->Set(String::New("init"), FunctionTemplate::New(init));
    o->Set(String::New("insert_id"), FunctionTemplate::New(insert_id));
    o->Set(String::New("kill"), FunctionTemplate::New(kill));
    o->Set(String::New("list_dbs"), FunctionTemplate::New(list_dbs));
    o->Set(String::New("list_fields"), FunctionTemplate::New(list_fields));
    o->Set(String::New("list_processes"), FunctionTemplate::New(list_processes));
    o->Set(String::New("list_tables"), FunctionTemplate::New(list_tables));
    o->Set(String::New("store_result"), FunctionTemplate::New(store_result));
    o->Set(String::New("real_connect"), FunctionTemplate::New(real_connect));
    o->Set(String::New("query"), FunctionTemplate::New(query));
    builtinObject->Set(String::New("mysql"), o);
}
