/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * parport.js
 * Copyright (C) 2012 Xavier Mendez <jmendeth@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <exception>

// Node
#include <node.h>
#include <v8.h>

// Parallel-Port
#include "ParallelPort.h"

using namespace std;
using namespace v8;
using namespace node;


/////////////////////////////////
// WRAPPING UTILITIES
/////////////////////////////////

// JS arguments

void CheckArguments(int min, const Arguments& args) {
    if (args.Length() < min)
        throw Exception::RangeError(String::New("Not enough arguments."));
}

// V8 exception wrapping

#define V8_WRAP_START()                                                        \
  HandleScope scope;                                                           \
  try {

#define V8_WRAP_END()                                                          \
    return scope.Close(Undefined());                                           \
  } catch (Handle<Value> err) {                                                \
    return ThrowException(err);                                                \
  } catch (exception err) {                                                    \
    return ThrowException(Exception::Error(String::New(err.what())));          \
  }                                                                            \
}

#define V8_WRAP_END_NR()                                                       \
  } catch (Handle<Value> err) {                                                \
    ThrowException(err);                                                       \
  } catch (exception err) {                                                    \
    ThrowException(Exception::Error(String::New(err.what())));                 \
  }                                                                            \
}

// V8 callback templates

#define V8_CALLBACK(IDENTIFIER, MIN)                                           \
Handle<Value> IDENTIFIER(const Arguments& args) {                              \
  V8_WRAP_START()                                                              \
    CheckArguments(MIN, args);

#define V8_GETTER(IDENTIFIER)                                                  \
Handle<Value> IDENTIFIER(Local<String> name, const AccessorInfo& info) {       \
  V8_WRAP_START()

#define V8_SETTER(IDENTIFIER)                                                  \
void IDENTIFIER(Local<String> name, Local<Value> value,                        \
                const AccessorInfo& info) {                                    \
  V8_WRAP_START()

// Dealing with V8 persistent handles

template <class T> void ClearPersistent(Persistent<T>& handle) {
    if (handle.IsEmpty()) return;
    handle.Dispose();
    handle.Clear();
}

template <class T> void SetPersistent(Persistent<T>& handle, Handle<T> value) {
    ClearPersistent<T>(handle);
    if (value.IsEmpty()) return;
    handle = Persistent<T>::New(value);
}


/////////////////////////////////
// ACTUAL WRAPPING
/////////////////////////////////

class ParportWrap: public ObjectWrap {
public:
  ParportWrap() {}
  ~ParportWrap() {}

  static V8_CALLBACK(NewInstance, 0) {
    (new ParportWrap)->Wrap(args.This());
    return scope.Close(args.This());
  } V8_WRAP_END()
  
  static V8_CALLBACK(Open, 0) {
    ParallelPort& port = ObjectWrap::Unwrap<ParportWrap>(args.This())->port;
    
    //Extract arguments
    short portid = 0;
    if (args.Length() >= 1)
      portid = args[0]->IntegerValue();
    
    port.open(portid);
  } V8_WRAP_END()
  
  
private:
  ParallelPort port;
};


/////////////////////////////////
// INTERFACE DECLARATION
/////////////////////////////////

void init(Handle<Object> target) {
  //TODO
}

NODE_MODULE(parport, init);
