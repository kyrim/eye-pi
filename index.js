"use strict";
const Koa = require("koa");
const Serve = require("koa-static");
const IO = require("koa-socket");
const Router = require("koa-router");
var compress = require("koa-compress");
const path = require("path");
const through = require("through");
const worker = require("streaming-worker");

const addon_path = path.join(__dirname, "./build/Release/eye-pi");
const eye_pi = worker(addon_path);

const app = new Koa();
const router = new Router();
const eyePiSocket = new IO();

const PORT = 3000;
const staticDirectory = __dirname + "/client";

app.use(compress());
app.use(Serve(staticDirectory));
app.use(router.routes());
eyePiSocket.attach(app);

eyePiSocket.on("connection", (ctx, data) => {
  console.log(`'${ctx.socket.id}' joined the stream`);
});

eyePiSocket.on("disconnect", ctx => {
  console.log(`'${ctx.socket.id}' left the stream`);
});

eye_pi.from.on("newframe", function(value) {
  var json = JSON.parse(value);
  var frame = json.frame;
  eyePiSocket.broadcast("newframe", frame);
});

console.log(`Starting on port: ${PORT}`);
app.listen(PORT);
