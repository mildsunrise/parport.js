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
#define PARPORT_VERSION 1,1,1  //comma-separated

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
  V8_CTOR(ParportWrap) {
    short portid = 0;
    if (args.Length() >= 1)
      portid = v8u::Int(args[0]);
    
    V8_WRAP(new ParportWrap(portid));
  } V8_CTOR_END()

  //Read methods
  static V8_CB(ReadData) {
    ParportWrap* inst = Unwrap(args.This());
    V8_RET(v8u::Int(inst->port.readData()));
  } V8_CB_END()
  static V8_CB(ReadControl) {
    ParportWrap* inst = Unwrap(args.This());
    V8_RET(v8u::Int(inst->port.readControl()));
  } V8_CB_END()
  static V8_CB(ReadStatus) {
    ParportWrap* inst = Unwrap(args.This());
    V8_RET(v8u::Int(inst->port.readStatus()));
  } V8_CB_END()

  //Write methods
  static V8_CB(WriteData) {
    ParportWrap* inst = Unwrap(args.This());
    inst->port.writeData(v8u::Int(args[0]));
    V8_RET(v8::Undefined());
  } V8_CB_END()
  static V8_CB(WriteControl) {
    ParportWrap* inst = Unwrap(args.This());
    inst->port.writeControl(v8u::Int(args[0]));
    V8_RET(v8::Undefined());
  } V8_CB_END()
  //static V8_CB(WriteStatus) {
  //  ParportWrap* inst = Unwrap(args.This());
  //  inst->port.writeStatus(v8u::Int(args[0]));
  //  V8_RET(v8::Undefined());
  //} V8_CB_END()

  //Getters
  V8_GET(GetId) {
    ParportWrap* inst = Unwrap(info.Holder());
    V8_RET(v8u::Int(inst->ptid));
  } V8_GET_END()

  //TYPE DEFINITION
  NODE_TYPE(ParportWrap, "Port") {
    V8_DEF_CB("readData", ReadData);
    V8_DEF_CB("readStatus", ReadStatus);
    V8_DEF_CB("readControl", ReadControl);
    V8_DEF_CB("writeData", WriteData);
    //V8_DEF_CB("writeStatus", WriteStatus);
    V8_DEF_CB("writeControl", WriteControl);

    V8_DEF_GET("id", GetId);
  } NODE_TYPE_END()
private:
  ParallelPort port;
  const short ptid;
};
V8_POST_TYPE(ParportWrap);

///////////////////////
// MODULE DEFINITION //
///////////////////////

NODE_DEF_MAIN() {
  // Version stuff
  v8u::Version::init(target);
  target->Set(v8u::Symbol("version"), (new v8u::Version(PARPORT_VERSION))->Wrapped());

  // The Parport class
  ParportWrap::init(target);
} NODE_DEF_MAIN_END(parport)

};
