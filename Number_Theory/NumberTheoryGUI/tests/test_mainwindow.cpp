#include <QtTest/QtTest>
#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QTabWidget>

#include "../src/MainWindow.h"
#include "../src/core/NumberTheoryEngine.h"

class TestMainWindow : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    // UI Component Tests
    void testInputValidation();
    void testInputFieldVisibility();
    void testButtonConnections();
    void testResultsDisplay();

    // Algorithm Integration Tests
    void testGCDComputation();
    void testExtendedGCDComputation();
    void testPrimalityTest();

private:
    QApplication* m_app;
    MainWindow* m_window;
};

void TestMainWindow::initTestCase()
{
    int argc = 1;
    char* argv[] = {(char*)"test"};
    m_app = new QApplication(argc, argv);

    m_window = new MainWindow();
    m_window->show();
    QTest::qWaitForWindowActive(m_window);
}

void TestMainWindow::cleanupTestCase()
{
    delete m_window;
    delete m_app;
}

void TestMainWindow::testInputValidation()
{
    // Test that input fields accept valid integers
    QLineEdit* inputA = m_window->findChild<QLineEdit*>("inputA");
    QVERIFY(inputA != nullptr);

    // Test valid input
    inputA->setText("42");
    QCOMPARE(inputA->text(), QString("42"));

    // Test that validator is set
    QVERIFY(inputA->validator() != nullptr);
}

void TestMainWindow::testInputFieldVisibility()
{
    // Test that input fields are properly shown/hidden based on algorithm
    QLineEdit* inputA = m_window->findChild<QLineEdit*>("inputA");
    QLineEdit* inputB = m_window->findChild<QLineEdit*>("inputB");
    QLineEdit* inputN = m_window->findChild<QLineEdit*>("inputN");

    QVERIFY(inputA != nullptr);
    QVERIFY(inputB != nullptr);
    QVERIFY(inputN != nullptr);

    // Initially should show GCD inputs (a and b)
    QVERIFY(inputA->isVisible());
    QVERIFY(inputB->isVisible());
    QVERIFY(!inputN->isVisible());
}

void TestMainWindow::testButtonConnections()
{
    // Test that buttons exist and are properly connected
    QPushButton* computeButton = m_window->findChild<QPushButton*>("computeButton");
    QPushButton* clearButton = m_window->findChild<QPushButton*>("clearButton");
    QPushButton* exportButton = m_window->findChild<QPushButton*>("exportButton");

    QVERIFY(computeButton != nullptr);
    QVERIFY(clearButton != nullptr);
    QVERIFY(exportButton != nullptr);

    // Test that buttons are enabled
    QVERIFY(computeButton->isEnabled());
    QVERIFY(clearButton->isEnabled());
    QVERIFY(exportButton->isEnabled());
}

void TestMainWindow::testResultsDisplay()
{
    // Test that results text area exists and is read-only
    QTextEdit* resultsText = m_window->findChild<QTextEdit*>("resultsText");
    QVERIFY(resultsText != nullptr);
    QVERIFY(resultsText->isReadOnly());

    // Test initial state
    QVERIFY(resultsText->toPlainText().isEmpty());
}

void TestMainWindow::testGCDComputation()
{
    // Test basic GCD computation through UI
    QLineEdit* inputA = m_window->findChild<QLineEdit*>("inputA");
    QLineEdit* inputB = m_window->findChild<QLineEdit*>("inputB");
    QPushButton* computeButton = m_window->findChild<QPushButton*>("computeButton");
    QTextEdit* resultsText = m_window->findChild<QTextEdit*>("resultsText");

    QVERIFY(inputA != nullptr);
    QVERIFY(inputB != nullptr);
    QVERIFY(computeButton != nullptr);
    QVERIFY(resultsText != nullptr);

    // Set test inputs
    inputA->setText("54");
    inputB->setText("24");

    // Trigger computation (this would normally be done by clicking the GCD button)
    // For testing, we'll directly call the engine
    NumberTheoryEngine engine;
    auto result = engine.computeGCD(54, 24);

    QVERIFY(result != nullptr);
    QCOMPARE(result->getStatus(), ResultStatus::Success);
    QCOMPARE(result->getMainResult().toLongLong(), 6LL);
}

void TestMainWindow::testExtendedGCDComputation()
{
    // Test extended GCD computation
    NumberTheoryEngine engine;
    auto result = engine.computeExtendedGCD(30, 18);

    QVERIFY(result != nullptr);
    QCOMPARE(result->getStatus(), ResultStatus::Success);

    auto resultMap = result->getMainResult().toMap();
    QCOMPARE(resultMap["gcd"].toLongLong(), 6LL);
    QCOMPARE(resultMap["x"].toLongLong(), -1LL);
    QCOMPARE(resultMap["y"].toLongLong(), 2LL);
}

void TestMainWindow::testPrimalityTest()
{
    // Test primality testing
    NumberTheoryEngine engine;

    // Test prime number
    auto primeResult = engine.testPrimality(17);
    QVERIFY(primeResult != nullptr);
    QCOMPARE(primeResult->getStatus(), ResultStatus::Success);
    QCOMPARE(primeResult->getMainResult().toBool(), true);

    // Test composite number
    auto compositeResult = engine.testPrimality(15);
    QVERIFY(compositeResult != nullptr);
    QCOMPARE(compositeResult->getStatus(), ResultStatus::Success);
    QCOMPARE(compositeResult->getMainResult().toBool(), false);
}

QTEST_MAIN(TestMainWindow)
#include "test_mainwindow.moc"
