import QtQuick 2.0
import QtLocation 5.3
import QtPositioning 5.2
import QtLocation.examples 5.0
import "content/map"
import "content/dialogs"

Item {
    id: page
    width: parent ? parent.width : 360
    height: parent ? parent.height : 640
    property variant map
    property variant minimap
    property variant parameters
    property variant startC
    property variant endC
    property variant routePoint
    property variant routePoint2

    Rectangle {
        id: backgroundRect
        anchors.fill: parent
        color: "transparent"
        z:0
    }

    //=====================Menu=====================
    Menu {
        id:mainMenu
        anchors.bottom: parent.bottom
        z: backgroundRect.z + 0

        Component.onCompleted: {
            //addItem("Tools")
            //addItem("Map Type")
            //addItem("Provider")
            //createMap()


        }

     }



    Menu {
        id: providerMenu

        exclusive: true

        Component.onCompleted: {
            var plugins = getPlugins()
            for (var i = 0; i<plugins.length; i++)
                addItem(plugins[i])
        }

        onClicked: {
            page.state = ""
        }

        onExclusiveButtonChanged: createMap()
    }

    //=====================Dialogs=====================
    Message {
        id: messageDialog
        z: backgroundRect.z + 2
        onOkButtonClicked: {
            page.state = ""
        }
        onCancelButtonClicked: {
            page.state = ""
        }

        states: [
            State{
                name: "GeocodeError"
                PropertyChanges { target: messageDialog; title: "Geocode Error" }
                PropertyChanges { target: messageDialog; text: "No data available for the specified location" }
            },
            State{
                name: "UnknownGeocodeError"
                PropertyChanges { target: messageDialog; title: "Geocode Error" }
                PropertyChanges { target: messageDialog; text: "Unsuccessful geocode" }
            },
            State{
                name: "AmbiguousGeocode"
                PropertyChanges { target: messageDialog; title: "Ambiguous geocode" }
                PropertyChanges { target: messageDialog; text: map.geocodeModel.count + " results found for the given address, please specify location" }
            },
            State{
                name: "RouteError"
                PropertyChanges { target: messageDialog; title: "Route Error" }
                PropertyChanges { target: messageDialog; text: "Unable to find a route for the given points"}
            },
            State{
                name: "Coordinates"
                PropertyChanges { target: messageDialog; title: "Coordinates" }
            },
            State{
                name: "LocationInfo"
                PropertyChanges { target: messageDialog; title: "Location" }
                PropertyChanges { target: messageDialog; text: geocodeMessage() }
            },
            State{
                name: "Distance"
                PropertyChanges { target: messageDialog; title: "Distance" }
            }
        ]
    }

    //Route Dialog

    RouteDialog {
         id: routeDialog

         property variant startCoordinate
         property variant endCoordinate

 //! [routedialog0]
         Address { id: startAddress }
         Address { id: endAddress }

         z: backgroundRect.z + 2


         GeocodeModel {
             id: tempGeocodeModel

             property int success: 0

             onCountChanged: {
                 if (success == 1 && count == 1) {
                     query = endAddress
                     update();
                 }
             }

             onStatusChanged: {
                 if ((status == GeocodeModel.Ready) && (count == 1)) {
                     success++
                     if (success == 1){
                         startCoordinate.latitude = get(0).coordinate.latitude
                         startCoordinate.longitude = get(0).coordinate.longitude
                     }
                     if (success == 2) {
                         endCoordinate.latitude = get(0).coordinate.latitude
                         endCoordinate.longitude = get(0).coordinate.longitude
                         success = 0
                         routeDialog.calculateRoute()
                     }
                 }
                 else if ((status == GeocodeModel.Ready) || (status == GeocodeModel.Error)){
                     var st = (success == 0 ) ? "start" : "end"
                     messageDialog.state = ""
                     if ((status == GeocodeModel.Ready) && (count == 0 )) messageDialog.state = "UnknownGeocodeError"
                     else if (status == GeocodeModel.Error) {
                         messageDialog.state = "GeocodeError"
                         messageDialog.text = "Unable to find location for the " + st + " point"
                     }
                     else if ((status == GeocodeModel.Ready) && (count > 1 )){
                         messageDialog.state = "AmbiguousGeocode"
                         messageDialog.text = count + " results found for the " + st + " point, please specify location"
                     }
                     success = 0
                     page.state = "Message"
                     map.routeModel.clearAll()
                 }
             }
         }

         onGoButtonClicked: {
             tempGeocodeModel.reset()
             messageDialog.state = ""
             if (routeDialog.byCoordinates) {
                 startCoordinate = QtPositioning.coordinate(parseFloat(routeDialog.startLatitude),
                                                         parseFloat(routeDialog.startLongitude));
                 endCoordinate = QtPositioning.coordinate(parseFloat(routeDialog.endLatitude),
                                                       parseFloat(routeDialog.endLongitude));

                 calculateRoute()
             }
             else {
                 startAddress.country = routeDialog.startCountry
                 startAddress.street = routeDialog.startStreet
                 startAddress.city = routeDialog.startCity


                 endAddress.country = routeDialog.endCountry
                 endAddress.street = routeDialog.endStreet
                 endAddress.city = routeDialog.endCity

                 tempGeocodeModel.query = startAddress
                 tempGeocodeModel.update();
             }
             page.state = ""
         }

         onCancelButtonClicked: {
             page.state = ""
         }

 //! [routerequest0]
         function calculateRoute() {
             // clear away any old data in the query
             map.routeQuery.clearWaypoints();

             // add the start and end coords as waypoints on the route
             map.routeQuery.addWaypoint(startCoordinate)
             map.routeQuery.addWaypoint(endCoordinate)
             map.routeQuery.travelModes = routeDialog.travelMode
             map.routeQuery.routeOptimizations = routeDialog.routeOptimization
 //! [routerequest0]

 //! [routerequest0 feature weight]
             for (var i=0; i<9; i++) {
                 map.routeQuery.setFeatureWeight(i, 0)
             }

             for (var i=0; i<routeDialog.features.length; i++) {
                 map.routeQuery.setFeatureWeight(routeDialog.features[i], RouteQuery.AvoidFeatureWeight)
             }
 //! [routerequest0 feature weight]

 //! [routerequest1]
             map.routeModel.update();

             // center the map on the start coord
             map.center = startCoordinate;
 //! [routerequest1]
         }
 //! [routedialog1]
     }

    //Geocode Dialog
//! [geocode0]
    InputDialog {
        id: geocodeDialog
//! [geocode0]
        title: "Geocode"
        z: backgroundRect.z + 2

        Component.onCompleted: {
            var obj = [["Street", "Brandl St"],["City", "Eight Mile Plains"],["State", ""],["Country","Australia"], ["Postal code", ""]]
            setModel(obj)
        }

//! [geocode1]
        Address {
            id: geocodeAddress
        }

        onGoButtonClicked: {
            // manage the UI state transitions
            page.state = ""
            messageDialog.state = ""

            // fill out the Address element

            geocodeAddress.street = dialogModel.get(0).inputText
            geocodeAddress.city = dialogModel.get(1).inputText
            geocodeAddress.state = dialogModel.get(2).inputText
            geocodeAddress.country = dialogModel.get(3).inputText
            geocodeAddress.postalCode = dialogModel.get(4).inputText

            // send the geocode request
            map.geocodeModel.query = geocodeAddress
            map.geocodeModel.update()
        }
//! [geocode1]

        onCancelButtonClicked: {
            page.state = ""
        }
//! [geocode2]
    }
//! [geocode2]

    //Reverse Geocode Dialog
    InputDialog {
        id: reverseGeocodeDialog
        title: "Reverse Geocode"
        z: backgroundRect.z + 2

        Component.onCompleted: {
            var obj = [["Latitude","-27.575"],["Longitude", "153.088"]]
            setModel(obj)
        }

        onGoButtonClicked: {
            page.state = ""
            messageDialog.state = ""
            map.geocodeModel.query = QtPositioning.coordinate(parseFloat(dialogModel.get(0).inputText),
                                                           parseFloat(dialogModel.get(1).inputText));
            map.geocodeModel.update();
        }

        onCancelButtonClicked: {
            page.state = ""
        }
    }

    //Get new coordinates for marker
    InputDialog {
        id: coordinatesDialog


        Component.onCompleted: {
            var obj = [["Latitude", ""],["Longitude", ""]]
            setModel(obj)
        }

        onGoButtonClicked: {
            page.state = ""
            messageDialog.state = ""
            var newLat = parseFloat(dialogModel.get(0).inputText)
            var newLong = parseFloat(dialogModel.get(1).inputText)

            if (newLat !== "NaN" && newLong !== "NaN") {
                var c = QtPositioning.coordinate(newLat, newLong);
                if (c.isValid) {
                    map.markers[map.currentMarker].coordinate = c;
                    map.center = c;
                }
            }
        }

        onCancelButtonClicked: {
            page.state = ""
        }
    }



    function geocodeMessage(){
        var street, district, city, county, state, countryCode, country, postalCode, latitude, longitude, text
        latitude = Math.round(map.geocodeModel.get(0).coordinate.latitude * 10000) / 10000
        longitude =Math.round(map.geocodeModel.get(0).coordinate.longitude * 10000) / 10000
        street = map.geocodeModel.get(0).address.street
        district = map.geocodeModel.get(0).address.district
        city = map.geocodeModel.get(0).address.city
        county = map.geocodeModel.get(0).address.county
        state = map.geocodeModel.get(0).address.state
        countryCode = map.geocodeModel.get(0).address.countryCode
        country = map.geocodeModel.get(0).address.country
        postalCode = map.geocodeModel.get(0).address.postalCode

        text = "<b>Latitude:</b> " + latitude + "<br/>"
        text +="<b>Longitude:</b> " + longitude + "<br/>" + "<br/>"
        if (street) text +="<b>Street: </b>"+ street + " <br/>"
        if (district) text +="<b>District: </b>"+ district +" <br/>"
        if (city) text +="<b>City: </b>"+ city + " <br/>"
        if (county) text +="<b>County: </b>"+ county + " <br/>"
        if (state) text +="<b>State: </b>"+ state + " <br/>"
        if (countryCode) text +="<b>Country code: </b>"+ countryCode + " <br/>"
        if (country) text +="<b>Country: </b>"+ country + " <br/>"
        if (postalCode) text +="<b>PostalCode: </b>"+ postalCode + " <br/>"
        return text
    }



    function calcRoute() {
        // clear away any old data in the query
        map.routeQuery.clearWaypoints();



        // add the start and end coords as waypoints on the route
        map.routeQuery.addWaypoint(startC)

        map.routeQuery.addWaypoint(endC)

        map.routeQuery.travelModes = RouteQuery.CarTravel

        map.routeQuery.routeOptimizations = RouteQuery.ShortestRoute
/*
        for (var i=0; i<9; i++) {
            map.routeQuery.setFeatureWeight(i, 0)
        }

        for (var i=0; i<routeDialog.features.length; i++) {
            map.routeQuery.setFeatureWeight(routeDialog.features[i], RouteQuery.AvoidFeatureWeight)
        }
*/
        //map.center = startC
        //map.zoomLevel = 11

        map.routeModel.update();


//! [routerequest0]

//! [routerequest0 feature weight]
        /*
        for (var i=0; i<9; i++) {
            map.routeQuery.setFeatureWeight(i, 0)
        }

        for (var i=0; i<routeDialog.features.length; i++) {
            map.routeQuery.setFeatureWeight(routeDialog.features[i], RouteQuery.AvoidFeatureWeight)
        }*/


        // center the map on the start coord



//! [routerequest1]
    }

    function startRoute(x1, y1){
        routePoint = QtPositioning.coordinate(parseFloat(x1),
                                                        parseFloat(y1));
        //map.routeQuery.addWaypoint(routePoint)

        map.center = routePoint
        map.zoomLevel = 16
    }

    function addPoint(x1, y1){
        routePoint = QtPositioning.coordinate(parseFloat(x1),
                                                        parseFloat(y1));
        map.routeQuery.addWaypoint(routePoint)
/*
        map.geocodeModel.query = routePoint
        map.geocodeModel.update()
*/

    }

    function calcRoute2() {
        map.routeQuery.clearWaypoints();


        //console.log("point1: ", routePoint, " point2: ", routePoint2);
        for (var i=0; i<9; i++) {
            map.routeQuery.setFeatureWeight(i, 0)
        }


        map.routeQuery.travelModes = RouteQuery.CarTravel

        map.routeQuery.routeOptimizations = RouteQuery.ShortestRoute



        //map.center = startC
        //map.zoomLevel = 11

        map.routeModel.update();


    }


    function mapcenter(x1, y1){
        startC = QtPositioning.coordinate(parseFloat(x1),
                                                parseFloat(y1));
        map.center = startC
        map.zoomLevel = 16
        //map.update();
    }


    function createMap(){
        var plugin
        if (parameters.length>0)
            plugin = Qt.createQmlObject ('import QtLocation 5.3; Plugin{ name:"' + "osm" + '"; parameters: page.parameters}', page)
        else
            plugin = Qt.createQmlObject ('import QtLocation 5.3; Plugin{ name:"' + "osm" + '"}', page)
        if (plugin.supportsMapping()
                && plugin.supportsGeocoding(Plugin.ReverseGeocodingFeature)
                && plugin.supportsRouting()) {

            if (map) {
                map.destroy()
                minimap = null
            }

            map = Qt.createQmlObject ('import QtLocation 5.3;\
                                       import "content/map";\
                                       MapComponent{\
                                           z : backgroundRect.z + 1;\
                                           width: page.width;\
                                           height: page.height - mainMenu.height;\
                                           onFollowmeChanged: {toolsMenu.update()}\
                                           onSupportedMapTypesChanged: {mapTypeMenu.update()}\
                                           onCoordinatesCaptured: {\
                                               messageDialog.state = "Coordinates";\
                                               messageDialog.text = "<b>Latitude:</b> " + roundNumber(latitude,4) + "<br/><b>Longitude:</b> " + roundNumber(longitude,4);\
                                               page.state = "Message";\
                                           }\
                                           onGeocodeFinished:{\
                                               if (map.geocodeModel.status == GeocodeModel.Ready){\
                                                   if (map.geocodeModel.count == 0) {messageDialog.state = "UnknownGeocodeError";}\
                                                   else if (map.geocodeModel.count > 1) {messageDialog.state = "AmbiguousGeocode";}\
                                                   else {messageDialog.state = "LocationInfo";}\
                                               }\
                                               else if (map.geocodeModel.status == GeocodeModel.Error) {messageDialog.state = "GeocodeError";}\
                                               page.state = "Message";\
                                           }\
                                           onShowDistance:{\
                                               messageDialog.state = "Distance";\
                                               messageDialog.text = "<b>Distance:</b> " + distance;\
                                               page.state = "Message";\
                                           }\
                                           onMoveMarker: {\
                                               page.state = "Coordinates";\
                                           }\
                                           onRouteError: {\
                                               messageDialog.state = "RouteError";\
                                               page.state = "Message";\
                                           }\
                                           onRequestLocale:{\
                                               page.state = "Locale";\
                                           }\
                                           onShowGeocodeInfo:{\
                                               messageDialog.state = "LocationInfo";\
                                               page.state = "Message";\
                                           }\
                                           onResetState: {\
                                               page.state = "";\
                                           }\
                                       }',page)
            map.plugin = plugin;
            tempGeocodeModel.plugin = plugin;
            mapTypeMenu.update();
            toolsMenu.update();
        }
    }


    function changeCoorinates(x1, y1, x2, y2){
        startC = QtPositioning.coordinate(parseFloat(x1),
                                                parseFloat(y1));
        endC = QtPositioning.coordinate(parseFloat(x2),
                                              parseFloat(y2));


    }

    function changeEndCoorinate(x2, y2){

        endC = QtPositioning.coordinate(parseFloat(x2),
                                              parseFloat(y2));
    }

    function changeStartCoordinate(x1, y1){
        startC = QtPositioning.coordinate(parseFloat(x1),
                                                parseFloat(y1));
    }


    function getPlugins(){
        var plugin = Qt.createQmlObject ('import QtLocation 5.3; Plugin {}', page)
        var tempPlugin
        var myArray = new Array()
        for (var i = 0; i<plugin.availableServiceProviders.length; i++){
            tempPlugin = Qt.createQmlObject ('import QtLocation 5.3; Plugin {name: "' + plugin.availableServiceProviders[i]+ '"}', page)

            if (tempPlugin.supportsMapping()
                    && tempPlugin.supportsGeocoding(Plugin.ReverseGeocodingFeature)
                    && tempPlugin.supportsRouting()) {
                myArray.push(tempPlugin.name)
            }
        }

        return myArray
    }

    function setPluginParameters(pluginParameters) {
        var parameters = new Array()
        for (var prop in pluginParameters){
            var parameter = Qt.createQmlObject('import QtLocation 5.3; PluginParameter{ name: "'+ prop + '"; value: "' + pluginParameters[prop]+'"}',page)
            parameters.push(parameter)
        }
        page.parameters=parameters
        if (providerMenu.exclusiveButton !== "")
            createMap();
        else if (providerMenu.children.length > 0)
            createMap();
    }

    //=====================States of page=====================


}
