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
const staticDirectory = path.join(__dirname, "./client");

const PORT = 3000;

const app = new Koa();
const router = new Router();
const eyePiSocket = new IO();
const eye_pi = worker(addon_path);

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

eye_pi.from.stream().pipe(
  through(data => {
    const json = JSON.parse(data[1]);
    eyePiSocket.broadcast("newframe", json.frame);
  })
);

console.log(`Starting on port: ${PORT}`);
app.listen(PORT);
