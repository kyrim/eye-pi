"use strict";
const path = require("path");
const Koa = require("koa");
const websockify = require("koa-websocket");

const Serve = require("koa-static");
const Router = require("koa-router");
const through = require("through");
var Stream = require("stream");

const worker = require("streaming-worker");
const addon_path = path.join(__dirname, "./build/Release/eye-pi");
const eye_pi = worker(addon_path);

// eye_pi.from.on("eyepi", function(value) {
//   console.log(value);
// });

// const through = require("through");
// var output = eye_pi.from.stream();
// output
//   .pipe(
//     through(function(data) {
//       // the data coming in is an array,
//       // Element 0 is the name of the event emitted by the addon ("integer")
//       // Element 1 is the data - which in this case is just the integer
//       this.queue(data[1]);
//     })
//   )
//   .pipe(process.stdout);

const app = new Koa();
let sock = websockify(app);

const router = new Router();

app.use(Serve(__dirname + "/client"));

const PORT = 3000;

// response
// app.get("/", async ctx => {
//   ctx.body = "Welcome to eye-pi";
// });

router.get("/stream", async ctx => {
  eye_pi.from.on("eyepi", function(value) {
    var json = JSON.parse(value);
    var frame = json.frame;
    ctx.websocket.send(frame);
  });
});

// app.ws.use(
//   route.all("/stream", function(ctx) {
//     // `ctx` is the regular koa context created from the `ws` onConnection `socket.upgradeReq` object.
//     // the websocket is added to the context on `ctx.websocket`.
//     ctx.websocket.send("Hello World");
//     ctx.websocket.on("message", function(message) {
//       // do something with the message from client
//       console.log(message);
//     });
//   })
// );

// router.get("/stream", async ctx => {
//   var stream = (this.body = new Stream.PassThrough());

//   eye_pi.from
//     .stream()
//     .pipe(
//       through(function(data) {
//         var json = JSON.parse(data[1]);
//         var frame = json.frame;
//         var returnString = `Content-Type: image/jpeg\n Content-Length: ${frame.length}\n\n${frame}\n--${boundary}\n`;
//         this.queue(returnString);
//       })
//     )
//     .pipe(stream);

//   ctx.body = stream;
// });

// router.use(function*(next) {
//   if (this.path !== "/stream") return yield next;

//   this.set("Content-Type", "text/event-stream");
//   this.set("Cache-Control", "max-age=0, no-cache");
//   this.set("Connection", "keep-alive");

//   var stream = (this.body = new Stream.PassThrough());

//   snapshots.pipe(stream);

//   this.req.socket.once("close", function() {
//     snapshots.unpipe(stream);
//   });
// });

// router.use(function*(next) {
//   if (this.path !== "/") return yield next;

//   this.body = template.replace(
//     "{{src}}",
//     lastSnapshot ? "data:image/jpeg;base64," + lastSnapshot : ""
//   );
// });

app.ws.use(router.routes()).use(router.allowedMethods());

//app.use(router.routes());

console.log(`Starting on port: ${PORT}`);

app.listen(PORT);
