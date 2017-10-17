"use strict";
const path = require("path");
const Koa = require("koa");
const Router = require("koa-router");

const worker = require("streaming-worker");
const addon_path = path.join(__dirname, "./build/Release/eye-pi");
const eye_pi = worker(addon_path);

eye_pi.from.on("integer", function(value) {
  console.log(value);
});

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

const koa = new Koa();
const app = new Router();

const PORT = 3000;

// response
app.get("/", async ctx => {
  ctx.body = "Welcome to eye-pi";
});

koa.use(app.routes());

console.log(`Starting on port: ${PORT}`);

koa.listen(PORT);
