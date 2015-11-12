import QtQuick 2.0

import QtLocation 5.3
import QtPositioning 5.2
import QtLocation.examples 5.0
import "content/map"
import "content/dialogs"


Map {
    id: map


    center {
        latitude: -27
        longitude: 153
    }
    zoomLevel: map.minimumZoomLevel

    gesture.enabled: true
}

