#!/usr/bin/env node
const path = require("path");
const fs = require("fs");

const ConsoleCGIHTMLBuildPath = path.resolve("./build/web/index.html");

const buf = Uint8Array.from(fs.readFileSync(ConsoleCGIHTMLBuildPath));
process.stdout.write(`const std::vector<uint8_t> CONSOLE_CGI_HTML({`);
process.stdout.write(buf.join(","));
process.stdout.write(`});`);
