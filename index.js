const Koa = require("koa");
const Router = require("koa-router");

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
