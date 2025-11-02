#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QGroupBox>
#include <QProgressBar>
#include <QStatusBar>
#include <QMenuBar>
#include <QAction>
#include <QSplitter>

#include "core/NumberTheoryEngine.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Algorithm computation slots
    void computeGCD();
    void computeExtendedGCD();
    void computeModularInverse();
    void computeModularExponentiation();
    void testPrimality();
    void generatePrimes();
    void computeEulerTotient();
    void computeMatrixExponentiation();
    void checkCubeFree();
    void computeDivisorFunction();
    void computeLCMSum();

    // UI slots
    void clearInputs();
    void exportResults();
    void showAbout();

    // Engine result handling
    void onComputationFinished(AlgorithmResult* result);
    void onComputationError(const QString& error);

private:
    Ui::MainWindow *ui;

    // Core components
    NumberTheoryEngine* m_engine;

    // UI components
    QTabWidget* m_tabWidget;
    QWidget* m_basicOpsTab;
    QWidget* m_primesTab;
    QWidget* m_advancedTab;
    QWidget* m_cryptoTab;

    // Input panels
    QGroupBox* m_inputGroup;
    QLineEdit* m_inputA;
    QLineEdit* m_inputB;
    QLineEdit* m_inputM;
    QLineEdit* m_inputBase;
    QLineEdit* m_inputExponent;
    QLineEdit* m_inputModulus;
    QLineEdit* m_inputN;
    QLineEdit* m_inputLimit;

    // Control buttons
    QPushButton* m_computeButton;
    QPushButton* m_clearButton;
    QPushButton* m_exportButton;

    // Results display
    QGroupBox* m_resultsGroup;
    QTextEdit* m_resultsText;
    QLabel* m_statusLabel;
    QProgressBar* m_progressBar;

    // Menu and status
    QMenuBar* m_menuBar;
    QStatusBar* m_statusBar;

    // Helper methods
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void setupBasicOpsTab();
    void setupPrimesTab();
    void setupAdvancedTab();
    void setupCryptoTab();
    void setupInputPanel();
    void setupResultsPanel();

    void updateInputFields(AlgorithmType type);
    void displayResult(const AlgorithmResult* result);
    void setInputValidators();
    bool validateCurrentInputs();
    QString formatMathematicalResult(const AlgorithmResult* result) const;

    // Current algorithm context
    AlgorithmType m_currentAlgorithm;
};

#endif // MAINWINDOW_H
