/*
  ==============================================================================
   
   Copyright (c) 2014 Jacob Sologub
   
   Permission is hereby granted, free of charge, to any person obtaining a copy 
   of this software and associated documentation files (the "Software"), to deal 
   in the Software without restriction, including without limitation the rights 
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
   copies of the Software, and to permit persons to whom the Software is 
   furnished to do so, subject to the following conditions:
   
   The above copyright notice and this permission notice shall be included in 
   all copies or substantial portions of the Software.
   
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
   SOFTWARE.

  ==============================================================================
*/

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

Handle<Value> getType (const Arguments& args) {
    HandleScope scope;

    if (args.Length() < 1) {
        return ThrowException (Exception::TypeError (String::New ("Wrong number of arguments.")));
    }

    if (!args [0]->IsObject()) {
        return ThrowException (Exception::TypeError (String::New ("First argument must be be a buffer.")));
    }

    Local<Object> buffer = args [0]->ToObject();
    const size_t size = Buffer::Length (buffer);
    const char* data = Buffer::Data (buffer);

    Magick::Blob blob (data, size);
    Magick::Image image;
    image.read (blob);
    string type = image.magick().c_str();

    const unsigned argc = 2;
    Local<Value> argv [argc] = {
        Local<Value>::New (Null()),
        Local<Value>::New (String::New (type.c_str()))
    };

    Local<Function> callback = Local<Function>::Cast (args [1]);
    callback->Call (Context::GetCurrent()->Global(), argc, argv);

    return Undefined();
}

void RegisterModule (v8::Handle<v8::Object> target) {
     target->Set (String::NewSymbol ("convert"), FunctionTemplate::New (convert)->GetFunction());
     target->Set (String::NewSymbol ("getType"), FunctionTemplate::New (getType)->GetFunction());
}

NODE_MODULE (gif2jpg, RegisterModule);
