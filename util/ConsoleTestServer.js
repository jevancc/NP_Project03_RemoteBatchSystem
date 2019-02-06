#!/usr/bin/env node
const fs = require("fs");
const net = require("net");
const argv = require("yargs")
  .usage("$0 <port>")
  .demand(1).argv;

const sleep = t => new Promise(r => setTimeout(() => r(), t));

let [Port] = argv._;

let server = net.createServer(function(socket) {
  console.log("Client connected");
  socket.write(`% `);

  socket.on("data", async function(data) {
    data = data.toString().trim();
    console.log("RECEIVED:", data);

    await sleep(2000);
    if (data === "exit") {
      socket.end();
    } else {
      socket.write(`************ RECEIVED ${data} *************\n`);
      socket.write(`% `);
    }
  });

  socket.on("end", function() {
    console.log("Client disconnected");
  });
});

server.listen(Port, function() {
  console.log(`Console test server is listening on port ${Port}`);
});
