import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import NumberTheory

ApplicationWindow {
    id: root
    width: 1600
    height: 1000
    visible: true
    title: qsTr("Number Theory Explorer")

    // Theme Configuration
    Material.theme: Material.Light
    Material.primary: "#1e3a8a" // Dark Blue
    Material.accent: "#3b82f6" // Lighter Blue
    Material.background: "#f3f4f6" // Light Gray Background

    // Computation History
    property var computationHistory: []

    // Engine instance
    NumberTheoryEngine {
        id: engine

        property bool isComputing: false
        property string currentAlgorithm: ""
        property var currentParams: ({})

        onComputationFinished: function(algorithmName, status, executionTime, inputParams, mainResult, steps, algorithmType) {
            isComputing = false
            var resultData = {
                algorithmName: algorithmName,
                statusString: status,
                executionTime: executionTime,
                inputParameters: inputParams,
                mainResult: mainResult,
                executionSteps: steps,
                algorithmType: algorithmType,
                timestamp: new Date().toLocaleString()
            }
            
            // Add to history
            var history = [resultData]
            for (var i = 0; i < root.computationHistory.length && i < 49; i++) {
                history.push(root.computationHistory[i])
            }
            root.computationHistory = history
            
            resultsLaboratory.displayResult(resultData)
            inputWorkspace.isComputing = false
        }

        onComputationError: function(error) {
            isComputing = false
            inputWorkspace.isComputing = false
            console.error("Computation Error: " + error)
        }
    }

    // Header
    header: ToolBar {
        Material.background: "#1e3a8a" // Dark Blue header
        Material.elevation: 0
        height: 60

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 24
            anchors.rightMargin: 24
            spacing: 16

            Label {
                text: "Number Theory Explorer"
                font.pixelSize: 20
                font.bold: true
                color: "white"
                Layout.alignment: Qt.AlignVCenter
            }

            Item { Layout.fillWidth: true }

            ToolButton {
                text: "📜 History"
                font.pixelSize: 14
                Material.foreground: "white"
                onClicked: historyDialog.open()
            }
        }
    }

    // Main Layout
    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal
        spacing: 0

        // Left Panel: Sidebar (Algorithm Browser)
        InputPanel {
            id: sideBar
            SplitView.preferredWidth: 320
            SplitView.minimumWidth: 280
            SplitView.maximumWidth: 400
            
            onAlgorithmSelected: function(algorithmName) {
                inputWorkspace.showAlgorithm(algorithmName)
            }
        }

        // Center Panel: Input Workspace
        InputWorkspace {
            id: inputWorkspace
            SplitView.preferredWidth: 500
            SplitView.minimumWidth: 450
            
            onComputeRequested: function(algorithm, params) {
                engine.isComputing = true
                inputWorkspace.isComputing = true
                executeAlgorithm(algorithm, params)
            }
        }

        // Right Panel: Results Laboratory
        ResultsLaboratory {
            id: resultsLaboratory
            SplitView.fillWidth: true
            SplitView.minimumWidth: 400
        }
    }

    // Helper function to bridge QML to C++ Engine
    function executeAlgorithm(algorithm, params) {
        // console.log("Executing " + algorithm + " with params: " + JSON.stringify(params))

        switch(algorithm) {
            case "GCD":
                engine.computeGCDAsync(params.a, params.b)
                break
            case "ExtendedGCD":
                engine.computeExtendedGCDAsync(params.a, params.b)
                break
            case "ModularInverse":
                engine.computeModularInverseAsync(params.a, params.m)
                break
            case "ModularExponentiation":
                engine.computeModularExponentiationAsync(params.base, params.exponent, params.modulus)
                break
            case "PrimalityTest":
                engine.testPrimalityAsync(params.n)
                break
            case "PrimeSieve":
                engine.generatePrimesAsync(params.limit)
                break
            case "EulerTotient":
                engine.computeEulerTotientAsync(params.n)
                break
            case "MatrixExponentiation":
                engine.computeMatrixExponentiationAsync([1, 1], params.n) // Default Fib params
                break
            case "DivisorFunction":
                engine.computeDivisorFunctionAsync(params.n)
                break
            case "LCMSum":
                engine.computeLCMSumAsync(params.n)
                break
            case "CubeFreeCheck":
                engine.checkCubeFreeAsync(params.n)
                break
        }
    }

    // History Dialog
    Dialog {
        id: historyDialog
        title: "📜 Computation History"
        width: 800
        height: 600
        standardButtons: Dialog.Close
        modal: true
        parent: Overlay.overlay
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        contentItem: ColumnLayout {
            spacing: 0
            
            Label {
                text: "Recent Computations"
                font.pixelSize: 18
                font.bold: true
                color: Material.primary
                Layout.margins: 16
            }
            
            ListView {
                id: historyList
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                model: root.computationHistory
                spacing: 8
                
                delegate: Rectangle {
                    width: historyList.width - 32
                    height: 80
                    color: "white"
                    radius: 8
                    border.color: Material.color(Material.Grey, Material.Shade300)
                    border.width: 1
                    anchors.horizontalCenter: parent.horizontalCenter
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 16
                        
                        Rectangle {
                            width: 40
                            height: 40
                            radius: 20
                            color: Material.color(Material.Blue, Material.Shade50)
                            Layout.alignment: Qt.AlignVCenter
                            
                            Label {
                                text: "⚡"
                                anchors.centerIn: parent
                                font.pixelSize: 20
                            }
                        }
                        
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4
                            
                            Label {
                                text: modelData.algorithmName
                                font.bold: true
                                color: Material.color(Material.Grey, Material.Shade900)
                            }
                            
                            Label {
                                text: "Result: " + (modelData.mainResult ? modelData.mainResult.toString().substring(0, 50) + (modelData.mainResult.toString().length > 50 ? "..." : "") : "N/A")
                                font.pixelSize: 12
                                color: Material.color(Material.Grey, Material.Shade600)
                                elide: Text.ElideRight
                            }
                        }
                        
                        ColumnLayout {
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                            spacing: 4
                            
                            Label {
                                text: modelData.executionTime + " ms"
                                font.bold: true
                                color: Material.accent
                                horizontalAlignment: Text.AlignRight
                                Layout.fillWidth: true
                            }
                            
                            Label {
                                text: modelData.timestamp
                                font.pixelSize: 10
                                color: Material.color(Material.Grey, Material.Shade500)
                                horizontalAlignment: Text.AlignRight
                                Layout.fillWidth: true
                            }
                        }
                    }
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            resultsLaboratory.displayResult(modelData)
                            historyDialog.close()
                        }
                    }
                }
            }
            
            Label {
                visible: root.computationHistory.length === 0
                text: "No history yet. Run some algorithms!"
                Layout.alignment: Qt.AlignCenter
                color: Material.color(Material.Grey, Material.Shade500)
            }
        }
    }
}
