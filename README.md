<p align="center">
    <img src="https://raw.githubusercontent.com/kyrim/eye-pi/master/client/img/favicon-96x96.png" alt="" width=96 height=96>

  <h3 align="center">Eye Pi</h3>

  <p align="center">
	Eye Pi is a motion detection application written in C++ and Node.js for the <a href="https://www.raspberrypi.org/">Raspberry Pi</a> with a camera attached.
</p>

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
Open up http://localhost:3000, and view your camera streaming to the browser!

## Todos
 - Test on Raspberry Pi Zero.
 - Use faster and more efficient serialisation and deserialisation from C++ to Node (currently using JSON with Base64 encoding for the data, which is slow).
 - Use GridFS and MongoDB to store video and motion detection events.
 - Timeline view of motion detection events and scrubbing (in the browser).
 - Better motion detection implementation (currently very simple).

License
----

MIT