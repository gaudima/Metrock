(function(root, factory) {
    if (typeof module !== 'undefined' && module.exports) module.exports = factory();
    else root.parseXml = factory();
})(this, function() {

    function parse(resp) {
        var response = resp.replace(/<!--[\s\S]*?-->/g, '');
        var index = 0;
        var state = '';
        var token = '';

        function getNextChar() {
            if (index < response.length) {
                index += 1;
                return response[index - 1];
            } else {
                return false;
            }
        }

        function peekNextChar() {
            return response[index];
        }

        function skipWhitespace() {
            while (/\s/.test(peekNextChar())) {
                getNextChar();
            }
        }

        function getNext() {
            token = '';
            skipWhitespace();
            var ch = getNextChar();
            if (ch == false) {
                return false;
            }
            if (ch == '<') {
                var nch = peekNextChar();
                if (nch == '/') {
                    getNextChar();
                    state = 'ctag';
                } else if (nch == '?') {
                    getNextChar();
                    state = 'otag';
                } else {
                    state = 'otag';
                }
                while (!/\s|\?|\/|>/.test(peekNextChar())) {
                    token += getNextChar();
                }
                if (state == 'ctag') {
                    getNextChar();
                }
            } else if (ch == '"') {
                while (peekNextChar() != '"') {
                    token += getNextChar();
                }
                getNextChar();
                state = 'pval';
            } else if (ch == "'") {
                while (peekNextChar() != "'") {
                    token += getNextChar();
                }
                getNextChar();
                state = 'pval';
            } else if (ch == '/' || ch == '?') {
                if (peekNextChar() == '>') {
                    getNextChar();
                    state = "noctag";
                }
            } else {
                if (state == 'endotag') {
                    if (ch != '<') {
                        token += ch;
                        while (!/</.test(peekNextChar())) {
                            token += getNextChar();
                        }
                        state = 'text';
                    }
                } else if (state == 'otag' || state == 'xmltag' || state == 'pval') {
                    if (ch == '>') {
                        state = 'endotag';
                    } else {
                        token += ch;
                        while (!/=/.test(peekNextChar())) {
                            token += getNextChar();
                        }
                        getNextChar();
                        state = "pname";
                    }
                }
            }
            return true;
        }

        function parseTag() {
            var ret = {};
            if (state == 'otag') {
                ret.tag = token;
                getNext();

                var exClose = true;

                while (state != 'endotag' && state != 'noctag') {
                    if (state == 'pname') {
                        var pname = token;
                        getNext();
                        ret[pname] = token;
                    }
                    getNext();
                }

                if (state == 'noctag') {
                    exClose = false;
                }

                if (exClose) {
                    getNext();
                    ret.inside = parseTags(ret.tag);
                }
            }
            return ret;
        }

        function parseText() {
            var ret;
            if (state == 'text') {
                ret = token;
                getNext();
            } else {
                ret = parseTag();
                getNext();
            }
            return ret;
        }

        function parseTags(closing) {
            var ret = [];
            while (true) {
                if (state == 'ctag' && token == closing) {
                    break;
                }
                var r = parseText();
                ret.push(r);
            }
            return ret;
        }

        var ret = [];
        while (getNext()) {
            var tag = parseTag();
            if(Object.keys(tag).length > 0) {
                ret.push(tag);
            }
        }
        return ret;
    }

    return parse;
});