#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QStandardPaths>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include <QIcon>

#include "core/NumberTheoryEngine.h"
#include "core/AlgorithmResult.h"

// Register meta types for Qt signals
#include <QMetaType>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application properties
    app.setApplicationName("Number Theory Explorer");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Math-Problems");
    app.setOrganizationDomain("math-problems.local");

    // Set application icon (if available)
    // app.setWindowIcon(QIcon(":/icons/app.png"));

    // Register C++ types with QML
    qmlRegisterType<NumberTheoryEngine>("NumberTheory", 1, 0, "NumberTheoryEngine");
    qmlRegisterType<AlgorithmResult>("NumberTheory", 1, 0, "AlgorithmResult");

    // Register meta types for Qt signals
    qRegisterMetaType<QList<ExecutionStep>>("QList<ExecutionStep>");

    // Create QML application engine
    QQmlApplicationEngine engine;

    // Load translations if available
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "NumberTheoryGUI_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    // Load main QML file
    engine.load(QUrl("qrc:/qml/main.qml"));

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load main.qml";
        return -1;
    }

    return app.exec();
}
