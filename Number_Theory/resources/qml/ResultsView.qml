import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Pane {
    id: root

    Material.background: Material.color(Material.Grey, Material.Shade800)
    Material.elevation: 4

    property var currentResult: null

    Rectangle {
        anchors.fill: parent
        color: Material.color(Material.Grey, Material.Shade800)
        border.color: Material.color(Material.Teal, Material.Shade600)
        border.width: 1
        radius: 8
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        Label {
            text: "Results"
            font.pixelSize: 16
            font.bold: true
            color: Material.primary
        }

        ScrollView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            TextArea {
                id: resultsText
                readOnly: true
                wrapMode: Text.Wrap
                font.family: "Monospace"
                font.pixelSize: 11
                background: Rectangle {
                    color: Material.background
                    border.color: Material.dividerColor
                    border.width: 1
                    radius: 4
                }
                padding: 12
            }
        }

        RowLayout {
            spacing: 8

            Label {
                id: statusLabel
                text: "Ready"
                Layout.fillWidth: true
                font.pixelSize: 12
                opacity: 0.8
            }

            ProgressBar {
                id: progressBar
                visible: false
                indeterminate: true
                Layout.preferredWidth: 100
            }

            Button {
                text: "Export"
                onClicked: exportResults()
            }

            Button {
                text: "Clear"
                onClicked: clearResults()
            }
        }
    }

    function displayResult(result) {
        progressBar.visible = false
        statusLabel.text = "Computation completed"

        var output = ""

        // Header with algorithm name and status
        output += "╔══════════════════════════════════════════════════════════════════════════════╗\n"
        output += "║ " + result.algorithmName.toUpperCase().padEnd(76, ' ') + " ║\n"
        output += "╠══════════════════════════════════════════════════════════════════════════════╣\n"

        // Status and timing
        var statusStr = result.statusString || "Success"
        var timeStr = result.executionTime + " ms"
        output += "║ Status: " + statusStr.padEnd(20, ' ') + "Execution Time: " + timeStr.padEnd(35, ' ') + " ║\n"
        output += "╠══════════════════════════════════════════════════════════════════════════════╣\n"

        // Input parameters
        var inputParams = result.inputParameters
        if (inputParams && Object.keys(inputParams).length > 0) {
            output += "║ INPUT PARAMETERS:                                                        ║\n"
            for (var key in inputParams) {
                var paramKey = key + ":"
                var paramValue = inputParams[key].toString()
                output += "║   " + paramKey.padEnd(15, ' ') + " = " + paramValue.padEnd(55, ' ') + " ║\n"
            }
            output += "╠══════════════════════════════════════════════════════════════════════════════╣\n"
        }

        // Main result with mathematical formatting
        output += "║ RESULT:                                                                   ║\n"
        var resultStr = formatMathematicalResult(result)
        var resultLines = resultStr.split('\n')
        for (var i = 0; i < resultLines.length; i++) {
            output += "║ " + resultLines[i].padEnd(76, ' ') + " ║\n"
        }

        // Execution steps (if available)
        var executionSteps = result.executionSteps
        if (executionSteps && executionSteps.length > 0) {
            output += "╠══════════════════════════════════════════════════════════════════════════════╣\n"
            output += "║ EXECUTION STEPS:                                                          ║\n"
            for (var j = 0; j < executionSteps.length; j++) {
                var step = executionSteps[j]
                var stepLine = (j + 1) + ". " + step.description
                if (step.result) {
                    stepLine += " → " + step.result
                }
                output += "║ " + stepLine.substring(0, 76).padEnd(76, ' ') + " ║\n"
            }
        }

        output += "╚══════════════════════════════════════════════════════════════════════════════╝\n"

        // Add educational note
        output += "\n💡 " + getAlgorithmDescription(result.algorithmType) + "\n"

        resultsText.text = output
        root.currentResult = result
    }

    function showError(error) {
        progressBar.visible = false
        statusLabel.text = "Error occurred"
        resultsText.text = "❌ Error: " + error
    }

    function formatMathematicalResult(result) {
        var formatted = ""
        var inputParams = result.inputParameters
        var mainResult = result.mainResult

        switch(result.algorithmType) {
            case 0: // GCD
                formatted = "gcd(" + inputParams.a + ", " + inputParams.b + ") = " + mainResult
                break

            case 1: // ExtendedGCD
                var map = mainResult
                formatted = "gcd(" + inputParams.a + ", " + inputParams.b + ") = " + map.gcd + "\n"
                formatted += "Bézout coefficients: x = " + map.x + ", y = " + map.y + "\n"
                formatted += "Verification: " + inputParams.a + "×" + map.x + " + " + inputParams.b + "×" + map.y + " = " + map.gcd
                break

            case 2: // ModularInverse
                formatted = inputParams.a + "⁻¹ ≡ " + mainResult + " (mod " + inputParams.m + ")"
                break

            case 3: // ModularExponentiation
                formatted = inputParams.base + "^" + inputParams.exponent + " ≡ " + mainResult + " (mod " + inputParams.modulus + ")"
                break

            case 4: // PrimalityTest
                formatted = "Is " + inputParams.n + " prime? " + (mainResult ? "YES ✓" : "NO ✗")
                break

            case 5: // PrimeSieve
                var primes = mainResult
                formatted = "Found " + primes.length + " prime numbers ≤ " + inputParams.limit + "\n"
                if (primes.length <= 20) {
                    formatted += "Primes: " + primes.join(", ")
                } else {
                    formatted += "First 10: " + primes.slice(0, 10).join(", ")
                    formatted += "\nLast 10: " + primes.slice(-10).join(", ")
                }
                break

            case 6: // EulerTotient
                formatted = "φ(" + inputParams.n + ") = " + mainResult
                break

            case 7: // MatrixExponentiation
                formatted = "F(" + inputParams.n + ") = " + mainResult + " (nth Fibonacci number)"
                break

            case 8: // CubeFreeCheck
                formatted = "Is " + inputParams.n + " cube-free? " + (mainResult ? "YES ✓" : "NO ✗")
                break

            case 9: // DivisorFunction
                formatted = "d(" + inputParams.n + ") = " + mainResult + " (number of positive divisors)"
                break

            case 10: // LCMSum
                formatted = "∑ LCM(i," + inputParams.n + ") for i=1 to " + inputParams.n + " = " + mainResult
                break

            default:
                formatted = mainResult ? mainResult.toString() : "No result"
        }

        return formatted
    }

    function getAlgorithmDescription(type) {
        switch(type) {
            case 0: return "Computes the Greatest Common Divisor of two integers using the Euclidean algorithm."
            case 1: return "Computes GCD and the coefficients of Bézout's identity using the extended Euclidean algorithm."
            case 2: return "Computes the modular multiplicative inverse using the extended Euclidean algorithm."
            case 3: return "Computes (base^exponent) mod modulus using fast exponentiation."
            case 4: return "Tests whether a number is prime using trial division."
            case 5: return "Generates all prime numbers up to a limit using the Sieve of Eratosthenes."
            case 6: return "Computes Euler's Totient function φ(n) - the count of integers up to n that are coprime to n."
            case 7: return "Computes linear recurrence sequences using matrix exponentiation."
            case 8: return "Checks if a number is cube-free (not divisible by any cube > 1)."
            case 9: return "Computes the divisor function d(n) - the number of positive divisors of n."
            case 10: return "Computes the sum of LCM(i,n) for i from 1 to n."
            default: return "Unknown algorithm."
        }
    }

    function exportResults() {
        if (!root.currentResult) {
            return
        }

        exportDialog.open()
    }

    function exportToFile(format) {
        if (!root.currentResult) {
            return
        }

        var fileName = "number_theory_result_" + Date.now()
        var content = ""

        switch(format) {
            case "json":
                fileName += ".json"
                content = JSON.stringify({
                    algorithm: root.currentResult.algorithmName,
                    timestamp: root.currentResult.timestamp || new Date().toISOString(),
                    inputParameters: root.currentResult.inputParameters,
                    result: root.currentResult.mainResult,
                    executionTime: root.currentResult.executionTime,
                    status: root.currentResult.statusString,
                    executionSteps: root.currentResult.executionSteps
                }, null, 2)
                break

            case "csv":
                fileName += ".csv"
                content = "Algorithm,Timestamp,Input Parameters,Result,Execution Time (ms),Status\n"
                var params = JSON.stringify(root.currentResult.inputParameters).replace(/"/g, '""')
                var result = JSON.stringify(root.currentResult.mainResult).replace(/"/g, '""')
                content += '"' + root.currentResult.algorithmName + '",'
                content += '"' + (root.currentResult.timestamp || new Date().toISOString()) + '",'
                content += '"' + params + '",'
                content += '"' + result + '",'
                content += '"' + root.currentResult.executionTime + '",'
                content += '"' + (root.currentResult.statusString || "Success") + '"\n'
                break

            case "txt":
            default:
                fileName += ".txt"
                content = resultsText.text
                break
        }

        // In a real Qt application, we'd use QFileDialog and QFile to save
        // For now, we'll simulate by copying to clipboard and showing a message
        resultsText.text = content
        resultsText.selectAll()
        resultsText.copy()

        // Restore original content
        displayResult(root.currentResult)

        statusLabel.text = "Results exported as " + format.toUpperCase() + " (copied to clipboard)"
    }

    function clearResults() {
        resultsText.text = ""
        statusLabel.text = "Ready"
        root.currentResult = null
    }

    function setComputing(algorithm) {
        progressBar.visible = true
        statusLabel.text = "Computing " + algorithm + "..."
    }

    Dialog {
        id: exportDialog
        title: "Export Results"
        standardButtons: Dialog.Cancel
        modal: true

        ColumnLayout {
            spacing: 16

            Label {
                text: "Choose export format:"
                font.bold: true
            }

            ButtonGroup {
                id: formatGroup
            }

            RadioButton {
                text: "📄 Plain Text (.txt) - Formatted display"
                checked: true
                ButtonGroup.group: formatGroup
                property string format: "txt"
            }

            RadioButton {
                text: "📊 JSON (.json) - Structured data"
                ButtonGroup.group: formatGroup
                property string format: "json"
            }

            RadioButton {
                text: "📈 CSV (.csv) - Spreadsheet compatible"
                ButtonGroup.group: formatGroup
                property string format: "csv"
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight

                Button {
                    text: "Export"
                    highlighted: true
                    onClicked: {
                        var selectedFormat = "txt"
                        for (var i = 0; i < formatGroup.buttons.length; i++) {
                            if (formatGroup.buttons[i].checked) {
                                selectedFormat = formatGroup.buttons[i].format
                                break
                            }
                        }
                        exportToFile(selectedFormat)
                        exportDialog.close()
                    }
                }

                Button {
                    text: "Cancel"
                    onClicked: exportDialog.close()
                }
            }
        }
    }
}
