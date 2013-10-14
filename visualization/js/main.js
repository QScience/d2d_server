jQuery(document).ready(function () {
    /* Activating Google Maps */
    var map;
    var mapOptions = {
        zoom: 2,
        center: new google.maps.LatLng(41.5, 13.5),
        mapTypeId: google.maps.MapTypeId.ROADMAP
    };
    map = new google.maps.Map(document.getElementById('d2d-server-world-usage-map'), mapOptions);

    /* Condifuration */
    var lines = [];
    var timeLineStaysOnMap = 10000;
    var colors = ['blue', 'red', 'green', 'yellow', 'purple', 'orange', 'pink'];
    var delayBeforeUnsuccessfullSearch = 1000;
    var delayBetween2DisplayedConnections = 1000;
    var FROM = 0; // Math.floor(((new Date()).getTime() - 1200000) / 1000); // Setup initial from: 20 minutes ago;
    var POST_URL = Drupal.settings.basePath + (Drupal.settings.clean_url === "1" ? '' : '?q=') + 'd2d_server/get_actions';

    /* Main function to be called */
    window.getNewConnections = function () {
        jQuery.ajax({
            url: POST_URL,
            data: {
                'from': FROM
            },
            type: 'POST',
            success: function (data) {
                if (data.success) {
                    var iter, retrieveAndShowData;

                    if (data.actions.length === 0) {
                        // setTimeout(getNewConnections, delayBeforeUnsuccessfullSearch);
                        return false;
                    }

                    for (iter in data.actions) {
                        setTimeout((function (i) {
                            return function () {
                                var cleanUrl1, cleanUrl2;
                                cleanUrl1 = cleanUrl(data.actions[i].from_url);
                                cleanUrl2 = cleanUrl(data.actions[i].to_url);
                                getUrlLocation(cleanUrl1, cleanUrl2, function (adr1, adr2) {
                                    var x, y;
                                    x = [adr1.latitude, adr1.longitude];
                                    y = [adr2.latitude, adr2.longitude];
                                    if (x[0] === undefined || x[1] === undefined || (x[0] === 0 && x[1] === 0)) {
                                        x[0] = getRandomInt(-70, 70);
                                        x[1] = getRandomInt(-70, 70);
                                    }
                                    if (y[0] === undefined || y[1] === undefined || (y[0] === 0 && y[1] === 0)) {
                                        y[0] = getRandomInt(-70, 70);
                                        y[1] = getRandomInt(-70, 70);
                                    }
                                    showNewConnection(x, y);
                                });
                            };
                        })(iter), iter * delayBetween2DisplayedConnections);
}

                    //setTimeout(getNewConnections, (iter) * delayBetween2DisplayedConnections);
                } else {
                    console.log('An error has occurred: ' + data.message);
                }

            },
            error: function () {
                console.log('Generic failure');
            }
        });
};

function cleanUrl(url) {
    var index;
    url = url.replace(/http:\/\//, '');
    index = url.indexOf('/') === -1 ? url.length : url.indexOf('/');
    url = url.substring(0, index);
    index = url.indexOf(':') === -1 ? url.length : url.indexOf('/');
    url = url.substring(0, index);
    return url;
}

function getRandomInt(min, max) {
    return Math.floor(Math.random() * (max - min + 1) + min);
}

    /*
     * Problem: When doing a cross-domain request, can't handle a failure.
     * -> Connection not displayed if freegeoip returns a not found.
     */
     function getUrlLocation(url, url2, callback) {
        var d1, d2, exec, timeoutId;
        // create a timeOut here.
        // see http://stackoverflow.com/questions/1434519/cancel-a-jquery-ajax-call-before-it-returns
        jQuery.ajax({
            url: 'http://freegeoip.net/json/' + url,
            type: 'GET',
            crossDomain: true,
            success: function (data1) {
                d1 = data1;
                if (d2 && !exec) {
                    exec = true;
                    callback(d1, d2);
                }
            },
            error: function (data1) {
                d1 = {};
                if (d2 && !exec) {
                    exec = true;
                    callback(d1, d2);
                }
            }
        });

        jQuery.ajax({
            url: 'http://freegeoip.net/json/' + url2,
            type: 'GET',
            crossDomain: true,
            success: function (data2) {
                d2 = data2;
                if (d1 && !exec) {
                    exec = true;
                    callback(d1, d2);
                }
            },
            error: function (data2) {
                d2 = {};
                if (d1 && !exec) {
                    exec = true;
                    callback(d1, d2);
                }
            }
        });
    }

    function showNewConnection(from, to) {
        var lineId, colorId;
        from = new google.maps.LatLng(from[0], from[1]);
        to = new google.maps.LatLng(to[0], to[1]);
        lineId = lines.length;
        colorId = getRandomInt(0, colors.length - 1);
        lines[lineId] = {};
        lines[lineId].m = new google.maps.Marker({
            position: from,
            map: map,
            title: 'From',
            animation: google.maps.Animation.DROP,
            icon: 'http://maps.google.com/mapfiles/ms/icons/' + colors[colorId] + '-dot.png'
        });
        lines[lineId].p = new google.maps.Marker({
            position: to,
            map: map,
            title: 'To',
            animation: google.maps.Animation.DROP,
            icon: 'http://maps.google.com/mapfiles/ms/icons/' + colors[colorId] + '-dot.png'
        });
        setTimeout(function () {
            addArrow(from, to, lineId, colorId);
        }, 333);
    }

    function addArrow(from, to, lineId, colorId) {
        var lineCoordinates = [from, to];
        animateLine(lineCoordinates, 0, lineId, colorId);
    }

    function animateLine(coordinates, percent, lineId, colorId) {
        setTimeout(function () {
            if (percent >= 100) {
                setTimeout(function () {
                    lines[lineId].l.setMap(null);
                    lines[lineId].p.setMap(null);
                    lines[lineId].m.setMap(null);
                }, timeLineStaysOnMap);
                return false;
            } else {
                var line, newX, newY, newCoord;
                percent += 10;
                newX = coordinates[0].lb + (((coordinates[1].lb - coordinates[0].lb)) * percent / 100);
                newY = coordinates[0].mb + (((coordinates[1].mb - coordinates[0].mb)) * percent / 100);
                newCoord = new google.maps.LatLng(newX, newY);
                newCoord = [coordinates[0], newCoord];
                if (lines[lineId].l) {
                    lines[lineId].l.setMap(null);
                }
                lines[lineId].l = new google.maps.Polyline({
                    path: newCoord,
                    map: map,
                    strokeColor: colors[colorId],
                    strokeOpacity: '' + percent / 100,
                    strokeWeight: '2',
                    geodesic: true,
                    icons: [{
                        icon: {
                            path: google.maps.SymbolPath.FORWARD_OPEN_ARROW,
                        },
                        offset: '100%',
                    }],
                });

                animateLine(coordinates, percent, lineId, colorId);
            }
        }, 100);
}

window.getNewConnections();
});