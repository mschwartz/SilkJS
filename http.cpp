#include "SilkJS.h"

static JSVAL OpenStream(JSARGS args) {
	HandleScope scope;
	InputStream *s = new InputStream(args[0]->IntegerValue());
	return scope.Close(External::New(s));
}

static JSVAL CloseStream(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	InputStream *s = (InputStream *)wrap->Value();
	delete s;
	return Undefined();
}

static JSVAL ReadByte(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	InputStream *s = (InputStream *)wrap->Value();	
	return scope.Close(Integer::New(s->Read()));
}

static JSVAL ReadHeaders(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	InputStream *s = (InputStream *)wrap->Value();	
	
	string out;
	int newlineCount = 0;
	while (newlineCount < 2) {
		char c = s->Read();
		switch (c) {
			case 0:
			case -1:	// connection closed
//				if (out.size()) {
//					break;
//				}
				return Null();
			case '\r':
				continue;
			case '\n':
				newlineCount++;
				out += c;
				break;
			default:
				out += c;
				newlineCount = 0;
				break;
		}
	}
	return scope.Close(String::New(out.c_str(), out.size()));
}

static JSVAL ReadPost(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	InputStream *s = (InputStream *)wrap->Value();
	long size = args[1]->IntegerValue();
	
	char buf[size];
	if (s->Read((unsigned char *)buf, size) < 0) {
		return False();
	}
	Handle<String> out = String::New(buf, size);
	return scope.Close(out);
}

static JSVAL ReadMime(JSARGS args) {
	HandleScope scope;
	Local<External>wrap = Local<External>::Cast(args[0]);
	InputStream *s = (InputStream *)wrap->Value();
	long size = args[1]->IntegerValue();
    String::Utf8Value jsboundary(args[2]);
	char *boundary = *jsboundary;

	char *in = new char[size];
	if (s->Read((unsigned char *)in, size) < 0) {
		delete [] in;
		return Null();
	}
	
	char b[strlen(boundary)+3];
	sprintf(b, "--%s", boundary);
	int bLength = strlen(b);
	char end[strlen(boundary)+3+2];
	sprintf(end, "--%s--", boundary);
//	int endLength = strlen(end);
	
	string out;
	
	long ndx = 0;
	while (ndx < size) {
		while (strncmp(&in[ndx], b, bLength)) {
			out += in[ndx++];
		}
		out.append(&in[ndx], bLength);
		ndx += bLength;
		
		if (!strncmp(&in[ndx], "--", 2)) {
			out += "--\r\n";
			ndx += 4;
			break;
		}
		else {
			out += "\r\n";
			ndx += 2;
		}
		// mime part header
		long lookAhead = ndx;
		while (in[lookAhead] != '\r' && (in[lookAhead] != '\n')) {
			out += in[lookAhead];
			lookAhead++;
		}
		char save = in[lookAhead];
		in[lookAhead] = '\0';
		if (strcasestr(&in[ndx], "filename=")) {
			in[lookAhead] = save;
			ndx = lookAhead;
			while (strncmp(&in[ndx], "\r\n\r\n", 4)) {
				out += in[ndx++];
			}
			out += "\r\n";
			ndx += 4;
			// in[ndx] is start of binary data
			lookAhead = ndx;
			while (in[lookAhead] != '-' || strncmp(&in[lookAhead], b, bLength)) {
				lookAhead++;
			}
			// in[lookAhead] is start of boundary
			string base64 = Base64Encode((unsigned char *)&in[ndx], lookAhead-ndx-2);
			char buf[512];
			sprintf(buf, "Content-Length: %ld\r\n", lookAhead-ndx-2);
			out.append(buf, strlen(buf));
			strcpy(buf, "Content-Encoding: base64\r\n");
			out.append(buf, strlen(buf));
			out += "\r\n";
			out += base64;
			out += "\r\n";
			ndx = lookAhead;
		}
		else {
			in[lookAhead] = save;
			ndx = lookAhead;
		}
	}
	delete [] in;
	return scope.Close(String::New(out.c_str()));
}

void init_http_object() {
	HandleScope scope;
	
	Handle<ObjectTemplate>http  = ObjectTemplate::New();
	http->Set(String::New("openStream"), FunctionTemplate::New(OpenStream));
	http->Set(String::New("closeStream"), FunctionTemplate::New(CloseStream));
	http->Set(String::New("readByte"), FunctionTemplate::New(ReadByte));
	http->Set(String::New("readHeaders"), FunctionTemplate::New(ReadHeaders));
	http->Set(String::New("readPost"), FunctionTemplate::New(ReadPost));
	http->Set(String::New("readMime"), FunctionTemplate::New(ReadMime));
	
	globalObject->Set(String::New("http"), http);
}
