# Eye Pi
Eye Pi is a motion detection application written in C++ and Node.js for the [Raspberry Pi](https://www.raspberrypi.org/) with a camera attached.

## Current implementation
 - OpenCV detection of movement.
 - C++ to Node communication using [Node.js Native abstractions](https://github.com/nodejs/nan).
 - Uses [cmake-js](https://github.com/cmake-js/cmake-js) and [CMake](https://cmake.org/), rather than node-gyp for easier OpenCV compiling.
 - Streaming of camera frames to the browser using WebSockets.

## Usage
Firstly, OpenCV needs to be installed, either built from source or retrieved. You can find OS specific installation instructions [here](https://docs.opencv.org/2.4/doc/tutorials/introduction/table_of_content_introduction/table_of_content_introduction.html)

Then: 
```
npm install
```
If no errors, then:
```
npm start
```
Open up localhost:3000, and view your camera streaming to the browser!

## Todos
 - Test on Raspberry Pi Zero
 - Use GridFS and MongoDB to store video and motion detection events.
 - Timeline view of motion detection events and scrubbing (in the browser).
 - Better motion detection implementation (currently very simple).

License
----

MIT