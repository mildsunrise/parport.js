/* -*- Mode: CPP; c-basic-offset: 2; tab-width: 2 -*- */
/**
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
 **/

// Node, V8, V8U
#include "v8u.hpp"
#include "version.hpp"

// Parallel-Port
#include "ParallelPort.h"

namespace parport {

///////////////////
// PARPORT CLASS //
///////////////////

class ParportWrap: public node::ObjectWrap {
public:
  ParportWrap(short portid) : ptid(portid) {
    port.open(portid);
  }
  ~ParportWrap() {
    port.close();
  }

  //Constructor
  V8_CL_CTOR(ParportWrap) {
    short portid = 0;
    if (args.Length() >= 1)
      portid = v8u::Int(args[0]);
    
    inst = new ParportWrap(portid);
  } V8_CL_CTOR_END()

  //Read methods
  V8_CL_CALLBACK(ParportWrap, ReadData) {
    return scope.Close(v8u::Int(inst->port.readData()));
  } V8_CALLBACK_END()
  V8_CL_CALLBACK(ParportWrap, ReadControl) {
    return scope.Close(v8u::Int(inst->port.readControl()));
  } V8_CALLBACK_END()
  V8_CL_CALLBACK(ParportWrap, ReadStatus) {
    return scope.Close(v8u::Int(inst->port.readStatus()));
  } V8_CALLBACK_END()

  //Write methods
  V8_CL_CALLBACK(ParportWrap, WriteData) {
    v8u::CheckArguments(1, args);
    inst->port.writeData(v8u::Int(args[0]));
  } V8_CALLBACK_END()
  V8_CL_CALLBACK(ParportWrap, WriteControl) {
    v8u::CheckArguments(1, args);
    inst->port.writeControl(v8u::Int(args[0]));
  } V8_CALLBACK_END()
  //V8_CL_CALLBACK(ParportWrap, WriteStatus) {
  //  v8u::CheckArguments(1, args);
  //  inst->port.writeStatus(v8u::Int(args[0]));
  //} V8_CALLBACK_END()

  //Getters
  V8_CL_GETTER(ParportWrap, Id) {
    return scope.Close(v8u::Int(inst->ptid));
  } V8_GETTER_END()

  //TYPE DEFINITION
  NODE_DEF_TYPE("Port") {
    V8_DEF_METHOD(ReadData, "readData")
    V8_DEF_METHOD(ReadStatus, "readStatus")
    V8_DEF_METHOD(ReadControl, "readControl")
    V8_DEF_METHOD(WriteData, "writeData")
    //V8_DEF_METHOD(WriteStatus, "writeStatus")
    V8_DEF_METHOD(WriteControl, "writeControl")

    V8_DEF_RPROP(Id, "id")
  } NODE_DEF_TYPE_END()
private:
  ParallelPort port;
  const short ptid;
};


///////////////////////
// MODULE DEFINITION //
///////////////////////

NODE_DEF_MAIN() {
  // Version stuff
  v8u::Version::init(target);
  target->Set(v8u::Symbol("version"), (new v8u::Version(1,0,2))->Wrapped());

  // The Parport class
  ParportWrap::init(target);
} NODE_DEF_MAIN_END(parport)

};
