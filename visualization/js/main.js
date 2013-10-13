jQuery(document).ready(function() {
////////////////////////////////////////////
//Activate Google Maps:
    var map;
    var mapOptions = {
        zoom: 3,
        center: new google.maps.LatLng(41.5,13.5),
        mapTypeId: google.maps.MapTypeId.ROADMAP
    };
    map = new google.maps.Map(document.getElementById('d2d-server-world-usage-map'), mapOptions);

////////////////////////////////////////////
    var FROM;

    var lines = [];
    var timeLineStaysOnMap = 10000;
    var colors = ['blue', 'red', 'green', 'yellow', 'purple', 'orange', 'pink'];
    var delayBeforeUnsuccessfullSearch = 2000;
    var delayBetween2DisplayedConnections = 2000;



//////////////////////////////////////////////////////////
//To Clean
//////////////////////////////////////////////////////////

var POST_URL, BASE_PATH, CLEAN_URL;
BASE_PATH = Drupal.settings.basePath;
CLEAN_URL = Drupal.settings.clean_url === "1";
POST_URL = BASE_PATH  + (CLEAN_URL ? '' : '?q=') + 'd2d_server/get_actions';

    // Setup initial from: 20 minutes ago;
    FROM = Math.floor(((new Date()).getTime() - 1200000) / 1000);

    window.getNewConnections = function() {

        jQuery.ajax({
            url: POST_URL,
            data: { 'from': FROM },
            type: 'POST',
            success: function(data) {
                ///////////////////////////////////////////////////////////////////////////
                //Testing:
                data = {
                    success: true,
                    actions: [
                    {from: 'tooski.ch', to: 'facebook.com'},
                    {from: 'facebook.com', to: 'tooski.ch'},
                    {from: 'microsoft.com', to: 'drupal.org'},
                    {from: 'asdf.com', to: 'facebook.com'},
                    ]
                };
                ///////////////////////////////////////////////////////////////////////////
                if (data.success) {
                    var iter;

                    if (data.actions.length === 0) {
                        setTimeout(getNewConnections, delayBeforeUnsuccessfullSearch);
                        return false;
                    }
                    for (iter in data.actions) {
                        setTimeout(function() {

                            getUrlLocation(data.actions[iter].from, data.actions[iter].to, function(adr1, adr2) {
                              var x,y;
                              x = [adr1.latitude, adr1.longitude];
                              y = [adr2.latitude, adr2.longitude];
                              showNewConnection(x, y);
                          });
                        }, iter*delayBetween2DisplayedConnections);
                    }
                    setTimeout(getNewConnections, (iter+1)*delayBetween2DisplayedConnections);
                    console.log(data);
                }
                else {
                    alert('An error has occurred: ' + data.message);
                }

            },
            error: function() {
                alert('Generic failure');
            }
        });
};

    //////////////////////////////////////////////////////////////////////////////////

    /*
     * solve adresse with IP
     */

     function getRandomInt(min, max) {
        return Math.floor(Math.random() * (max - min + 1) + min);
    }

    function getUrlLocation(url, url2, callback) {
        jQuery.getJSON('http://freegeoip.net/json/' + url, {}, function(data1) {
          jQuery.getJSON('http://freegeoip.net/json/' + url2, {}, function(data2) {
              callback(data1, data2);
          });
      });
    }

    function showNewConnection (from, to) {
        var lineId, colorId;
        from = new google.maps.LatLng(from[0],from[1]);
        to = new google.maps.LatLng(to[0],to[1]);
        lineId = lines.length;
        colorId = getRandomInt(0, colors.length-1);
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
                 lines[lineId].l.setMap(null);
                 lines[lineId].p.setMap(null);
                 lines[lineId].m.setMap(null);
             }, timeLineStaysOnMap);
              return false;
          }
          else {

              var line, newX, newY, newCoord;
              percent += 10;
              newX = coordinates[0].lb + (((coordinates[1].lb - coordinates[0].lb)) * percent/100);
              newY = coordinates[0].mb + (((coordinates[1].mb - coordinates[0].mb)) * percent/100);
              newCoord = new google.maps.LatLng(newX, newY);
              newCoord = [coordinates[0], newCoord];
              if (lines[lineId].l) {
                 lines[lineId].l.setMap(null);
             }
             lines[lineId].l = new google.maps.Polyline({
                 path: newCoord,
                 map: map,
                 strokeColor: colors[colorId],
                 strokeOpacity: '' + percent/100,
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
});