# Parport.JS

> **Note:** not finished yet. I'll have it in these weeks. Stay tuned!

Parport.js is a Node.JS addon you can use to **access,
control and communicate with parallel ports**.  
It uses the C++/Java library [Parallel-Port](http://parallel-port.googlecode.com) to
provide a **high-level** and **cross-platform**[<sup>✝</sup>](#compatibility) interface.

## Usage

TODO

## Compatibility

**Important:** these are only the bindings to Node.JS. The common interface
to the platform-dependent functions is in Parallel-Port.

Parallel-Port currently works on the following platforms:

 * **Linux**: through the [parport driver](http://cyberelk.net/tim/parport/parport.html) (usually comes with kernel, see `/dev/parport0`).

 * **Windows**: through the [Inpout32](http://logix4u.net/component/content/article/14-parallel-port/16-inpout32dll-for-windows-982000ntxp) library (free for non-commercial use only).  
   It seems it doesn't work on 64-bit Windows, though.

Platforms with no (but planned) support:

 * **Darwin** (that is, Mac OSX and iOS) not supported.
