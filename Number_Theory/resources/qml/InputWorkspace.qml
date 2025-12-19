import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Pane {
    id: root
    padding: 24

    property string currentAlgorithm: ""
    property string currentAlgorithmTitle: "Select an Algorithm"
    property string currentIcon: "⚡"
    property bool isComputing: false
    
    signal computeRequested(string algorithm, var params)

    Material.background: Material.color(Material.Grey, Material.Shade50)

    // Main Card
    Rectangle {
        anchors.fill: parent
        color: "white"
        radius: 12
        
        // Shadow effect
        border.color: Material.color(Material.Grey, Material.Shade200)
        border.width: 1

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 24
            spacing: 20

            // Header
            RowLayout {
                spacing: 16
                Label {
                    text: root.currentIcon
                    font.pixelSize: 32
                    color: Material.primary
                }
                Label {
                    text: root.currentAlgorithmTitle
                    font.pixelSize: 24
                    font.bold: true
                    color: Material.color(Material.Grey, Material.Shade900)
                }
            }

            // Input Fields Container
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                ColumnLayout {
                    width: parent.width
                    spacing: 16

                    // Integer 'a'
                    ColumnLayout {
                        visible: inputA.visible
                        Layout.fillWidth: true
                        spacing: 8
                        Label { text: "Integer 'a'"; color: Material.color(Material.Grey, Material.Shade700) }
                        TextField {
                            id: inputA
                            visible: false
                            placeholderText: "99"
                            Layout.fillWidth: true
                            color: "black"
                            selectByMouse: true
                            validator: IntValidator { bottom: -999999999; top: 999999999 }
                            background: Rectangle {
                                color: Material.color(Material.Grey, Material.Shade100)
                                radius: 8
                                border.width: 0
                            }
                        }
                    }

                    // Integer 'b'
                    ColumnLayout {
                        visible: inputB.visible
                        Layout.fillWidth: true
                        spacing: 8
                        Label { text: "Integer 'b'"; color: Material.color(Material.Grey, Material.Shade700) }
                        TextField {
                            id: inputB
                            visible: false
                            placeholderText: "78"
                            Layout.fillWidth: true
                            color: "black"
                            selectByMouse: true
                            validator: IntValidator { bottom: -999999999; top: 999999999 }
                            background: Rectangle {
                                color: Material.color(Material.Grey, Material.Shade100)
                                radius: 8
                                border.width: 0
                            }
                        }
                    }

                    // Modulus 'm'
                    ColumnLayout {
                        visible: inputM.visible
                        Layout.fillWidth: true
                        spacing: 8
                        Label { text: "Modulus 'm'"; color: Material.color(Material.Grey, Material.Shade700) }
                        TextField {
                            id: inputM
                            visible: false
                            placeholderText: "Enter modulus"
                            Layout.fillWidth: true
                            color: "black"
                            selectByMouse: true
                            validator: IntValidator { bottom: 1; top: 999999999 }
                            background: Rectangle {
                                color: Material.color(Material.Grey, Material.Shade100)
                                radius: 8
                                border.width: 0
                            }
                        }
                    }

                    // Base
                    ColumnLayout {
                        visible: inputBase.visible
                        Layout.fillWidth: true
                        spacing: 8
                        Label { text: "Base"; color: Material.color(Material.Grey, Material.Shade700) }
                        TextField {
                            id: inputBase
                            visible: false
                            placeholderText: "Enter base"
                            Layout.fillWidth: true
                            color: "black"
                            selectByMouse: true
                            validator: IntValidator { bottom: 0; top: 999999999 }
                            background: Rectangle {
                                color: Material.color(Material.Grey, Material.Shade100)
                                radius: 8
                                border.width: 0
                            }
                        }
                    }

                    // Exponent
                    ColumnLayout {
                        visible: inputExponent.visible
                        Layout.fillWidth: true
                        spacing: 8
                        Label { text: "Exponent"; color: Material.color(Material.Grey, Material.Shade700) }
                        TextField {
                            id: inputExponent
                            visible: false
                            placeholderText: "Enter exponent"
                            Layout.fillWidth: true
                            color: "black"
                            selectByMouse: true
                            validator: IntValidator { bottom: 0; top: 999999999 }
                            background: Rectangle {
                                color: Material.color(Material.Grey, Material.Shade100)
                                radius: 8
                                border.width: 0
                            }
                        }
                    }

                    // Modulus (for exp)
                    ColumnLayout {
                        visible: inputModulus.visible
                        Layout.fillWidth: true
                        spacing: 8
                        Label { text: "Modulus"; color: Material.color(Material.Grey, Material.Shade700) }
                        TextField {
                            id: inputModulus
                            visible: false
                            placeholderText: "Enter modulus"
                            Layout.fillWidth: true
                            color: "black"
                            selectByMouse: true
                            validator: IntValidator { bottom: 1; top: 999999999 }
                            background: Rectangle {
                                color: Material.color(Material.Grey, Material.Shade100)
                                radius: 8
                                border.width: 0
                            }
                        }
                    }

                    // N (Generic)
                    ColumnLayout {
                        visible: inputN.visible
                        Layout.fillWidth: true
                        spacing: 8
                        Label { text: "Integer 'n'"; color: Material.color(Material.Grey, Material.Shade700) }
                        TextField {
                            id: inputN
                            visible: false
                            placeholderText: "Enter n"
                            Layout.fillWidth: true
                            color: "black"
                            selectByMouse: true
                            validator: IntValidator { bottom: 0; top: 999999999 }
                            background: Rectangle {
                                color: Material.color(Material.Grey, Material.Shade100)
                                radius: 8
                                border.width: 0
                            }
                        }
                    }

                    // Limit
                    ColumnLayout {
                        visible: inputLimit.visible
                        Layout.fillWidth: true
                        spacing: 8
                        Label { text: "Limit"; color: Material.color(Material.Grey, Material.Shade700) }
                        TextField {
                            id: inputLimit
                            visible: false
                            placeholderText: "Enter limit"
                            Layout.fillWidth: true
                            color: "black"
                            selectByMouse: true
                            validator: IntValidator { bottom: 2; top: 10000000 }
                            background: Rectangle {
                                color: Material.color(Material.Grey, Material.Shade100)
                                radius: 8
                                border.width: 0
                            }
                        }
                    }

                    // Try Example Button
                    Button {
                        text: "Try Example: " + getExampleText(root.currentAlgorithm)
                        flat: true
                        Layout.alignment: Qt.AlignLeft
                        visible: root.currentAlgorithm !== ""
                        
                        background: Rectangle {
                            color: "transparent"
                            border.color: Material.primary
                            border.width: 1
                            radius: 20
                        }
                        contentItem: Text {
                            text: parent.text
                            color: Material.primary
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.weight: Font.Medium
                        }

                        onClicked: setExampleValues(root.currentAlgorithm)
                    }

                    // Description Box
                    Rectangle {
                        Layout.fillWidth: true
                        height: descriptionLabel.implicitHeight + 24
                        color: Material.color(Material.Grey, Material.Shade100)
                        radius: 8
                        
                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 12
                            
                            Label {
                                text: "❯"
                                color: Material.color(Material.Grey, Material.Shade500)
                                font.bold: true
                            }
                            
                            Label {
                                id: descriptionLabel
                                text: ""
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                                color: Material.color(Material.Grey, Material.Shade800)
                            }
                        }
                    }
                }
            }

            // Action Bar
            RowLayout {
                spacing: 12
                Layout.fillWidth: true

                Button {
                    id: computeButton
                    Layout.fillWidth: true
                    height: 48
                    enabled: !isComputing && root.currentAlgorithm !== ""
                    
                    background: Rectangle {
                        color: computeButton.enabled ? "#1e3a8a" : Material.color(Material.Grey, Material.Shade400) // Dark blue
                        radius: 24
                    }
                    
                    contentItem: RowLayout {
                        spacing: 8
                        anchors.centerIn: parent
                        
                        Label {
                            text: isComputing ? "Computing..." : "⚡ Compute"
                            color: "white"
                            font.bold: true
                        }
                        
                        BusyIndicator {
                            visible: isComputing
                            running: isComputing
                            Layout.preferredHeight: 20
                            Layout.preferredWidth: 20
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
                    text: "🗑 Clear"
                    Layout.preferredWidth: 100
                    height: 48
                    
                    background: Rectangle {
                        color: Material.color(Material.Grey, Material.Shade200)
                        radius: 24
                    }
                    contentItem: Text {
                        text: parent.text
                        color: Material.color(Material.Grey, Material.Shade700)
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                    }
                    
                    onClicked: clearInputs()
                }
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

        // Update Title and Icon
        updateTitleAndIcon(algorithm)

        // Show relevant inputs and set description
        switch(algorithm) {
            case "GCD":
                inputA.visible = true
                inputB.visible = true
                descriptionLabel.text = "Finds the largest positive integer that divides each of the integers."
                break

            case "ExtendedGCD":
                inputA.visible = true
                inputB.visible = true
                descriptionLabel.text = "Finds x, y for ax+by=gcd(a,b)"
                break

            case "ModularInverse":
                inputA.visible = true
                inputM.visible = true
                descriptionLabel.text = "Finds x such that ax ≡ 1 (mod m)"
                break

            case "ModularExponentiation":
                inputBase.visible = true
                inputExponent.visible = true
                inputModulus.visible = true
                descriptionLabel.text = "Computes (base^exponent) mod modulus efficiently."
                break

            case "PrimalityTest":
                inputN.visible = true
                descriptionLabel.text = "Determines if a number is prime."
                break

            case "PrimeSieve":
                inputLimit.visible = true
                descriptionLabel.text = "Generates all prime numbers up to the limit."
                break

            case "EulerTotient":
                inputN.visible = true
                descriptionLabel.text = "Counts positive integers up to n that are relatively prime to n."
                break

            case "MatrixExponentiation":
                inputN.visible = true
                descriptionLabel.text = "Computes the nth Fibonacci number using matrix exponentiation."
                break

            case "DivisorFunction":
                inputN.visible = true
                descriptionLabel.text = "Counts the number of divisors of n."
                break

            case "LCMSum":
                inputN.visible = true
                descriptionLabel.text = "Computes the sum of LCM(i, n) for 1 <= i <= n."
                break

            case "CubeFreeCheck":
                inputN.visible = true
                descriptionLabel.text = "Checks if n is not divisible by any perfect cube > 1."
                break
            
            default:
                descriptionLabel.text = "Select an algorithm to begin."
        }
    }
    
    function updateTitleAndIcon(algorithm) {
        var map = {
            "GCD": ["Greatest Common Divisor", "💎"],
            "ExtendedGCD": ["Extended GCD", "⚡"],
            "ModularInverse": ["Modular Inverse", "🔄"],
            "ModularExponentiation": ["Modular Exponentiation", "🚀"],
            "PrimalityTest": ["Primality Test", "❓"],
            "PrimeSieve": ["Generate Primes", "⚡"],
            "EulerTotient": ["Euler's Totient", "ϕ"],
            "MatrixExponentiation": ["Matrix Exponentiation", "▦"],
            "DivisorFunction": ["Divisor Function", "🔢"],
            "LCMSum": ["LCM Sum", "Σ"],
            "CubeFreeCheck": ["Cube Free Check", "🧊"]
        }
        
        if (map[algorithm]) {
            root.currentAlgorithmTitle = map[algorithm][0]
            root.currentIcon = map[algorithm][1]
        } else {
            root.currentAlgorithmTitle = "Select Algorithm"
            root.currentIcon = "🎓"
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

    function getExampleText(algorithm) {
        switch(algorithm) {
            case "GCD": return "a=48, b=18"
            case "ExtendedGCD": return "a=99, b=78"
            case "ModularInverse": return "a=3, m=11"
            case "ModularExponentiation": return "2^10 mod 1000"
            case "PrimalityTest": return "n=17"
            case "PrimeSieve": return "limit=100"
            case "EulerTotient": return "n=12"
            case "MatrixExponentiation": return "n=10"
            case "DivisorFunction": return "n=12"
            case "LCMSum": return "n=5"
            case "CubeFreeCheck": return "n=12"
            default: return ""
        }
    }

    function setExampleValues(algorithm) {
        switch(algorithm) {
            case "GCD":
                inputA.text = "48"
                inputB.text = "18"
                break
            case "ExtendedGCD":
                inputA.text = "99"
                inputB.text = "78"
                break
            case "ModularInverse":
                inputA.text = "3"
                inputM.text = "11"
                break
            case "ModularExponentiation":
                inputBase.text = "2"
                inputExponent.text = "10"
                inputModulus.text = "1000"
                break
            case "PrimalityTest":
                inputN.text = "17"
                break
            case "PrimeSieve":
                inputLimit.text = "100"
                break
            case "EulerTotient":
                inputN.text = "12"
                break
            case "MatrixExponentiation":
                inputN.text = "10"
                break
            case "DivisorFunction":
                inputN.text = "12"
                break
            case "LCMSum":
                inputN.text = "5"
                break
            case "CubeFreeCheck":
                inputN.text = "12"
                break
        }
    }
}
