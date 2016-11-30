var msgPackSerialize = require("../src/pkjs/msgpack");
var fs = require('fs');
var locales = {};

locales["ru_RU"] = require("./locales/ru_RU.json");
locales["en_US"] = require("./locales/en_US.json");

console.log("Generating locales.");

for(var k in locales) {
    console.log("generating: " + k);
    var ws = fs.createWriteStream('resources/locales/' + k + '.bin');
    ws.write(new Buffer(msgPackSerialize(locales[k])));
    ws.end();
}