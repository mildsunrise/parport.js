/* -*- Mode: C; c-basic-offset: 2; tab-width: 2 -*- */
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
  ParportWrap(short portid = 0) : ptid(portid) {
    port.open(portid);
  }
  ~ParportWrap() {
    port.close();
  }

  static V8_CALLBACK(NewInstance, 0) {
    //Extract arguments
    short portid = 0;
    if (args.Length() >= 1)
      portid = args[0]->IntegerValue();
    
    ParportWrap* inst = new ParportWrap (portid);
    inst->Wrap(args.This());
    
    return scope.Close(args.This());
  } V8_WRAP_END()
  
  static V8_CALLBACK(ReadData, 0) {
    ParallelPort& port = ObjectWrap::Unwrap<ParportWrap>(args.This())->port;
    
    return scope.Close(Integer::New(port.readData()));
  } V8_WRAP_END()
  static V8_CALLBACK(ReadControl, 0) {
    ParallelPort& port = ObjectWrap::Unwrap<ParportWrap>(args.This())->port;
    
    return scope.Close(Integer::New(port.readControl()));
  } V8_WRAP_END()
  static V8_CALLBACK(ReadStatus, 0) {
    ParallelPort& port = ObjectWrap::Unwrap<ParportWrap>(args.This())->port;
    
    return scope.Close(Integer::New(port.readStatus()));
  } V8_WRAP_END()
  
  static V8_CALLBACK(WriteData, 1) {
    ParallelPort& port = ObjectWrap::Unwrap<ParportWrap>(args.This())->port;
    char data = args[0]->IntegerValue();
    port.writeData(data);
  } V8_WRAP_END()
  static V8_CALLBACK(WriteControl, 1) {
    ParallelPort& port = ObjectWrap::Unwrap<ParportWrap>(args.This())->port;
    char data = args[0]->IntegerValue();
    port.writeControl(data);
  } V8_WRAP_END()
  //static V8_CALLBACK(WriteStatus, 1) {
  //  ParallelPort& port = ObjectWrap::Unwrap<ParportWrap>(args.This())->port;
  //  char data = args[0]->IntegerValue();
  //  port.writeStatus(data);
  //} V8_WRAP_END()
  
  static V8_GETTER(GetPortId) {
    ParportWrap* inst = ObjectWrap::Unwrap<ParportWrap>(info.Holder());
    return scope.Close(Integer::New(inst->ptid));
  } V8_WRAP_END()
private:
  ParallelPort port;
  const short ptid;
};


/////////////////////////////////
// INTERFACE DECLARATION
/////////////////////////////////

Persistent<FunctionTemplate> initParport(Handle<Object> target) {
  HandleScope scope;
  
  Local<FunctionTemplate> protL = FunctionTemplate::New(ParportWrap::NewInstance);
  Persistent<FunctionTemplate> prot = Persistent<FunctionTemplate>::New(protL);
  prot->InstanceTemplate()->SetInternalFieldCount(1);
  prot->SetClassName(String::NewSymbol("Port"));

  NODE_SET_PROTOTYPE_METHOD(prot, "readData", ParportWrap::ReadData);
  NODE_SET_PROTOTYPE_METHOD(prot, "readStatus", ParportWrap::ReadStatus);
  NODE_SET_PROTOTYPE_METHOD(prot, "readControl", ParportWrap::ReadControl);
  NODE_SET_PROTOTYPE_METHOD(prot, "writeData", ParportWrap::WriteData);
  //NODE_SET_PROTOTYPE_METHOD(prot, "writeStatus", ParportWrap::WriteStatus);
  NODE_SET_PROTOTYPE_METHOD(prot, "writeControl", ParportWrap::WriteControl);

  prot->InstanceTemplate()->SetAccessor(String::NewSymbol("id"), ParportWrap::GetPortId);

  target->Set(String::NewSymbol("Port"), prot->GetFunction());
  return prot;
}

void init(Handle<Object> target) {
  HandleScope scope;
  
  Persistent<FunctionTemplate> parport = initParport(target);
}

NODE_MODULE(parport, init);
