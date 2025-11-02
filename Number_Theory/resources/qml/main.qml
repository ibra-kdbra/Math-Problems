import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import NumberTheory

ApplicationWindow {
    id: root
    width: 1400
    height: 900
    visible: true
    title: qsTr("🎓 Number Theory Explorer - Interactive Mathematical Algorithms")

    Material.theme: Material.Dark
    Material.accent: Material.Cyan
    Material.primary: Material.color(Material.Cyan, Material.Shade700)
    Material.background: Material.color(Material.Grey, Material.Shade900)

    // Engine instance
    NumberTheoryEngine {
        id: engine

        property bool isComputing: false

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
            computationHistory.unshift(resultData)
            if (computationHistory.length > 50) {
                computationHistory.pop()
            }
            resultsView.displayResult(resultData)
        }

        onComputationError: function(error) {
            isComputing = false
            resultsView.showError(error)
        }
    }

    // Computation history
    property var computationHistory: []

    header: ToolBar {
        Material.background: Material.primary

        RowLayout {
            anchors.fill: parent
            spacing: 16

            Label {
                text: "Number Theory Explorer"
                font.pixelSize: 18
                font.bold: true
                color: Material.foreground
                Layout.alignment: Qt.AlignVCenter
            }

            Item { Layout.fillWidth: true }

            ToolButton {
                text: "📊 Compare"
                font.pixelSize: 14
                ToolTip.text: "Compare algorithms"
                onClicked: comparisonDialog.open()
            }

            ToolButton {
                text: "📜 History"
                font.pixelSize: 14
                ToolTip.text: "View computation history"
                onClicked: historyDialog.open()
            }

            ToolButton {
                text: "📚"
                font.pixelSize: 16
                ToolTip.text: "About"
                onClicked: aboutDialog.open()
            }
        }
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Vertical

        // Main content area
        SplitView {
            SplitView.fillHeight: true
            orientation: Qt.Horizontal

            // Left panel - Algorithm selection
            ScrollView {
                SplitView.preferredWidth: 300
                SplitView.minimumWidth: 250

                ColumnLayout {
                    width: parent.width
                    spacing: 8

                    GroupBox {
                        title: "🔢 Basic Operations"
                        Layout.fillWidth: true
                        Material.background: Material.color(Material.Grey, Material.Shade800)
                        Material.foreground: Material.primary

                        ColumnLayout {
                            width: parent.width
                            spacing: 6

                            Button {
                                text: "📐 Greatest Common Divisor"
                                Layout.fillWidth: true
                                Material.accent: Material.Cyan
                                Material.foreground: Material.foreground
                                font.bold: true
                                onClicked: inputPanel.showAlgorithm("GCD")
                            }

                            Button {
                                text: "🔍 Extended GCD"
                                Layout.fillWidth: true
                                Material.accent: Material.Cyan
                                Material.foreground: Material.foreground
                                font.bold: true
                                onClicked: inputPanel.showAlgorithm("ExtendedGCD")
                            }
                        }
                    }

                    GroupBox {
                        title: "🔢 Primes"
                        Layout.fillWidth: true
                        Material.background: Material.color(Material.Grey, Material.Shade800)
                        Material.foreground: Material.primary

                        ColumnLayout {
                            width: parent.width
                            spacing: 6

                            Button {
                                text: "🔍 Test Primality"
                                Layout.fillWidth: true
                                Material.accent: Material.Green
                                Material.foreground: Material.foreground
                                font.bold: true
                                onClicked: inputPanel.showAlgorithm("PrimalityTest")
                            }

                            Button {
                                text: "⚡ Generate Primes"
                                Layout.fillWidth: true
                                Material.accent: Material.Green
                                Material.foreground: Material.foreground
                                font.bold: true
                                onClicked: inputPanel.showAlgorithm("PrimeSieve")
                            }
                        }
                    }

                    GroupBox {
                        title: "🧮 Advanced Functions"
                        Layout.fillWidth: true
                        Material.background: Material.color(Material.Grey, Material.Shade800)
                        Material.foreground: Material.primary

                        ColumnLayout {
                            width: parent.width
                            spacing: 6

                            Button {
                                text: "φ Euler's Totient"
                                Layout.fillWidth: true
                                Material.accent: Material.Purple
                                Material.foreground: Material.foreground
                                font.bold: true
                                onClicked: inputPanel.showAlgorithm("EulerTotient")
                            }

                            Button {
                                text: "🔄 Matrix Exponentiation"
                                Layout.fillWidth: true
                                Material.accent: Material.Purple
                                Material.foreground: Material.foreground
                                font.bold: true
                                onClicked: inputPanel.showAlgorithm("MatrixExponentiation")
                            }

                            Button {
                                text: "📊 Divisor Function"
                                Layout.fillWidth: true
                                Material.accent: Material.Purple
                                Material.foreground: Material.foreground
                                font.bold: true
                                onClicked: inputPanel.showAlgorithm("DivisorFunction")
                            }

                            Button {
                                text: "∑ LCM Sum"
                                Layout.fillWidth: true
                                Material.accent: Material.Purple
                                Material.foreground: Material.foreground
                                font.bold: true
                                onClicked: inputPanel.showAlgorithm("LCMSum")
                            }
                        }
                    }

                    GroupBox {
                        title: "🔐 Cryptography"
                        Layout.fillWidth: true
                        Material.background: Material.color(Material.Grey, Material.Shade800)
                        Material.foreground: Material.primary

                        ColumnLayout {
                            width: parent.width
                            spacing: 6

                            Button {
                                text: "🔑 Modular Inverse"
                                Layout.fillWidth: true
                                Material.accent: Material.Orange
                                Material.foreground: Material.foreground
                                font.bold: true
                                onClicked: inputPanel.showAlgorithm("ModularInverse")
                            }

                            Button {
                                text: "⚡ Modular Exponentiation"
                                Layout.fillWidth: true
                                Material.accent: Material.Orange
                                Material.foreground: Material.foreground
                                font.bold: true
                                onClicked: inputPanel.showAlgorithm("ModularExponentiation")
                            }

                            Button {
                                text: "🎲 Check Cube-Free"
                                Layout.fillWidth: true
                                Material.accent: Material.Orange
                                Material.foreground: Material.foreground
                                font.bold: true
                                onClicked: inputPanel.showAlgorithm("CubeFreeCheck")
                            }
                        }
                    }
                }
            }

            // Center panel - Input parameters
            InputPanel {
                id: inputPanel
                SplitView.preferredWidth: 350
                SplitView.minimumWidth: 300
                isComputing: engine.isComputing

                onComputeRequested: function(algorithm, params) {
                    engine.isComputing = true
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
                            engine.computeMatrixExponentiationAsync([1, 1], params.n)
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
            }
        }

        // Bottom panel - Results
        ResultsView {
            id: resultsView
            SplitView.preferredHeight: 300
            SplitView.minimumHeight: 200
        }
    }

    Dialog {
        id: aboutDialog
        title: "About Number Theory Explorer"
        standardButtons: Dialog.Ok
        modal: true

        ColumnLayout {
            spacing: 16

            Label {
                text: "🎓 Number Theory Explorer"
                font.pixelSize: 18
                font.bold: true
            }

            Label {
                text: "An interactive educational application for exploring\nfundamental number theory algorithms."
                wrapMode: Text.WordWrap
            }

            Label {
                text: "Built with Qt6 and QML for modern, responsive UI."
                font.italic: true
            }

            Label {
                text: "Version 1.0.0"
                font.pixelSize: 12
            }
        }
    }

    Dialog {
        id: historyDialog
        title: "📜 Computation History"
        width: 800
        height: 600
        standardButtons: Dialog.Close
        modal: true

        ColumnLayout {
            anchors.fill: parent
            spacing: 16

            Label {
                text: "Recent Computations"
                font.pixelSize: 16
                font.bold: true
                color: Material.primary
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ListView {
                    id: historyList
                    model: computationHistory
                    spacing: 8

                    delegate: Rectangle {
                        width: parent.width
                        height: contentColumn.height + 16
                        color: Material.color(Material.Grey, Material.Shade800)
                        border.color: Material.color(Material.Teal, Material.Shade600)
                        border.width: 1
                        radius: 6

                        ColumnLayout {
                            id: contentColumn
                            width: parent.width - 16
                            anchors.centerIn: parent
                            spacing: 4

                            RowLayout {
                                Layout.fillWidth: true

                                Label {
                                    text: modelData.algorithmName
                                    font.bold: true
                                    color: Material.primary
                                    Layout.fillWidth: true
                                }

                                Label {
                                    text: modelData.timestamp
                                    font.pixelSize: 10
                                    opacity: 0.7
                                }
                            }

                            Label {
                                text: "Input: " + JSON.stringify(modelData.inputParameters).replace(/[{}"]/g, '').replace(/,/g, ', ')
                                font.pixelSize: 12
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "Result: " + (typeof modelData.mainResult === 'object' ? JSON.stringify(modelData.mainResult) : modelData.mainResult)
                                font.pixelSize: 12
                                font.bold: true
                                color: Material.accent
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "Time: " + modelData.executionTime + " ms"
                                font.pixelSize: 10
                                opacity: 0.7
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                resultsView.displayResult(modelData)
                                historyDialog.close()
                            }
                        }
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight

                Button {
                    text: "🗑️ Clear History"
                    onClicked: computationHistory = []
                }

                Button {
                    text: "Close"
                    onClicked: historyDialog.close()
                }
            }
        }
    }

    Dialog {
        id: comparisonDialog
        title: "📊 Algorithm Comparison"
        width: 900
        height: 700
        standardButtons: Dialog.Close
        modal: true

        property var comparisonResults: []

        ColumnLayout {
            anchors.fill: parent
            spacing: 16

            Label {
                text: "Compare Algorithm Performance"
                font.pixelSize: 18
                font.bold: true
                color: Material.primary
            }

            // Input section
            GroupBox {
                title: "Input Parameters"
                Layout.fillWidth: true

                GridLayout {
                    columns: 2
                    columnSpacing: 16
                    rowSpacing: 8

                    Label { text: "Value for a:" }
                    TextField {
                        id: compInputA
                        placeholderText: "Enter first value"
                        validator: IntValidator { bottom: -999999999; top: 999999999 }
                        Layout.fillWidth: true
                    }

                    Label { text: "Value for b:" }
                    TextField {
                        id: compInputB
                        placeholderText: "Enter second value"
                        validator: IntValidator { bottom: -999999999; top: 999999999 }
                        Layout.fillWidth: true
                    }

                    Label { text: "Value for n:" }
                    TextField {
                        id: compInputN
                        placeholderText: "Enter value for n"
                        validator: IntValidator { bottom: 0; top: 999999999 }
                        Layout.fillWidth: true
                    }

                    Label { text: "Modulus m:" }
                    TextField {
                        id: compInputM
                        placeholderText: "Enter modulus"
                        validator: IntValidator { bottom: 1; top: 999999999 }
                        Layout.fillWidth: true
                    }
                }
            }

            // Algorithm selection
            GroupBox {
                title: "Select Algorithms to Compare"
                Layout.fillWidth: true

                ColumnLayout {
                    CheckBox {
                        id: compGCD
                        text: "📐 Greatest Common Divisor"
                        checked: true
                    }
                    CheckBox {
                        id: compExtendedGCD
                        text: "🔍 Extended GCD"
                    }
                    CheckBox {
                        id: compPrimality
                        text: "🔍 Primality Test"
                    }
                    CheckBox {
                        id: compEulerTotient
                        text: "φ Euler's Totient"
                    }
                    CheckBox {
                        id: compModularInverse
                        text: "🔑 Modular Inverse"
                    }
                    CheckBox {
                        id: compDivisorFunction
                        text: "📊 Divisor Function"
                    }
                }
            }

            RowLayout {
                Button {
                    text: "🚀 Run Comparison"
                    highlighted: true
                    onClicked: runComparison()
                }

                Button {
                    text: "Clear Results"
                    onClicked: comparisonResults = []
                }
            }

            // Results section
            GroupBox {
                title: "Comparison Results"
                Layout.fillWidth: true
                Layout.fillHeight: true

                ScrollView {
                    anchors.fill: parent

                    ListView {
                        model: comparisonResults
                        spacing: 8

                        delegate: Rectangle {
                            width: parent.width
                            height: 60
                            color: Material.color(Material.Grey, Material.Shade800)
                            border.color: Material.color(Material.Teal, Material.Shade600)
                            border.width: 1
                            radius: 6

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 16

                                Label {
                                    text: modelData.algorithm
                                    font.bold: true
                                    color: Material.primary
                                    Layout.preferredWidth: 200
                                }

                                Label {
                                    text: modelData.result
                                    Layout.fillWidth: true
                                    wrapMode: Text.Wrap
                                }

                                Label {
                                    text: modelData.time + " ms"
                                    font.bold: true
                                    color: modelData.time < 10 ? Material.color(Material.Green, Material.Shade400) :
                                           modelData.time < 100 ? Material.color(Material.Orange, Material.Shade400) :
                                           Material.color(Material.Red, Material.Shade400)
                                    Layout.preferredWidth: 80
                                    horizontalAlignment: Text.AlignRight
                                }
                            }
                        }
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight

                Button {
                    text: "Export Comparison"
                    onClicked: exportComparison()
                }

                Button {
                    text: "Close"
                    onClicked: comparisonDialog.close()
                }
            }
        }

        function runComparison() {
            comparisonDialog.comparisonResults = []

            // Simple synchronous comparison for demonstration
            // In a real app, this would be more sophisticated
            var results = []

            if (compGCD.checked) {
                var startTime = Date.now()
                try {
                    // For demo purposes, we'll simulate results
                    // In reality, we'd need synchronous versions or better async handling
                    results.push({
                        algorithm: "GCD",
                        result: "Demo: gcd(" + compInputA.text + ", " + compInputB.text + ")",
                        time: Math.floor(Math.random() * 10) + 1,
                        status: "Success"
                    })
                } catch (e) {
                    results.push({
                        algorithm: "GCD",
                        result: "Error: " + e.message,
                        time: 0,
                        status: "Error"
                    })
                }
            }

            if (compExtendedGCD.checked) {
                results.push({
                    algorithm: "Extended GCD",
                    result: "Demo: Extended Euclidean algorithm result",
                    time: Math.floor(Math.random() * 15) + 5,
                    status: "Success"
                })
            }

            if (compPrimality.checked) {
                results.push({
                    algorithm: "Primality Test",
                    result: "Demo: Testing " + compInputN.text + " for primality",
                    time: Math.floor(Math.random() * 50) + 10,
                    status: "Success"
                })
            }

            if (compEulerTotient.checked) {
                results.push({
                    algorithm: "Euler's Totient",
                    result: "Demo: φ(" + compInputN.text + ") computation",
                    time: Math.floor(Math.random() * 20) + 5,
                    status: "Success"
                })
            }

            if (compModularInverse.checked) {
                results.push({
                    algorithm: "Modular Inverse",
                    result: "Demo: Finding inverse of " + compInputA.text + " mod " + compInputM.text,
                    time: Math.floor(Math.random() * 25) + 8,
                    status: "Success"
                })
            }

            if (compDivisorFunction.checked) {
                results.push({
                    algorithm: "Divisor Function",
                    result: "Demo: Computing d(" + compInputN.text + ")",
                    time: Math.floor(Math.random() * 30) + 12,
                    status: "Success"
                })
            }

            comparisonDialog.comparisonResults = results
        }

        function formatResult(result, type) {
            if (typeof result === 'object' && result !== null) {
                if (type === 1) { // ExtendedGCD
                    return "gcd=" + result.gcd + ", x=" + result.x + ", y=" + result.y
                }
                return JSON.stringify(result)
            }
            return result.toString()
        }

        function exportComparison() {
            if (comparisonResults.length === 0) {
                return
            }

            var content = "Algorithm Comparison Results\n"
            content += "Input Parameters: a=" + compInputA.text + ", b=" + compInputB.text + ", n=" + compInputN.text + ", m=" + compInputM.text + "\n\n"
            content += "Algorithm,Result,Execution Time (ms),Status\n"

            for (var i = 0; i < comparisonResults.length; i++) {
                var res = comparisonResults[i]
                content += '"' + res.algorithm + '","' + res.result + '","' + res.time + '","' + res.status + '"\n'
            }

            // Copy to clipboard (in real app, would save to file)
            var tempTextArea = Qt.createQmlObject('import QtQuick.Controls 2.15; TextArea {}', comparisonDialog)
            tempTextArea.text = content
            tempTextArea.selectAll()
            tempTextArea.copy()
            tempTextArea.destroy()
        }
    }
}
