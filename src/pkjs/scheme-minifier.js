(function(root, factory) {
    if (typeof module !== 'undefined' && module.exports) module.exports = factory();
    else root.minifyScheme = factory();
})(this, function() {

    function findInside(d, tag) {
        for(var i = 0; i < d.inside.length; i++) {
            if(d.inside[i].tag == tag) {
                return d.inside[i];
            }
        }
    }

    function minifyData(data) {
        var names = 0;
        var SAFSNames = [];
        var id2ind = [];

        function minifyExits(d) {
            var ret = [];
            for(var i = 0; i < d.inside.length; i++) {
                ret.push(parseInt(d.inside[i].inside[0]));
            }
            return ret;
        }

        function minifyTransfer(d) {
            var ret = [];
            for(var i = 0; i < d.inside.length; i++) {
                if(d.inside[i].inside) {
                    var prev = d.inside[i].prevStation;
                    var to = d.inside[i].toStation;
                    var next = d.inside[i].nextStation;
                    ret.push([
                        (prev ? parseInt(prev) - 1 : -1),
                        (to ? parseInt(to) - 1 : -1),
                        (next ? parseInt(next) - 1 : -1),
                        minifyExits(d.inside[i])
                    ]);
                }
            }
            return ret;
        }

        function minifyBoardInfo(d) {
            var exit = findInside(d, 'exit');
            var transfer = findInside(d, 'transfer');
            return [
                (exit ? minifyExits(exit.inside[0]) : null),
                (transfer ? minifyTransfer(transfer) : null)
            ];
        }

        function minifyStation(d, lines, i) {
            var line = parseInt(d.inside[0].inside[0]) - 1;

            if(lines[line][2] == -1) {
                lines[line][2] = i;
            }

            lines[line][3]++;
            var nameTag = findInside(d, 'name');
            var name;
            if(nameTag.sameAsForStation) {
                name = -1;
                SAFSNames.push({'id': i, 'sameAs': parseInt(nameTag.sameAsForStation) - 1});
            } else {
                names++;
                name = names - 1;
            }
            var boardInfo = findInside(d, 'boardInfo');
            return [
                parseInt(d.id) - 1,
                line,
                name,
                (boardInfo ? minifyBoardInfo(boardInfo) : null)
            ];
        }

        function minifyStations(d, lines) {
            var ret = [];
            for(var i = 0; i < d.inside.length; i++) {
                ret.push(minifyStation(d.inside[i], lines, i));
            }
            id2ind.length = ret.length;
            for(var j = 0; j < ret.length; j++) {
                id2ind[ret[j][0]] = j;
            }
            SAFSNames.forEach(function(v, i, arr) {
                console.log(JSON.stringify(v));
                ret[v.id][2] = ret[id2ind[v.sameAs]][2];
            });
            return ret;
        }

        function minifyLinks(d, stationsLength) {
            var ret = [];
            var i;
            for(i = 0; i < stationsLength; i++) {
                ret.push([]);
            }
            for(i = 0; i < d.inside.length; i++) {
                var from = parseInt(d.inside[i].fromStation) - 1;
                var to = parseInt(d.inside[i].toStation) - 1;
                var weight = parseInt(d.inside[i].inside[1].inside[0].inside[0]);
                ret[from].push([
                    to,
                    weight
                ]);
                ret[to].push([
                    from,
                    weight
                ]);
            }
            return ret;
        }

        function minifyLines(d) {
            var ret = [];
            for(var i = 0; i < d.inside.length; i++) {
                var dd = d.inside[i];
                names++;
                ret.push([
                    names - 1,
                    parseInt(dd.inside[1].inside[0].slice(1), 16),
                    -1,
                    0
                ]);
            }
            return ret;
        }

        function minifyScheme(d) {
            var lines = minifyLines(findInside(d, 'lines'));
            var stations = minifyStations(findInside(d, 'stations'), lines);
            return [
                lines,
                stations,
                minifyLinks(findInside(d, 'links'), stations.length)
            ];
        }

        function minifySchemePack(d) {
            return [
                parseInt(d.id),
                parseInt(d.ver),
                minifyScheme(findInside(d, 'scheme')),
                id2ind
            ];
        }
        return minifySchemePack(data[1]);
    }

    function minifyNames(data, scheme) {
        var names = {};
        var locale = '';

        function minifySchemePack(d) {
            for(var i = 0; i < d.inside.length; i++) {
                var dd = d.inside[i];
                if(dd.tag = 'scheme') {
                    locale = dd.locale;
                    names[locale] = [];
                    for(var j = 0; j < scheme[2][0].length; j++) {
                        var lineName = findInside(findInside(dd, 'lines').inside[j], 'name').inside[0];
                        names[locale].push(lineName);
                    }
                    for(var j = 0; j < scheme[2][1].length; j++) {
                        var nameTagDefault = findInside(findInside(d.inside[0], 'stations').inside[j], 'name');
                        if(!nameTagDefault.sameAsForStation) {
                            var stationName = findInside(
                                findInside(
                                    findInside(dd, 'stations').inside[j],
                                    'name'),
                                "text").inside[0];
                            names[locale].push(stationName);
                        }
                    }
                }
            }
        }

        minifySchemePack(data[1]);
        return names;
    }

    function minify(data) {
        var scheme = minifyData(data);
        var names = minifyNames(data, scheme);
        return {'scheme': scheme, 'names': names};
    }
    return minify;
});