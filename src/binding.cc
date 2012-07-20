// Node
#include <node.h>
#include <v8.h>

// Parallel-Port
#include "ParallelPort.h"

using namespace v8;
using namespace node;



void init(Handle<Object> target) {
  //TODO
}

NODE_MODULE(parport, init);

