var fossilDelta = require('./fossil-delta');
var minify = require('./scheme-minifier');
var parse = require('./xml-parser');
var msgPackSerialize = require('./msgpack');

var clay = require('pebble-clay');
var clayConfig = require('./config/config');

var keys = require('message_keys');

var clayInst = new clay(clayConfig, null, { autoHandleEvents: false });

Pebble.addEventListener('ready', function() {
    console.log('PebbleKit JS');
    // var moscow = require('./schemes/moscow');
    // var spb = require('./schemes/spb');
    // var kiev = require('./schemes/kiev');
    // var kharkiv = require('./schemes/kharkiv');
    // var minsk = require('./schemes/minsk');
    // var idToScheme = {
    //     '1': moscow,
    //     '2': spb,
    //     '8': kiev,
    //     '9': kharkiv,
    //     '13': minsk
    // };
    // var xhr = new XMLHttpRequest();
    // xhr.open('GET', 'https://metro.mobile.yandex.net/metro/get_file?file=scheme_1.xml');
    // xhr.addEventListener('load', function() {
    //     var minified = minify(parse(this.responseText));
    //     var serialized = msgPackSerialize(minified);
    //     var delta = fossilDelta.create(moscow, serialized);
    //     console.log(moscow.length);
    //     console.log(serialized.length);
    //     console.log(fossilDelta.outputSize(delta));
    //     console.log(delta.length);
    // });
    // xhr.send();
});

Pebble.addEventListener('showConfiguration', function(e) {
    Pebble.openURL(clayInst.generateUrl());
});

Pebble.addEventListener('webviewclosed', function(e) {
    if (e && !e.response) {
        return;
    }

    var dict = clayInst.getSettings(e.response);

    // Need this because clay doesnt support integer values in select items
    dict[keys.CITY_CHANGE] = parseInt(dict[keys.CITY_CHANGE]);
    dict[keys.LOCALE_CHANGE] = parseInt(dict[keys.LOCALE_CHANGE]);

    Pebble.sendAppMessage(dict, function(e) {
        console.log('Sent config data to Pebble');
    }, function(e) {
        console.log('Failed to send config data!');
        console.log(JSON.stringify(e));
    });
});