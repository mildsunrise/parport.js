// Node
#include <node.h>
#include <v8.h>

// Parallel-Port
#include "ParallelPort.h"

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

// TODO


/////////////////////////////////
// INTERFACE DECLARATION
/////////////////////////////////

void init(Handle<Object> target) {
  //TODO
}

NODE_MODULE(parport, init);
