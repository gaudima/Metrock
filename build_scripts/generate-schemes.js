var minify = require('../src/pkjs/scheme-minifier');
var parse = require('../src/pkjs/xml-parser');
var msgPackSerialize = require('../src/pkjs/msgpack');
var fs = require('fs');
var https = require('https');

console.log('Downloading schemes and serializing them.');

var idToName = {
    '1': 'moscow',
    '2': 'spb',
    '8': 'kiev',
    '9': 'kharkiv',
    '13': 'minsk'
};

var link = 'https://metro.mobile.yandex.net/metro/get_file?file=scheme_';

for(var id in idToName) {
    if(idToName.hasOwnProperty(id)) {
        var url = link + id + '.xml&ver=2';
        console.log('downloading: ' + url);
        https.get(url, (function (idd, urll) {
            return function (ress) {
                var schemeXml;
                ress.setEncoding('utf8');
                ress.on('data', function (d) {
                    schemeXml += d;
                });
                ress.on('end', function () {
                    //console.log(JSON.stringify(minify(parse(schemeXml))));
                    var minified = minify(parse(schemeXml));
                    var serialized = msgPackSerialize(minified.scheme);
                    console.log('writing: resources/schemes/' + idToName[idd] + '.bin');
                    var ws = fs.createWriteStream('resources/schemes/' + idToName[idd] + '.bin');
                    ws.write(new Buffer(serialized));
                    ws.end();
                    console.log('writing: src/pkjs/schemes/' + idToName[idd] + '.json');
                    ws = fs.createWriteStream('src/pkjs/schemes/' + idToName[idd] + '.json');
                    ws.write('[');
                    for (var j = 0; j < serialized.length; j++) {
                        if (j % 24 == 0) {
                            ws.write('\n        ');
                        }
                        ws.write(serialized[j].toString());
                        if (j < serialized.length - 1) {
                            ws.write(',');
                        }
                    }
                    ws.write('\n]');
                    ws.end();

                    for(var loc in minified.names) {
                        if(minified.names.hasOwnProperty(loc)) {
                            var serializedNames = msgPackSerialize(minified.names[loc]);
                            console.log('writing: resources/schemes/locales/' + idToName[idd] + '_' + loc + '.bin');
                            var ws = fs.createWriteStream('resources/schemes/locales/' + idToName[idd] + '_' + loc + '.bin');
                            ws.write(new Buffer(serializedNames));
                            ws.end();
                            console.log('writing: src/pkjs/schemes/locales/' + idToName[idd] + '_' + loc + '.json');
                            ws = fs.createWriteStream('src/pkjs/schemes/locales/' + idToName[idd] + '_' + loc + '.json');
                            ws.write('[');
                            for (var j = 0; j < serializedNames.length; j++) {
                                if (j % 24 == 0) {
                                    ws.write('\n        ');
                                }
                                ws.write(serializedNames[j].toString());
                                if (j < serializedNames.length - 1) {
                                    ws.write(',');
                                }
                            }
                            ws.write('\n]');
                            ws.end();
                        }
                    }
                });
            }
        })(id, url));
    }
}