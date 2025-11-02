#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_engine(new NumberTheoryEngine(this))
    , m_currentAlgorithm(AlgorithmType::GCD)
{
    ui->setupUi(this);
    setupUI();
    setupMenuBar();
    setupStatusBar();

    // Connect engine signals
    connect(m_engine, &NumberTheoryEngine::computationFinished,
            this, &MainWindow::onComputationFinished);
    connect(m_engine, &NumberTheoryEngine::computationError,
            this, &MainWindow::onComputationError);

    // Set window properties
    setWindowTitle("Number Theory Explorer");
    setMinimumSize(800, 600);
    resize(1000, 700);

    // Initialize with GCD tab
    updateInputFields(AlgorithmType::GCD);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    // Create central widget
    QWidget* centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Create tab widget
    m_tabWidget = new QTabWidget;
    mainLayout->addWidget(m_tabWidget);

    // Create tabs
    setupBasicOpsTab();
    setupPrimesTab();
    setupAdvancedTab();
    setupCryptoTab();

    // Input and results panels
    QSplitter* splitter = new QSplitter(Qt::Vertical);
    mainLayout->addWidget(splitter);

    setupInputPanel();
    setupResultsPanel();

    splitter->addWidget(m_inputGroup);
    splitter->addWidget(m_resultsGroup);
    splitter->setSizes({200, 400});
}

void MainWindow::setupBasicOpsTab()
{
    m_basicOpsTab = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(m_basicOpsTab);

    // GCD Section
    QGroupBox* gcdGroup = new QGroupBox("Greatest Common Divisor");
    QVBoxLayout* gcdLayout = new QVBoxLayout(gcdGroup);

    QPushButton* gcdButton = new QPushButton("Compute GCD");
    gcdButton->setToolTip("Calculate the Greatest Common Divisor of two integers using Euclidean algorithm");
    QPushButton* extGcdButton = new QPushButton("Extended GCD");
    extGcdButton->setToolTip("Compute GCD with Bézout coefficients (x, y) such that ax + by = gcd(a,b)");

    gcdLayout->addWidget(gcdButton);
    gcdLayout->addWidget(extGcdButton);

    connect(gcdButton, &QPushButton::clicked, this, &MainWindow::computeGCD);
    connect(extGcdButton, &QPushButton::clicked, this, &MainWindow::computeExtendedGCD);

    layout->addWidget(gcdGroup);

    // Basic Modular Arithmetic Section
    QGroupBox* modularGroup = new QGroupBox("Basic Modular Arithmetic");
    QVBoxLayout* modularLayout = new QVBoxLayout(modularGroup);

    QPushButton* modAddButton = new QPushButton("Modular Addition");
    modAddButton->setToolTip("(a + b) mod m");
    QPushButton* modMulButton = new QPushButton("Modular Multiplication");
    modMulButton->setToolTip("(a × b) mod m");

    modularLayout->addWidget(modAddButton);
    modularLayout->addWidget(modMulButton);

    connect(modAddButton, &QPushButton::clicked, [this]() {
        // Placeholder for modular addition
        QMessageBox::information(this, "Feature", "Modular addition will be implemented in Phase 3");
    });
    connect(modMulButton, &QPushButton::clicked, [this]() {
        // Placeholder for modular multiplication
        QMessageBox::information(this, "Feature", "Modular multiplication will be implemented in Phase 3");
    });

    layout->addWidget(modularGroup);
    layout->addStretch();

    m_tabWidget->addTab(m_basicOpsTab, "Basic Operations");
}

void MainWindow::setupPrimesTab()
{
    m_primesTab = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(m_primesTab);

    // Prime Operations
    QGroupBox* primeGroup = new QGroupBox("Prime Operations");
    QVBoxLayout* primeLayout = new QVBoxLayout(primeGroup);

    QPushButton* primalityButton = new QPushButton("Test Primality");
    QPushButton* sieveButton = new QPushButton("Generate Primes (Sieve)");

    primeLayout->addWidget(primalityButton);
    primeLayout->addWidget(sieveButton);

    connect(primalityButton, &QPushButton::clicked, this, &MainWindow::testPrimality);
    connect(sieveButton, &QPushButton::clicked, this, &MainWindow::generatePrimes);

    layout->addWidget(primeGroup);
    layout->addStretch();

    m_tabWidget->addTab(m_primesTab, "Primes");
}

void MainWindow::setupAdvancedTab()
{
    m_advancedTab = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(m_advancedTab);

    // Advanced Functions
    QGroupBox* advancedGroup = new QGroupBox("Advanced Functions");
    QVBoxLayout* advancedLayout = new QVBoxLayout(advancedGroup);

    QPushButton* totientButton = new QPushButton("Euler's Totient");
    QPushButton* matrixButton = new QPushButton("Matrix Exponentiation");
    QPushButton* divisorButton = new QPushButton("Divisor Function");
    QPushButton* lcmButton = new QPushButton("LCM Sum");

    advancedLayout->addWidget(totientButton);
    advancedLayout->addWidget(matrixButton);
    advancedLayout->addWidget(divisorButton);
    advancedLayout->addWidget(lcmButton);

    connect(totientButton, &QPushButton::clicked, this, &MainWindow::computeEulerTotient);
    connect(matrixButton, &QPushButton::clicked, this, &MainWindow::computeMatrixExponentiation);
    connect(divisorButton, &QPushButton::clicked, this, &MainWindow::computeDivisorFunction);
    connect(lcmButton, &QPushButton::clicked, this, &MainWindow::computeLCMSum);

    layout->addWidget(advancedGroup);
    layout->addStretch();

    m_tabWidget->addTab(m_advancedTab, "Advanced");
}

void MainWindow::setupCryptoTab()
{
    m_cryptoTab = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(m_cryptoTab);

    // Cryptographic Functions
    QGroupBox* cryptoGroup = new QGroupBox("Cryptographic Functions");
    QVBoxLayout* cryptoLayout = new QVBoxLayout(cryptoGroup);

    QPushButton* inverseButton = new QPushButton("Modular Inverse");
    QPushButton* expButton = new QPushButton("Modular Exponentiation");
    QPushButton* cubeFreeButton = new QPushButton("Check Cube-Free");

    cryptoLayout->addWidget(inverseButton);
    cryptoLayout->addWidget(expButton);
    cryptoLayout->addWidget(cubeFreeButton);

    connect(inverseButton, &QPushButton::clicked, this, &MainWindow::computeModularInverse);
    connect(expButton, &QPushButton::clicked, this, &MainWindow::computeModularExponentiation);
    connect(cubeFreeButton, &QPushButton::clicked, this, &MainWindow::checkCubeFree);

    layout->addWidget(cryptoGroup);
    layout->addStretch();

    m_tabWidget->addTab(m_cryptoTab, "Cryptography");
}

void MainWindow::setupInputPanel()
{
    m_inputGroup = new QGroupBox("Input Parameters");
    QVBoxLayout* layout = new QVBoxLayout(m_inputGroup);

    // Create input fields
    QFormLayout* formLayout = new QFormLayout;

    m_inputA = new QLineEdit;
    m_inputA->setToolTip("First integer parameter (e.g., for GCD, modular inverse)");
    m_inputB = new QLineEdit;
    m_inputB->setToolTip("Second integer parameter (e.g., for GCD operations)");
    m_inputM = new QLineEdit;
    m_inputM->setToolTip("Modulus parameter (must be > 0 for modular operations)");
    m_inputBase = new QLineEdit;
    m_inputBase->setToolTip("Base value for exponentiation operations");
    m_inputExponent = new QLineEdit;
    m_inputExponent->setToolTip("Exponent value (can be large for fast exponentiation)");
    m_inputModulus = new QLineEdit;
    m_inputModulus->setToolTip("Modulus for modular exponentiation (must be > 0)");
    m_inputN = new QLineEdit;
    m_inputN->setToolTip("Main parameter n (e.g., for primality, totient, etc.)");
    m_inputLimit = new QLineEdit;
    m_inputLimit->setToolTip("Upper limit for prime generation (recommended: ≤ 10,000,000)");

    formLayout->addRow("a:", m_inputA);
    formLayout->addRow("b:", m_inputB);
    formLayout->addRow("m/modulus:", m_inputM);
    formLayout->addRow("base:", m_inputBase);
    formLayout->addRow("exponent:", m_inputExponent);
    formLayout->addRow("modulus:", m_inputModulus);
    formLayout->addRow("n:", m_inputN);
    formLayout->addRow("limit:", m_inputLimit);

    layout->addLayout(formLayout);

    // Control buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    m_computeButton = new QPushButton("Compute");
    m_clearButton = new QPushButton("Clear");
    m_exportButton = new QPushButton("Export Results");

    buttonLayout->addWidget(m_computeButton);
    buttonLayout->addWidget(m_clearButton);
    buttonLayout->addWidget(m_exportButton);

    layout->addLayout(buttonLayout);

    // Connect buttons
    connect(m_computeButton, &QPushButton::clicked, this, [this]() {
        // This will be connected to the appropriate compute function based on current tab
        computeGCD(); // Default
    });
    connect(m_clearButton, &QPushButton::clicked, this, &MainWindow::clearInputs);
    connect(m_exportButton, &QPushButton::clicked, this, &MainWindow::exportResults);

    setInputValidators();
}

void MainWindow::setupResultsPanel()
{
    m_resultsGroup = new QGroupBox("Results");
    QVBoxLayout* layout = new QVBoxLayout(m_resultsGroup);

    m_resultsText = new QTextEdit;
    m_resultsText->setReadOnly(true);
    m_resultsText->setFont(QFont("Courier New", 10));

    layout->addWidget(m_resultsText);

    // Status and progress
    QHBoxLayout* statusLayout = new QHBoxLayout;
    m_statusLabel = new QLabel("Ready");
    m_progressBar = new QProgressBar;
    m_progressBar->setVisible(false);

    statusLayout->addWidget(m_statusLabel);
    statusLayout->addWidget(m_progressBar);

    layout->addLayout(statusLayout);
}

void MainWindow::setupMenuBar()
{
    m_menuBar = menuBar();

    // File menu
    QMenu* fileMenu = m_menuBar->addMenu("&File");
    QAction* exportAction = fileMenu->addAction("&Export Results...");
    QAction* exitAction = fileMenu->addAction("E&xit");

    connect(exportAction, &QAction::triggered, this, &MainWindow::exportResults);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // Help menu
    QMenu* helpMenu = m_menuBar->addMenu("&Help");
    QAction* aboutAction = helpMenu->addAction("&About");
    QAction* aboutQtAction = helpMenu->addAction("About &Qt");

    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::setupStatusBar()
{
    m_statusBar = statusBar();
    m_statusBar->showMessage("Ready");
}

void MainWindow::setInputValidators()
{
    // Use reasonable ranges for QIntValidator (Qt's validator has limitations)
    QIntValidator* intValidator = new QIntValidator(INT_MIN, INT_MAX, this);

    m_inputA->setValidator(intValidator);
    m_inputB->setValidator(intValidator);
    m_inputM->setValidator(intValidator);
    m_inputBase->setValidator(intValidator);
    m_inputExponent->setValidator(intValidator);
    m_inputModulus->setValidator(intValidator);
    m_inputN->setValidator(intValidator);
    m_inputLimit->setValidator(intValidator);
}

void MainWindow::updateInputFields(AlgorithmType type)
{
    m_currentAlgorithm = type;

    // Hide all inputs first
    m_inputA->setVisible(false);
    m_inputB->setVisible(false);
    m_inputM->setVisible(false);
    m_inputBase->setVisible(false);
    m_inputExponent->setVisible(false);
    m_inputModulus->setVisible(false);
    m_inputN->setVisible(false);
    m_inputLimit->setVisible(false);

    // Show relevant inputs based on algorithm
    switch (type) {
        case AlgorithmType::GCD:
        case AlgorithmType::ExtendedGCD:
            m_inputA->setVisible(true);
            m_inputB->setVisible(true);
            break;
        case AlgorithmType::ModularInverse:
            m_inputA->setVisible(true);
            m_inputM->setVisible(true);
            break;
        case AlgorithmType::ModularExponentiation:
            m_inputBase->setVisible(true);
            m_inputExponent->setVisible(true);
            m_inputModulus->setVisible(true);
            break;
        case AlgorithmType::PrimalityTest:
        case AlgorithmType::EulerTotient:
        case AlgorithmType::MatrixExponentiation:
        case AlgorithmType::CubeFreeCheck:
        case AlgorithmType::DivisorFunction:
        case AlgorithmType::LCMSum:
            m_inputN->setVisible(true);
            break;
        case AlgorithmType::PrimeSieve:
            m_inputLimit->setVisible(true);
            break;
    }
}

// Algorithm computation slots
void MainWindow::computeGCD()
{
    m_currentAlgorithm = AlgorithmType::GCD;
    updateInputFields(m_currentAlgorithm);

    long long a, b;
    if (!m_engine->validateInput(m_inputA->text(), a) ||
        !m_engine->validateInput(m_inputB->text(), b)) {
        QMessageBox::warning(this, "Invalid Input", "Please enter valid integer values.");
        return;
    }

    m_statusLabel->setText("Computing GCD...");
    m_progressBar->setVisible(true);
    m_engine->computeGCDAsync(a, b);
}

void MainWindow::computeExtendedGCD()
{
    m_currentAlgorithm = AlgorithmType::ExtendedGCD;
    updateInputFields(m_currentAlgorithm);

    long long a, b;
    if (!m_engine->validateInput(m_inputA->text(), a) ||
        !m_engine->validateInput(m_inputB->text(), b)) {
        QMessageBox::warning(this, "Invalid Input", "Please enter valid integer values.");
        return;
    }

    m_statusLabel->setText("Computing Extended GCD...");
    m_progressBar->setVisible(true);
    m_engine->computeExtendedGCDAsync(a, b);
}

void MainWindow::computeModularInverse()
{
    m_currentAlgorithm = AlgorithmType::ModularInverse;
    updateInputFields(m_currentAlgorithm);

    long long a, m;
    if (!m_engine->validateInput(m_inputA->text(), a) ||
        !m_engine->validateInput(m_inputM->text(), m)) {
        QMessageBox::warning(this, "Invalid Input", "Please enter valid integer values.");
        return;
    }

    m_statusLabel->setText("Computing modular inverse...");
    m_progressBar->setVisible(true);
    m_engine->computeModularInverseAsync(a, m);
}

void MainWindow::computeModularExponentiation()
{
    m_currentAlgorithm = AlgorithmType::ModularExponentiation;
    updateInputFields(m_currentAlgorithm);

    long long base, exponent, modulus;
    if (!m_engine->validateInput(m_inputBase->text(), base) ||
        !m_engine->validateInput(m_inputExponent->text(), exponent) ||
        !m_engine->validateInput(m_inputModulus->text(), modulus)) {
        QMessageBox::warning(this, "Invalid Input", "Please enter valid integer values.");
        return;
    }

    m_statusLabel->setText("Computing modular exponentiation...");
    m_progressBar->setVisible(true);
    m_engine->computeModularExponentiationAsync(base, exponent, modulus);
}

void MainWindow::testPrimality()
{
    m_currentAlgorithm = AlgorithmType::PrimalityTest;
    updateInputFields(m_currentAlgorithm);

    long long n;
    if (!m_engine->validateInput(m_inputN->text(), n, 2)) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid integer ≥ 2.");
        return;
    }

    m_statusLabel->setText("Testing primality...");
    m_progressBar->setVisible(true);
    m_engine->testPrimalityAsync(n);
}

void MainWindow::generatePrimes()
{
    m_currentAlgorithm = AlgorithmType::PrimeSieve;
    updateInputFields(m_currentAlgorithm);

    long long limit;
    if (!m_engine->validateInput(m_inputLimit->text(), limit, 2, 10000000)) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a limit between 2 and 10,000,000.");
        return;
    }

    m_statusLabel->setText("Generating primes...");
    m_progressBar->setVisible(true);
    m_engine->generatePrimesAsync(limit);
}

void MainWindow::computeEulerTotient()
{
    m_currentAlgorithm = AlgorithmType::EulerTotient;
    updateInputFields(m_currentAlgorithm);

    long long n;
    if (!m_engine->validateInput(m_inputN->text(), n, 1)) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid integer ≥ 1.");
        return;
    }

    m_statusLabel->setText("Computing Euler's totient...");
    m_progressBar->setVisible(true);
    m_engine->computeEulerTotientAsync(n);
}

void MainWindow::computeMatrixExponentiation()
{
    m_currentAlgorithm = AlgorithmType::MatrixExponentiation;
    updateInputFields(m_currentAlgorithm);

    long long n;
    if (!m_engine->validateInput(m_inputN->text(), n, 0)) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid integer ≥ 0.");
        return;
    }

    m_statusLabel->setText("Computing matrix exponentiation...");
    m_progressBar->setVisible(true);
    std::vector<long long> coefficients = {1, 1}; // Fibonacci coefficients
    m_engine->computeMatrixExponentiationAsync(coefficients, n);
}

void MainWindow::checkCubeFree()
{
    m_currentAlgorithm = AlgorithmType::CubeFreeCheck;
    updateInputFields(m_currentAlgorithm);

    long long n;
    if (!m_engine->validateInput(m_inputN->text(), n, 1)) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid integer ≥ 1.");
        return;
    }

    m_statusLabel->setText("Checking cube-free...");
    m_progressBar->setVisible(true);
    m_engine->checkCubeFreeAsync(n);
}

void MainWindow::computeDivisorFunction()
{
    m_currentAlgorithm = AlgorithmType::DivisorFunction;
    updateInputFields(m_currentAlgorithm);

    long long n;
    if (!m_engine->validateInput(m_inputN->text(), n, 1)) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid integer ≥ 1.");
        return;
    }

    m_statusLabel->setText("Computing divisor function...");
    m_progressBar->setVisible(true);
    m_engine->computeDivisorFunctionAsync(n);
}

void MainWindow::computeLCMSum()
{
    m_currentAlgorithm = AlgorithmType::LCMSum;
    updateInputFields(m_currentAlgorithm);

    long long n;
    if (!m_engine->validateInput(m_inputN->text(), n, 1)) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid integer ≥ 1.");
        return;
    }

    m_statusLabel->setText("Computing LCM sum...");
    m_progressBar->setVisible(true);
    m_engine->computeLCMSumAsync(n);
}

void MainWindow::clearInputs()
{
    m_inputA->clear();
    m_inputB->clear();
    m_inputM->clear();
    m_inputBase->clear();
    m_inputExponent->clear();
    m_inputModulus->clear();
    m_inputN->clear();
    m_inputLimit->clear();
    m_resultsText->clear();
    m_statusLabel->setText("Ready");
}

void MainWindow::exportResults()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Export Results", "", "Text Files (*.txt);;JSON Files (*.json);;All Files (*)");

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Export Error", "Could not open file for writing.");
        return;
    }

    QTextStream out(&file);
    if (fileName.endsWith(".json")) {
        // Export as JSON (placeholder for now)
        out << "{\n\"results\": \"" << m_resultsText->toPlainText().replace("\n", "\\n") << "\"\n}";
    } else {
        out << m_resultsText->toPlainText();
    }

    file.close();
    QMessageBox::information(this, "Export Complete", "Results exported successfully.");
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, "About Number Theory Explorer",
        "<h2>Number Theory Explorer</h2>"
        "<p>A comprehensive GUI application for exploring number theory algorithms.</p>"
        "<p><b>Version:</b> 1.0.0</p>"
        "<p><b>Built with:</b> Qt6</p>"
        "<p>This application provides interactive access to fundamental number theory algorithms "
        "including GCD, modular arithmetic, prime generation, and advanced functions.</p>");
}

void MainWindow::onComputationFinished(AlgorithmResult* result)
{
    m_progressBar->setVisible(false);
    displayResult(result);
    m_statusLabel->setText("Computation completed");
    // Note: result is managed by the engine, don't delete it here
}

void MainWindow::onComputationError(const QString& error)
{
    m_progressBar->setVisible(false);
    QMessageBox::critical(this, "Computation Error", error);
    m_statusLabel->setText("Error occurred");
}

void MainWindow::displayResult(const AlgorithmResult* result)
{
    if (!result) return;

    QString output;

    // Header with algorithm name and status
    output += "╔══════════════════════════════════════════════════════════════════════════════╗\n";
    output += QString("║ %1 ║\n").arg(result->getAlgorithmName().toUpper().leftJustified(76, ' ', true));
    output += "╠══════════════════════════════════════════════════════════════════════════════╣\n";

    // Status and timing
    QString statusStr = result->getStatusString();
    QString timeStr = QString("%1 ms").arg(result->getExecutionTime());
    QString statusLine = QString("║ Status: %1 Execution Time: %2 ║\n")
        .arg(statusStr.leftJustified(20, ' ', false))
        .arg(timeStr.leftJustified(35, ' ', false));
    output += statusLine;
    output += "╠══════════════════════════════════════════════════════════════════════════════╣\n";

    // Input parameters
    if (!result->getInputParameters().isEmpty()) {
        output += "║ INPUT PARAMETERS:                                                        ║\n";
        const QJsonObject& params = result->getInputParameters();
        for (auto it = params.begin(); it != params.end(); ++it) {
            QString paramKey = it.key() + ":";
            QString paramValue = it.value().toString();
            QString paramLine = QString("║   %1 = %2 ║\n")
                .arg(paramKey.leftJustified(15, ' ', false))
                .arg(paramValue.leftJustified(55, ' ', false));
            output += paramLine;
        }
        output += "╠══════════════════════════════════════════════════════════════════════════════╣\n";
    }

    // Main result with mathematical formatting
    output += "║ RESULT:                                                                   ║\n";
    QString resultStr = formatMathematicalResult(result);
    QStringList resultLines = resultStr.split('\n');
    for (const QString& line : resultLines) {
        output += QString("║ %1 ║\n").arg(line.leftJustified(76, ' ', false));
    }

    // Execution steps (if available)
    const QVector<ExecutionStep>& steps = result->getExecutionSteps();
    if (!steps.isEmpty()) {
        output += "╠══════════════════════════════════════════════════════════════════════════════╣\n";
        output += "║ EXECUTION STEPS:                                                          ║\n";
        for (int i = 0; i < steps.size(); ++i) {
            const ExecutionStep& step = steps[i];
            QString stepLine = QString("%1. %2").arg(i + 1).arg(step.description);
            if (!step.result.isEmpty()) {
                stepLine += QString(" → %1").arg(step.result);
            }
            output += QString("║ %1 ║\n").arg(stepLine.leftJustified(76, ' ', false));
        }
    }

    output += "╚══════════════════════════════════════════════════════════════════════════════╝\n";

    // Add educational note
    output += QString("\n💡 %1\n").arg(m_engine->getAlgorithmDescription(result->getAlgorithmType()));

    m_resultsText->setPlainText(output);

    // Update status bar with execution time
    QString status = QString("Completed in %1 ms").arg(result->getExecutionTime());
    m_statusBar->showMessage(status);
}

QString MainWindow::formatMathematicalResult(const AlgorithmResult* result) const
{
    QString formatted;

    switch (result->getAlgorithmType()) {
        case AlgorithmType::GCD:
            formatted = QString("gcd(%1, %2) = %3")
                .arg(result->getInputParameters()["a"].toString())
                .arg(result->getInputParameters()["b"].toString())
                .arg(result->getMainResult().toLongLong());
            break;

        case AlgorithmType::ExtendedGCD: {
            auto map = result->getMainResult().toMap();
            formatted = QString("gcd(%1, %2) = %3\n")
                .arg(result->getInputParameters()["a"].toString())
                .arg(result->getInputParameters()["b"].toString())
                .arg(map["gcd"].toLongLong());
            formatted += QString("Bézout coefficients: x = %1, y = %2\n")
                .arg(map["x"].toLongLong())
                .arg(map["y"].toLongLong());
            formatted += QString("Verification: %1×%2 + %3×%4 = %5")
                .arg(result->getInputParameters()["a"].toString())
                .arg(map["x"].toString())
                .arg(result->getInputParameters()["b"].toString())
                .arg(map["y"].toString())
                .arg(map["gcd"].toString());
            break;
        }

        case AlgorithmType::ModularInverse:
            formatted = QString("%1⁻¹ ≡ %2 (mod %3)")
                .arg(result->getInputParameters()["a"].toString())
                .arg(result->getMainResult().toLongLong())
                .arg(result->getInputParameters()["m"].toString());
            break;

        case AlgorithmType::ModularExponentiation:
            formatted = QString("%1^%2 ≡ %3 (mod %4)")
                .arg(result->getInputParameters()["base"].toString())
                .arg(result->getInputParameters()["exponent"].toString())
                .arg(result->getMainResult().toString())
                .arg(result->getInputParameters()["modulus"].toString());
            break;

        case AlgorithmType::PrimalityTest:
            formatted = QString("Is %1 prime? %2")
                .arg(result->getInputParameters()["n"].toString())
                .arg(result->getMainResult().toBool() ? "YES ✓" : "NO ✗");
            break;

        case AlgorithmType::PrimeSieve: {
            auto primes = result->getMainResult().toList();
            formatted = QString("Found %1 prime numbers ≤ %2\n")
                .arg(primes.size())
                .arg(result->getInputParameters()["limit"].toString());
            if (primes.size() <= 20) {
                formatted += "Primes: ";
                for (int i = 0; i < primes.size(); ++i) {
                    formatted += primes[i].toString();
                    if (i < primes.size() - 1) formatted += ", ";
                }
            } else {
                formatted += QString("First 10: %1, %2, %3, %4, %5, %6, %7, %8, %9, %10")
                    .arg(primes[0].toString()).arg(primes[1].toString()).arg(primes[2].toString())
                    .arg(primes[3].toString()).arg(primes[4].toString()).arg(primes[5].toString())
                    .arg(primes[6].toString()).arg(primes[7].toString()).arg(primes[8].toString())
                    .arg(primes[9].toString());
                formatted += QString("\nLast 10: %1, %2, %3, %4, %5, %6, %7, %8, %9, %10")
                    .arg(primes[primes.size()-10].toString()).arg(primes[primes.size()-9].toString())
                    .arg(primes[primes.size()-8].toString()).arg(primes[primes.size()-7].toString())
                    .arg(primes[primes.size()-6].toString()).arg(primes[primes.size()-5].toString())
                    .arg(primes[primes.size()-4].toString()).arg(primes[primes.size()-3].toString())
                    .arg(primes[primes.size()-2].toString()).arg(primes[primes.size()-1].toString());
            }
            break;
        }

        case AlgorithmType::EulerTotient:
            formatted = QString("φ(%1) = %2")
                .arg(result->getInputParameters()["n"].toString())
                .arg(result->getMainResult().toLongLong());
            break;

        case AlgorithmType::MatrixExponentiation:
            formatted = QString("F(%1) = %2 (nth Fibonacci number)")
                .arg(result->getInputParameters()["n"].toString())
                .arg(result->getMainResult().toLongLong());
            break;

        case AlgorithmType::CubeFreeCheck:
            formatted = QString("Is %1 cube-free? %2")
                .arg(result->getInputParameters()["n"].toString())
                .arg(result->getMainResult().toBool() ? "YES ✓" : "NO ✗");
            break;

        case AlgorithmType::DivisorFunction:
            formatted = QString("d(%1) = %2 (number of positive divisors)")
                .arg(result->getInputParameters()["n"].toString())
                .arg(result->getMainResult().toLongLong());
            break;

        case AlgorithmType::LCMSum:
            formatted = QString("∑ LCM(i,%1) for i=1 to %1 = %2")
                .arg(result->getInputParameters()["n"].toString())
                .arg(result->getMainResult().toString());
            break;

        default:
            formatted = result->formatResult();
    }

    return formatted;
}
