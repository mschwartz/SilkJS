#include "SilkJS.h"
#include <curl/curl.h>

struct CURL_WRAPPER {
	char *memory;
	size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	struct CURL_WRAPPER *w = (CURL_WRAPPER *)userp;
	w->memory = (char *)realloc(w->memory, w->size+realsize+1);
	if (w->memory == NULL) {
		return -1;
	}
	memcpy(&w->memory[w->size], contents, realsize);
	w->size += realsize;
	w->memory[w->size] = '\0';
	return realsize;
}

static JSVAL request(JSARGS args) {
	HandleScope scope;
	Handle<Object>o = args[0]->ToObject();
	String::Utf8Value method(o->Get(String::New("method"))->ToString());
	String::Utf8Value url(o->Get(String::New("url"))->ToString());
	CURLcode code;
	CURL *curl = curl_easy_init();
	if (!curl) {
		return scope.Close(String::New("Could not initialize CURL"));
	}
	struct CURL_WRAPPER *w = new CURL_WRAPPER;
	w->memory = (char *)malloc(1);
	w->size = 0;
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_URL, *url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)w);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "SilkJS/1.0");
	if (!strcasecmp(*method, "POST")) {
		String::Utf8Value data(o->Get(String::New("data")));
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, *data);
	}
	code = curl_easy_perform(curl);
	if (code != CURLE_OK) {
		return scope.Close(String::New(curl_easy_strerror(code)));
	}
	
	// get returned info
	long status;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);
	o->Set(String::New("status"), Integer::New(status));
	char *contentType;
	curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType);
	o->Set(String::New("contentType"), String::New(contentType));
	o->Set(String::New("responseText"), String::New(w->memory, w->size));
	curl_easy_cleanup(curl);
	free(w->memory);
	free(w);
	return scope.Close(o);
}

void init_xhrHelper_object() {
	HandleScope scope;

	JSOBJT o = ObjectTemplate::New();
	o->Set(String::New("request"), FunctionTemplate::New(request));
	globalObject->Set(String::New("xhrHelper"), o);
}
