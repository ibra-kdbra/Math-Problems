import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Pane {
    id: root
    padding: 24

    property var currentResult: null

    Material.background: Material.color(Material.Grey, Material.Shade50)

    ColumnLayout {
        anchors.fill: parent
        spacing: 16

        // Tabs
        TabBar {
            id: resultTabBar
            Layout.fillWidth: true
            Material.background: "transparent"
            Material.accent: Material.primary
            
            TabButton { text: "Result" }
            TabButton { text: "Steps" }
            TabButton { text: "Explanation" }
            TabButton { text: "Visualization" }
        }

        // Content
        StackLayout {
            currentIndex: resultTabBar.currentIndex
            Layout.fillWidth: true
            Layout.fillHeight: true

            // 1. Result Tab
            ScrollView {
                clip: true
                
                ColumnLayout {
                    width: parent.width
                    spacing: 16

                    // Welcome/Empty State
                    Rectangle {
                        visible: root.currentResult === null
                        Layout.fillWidth: true
                        height: 200
                        color: "transparent"
                        
                        Label {
                            anchors.centerIn: parent
                            text: "Select an algorithm and click Compute\nto see results here."
                            horizontalAlignment: Text.AlignHCenter
                            color: Material.color(Material.Grey, Material.Shade500)
                            font.pixelSize: 16
                        }
                    }

                    // Result Cards Repeater
                    Repeater {
                        model: resultCardModel
                        delegate: Rectangle {
                            Layout.fillWidth: true
                            height: childrenRect.height + 32
                            color: "white"
                            radius: 12
                            border.color: Material.color(Material.Grey, Material.Shade200)
                            border.width: 1

                            ColumnLayout {
                                width: parent.width
                                anchors.top: parent.top
                                anchors.margins: 16
                                spacing: 12

                                RowLayout {
                                    Layout.fillWidth: true
                                    Label {
                                        text: model.title
                                        font.pixelSize: 12
                                        color: Material.color(Material.Grey, Material.Shade700)
                                        Layout.fillWidth: true
                                    }
                                    
                                    Button {
                                        text: "COPY RESULT"
                                        font.pixelSize: 10
                                        font.bold: true
                                        flat: false
                                        height: 32
                                        leftPadding: 16
                                        rightPadding: 16
                                        Material.background: Material.color(Material.Blue, Material.Shade50)
                                        Material.foreground: Material.primary
                                        
                                        background: Rectangle {
                                            color: parent.down ? Material.color(Material.Blue, Material.Shade100) : Material.color(Material.Blue, Material.Shade50)
                                            radius: 16
                                            border.color: Material.primary
                                            border.width: 1
                                        }

                                        onClicked: {
                                            copyToClipboard(model.content)
                                        }
                                    }
                                }
                                
                                Label {
                                    text: model.content
                                    font.pixelSize: 24
                                    font.family: "Georgia" // Serif for math look
                                    color: Material.color(Material.Grey, Material.Shade900)
                                    wrapMode: Text.Wrap
                                    Layout.fillWidth: true
                                }
                            }
                        }
                    }

                    // Detail Button (Conditional)
                    Button {
                        visible: root.currentResult !== null && root.currentResult.algorithmType === 1 // ExtendedGCD
                        text: "Click coefficients for details"
                        Layout.fillWidth: true
                        height: 48
                        
                        background: Rectangle {
                            color: "#1e3a8a"
                            radius: 24
                        }
                        contentItem: Text {
                            text: parent.text
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.bold: true
                        }
                    }
                    
                    Item { Layout.fillHeight: true } // Spacer
                }
            }

            // 2. Steps Tab
            ScrollView {
                clip: true
                ListView {
                    model: root.currentResult ? root.currentResult.executionSteps : []
                    spacing: 12
                    delegate: Rectangle {
                        width: parent.width
                        height: stepColumn.implicitHeight + 24
                        color: "white"
                        radius: 8
                        border.color: Material.color(Material.Grey, Material.Shade200)
                        border.width: 1
                        
                        ColumnLayout {
                            id: stepColumn
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 8
                            
                            RowLayout {
                                Layout.fillWidth: true
                                Rectangle {
                                    width: 24
                                    height: 24
                                    radius: 12
                                    color: Material.primary
                                    Label {
                                        anchors.centerIn: parent
                                        text: (index + 1).toString()
                                        color: "white"
                                        font.bold: true
                                        font.pixelSize: 12
                                    }
                                }
                                
                                Label {
                                    text: modelData.description
                                    font.bold: true
                                    color: Material.color(Material.Grey, Material.Shade800)
                                    Layout.fillWidth: true
                                    wrapMode: Text.Wrap
                                }
                            }
                            
                            Label {
                                visible: modelData.result !== ""
                                text: modelData.result
                                font.family: "Monospace"
                                color: Material.accent
                                Layout.leftMargin: 32
                                Layout.fillWidth: true
                                wrapMode: Text.Wrap
                                background: Rectangle {
                                    color: Material.color(Material.Grey, Material.Shade100)
                                    radius: 4
                                }
                                padding: 8
                            }
                        }
                    }
                }
            }

            // 3. Explanation Tab
            ScrollView {
                clip: true
                TextArea {
                    readOnly: true
                    text: root.currentResult ? getAlgorithmExplanation(root.currentResult.algorithmType) : ""
                    wrapMode: Text.Wrap
                    background: null
                }
            }
            
            // 4. Visualization Tab
            Item {
                id: vizTab
                
                Loader {
                    anchors.fill: parent
                    sourceComponent: {
                        if (!root.currentResult) return defaultViz
                        
                        switch(root.currentResult.algorithmType) {
                            case 0: return gcdViz
                            case 1: return extendedGcdViz
                            case 2: return modularClockViz
                            case 3: return modularClockViz
                            case 4: return primalityScanningViz
                            case 5: return primeSieveViz
                            case 6: return eulerTotientViz
                            case 7: return matrixGridViz
                            case 9: return divisorCloudViz
                            default: return genericViz
                        }
                    }
                }

                Component {
                    id: defaultViz
                    Item {
                        Label {
                            anchors.centerIn: parent
                            text: "Select an algorithm to see visualization"
                            color: Material.color(Material.Grey, Material.Shade500)
                        }
                    }
                }

                Component {
                    id: gcdViz
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 32
                        spacing: 24
                        Label { text: "Euclidean Bar Visualization"; font.pixelSize: 20; font.bold: true; Layout.alignment: Qt.AlignHCenter }
                        RowLayout {
                            Layout.fillWidth: true; Layout.preferredHeight: 250; spacing: 50
                            ColumnLayout {
                                Layout.fillWidth: true; spacing: 10
                                Rectangle {
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: (root.currentResult.inputParameters.a / Math.max(root.currentResult.inputParameters.a, root.currentResult.inputParameters.b)) * 200
                                    color: Material.primary; radius: 8; Layout.alignment: Qt.AlignBottom
                                    Behavior on Layout.preferredHeight { NumberAnimation { duration: 500; easing.type: Easing.InOutQuad } }
                                }
                                Label { text: "a=" + root.currentResult.inputParameters.a; font.bold: true; Layout.alignment: Qt.AlignHCenter }
                            }
                            ColumnLayout {
                                Layout.fillWidth: true; spacing: 10
                                Rectangle {
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: (root.currentResult.inputParameters.b / Math.max(root.currentResult.inputParameters.a, root.currentResult.inputParameters.b)) * 200
                                    color: Material.accent; radius: 8; Layout.alignment: Qt.AlignBottom
                                    Behavior on Layout.preferredHeight { NumberAnimation { duration: 500; easing.type: Easing.InOutQuad } }
                                }
                                Label { text: "b=" + root.currentResult.inputParameters.b; font.bold: true; Layout.alignment: Qt.AlignHCenter }
                            }
                        }
                    }
                }

                Component {
                    id: extendedGcdViz
                    Item {
                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 30
                            width: parent.width * 0.8
                            Label { text: "Bézout's identity"; font.pixelSize: 24; font.bold: true; Layout.alignment: Qt.AlignHCenter }
                            RowLayout {
                                spacing: 15; Layout.alignment: Qt.AlignHCenter
                                ColumnLayout {
                                    Label { text: "(" + root.currentResult.mainResult.x + ")"; font.pixelSize: 28; color: Material.primary; font.bold: true }
                                    Label { text: "x"; font.italic: true; Layout.alignment: Qt.AlignHCenter }
                                }
                                Label { text: "×"; font.pixelSize: 24 }
                                Label { text: root.currentResult.inputParameters.a; font.pixelSize: 28 }
                                Label { text: "+"; font.pixelSize: 24 }
                                ColumnLayout {
                                    Label { text: "(" + root.currentResult.mainResult.y + ")"; font.pixelSize: 28; color: Material.primary; font.bold: true }
                                    Label { text: "y"; font.italic: true; Layout.alignment: Qt.AlignHCenter }
                                }
                                Label { text: "×"; font.pixelSize: 24 }
                                Label { text: root.currentResult.inputParameters.b; font.pixelSize: 28 }
                                Label { text: "="; font.pixelSize: 24 }
                                Label { text: root.currentResult.mainResult.gcd; font.pixelSize: 32; color: Material.accent; font.bold: true }
                            }
                        }
                    }
                }

                Component {
                    id: modularClockViz
                    Item {
                        id: clockRoot
                        property int m: root.currentResult.algorithmType === 2 ? root.currentResult.inputParameters.m : root.currentResult.inputParameters.modulus
                        property int res: root.currentResult.mainResult
                        
                        Canvas {
                            anchors.fill: parent
                            onPaint: {
                                var ctx = getContext("2d");
                                ctx.clearRect(0, 0, width, height);
                                var cx = width / 2;
                                var cy = height / 2;
                                var radius = Math.min(cx, cy) - 40;
                                
                                // Draw circle
                                ctx.strokeStyle = "#ddd";
                                ctx.lineWidth = 2;
                                ctx.beginPath();
                                ctx.arc(cx, cy, radius, 0, 2 * Math.PI);
                                ctx.stroke();
                                
                                // Draw points
                                var points = Math.min(clockRoot.m, 60);
                                for (var i = 0; i < points; i++) {
                                    var angle = (i / points) * 2 * Math.PI - Math.PI / 2;
                                    var px = cx + radius * Math.cos(angle);
                                    var py = cy + radius * Math.sin(angle);
                                    
                                    ctx.fillStyle = (i === clockRoot.res % points) ? Material.primary : "#999";
                                    ctx.beginPath();
                                    ctx.arc(px, py, (i === clockRoot.res % points) ? 8 : 4, 0, 2 * Math.PI);
                                    ctx.fill();
                                }
                            }
                        }
                        Label {
                            anchors.centerIn: parent
                            text: "mod " + clockRoot.m
                            font.bold: true; color: Material.color(Material.Grey, Material.Shade600)
                        }
                    }
                }

                Component {
                    id: eulerTotientViz
                    Flow {
                        anchors.fill: parent; anchors.margins: 20; spacing: 10
                        Repeater {
                            model: Math.min(root.currentResult.inputParameters.n, 200)
                            delegate: Rectangle {
                                width: 30; height: 30; radius: 15
                                color: isCoPrime(index + 1) ? Material.primary : Material.color(Material.Grey, Material.Shade200)
                                Label { anchors.centerIn: parent; text: index + 1; color: isCoPrime(index + 1) ? "white" : "black"; font.pixelSize: 10 }
                                function isCoPrime(val) {
                                    // Simplified for UI - in real app we'd get this from C++
                                    function gcd(a, b) { return b ? gcd(b, a % b) : a; }
                                    return gcd(val, root.currentResult.inputParameters.n) === 1;
                                }
                            }
                        }
                    }
                }

                Component {
                    id: primalityScanningViz
                    Item {
                        ColumnLayout {
                            anchors.centerIn: parent; spacing: 20
                            Label { text: "Scanning for factors..."; font.pixelSize: 18; color: Material.primary }
                            ProgressBar {
                                indeterminate: true; Layout.preferredWidth: 300
                            }
                            Label {
                                text: root.currentResult.mainResult ? "No factors found. It's Prime!" : "Factor found!"
                                font.bold: true; font.pixelSize: 22; color: root.currentResult.mainResult ? "green" : "red"
                            }
                        }
                    }
                }

                Component {
                    id: matrixGridViz
                    Item {
                        ColumnLayout {
                            anchors.centerIn: parent; spacing: 20
                            Label { text: "Transformation Matrix"; font.bold: true }
                            Rectangle {
                                width: 120; height: 120; border.color: "black"; border.width: 2; color: "transparent"
                                GridLayout {
                                    anchors.fill: parent; columns: 2; anchors.margins: 10
                                    Label { text: "1"; font.pixelSize: 24; Layout.alignment: Qt.AlignCenter }
                                    Label { text: "1"; font.pixelSize: 24; Layout.alignment: Qt.AlignCenter }
                                    Label { text: "1"; font.pixelSize: 24; Layout.alignment: Qt.AlignCenter }
                                    Label { text: "0"; font.pixelSize: 24; Layout.alignment: Qt.AlignCenter }
                                }
                            }
                            Label { text: "Raised to power " + root.currentResult.inputParameters.n; font.italic: true }
                        }
                    }
                }

                Component {
                    id: divisorCloudViz
                    Flow {
                        anchors.fill: parent; anchors.margins: 40; spacing: 20
                        Repeater {
                            model: root.currentResult.executionSteps.length > 0 ? root.currentResult.executionSteps.length : 0
                            delegate: Rectangle {
                                width: Math.max(60, txt.implicitWidth + 20); height: 40; radius: 20
                                color: Material.accent
                                Label { id: txt; anchors.centerIn: parent; text: modelData.result; color: "white"; font.bold: true }
                                scale: 0; Component.onCompleted: scaleAnim.start()
                                NumberAnimation { id: scaleAnim; target: parent; property: "scale"; to: 1; duration: 500; easing.type: Easing.OutBack }
                            }
                        }
                    }
                }

                Component {
                    id: primeSieveViz
                    ScrollView {
                        clip: true
                        Flow {
                            width: parent.width - 40; anchors.horizontalCenter: parent.horizontalCenter; spacing: 5; anchors.top: parent.top; anchors.topMargin: 20
                            Repeater {
                                model: Math.min(root.currentResult.inputParameters.limit + 1, 500)
                                delegate: Rectangle {
                                    visible: index > 1
                                    width: 35; height: 35; radius: 4
                                    color: isP ? Material.primary : "white"
                                    border.color: isP ? Material.primary : "#ddd"
                                    property bool isP: root.currentResult.mainResult.indexOf(index) !== -1
                                    Label { anchors.centerIn: parent; text: index; color: isP ? "white" : "#666"; font.bold: isP }
                                    Behavior on color { ColorAnimation { duration: 300 } }
                                }
                            }
                        }
                    }
                }

                Component {
                    id: genericViz
                    ColumnLayout {
                        anchors.centerIn: parent; spacing: 20
                        Label { text: "Execution Summary"; font.pixelSize: 22; font.bold: true }
                        Rectangle {
                            width: 300; height: 150; color: "white"; radius: 15; border.color: "#eee"
                            ColumnLayout {
                                anchors.centerIn: parent
                                Label { text: "Value"; font.pixelSize: 12; color: "#999" }
                                Label { text: root.currentResult.mainResult.toString(); font.pixelSize: 28; font.bold: true; color: Material.primary }
                                Label { text: "Time: " + root.currentResult.executionTime + "ms"; font.italic: true }
                            }
                        }
                    }
                }
            }
        }

        // Bottom Action Bar
        RowLayout {
            Layout.fillWidth: true; spacing: 12; visible: root.currentResult !== null
            Button {
                text: "Export PDF"; Layout.fillWidth: true; height: 48
                background: Rectangle { color: "#1e3a8a"; radius: 24 }
                contentItem: Label { text: parent.text; color: "white"; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter; font.bold: true }
            }
            Button {
                text: "Clear"; Layout.preferredWidth: 120; height: 48
                background: Rectangle { color: Material.color(Material.Grey, Material.Shade200); radius: 24 }
                contentItem: Label { text: parent.text; color: Material.color(Material.Grey, Material.Shade800); horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter; font.bold: true }
                onClicked: { root.currentResult = null; resultCardModel.clear() }
            }
        }
    }

    ListModel { id: resultCardModel }

    function displayResult(result) {
        root.currentResult = result
        resultCardModel.clear()
        var inputParams = result.inputParameters
        var mainResult = result.mainResult

        switch(result.algorithmType) {
            case 0: resultCardModel.append({ "title": "Greatest Common Divisor", "content": "gcd(" + inputParams.a + ", " + inputParams.b + ") = " + mainResult }); break
            case 1: 
                resultCardModel.append({ "title": "GCD", "content": mainResult.gcd })
                resultCardModel.append({ "title": "Bézout Coefficients", "content": "x=" + mainResult.x + ", y=" + mainResult.y })
                break
            case 2: resultCardModel.append({ "title": "Modular Inverse", "content": inputParams.a + "⁻¹ mod " + inputParams.m + " = " + mainResult }); break
            case 3: resultCardModel.append({ "title": "Result", "content": inputParams.base + "^" + inputParams.exponent + " mod " + inputParams.modulus + " = " + mainResult }); break
            case 4: resultCardModel.append({ "title": "Primality", "content": String(inputParams.n + (mainResult ? " is Prime" : " is Composite")) }); break
            case 5: resultCardModel.append({ "title": "Primes Found", "content": String(mainResult.length + " primes up to " + inputParams.limit) }); break
            case 6: resultCardModel.append({ "title": "φ(" + inputParams.n + ")", "content": String(mainResult) }); break
            default: resultCardModel.append({ "title": "Result", "content": String(mainResult) })
        }
        resultTabBar.currentIndex = 3 // Switch to Visualization by default
    }
    
    function copyToClipboard(text) { textHelper.text = text; textHelper.selectAll(); textHelper.copy() }
    TextArea { id: textHelper; visible: false }

    function getAlgorithmExplanation(type) {
        var explanations = {
            0: "GCD: The largest positive integer that divides two numbers without remainder.",
            1: "Extended GCD: Finds GCD and coefficients x,y such that ax + by = gcd(a,b).",
            2: "Modular Inverse: Finds x such that ax ≡ 1 (mod m).",
            3: "Modular Exponentiation: Efficiently calculates (base^exp) mod m.",
            4: "Primality: Determines if a number is divisible only by 1 and itself.",
            5: "Sieve: Efficiently finds all primes up to a limit by marking multiples.",
            6: "Totient: Counts numbers up to n that are relatively prime to n.",
            7: "Matrix: Calculates linear recurrences in logarithmic time.",
            9: "Divisors: Finds all numbers that divide n evenly."
        }
        return explanations[type] || "Mathematical details for this algorithm."
    }
}
