# Eye Pi

Eye Pi is a C++ and Node implementation of motion detection (using OpenCV) to be used on a RaspberryPi Zero with a camera, and streamed to a browser.

### Current implementation
 - OpenCV detection of movement (simple implementation for now)
 - C++ to Node communication using [Node.js Native abstractions](https://github.com/nodejs/nan)
 - Streaming to the browser using WebSockets
 
### Todos
 - Use GridFS and MongoDB to store video and motion detection events
 - Timeline view of motion detection events and scrubbing (in browser)
 - Better motion detection

License
----

MIT