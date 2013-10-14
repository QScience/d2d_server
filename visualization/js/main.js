jQuery(document).ready(function() {
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
    var allUrls = [];
    var timeLineStaysOnMap = 10000;
    var colors = ['blue', 'red', 'green', 'yellow', 'purple', 'orange', 'pink'];
    var delayBeforeUnsuccessfullSearch = 2000;
    var delayBetween2DisplayedConnections = 2000;
    var FROM = 0; // Math.floor(((new Date()).getTime() - 1200000) / 1000); // Setup initial from: 20 minutes ago;
    var POST_URL = Drupal.settings.basePath + (Drupal.settings.clean_url === "1" ? '' : '?q=') + 'd2d_server/get_actions';

    /* Main function to be called */
    window.getNewConnections = function() {
        jQuery.ajax({
            url: POST_URL,
            data: {
                'from': FROM
            },
            type: 'POST',
            success: function(data) {
                FROM = Math.floor(((new Date).getTime()) / 1000);
                if (data.success) {
                    var iter, retrieveAndShowData;

                    if (data.actions.length === 0) {
                        setTimeout(getNewConnections, delayBeforeUnsuccessfullSearch);
                        return false;
                    }

                    for (iter in data.actions) {
                        setTimeout((function(i) {
                            return function() {
                                var cleanUrl1, cleanUrl2;
                                cleanUrl1 = cleanUrl(data.actions[i].from_url);
                                cleanUrl2 = cleanUrl(data.actions[i].to_url);
                                getUrlLocation(cleanUrl1, cleanUrl2, function(adr1, adr2) {
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
                                    showNewConnection(x, y, [cleanUrl1, cleanUrl2], data.actions[i]);
                                });
                            };
                        })(iter), iter * delayBetween2DisplayedConnections);
                    }
                    setTimeout(getNewConnections, iter * 2 * delayBetween2DisplayedConnections);
                } else {
                    console.log('An error has occurred: ' + data.message);
                }

            },
            error: function() {
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
        var d1, d2, exec;
        if (allUrls[url]) {
            d1 = allUrls[url];
            if (d2 && !exec) {
                exec = true;
                callback(d1, d2);
            }
        } else {
            jQuery.ajax({
                url: 'http://freegeoip.net/json/' + url,
                type: 'GET',
                crossDomain: true,
                success: function(data1) {
                    d1 = data1;
                    allUrls[url] = data1;
                    if (d2 && !exec) {
                        exec = true;
                        callback(d1, d2);
                    }
                },
                error: function(data1) {
                    d1 = {};
                    allUrls[url] = data1;
                    if (d2 && !exec) {
                        exec = true;
                        callback(d1, d2);
                    }
                }
            });
        }

        if (allUrls[url2]) {
            d2 = allUrls[url2];
            if (d1 && !exec) {
                exec = true;
                callback(d1, d2);
            }
        } else {
            jQuery.ajax({
                url: 'http://freegeoip.net/json/' + url2,
                type: 'GET',
                crossDomain: true,
                success: function(data2) {
                    d2 = data2;
                    allUrls[url2] = d2;
                    if (d1 && !exec) {
                        exec = true;
                        callback(d1, d2);
                    }
                },
                error: function(data2) {
                    d2 = {};
                    allUrls[url2] = d2;
                    if (d1 && !exec) {
                        exec = true;
                        callback(d1, d2);
                    }
                }
            });
        }
    }

    function showNewConnection(from, to, urls, action) {
        var lineId, colorId, infoWindow, infoWindow2, linkD2dId;
        from = new google.maps.LatLng(from[0], from[1]);
        to = new google.maps.LatLng(to[0], to[1]);
        action.created = new Date(+action.created*1000);
        lineId = lines.length;
        colorId = getRandomInt(0, colors.length - 1);
        lines[lineId] = {};
        linkD2dId = Drupal.settings.basePath + (Drupal.settings.clean_url === "1" ? '' : '?q=') + 'd2d/';
        infoWindow = new google.maps.InfoWindow({
            content: '<p style="max-width:200px;"><a href="' + linkD2dId + action.from_d2did + '" target="_blank">' + urls[0] + '</a> requested <i style="margin-right:3px;">' + action.action_type + '</i> on <a href="' + linkD2dId + action.to_d2did + '" target="_blank">' + urls[1] + '</a>.<br /><span style="font-size:10px;">(' + action.action_length + ' bits, ' + action.created.getDate() + "/" + (action.created.getMonth() + 1) + "/" + action.created.getFullYear() + " at " + action.created.getHours() + ":" + action.created.getMinutes() + ":" + action.created.getSeconds() + ')</span></p>'
        });
        infoWindow2 = new google.maps.InfoWindow({
            content: '<p style="max-width:200px;"><a href="' + linkD2dId + action.to_d2did + '" target="_blank">' + urls[1] + '</a> executed <i style="margin-right:3px;">' + action.action_type + '</i> for <a href="' + linkD2dId + action.from_d2did + '" target="_blank">' + urls[0] + '</a>.<br /><span style="font-size:10px;">(' + action.action_length + ' bits, ' + action.created.getDate() + "/" + (action.created.getMonth() + 1) + "/" + action.created.getFullYear() + " at " + action.created.getHours() + ":" + action.created.getMinutes() + ":" + action.created.getSeconds() + ')</span></p>'
        });
        lines[lineId].m = new google.maps.Marker({
            position: from,
            map: map,
            animation: google.maps.Animation.DROP,
            icon: 'http://maps.google.com/mapfiles/ms/icons/' + colors[colorId] + '-dot.png',
            title: urls[0]
        });
        google.maps.event.addListener(lines[lineId].m, 'click', function() {
            infoWindow.open(map, lines[lineId].m);
            lines[lineId].mOpen = true;
            google.maps.event.addListener(infoWindow, 'closeclick', function() {
                lines[lineId].mOpen = false;
            });
        });
        lines[lineId].p = new google.maps.Marker({
            position: to,
            map: map,
            animation: google.maps.Animation.DROP,
            icon: 'http://maps.google.com/mapfiles/ms/icons/' + colors[colorId] + '-dot.png',
            title: urls[1]
        });
        google.maps.event.addListener(lines[lineId].p, 'click', function() {
            infoWindow2.open(map, lines[lineId].p);
            lines[lineId].pOpen = true;
            google.maps.event.addListener(infoWindow2, 'closeclick', function() {
                lines[lineId].pOpen = false;
            });
        });
        setTimeout(function() {
            addArrow(from, to, lineId, colorId);
        }, 333);
    }

    function addArrow(from, to, lineId, colorId) {
        var lineCoordinates = [from, to];
        animateLine(lineCoordinates, 0, lineId, colorId);
    }

    function animateLine(coordinates, percent, lineId, colorId) {
        setTimeout(function() {
            if (percent >= 100) {
                setTimeout(function() {
                    if (!lines[lineId].mOpen) {
                        lines[lineId].m.setMap(null);
                    } else {
                        animateLine(coordinates, percent, lineId, colorId);
                    }
                    if (!lines[lineId].pOpen) {
                        lines[lineId].p.setMap(null);
                    } else {
                        animateLine(coordinates, percent, lineId, colorId);
                    }
                    lines[lineId].l.setMap(null);
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