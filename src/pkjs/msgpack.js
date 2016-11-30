(function(root, factory) {
    if (typeof module !== 'undefined' && module.exports) module.exports = factory();
    else root.msgPackSerialize = factory();
})(this, function() {
    
    function msgPackSerialize(obj) {

        function serializeNumber(obj) {
            if(obj % 1 === 0) {
                //Integer
                var ints = [
                    [0, 127, 0x00, 0],
                    [-32, -1, 0xe0, 0],
                    [-128, 127, 0xd0, 1],
                    [-32768, 32767, 0xd1, 2],
                    [-2147483648, 2147483647, 0xd2, 4]];

                for(var i = 0; i < ints.length; i++) {
                    if(ints[i][0] <= obj && obj <= ints[i][1]) {
                        var ret = [];
                        if(i == 0) {
                            ret.push((ints[i][2] | obj) & 0xff);
                        } else if(i == 1) {
                            ret.push((ints[i][2] | -obj) & 0xff);
                        } else {
                            ret.push(ints[i][2]);
                            for(var j = ints[i][3] - 1; j >= 0 ; j--) {
                                ret.push((obj >>> (j * 8)) & 0xff);
                            }
                        }
                        return ret;
                    }
                }
            } else {
                //Float
                console.log('floats are unsupported');
            }
        }

        function serializeString(obj) {
            var utf8 = unescape(encodeURIComponent(obj));
            var ret = [];
            var strs = [
                [31, 0xa0, 0],
                [255, 0xd9, 1],
                [65535, 0xda, 2],
                [4294967295, 0xdb, 4]
            ];
            for(var j = 0; j < strs.length; j++) {
                if(utf8.length <= strs[j][0]) {
                    if(j == 0) {
                        ret.push(strs[j][1] | utf8.length);
                    } else {
                        ret.push(strs[j][1]);
                        for(var k = strs[j][2] - 1; k >= 0; k--) {
                            ret.push((utf8.length >>> (k * 8)) & 0xff);
                        }
                    }
                    for(var i = 0; i < utf8.length; i++) {
                        ret.push(utf8.charCodeAt(i));
                    }
                    return ret;
                }
            }
        }

        function serializeBoolean(obj) {
            if(obj) {
                return [0xc3];
            } else {
                return [0xc2]
            }
        }

        function serializeArray(obj) {
            var ret = [];
            var arrs = [
                [15, 0x90, 0],
                [65535, 0xdc, 2],
                [4294967295, 0xdd, 4]
            ];
            for(var j = 0; j < arrs.length; j++) {
                if(obj.length <= arrs[j][0]) {
                    if(j == 0) {
                        ret.push(arrs[j][1] | obj.length);
                    } else {
                        ret.push(arrs[j][1]);
                        for(var k = arrs[j][2] - 1; k >= 0; k--) {
                            ret.push((obj.length >>> (k * 8)) & 0xff);
                        }
                    }
                    for(var i = 0; i < obj.length; i++) {
                        ret = ret.concat(msgPackSerialize(obj[i]));
                    }
                    return ret;
                }
            }
        }

        function serializeNull(obj) {
            return [0xc0];
        }

        function serializeObject(obj) {
            var keysLength = 0;
            var prop;
            for(prop in obj) {
                if(obj.hasOwnProperty(prop)) {
                    if(obj[prop]) {
                        keysLength++;
                    }
                }
            }
            var ret = [];
            var objs = [
                [15, 0x80, 0],
                [65535, 0xde, 2],
                [4294967295, 0xdf, 4]
            ];
            for(var j = 0; j < objs.length; j++) {
                if(keysLength <= objs[j][0]) {
                    if(j == 0) {
                        ret.push(objs[j][1] | keysLength);
                    } else {
                        ret.push(objs[j][1]);
                        for(var k = objs[j][2] - 1; k >= 0; k--) {
                            ret.push((keysLength >>> (k * 8)) & 0xff);
                        }
                    }
                    for(prop in obj) {
                        if(obj.hasOwnProperty(prop)) {
                            if(obj[prop]) {
                                ret = ret.concat(msgPackSerialize(prop)).concat(msgPackSerialize(obj[prop]));
                            }
                        }
                    }
                    return ret;
                }
            }
        }

        var type = {
            'Number': serializeNumber,
            'String': serializeString,
            'Boolean': serializeBoolean,
            'Array': serializeArray,
            'Object': serializeObject,
            'Null' : serializeNull
        };

        var objType = Object.prototype.toString.call(obj).slice(8, -1);
        if(objType in type) {
            return type[objType](obj);
        } else {
            console.log(objType);
        }
    }
    
    return msgPackSerialize;
});
