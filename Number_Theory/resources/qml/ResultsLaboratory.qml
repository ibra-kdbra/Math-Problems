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
                                        text: "📄 Copy"
                                        flat: true
                                        font.pixelSize: 12
                                        Material.foreground: Material.primary
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
                    delegate: ItemDelegate {
                        width: parent.width
                        
                        contentItem: ColumnLayout {
                            spacing: 4
                            Label { 
                                text: "Step " + (index + 1)
                                font.bold: true
                                color: Material.primary
                            }
                            Label {
                                text: modelData.description
                                wrapMode: Text.Wrap
                                Layout.fillWidth: true
                            }
                            Label {
                                text: modelData.result || ""
                                font.family: "Monospace"
                                color: Material.accent
                                visible: text !== ""
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
                        if (root.currentResult && root.currentResult.algorithmType == 5) { // PrimeSieve
                            return primeSieveViz
                        }
                        return defaultViz
                    }
                }

                Component {
                    id: defaultViz
                    Item {
                        Label {
                            anchors.centerIn: parent
                            text: "Visualization not available for this algorithm"
                            color: Material.color(Material.Grey, Material.Shade500)
                        }
                    }
                }

                Component {
                    id: primeSieveViz
                    ScrollView {
                        contentWidth: parent.width
                        contentHeight: Math.max(parent.height, gridFlow.height + 32)
                        clip: true

                        Flow {
                            id: gridFlow
                            width: parent.width - 32
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.top: parent.top
                            anchors.topMargin: 16
                            spacing: 4

                            Repeater {
                                model: (root.currentResult && root.currentResult.inputParameters && root.currentResult.inputParameters.limit) ? root.currentResult.inputParameters.limit + 1 : 0
                                delegate: Rectangle {
                                    visible: index > 0 // Skip 0
                                    width: 40
                                    height: 40
                                    color: isPrime(index) ? Material.color(Material.Green, Material.Shade100) : "white"
                                    border.color: isPrime(index) ? Material.primary : Material.color(Material.Grey, Material.Shade300)
                                    border.width: isPrime(index) ? 2 : 1
                                    radius: 4

                                    Text {
                                        anchors.centerIn: parent
                                        text: index
                                        font.bold: isPrime(index)
                                        color: isPrime(index) ? Material.primary : Material.color(Material.Grey, Material.Shade500)
                                    }

                                    function isPrime(num) {
                                        if (!root.currentResult || !root.currentResult.mainResult) return false
                                        return root.currentResult.mainResult.indexOf(num) !== -1
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Bottom Action Bar
        RowLayout {
            Layout.fillWidth: true
            spacing: 12
            visible: root.currentResult !== null

            Button {
                text: "Export"
                Layout.fillWidth: true
                height: 48
                
                background: Rectangle {
                    color: "#1e3a8a" // Blue
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

            Button {
                text: "Clear"
                Layout.preferredWidth: 120
                height: 48
                
                background: Rectangle {
                    color: Material.color(Material.Grey, Material.Shade200)
                    radius: 24
                }
                contentItem: Text {
                    text: parent.text
                    color: Material.color(Material.Grey, Material.Shade800)
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.bold: true
                }
                
                onClicked: {
                    root.currentResult = null
                    resultCardModel.clear()
                }
            }
        }
    }

    ListModel {
        id: resultCardModel
    }

    function displayResult(result) {
        root.currentResult = result
        resultCardModel.clear()
        
        // Parse result and populate cards
        var inputParams = result.inputParameters
        var mainResult = result.mainResult

        switch(result.algorithmType) {
            case 0: // GCD
                resultCardModel.append({
                    "title": "Card: Greatest Common Divisor",
                    "content": "gcd(" + inputParams.a + ", " + inputParams.b + ") = " + mainResult
                })
                break

            case 1: // ExtendedGCD
                var map = mainResult
                resultCardModel.append({
                    "title": "Card: Greatest Common Divisor",
                    "content": "gcd " + inputParams.a + ", " + inputParams.b + " = " + map.gcd
                })
                resultCardModel.append({
                    "title": "Card: Bézout's Identity",
                    "content": "(" + map.x + ") · " + inputParams.a + " + " + map.y + " · " + inputParams.b + " = " + map.gcd
                })
                break

            case 2: // ModularInverse
                resultCardModel.append({
                    "title": "Card: Modular Inverse",
                    "content": inputParams.a + "⁻¹ ≡ " + mainResult + " (mod " + inputParams.m + ")"
                })
                break

            case 3: // ModularExponentiation
                resultCardModel.append({
                    "title": "Card: Result",
                    "content": inputParams.base + "^" + inputParams.exponent + " ≡ " + mainResult + " (mod " + inputParams.modulus + ")"
                })
                break
                
            case 5: // PrimeSieve
                var primes = mainResult
                var count = primes.length
                resultCardModel.append({
                    "title": "Card: Prime Count",
                    "content": "Found " + count + " primes ≤ " + inputParams.limit
                })
                var primeStr = count <= 20 ? primes.join(", ") : primes.slice(0, 20).join(", ") + "..."
                resultCardModel.append({
                    "title": "Card: Primes",
                    "content": primeStr
                })
                break

            default:
                resultCardModel.append({
                    "title": "Card: Result",
                    "content": mainResult.toString()
                })
        }
        
        resultTabBar.currentIndex = 0
    }
    
    function copyToClipboard(text) {
        textHelper.text = text
        textHelper.selectAll()
        textHelper.copy()
    }
    
    TextArea {
        id: textHelper
        visible: false
    }

    function getAlgorithmExplanation(type) {
        var explanations = {
            0: "The Greatest Common Divisor (GCD) is the largest positive integer that divides two numbers without remainder.\n\nEuclidean Algorithm:\n1. While b ≠ 0:\n   a. Set temp = b\n   b. Set b = a mod b\n   c. Set a = temp\n2. Return a",
            1: "Extended Euclidean Algorithm finds GCD(a,b) and coefficients x,y such that ax + by = gcd(a,b).\n\nThis is useful for finding modular inverses and solving Diophantine equations.\n\nBézout's Identity: For any integers a and b, there exist integers x and y such that ax + by = gcd(a,b).",
            2: "The modular inverse of a modulo m is a number x such that ax ≡ 1 (mod m). This exists only when gcd(a,m) = 1.",
            3: "Modular exponentiation computes (base^exponent) mod modulus efficiently using the method of repeated squaring (binary exponentiation).",
            4: "Primality testing determines if a number is prime (divisible only by 1 and itself).",
            5: "Sieve of Eratosthenes is an efficient algorithm to find all prime numbers up to any given limit.",
            6: "Euler's Totient function φ(n) counts the positive integers up to a given integer n that are relatively prime to n.",
            7: "Matrix exponentiation is used to calculate the n-th term of a linear recurrence relation (like Fibonacci) in O(log n) time.",
            8: "A number is cube-free if it is not divisible by any perfect cube greater than 1.",
            9: "The divisor function d(n) counts the number of divisors of n.",
            10: "Calculates the sum of LCM(i, n) for all i from 1 to n."
        }
        return explanations[type] || "No explanation available."
    }
}
