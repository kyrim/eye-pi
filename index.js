"use strict";
const Koa = require("koa");
const Serve = require("koa-static");
const websockify = require("koa-websocket");
const Router = require("koa-router");
const path = require("path");
const through = require("through");
const worker = require("streaming-worker");

const addon_path = path.join(__dirname, "./build/Release/eye-pi");
const eye_pi = worker(addon_path);

const app = new Koa();
const sock = websockify(app);
const router = new Router();

const PORT = 3000;

app.use(Serve(__dirname + "/client"));

router.get("/stream", async ctx => {
  eye_pi.from.on("eyepi", function(value) {
    var json = JSON.parse(value);
    var frame = json.frame;
    ctx.websocket.send(frame);
  });
});

app.ws.use(router.routes()).use(router.allowedMethods());
app.use(router.routes());

console.log(`Starting on port: ${PORT}`);

app.listen(PORT);
