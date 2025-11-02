import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Pane {
    id: root

    property string currentAlgorithm: ""
    property bool isComputing: false
    signal computeRequested(string algorithm, var params)

    Material.background: Material.color(Material.Grey, Material.Shade800)
    Material.elevation: 4

    Rectangle {
        anchors.fill: parent
        color: Material.color(Material.Grey, Material.Shade800)
        border.color: isComputing ? Material.color(Material.Orange, Material.Shade600) : Material.color(Material.Teal, Material.Shade600)
        border.width: 1
        radius: 8
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 16

        Label {
            text: "Input Parameters"
            font.pixelSize: 16
            font.bold: true
            color: Material.primary
        }

        ScrollView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ColumnLayout {
                width: parent.width
                spacing: 12

                // Dynamic input fields based on algorithm
                TextField {
                    id: inputA
                    visible: false
                    placeholderText: "Enter value for a"
                    validator: IntValidator { bottom: -999999999; top: 999999999 }
                    Layout.fillWidth: true
                }

                TextField {
                    id: inputB
                    visible: false
                    placeholderText: "Enter value for b"
                    validator: IntValidator { bottom: -999999999; top: 999999999 }
                    Layout.fillWidth: true
                }

                TextField {
                    id: inputM
                    visible: false
                    placeholderText: "Enter modulus m"
                    validator: IntValidator { bottom: 1; top: 999999999 }
                    Layout.fillWidth: true
                }

                TextField {
                    id: inputBase
                    visible: false
                    placeholderText: "Enter base"
                    validator: IntValidator { bottom: 0; top: 999999999 }
                    Layout.fillWidth: true
                }

                TextField {
                    id: inputExponent
                    visible: false
                    placeholderText: "Enter exponent"
                    validator: IntValidator { bottom: 0; top: 999999999 }
                    Layout.fillWidth: true
                }

                TextField {
                    id: inputModulus
                    visible: false
                    placeholderText: "Enter modulus"
                    validator: IntValidator { bottom: 1; top: 999999999 }
                    Layout.fillWidth: true
                }

                TextField {
                    id: inputN
                    visible: false
                    placeholderText: "Enter value for n"
                    validator: IntValidator { bottom: 0; top: 999999999 }
                    Layout.fillWidth: true
                }

                TextField {
                    id: inputLimit
                    visible: false
                    placeholderText: "Enter upper limit"
                    validator: IntValidator { bottom: 2; top: 10000000 }
                    Layout.fillWidth: true
                }

                // Algorithm description
                Label {
                    id: descriptionLabel
                    text: ""
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    font.pixelSize: 12
                    opacity: 0.8
                }
            }
        }

        RowLayout {
            spacing: 8

            Button {
                id: computeButton
                text: isComputing ? "⏳ Computing..." : "⚡ Compute"
                highlighted: !isComputing
                Layout.fillWidth: true
                enabled: !isComputing

                contentItem: RowLayout {
                    spacing: 8

                    Label {
                        text: computeButton.text
                        color: computeButton.enabled ? Material.foreground : Material.color(Material.Grey, Material.Shade500)
                        Layout.alignment: Qt.AlignVCenter
                    }

                    BusyIndicator {
                        visible: isComputing
                        running: isComputing
                        Layout.preferredWidth: 20
                        Layout.preferredHeight: 20
                        Material.accent: Material.Orange
                    }
                }

                onClicked: {
                    var params = {}

                    switch(root.currentAlgorithm) {
                        case "GCD":
                        case "ExtendedGCD":
                            params.a = parseInt(inputA.text) || 0
                            params.b = parseInt(inputB.text) || 0
                            break
                        case "ModularInverse":
                            params.a = parseInt(inputA.text) || 0
                            params.m = parseInt(inputM.text) || 1
                            break
                        case "ModularExponentiation":
                            params.base = parseInt(inputBase.text) || 0
                            params.exponent = parseInt(inputExponent.text) || 0
                            params.modulus = parseInt(inputModulus.text) || 1
                            break
                        case "PrimalityTest":
                        case "EulerTotient":
                        case "MatrixExponentiation":
                        case "DivisorFunction":
                        case "LCMSum":
                        case "CubeFreeCheck":
                            params.n = parseInt(inputN.text) || 0
                            break
                        case "PrimeSieve":
                            params.limit = parseInt(inputLimit.text) || 100
                            break
                    }

                    root.computeRequested(root.currentAlgorithm, params)
                }
            }

            Button {
                text: "🗑️ Clear"
                enabled: !isComputing
                onClicked: clearInputs()
            }
        }
    }

    function showAlgorithm(algorithm) {
        root.currentAlgorithm = algorithm
        clearInputs()

        // Hide all inputs first
        inputA.visible = false
        inputB.visible = false
        inputM.visible = false
        inputBase.visible = false
        inputExponent.visible = false
        inputModulus.visible = false
        inputN.visible = false
        inputLimit.visible = false

        // Show relevant inputs and set description
        switch(algorithm) {
            case "GCD":
                inputA.visible = true
                inputB.visible = true
                inputA.placeholderText = "Enter first integer (a)"
                inputB.placeholderText = "Enter second integer (b)"
                descriptionLabel.text = "Computes the Greatest Common Divisor of two integers using the Euclidean algorithm."
                break

            case "ExtendedGCD":
                inputA.visible = true
                inputB.visible = true
                inputA.placeholderText = "Enter first integer (a)"
                inputB.placeholderText = "Enter second integer (b)"
                descriptionLabel.text = "Computes GCD and the coefficients of Bézout's identity using the extended Euclidean algorithm."
                break

            case "ModularInverse":
                inputA.visible = true
                inputM.visible = true
                inputA.placeholderText = "Enter number (a)"
                inputM.placeholderText = "Enter modulus (m)"
                descriptionLabel.text = "Computes the modular multiplicative inverse using the extended Euclidean algorithm."
                break

            case "ModularExponentiation":
                inputBase.visible = true
                inputExponent.visible = true
                inputModulus.visible = true
                descriptionLabel.text = "Computes (base^exponent) mod modulus using fast exponentiation."
                break

            case "PrimalityTest":
                inputN.visible = true
                inputN.placeholderText = "Enter number to test (n)"
                descriptionLabel.text = "Tests whether a number is prime using trial division."
                break

            case "PrimeSieve":
                inputLimit.visible = true
                inputLimit.placeholderText = "Enter upper limit for prime generation"
                descriptionLabel.text = "Generates all prime numbers up to a limit using the Sieve of Eratosthenes."
                break

            case "EulerTotient":
                inputN.visible = true
                inputN.placeholderText = "Enter number (n)"
                descriptionLabel.text = "Computes Euler's Totient function φ(n) - the count of integers up to n that are coprime to n."
                break

            case "MatrixExponentiation":
                inputN.visible = true
                inputN.placeholderText = "Enter exponent (n)"
                descriptionLabel.text = "Computes linear recurrence sequences using matrix exponentiation (Fibonacci example)."
                break

            case "DivisorFunction":
                inputN.visible = true
                inputN.placeholderText = "Enter number (n)"
                descriptionLabel.text = "Computes the divisor function d(n) - the number of positive divisors of n."
                break

            case "LCMSum":
                inputN.visible = true
                inputN.placeholderText = "Enter upper limit (n)"
                descriptionLabel.text = "Computes the sum of LCM(i,n) for i from 1 to n."
                break

            case "CubeFreeCheck":
                inputN.visible = true
                inputN.placeholderText = "Enter number to check (n)"
                descriptionLabel.text = "Checks if a number is cube-free (not divisible by any cube > 1)."
                break
        }
    }

    function clearInputs() {
        inputA.text = ""
        inputB.text = ""
        inputM.text = ""
        inputBase.text = ""
        inputExponent.text = ""
        inputModulus.text = ""
        inputN.text = ""
        inputLimit.text = ""
    }
}
