var lines = [];
var timeLineStaysOnMap = 10000;
var colors = ['blue', 'red', 'green', 'yellow', 'purple', 'orange', 'pink'];
var googleGeocodingUrl = 'http://maps.google.com/maps/api/geocode/json?address=Bd+de+P%C3%83%C2%A9rolles+90,CH-1700+Fribourg+Switzerland&sensor=false';

/*
 * solve adresse with IP
 */

 function getRandomInt(min, max) {
 	return Math.floor(Math.random() * (max - min + 1) + min);
 }

 function getUrlLocation(url, url2 callback) {
 	jQuery.getJSON('http://freegeoip.net/jsonp/'+url, {}, function(data1) {
 		jQuery.getJSON('http://freegeoip.net/jsonp/'+url2, {}, function(data2) {
 		  callback(data1, data2);
 		});
 	});
 }

 function receiveEmit() {
 	// var x = [], y = [], min, max;
 	// min = -70;
 	// max = 70;
 	// x[0] = getRandomInt(min, max);
 	// x[1] = getRandomInt(min, max);
 	// y[0] = getRandomInt(min, max);
 	// y[1] = getRandomInt(min, max);


 	var url='http://www.tooski.ch';
 	var url2='http://www.facebook.ch';
 	getUrlLocation(url, url2, function(data1, data2) {
 		var x,y;
 		// Here you should fetch the coordinates and pass them to x and y as done before.
 		showNewConnection(x, y);
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