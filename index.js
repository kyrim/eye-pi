"use strict";
const Koa = require("koa");
const Serve = require("koa-static");
const IO = require("koa-socket");
const Router = require("koa-router");
var compress = require("koa-compress");
const path = require("path");
const stream = require("stream");
const through = require("through");
const worker = require("streaming-worker");
const ffmpeg = require("fluent-ffmpeg");

const addon_path = path.join(__dirname, "./build/Release/eye-pi");
const staticDirectory = path.join(__dirname, "./client");

const PORT = 3000;

const app = new Koa();
const router = new Router();
const eyePiSocket = new IO();
const eye_pi = worker(addon_path);

app.use(compress());
eyePiSocket.attach(app);
app.use(Serve(staticDirectory));

router.get("/stream", (ctx, next) => {
  ctx.set("Content-Type", "multipart/x-mixed-replace; boundary=theboundary"),
    ctx.set("Cache-Control", "no-cache"),
    ctx.set("Connection", "close"),
    ctx.set("Pragma", "no-cache");

  ctx.status = 200;

  eye_pi.from.stream().pipe(
    through(function(data) {
      const json = JSON.parse(data[1]);

      var base64Encodedmp4 = json.frame.data;
      var buf = Buffer.from(b64string, "base64");
    })
  );
});

app.use(router.routes());
app.use(router.allowedMethods());

eyePiSocket.on("connection", (ctx, data) => {
  console.log(`'${ctx.socket.id}' joined the stream`);
});

eyePiSocket.on("disconnect", ctx => {
  console.log(`'${ctx.socket.id}' left the stream`);
});

eye_pi.from.stream().pipe(
  through(data => {
    const json = JSON.parse(data[1]);
    //eyePiSocket.broadcast("newframe", json.frame);
  })
);

console.log(`Starting on port: ${PORT}`);
app.listen(PORT);
