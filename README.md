# Eye Pi
Eye Pi is a motion detection application written in C++ and Node.js for the [Raspberry Pi](https://www.raspberrypi.org/) with a camera attached.

### Current implementation
 - OpenCV detection of movement.
 - C++ to Node communication using [Node.js Native abstractions](https://github.com/nodejs/nan).
 - Uses [cmake-js](https://github.com/cmake-js/cmake-js) and [CMake](https://cmake.org/), rather than node-gyp for easier OpenCV compiling.
 - Streaming of camera frames to the browser using WebSockets.
 
### Todos
 - Use GridFS and MongoDB to store video and motion detection events.
 - Timeline view of motion detection events and scrubbing (in the browser).
 - Better motion detection implementation (currently very simple).

License
----

MIT