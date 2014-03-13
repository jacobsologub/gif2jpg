#include <Magick++.h>
#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <string.h>

using namespace std;
using namespace v8;
using namespace node;

Handle<Value> convert (const Arguments& args) {
    HandleScope scope;

    if (args.Length() < 2) {
        return ThrowException (Exception::TypeError (String::New ("Wrong number of arguments.")));
    }

    if (!args [0]->IsObject()) {
        return ThrowException (Exception::TypeError (String::New ("First argument must be be a buffer.")));
    }

    if (!args [1]->IsFunction()) {
        return ThrowException (Exception::TypeError (String::New ("Second argument must be a callback function.")));
    }

    Local<Object> buffer = args [0]->ToObject();
    const size_t size = Buffer::Length (buffer);
  	const char* data = Buffer::Data (buffer);

  	Magick::Blob blob (data, size);
  	list <Magick::Image> imageList;
    Magick::readImages (&imageList, blob);
    Handle<Array> bufferArray = v8::Array::New (imageList.size());
    Local<Object> globalObj = v8::Context::GetCurrent()->Global();

    for (list <Magick::Image>::iterator it = imageList.begin(); it != imageList.end(); ++it) {
    	Magick::Blob blob;
    	it->write (&blob, "jpeg");

    	Buffer* slowBuffer = node::Buffer::New (blob.length());
    	memcpy (node::Buffer::Data (slowBuffer), blob.data(), blob.length());
    	Local<Function> bufferConstructor = Local<Function>::Cast (globalObj->Get (String::New ("Buffer")));

    	Handle<Value> constructorArgs [3] = { 
    		slowBuffer->handle_, 
    		Integer::New (blob.length()), 
    		Integer::New (0) 
    	};

    	const int index = distance (imageList.begin(), it);
    	Local<Object> actualBuffer = bufferConstructor->NewInstance (3, constructorArgs);
    	bufferArray->Set (index, actualBuffer);
    }

	const unsigned argc = 2;
    Local<Value> argv [argc] = {
        Local<Value>::New (Null()),
        Local<Value>::New (bufferArray)
    };

    Local<Function> callback = Local<Function>::Cast (args [1]);
    callback->Call (Context::GetCurrent()->Global(), argc, argv);

	return Undefined();
}

void RegisterModule (v8::Handle<v8::Object> target) {
     target->Set (String::NewSymbol ("convert"), FunctionTemplate::New (convert)->GetFunction());
}

NODE_MODULE (gif2jpg, RegisterModule);
