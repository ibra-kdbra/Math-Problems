import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Pane {
    id: root
    padding: 0
    
    // Signal to notify main window of selection
    signal algorithmSelected(string algorithmName)

    property string searchText: ""

    Material.background: Material.color(Material.Grey, Material.Shade50)

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Search Bar
        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: "transparent"
            
            TextField {
                id: searchField
                anchors.centerIn: parent
                width: parent.width - 32
                placeholderText: "Search algorithms..."
                leftPadding: 40
                
                onTextChanged: root.searchText = text.toLowerCase()
                
                background: Rectangle {
                    color: Material.color(Material.Grey, Material.Shade200)
                    radius: 8
                    border.width: 0
                }

                Label {
                    text: "🔍"
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 12
                    font.pixelSize: 14
                    color: Material.color(Material.Grey, Material.Shade600)
                }
            }
        }

        // Algorithm List
        ListView {
            id: algorithmList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: algorithmModel
            
            // Section handling
            section.property: "category"
            section.delegate: Item {
                width: parent.width
                height: 40
                
                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    spacing: 8
                    
                    Text {
                        text: "−" // Minus sign for expanded state (visual only for now)
                        font.pixelSize: 12
                        color: Material.color(Material.Grey, Material.Shade600)
                    }
                    
                    Label {
                        text: section
                        font.bold: true
                        font.pixelSize: 13
                        color: Material.color(Material.Grey, Material.Shade900)
                    }
                }
            }

            delegate: ItemDelegate {
                id: delegateItem
                width: parent.width
                
                property bool isMatch: model.title.toLowerCase().includes(root.searchText) || model.category.toLowerCase().includes(root.searchText)
                visible: isMatch
                height: isMatch ? (model.isCard ? 140 : 50) : 0
                
                highlighted: ListView.isCurrentItem

                contentItem: Item {
                    // Regular List Item
                    RowLayout {
                        visible: !model.isCard
                        anchors.fill: parent
                        anchors.leftMargin: 32
                        anchors.rightMargin: 16
                        spacing: 12

                        Label {
                            text: model.icon
                            font.pixelSize: 16
                            Layout.preferredWidth: 24
                        }

                        Label {
                            text: model.title
                            font.pixelSize: 14
                            color: delegateItem.highlighted ? Material.primary : Material.foreground
                            Layout.fillWidth: true
                        }
                    }

                    // Card Style Item (for Featured/First item)
                    Rectangle {
                        visible: model.isCard
                        anchors.fill: parent
                        anchors.margins: 12
                        anchors.leftMargin: 16
                        anchors.rightMargin: 16
                        color: "white"
                        radius: 8
                        border.color: Material.color(Material.Grey, Material.Shade300)
                        border.width: 1

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 4

                            RowLayout {
                                spacing: 8
                                Label { text: model.icon; font.pixelSize: 18 }
                                Label { 
                                    text: model.title
                                    font.bold: true
                                    font.pixelSize: 14
                                }
                            }

                            RowLayout {
                                spacing: 2
                                Repeater {
                                    model: 5
                                    Text {
                                        text: "★"
                                        color: index < model.stars ? "#fbbf24" : "#e5e7eb"
                                        font.pixelSize: 12
                                    }
                                }
                            }

                            Label {
                                text: model.level
                                color: Material.color(Material.Grey, Material.Shade600)
                                font.pixelSize: 12
                            }

                            Label {
                                text: model.description
                                color: Material.color(Material.Grey, Material.Shade600)
                                font.pixelSize: 11
                                elide: Text.ElideRight
                                maximumLineCount: 2
                                Layout.fillWidth: true
                            }
                        }
                    }
                }

                background: Rectangle {
                    visible: !model.isCard
                    color: delegateItem.highlighted ? Material.color(Material.Blue, Material.Shade50) : "transparent"
                    radius: 0
                    
                    Rectangle {
                        visible: delegateItem.highlighted
                        width: 4
                        height: parent.height
                        color: Material.primary
                        anchors.left: parent.left
                    }
                }

                onClicked: {
                    algorithmList.currentIndex = index
                    root.algorithmSelected(model.algorithmId)
                }
            }
        }
    }

    // Data Model
    ListModel {
        id: algorithmModel

        ListElement {
            category: "Basic Operations"
            title: "Greatest Common Divisor"
            algorithmId: "GCD"
            icon: "💎"
            isCard: true
            level: "Beginner"
            stars: 4
            description: "The fundamental algorithm for finding the largest common divisor of two numbers."
        }

        ListElement {
            category: "Basic Operations"
            title: "Extended GCD"
            algorithmId: "ExtendedGCD"
            icon: "🔍"
            isCard: false
            level: "Intermediate"
            stars: 0
            description: ""
        }
        
        ListElement {
            category: "Basic Operations"
            title: "Modular Inverse"
            algorithmId: "ModularInverse"
            icon: "🔄"
            isCard: false
            level: "Intermediate"
            stars: 0
            description: ""
        }
        
        ListElement {
            category: "Basic Operations"
            title: "Modular Exponentiation"
            algorithmId: "ModularExponentiation"
            icon: "🚀"
            isCard: false
            level: "Intermediate"
            stars: 0
            description: ""
        }

        ListElement {
            category: "Primes"
            title: "Generate Primes"
            algorithmId: "PrimeSieve"
            icon: "⚡"
            isCard: false
            level: "Intermediate"
            stars: 0
            description: ""
        }

        ListElement {
            category: "Primes"
            title: "Primality Test"
            algorithmId: "PrimalityTest"
            icon: "❓"
            isCard: false
            level: "Beginner"
            stars: 0
            description: ""
        }

        ListElement {
            category: "Number Functions"
            title: "Divisor Function"
            algorithmId: "DivisorFunction"
            icon: "🔢"
            isCard: false
            level: "Intermediate"
            stars: 0
            description: ""
        }

        ListElement {
            category: "Number Functions"
            title: "Euler's Totient"
            algorithmId: "EulerTotient"
            icon: "ϕ"
            isCard: false
            level: "Advanced"
            stars: 0
            description: ""
        }
        
        ListElement {
            category: "Number Functions"
            title: "LCM Sum"
            algorithmId: "LCMSum"
            icon: "Σ"
            isCard: false
            level: "Advanced"
            stars: 0
            description: ""
        }

        ListElement {
            category: "Advanced"
            title: "Matrix Exponentiation"
            algorithmId: "MatrixExponentiation"
            icon: "▦"
            isCard: false
            level: "Advanced"
            stars: 0
            description: ""
        }
        
        ListElement {
            category: "Advanced"
            title: "Cube Free Check"
            algorithmId: "CubeFreeCheck"
            icon: "🧊"
            isCard: false
            level: "Beginner"
            stars: 0
            description: ""
        }
    }
}
