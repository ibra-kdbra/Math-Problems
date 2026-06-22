#include "PrimeSieveWidget.h"
#include <QGraphicsTextItem>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <cmath>
#include <algorithm>

PrimeSieveWidget::PrimeSieveWidget(QWidget* parent)
    : QWidget(parent)
    , m_graphicsView(nullptr)
    , m_scene(nullptr)
    , m_timer(nullptr)
    , m_limit(100)
    , m_currentIndex(2)
    , m_currentPrime(2)
    , m_isRunning(false)
    , m_gridSize(10)
    , m_cellSize(40.0)
{
    setupUI();
    initializeGrid();
}

void PrimeSieveWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Control panel
    QHBoxLayout* controlLayout = new QHBoxLayout;

    QLabel* limitLabel = new QLabel("Limit:");
    m_limitSpinBox = new QSpinBox;
    m_limitSpinBox->setRange(10, 1000);
    m_limitSpinBox->setValue(m_limit);
    m_limitSpinBox->setSingleStep(10);

    m_startButton = new QPushButton("Start");
    m_stopButton = new QPushButton("Stop");
    m_resetButton = new QPushButton("Reset");

    m_stopButton->setEnabled(false);

    controlLayout->addWidget(limitLabel);
    controlLayout->addWidget(m_limitSpinBox);
    controlLayout->addWidget(m_startButton);
    controlLayout->addWidget(m_stopButton);
    controlLayout->addWidget(m_resetButton);
    controlLayout->addStretch();

    mainLayout->addLayout(controlLayout);

    // Graphics view
    m_graphicsView = new QGraphicsView;
    m_scene = new QGraphicsScene(this);
    m_graphicsView->setScene(m_scene);
    m_graphicsView->setMinimumHeight(400);
    m_graphicsView->setRenderHint(QPainter::Antialiasing);

    mainLayout->addWidget(m_graphicsView);

    // Status and progress
    QHBoxLayout* statusLayout = new QHBoxLayout;
    m_statusLabel = new QLabel("Ready");
    m_progressBar = new QProgressBar;
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);

    statusLayout->addWidget(m_statusLabel);
    statusLayout->addWidget(m_progressBar);

    mainLayout->addLayout(statusLayout);

    // Timer for animation
    m_timer = new QTimer(this);
    m_timer->setInterval(TIMER_INTERVAL);

    // Connect signals
    connect(m_startButton, &QPushButton::clicked, this, &PrimeSieveWidget::onStartClicked);
    connect(m_stopButton, &QPushButton::clicked, this, &PrimeSieveWidget::onStopClicked);
    connect(m_resetButton, &QPushButton::clicked, this, &PrimeSieveWidget::onResetClicked);
    connect(m_limitSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &PrimeSieveWidget::onLimitChanged);
    connect(m_timer, &QTimer::timeout, this, &PrimeSieveWidget::onTimerTimeout);
}

void PrimeSieveWidget::initializeGrid()
{
    m_scene->clear();
    m_gridItems.clear();
    m_numberItems.clear();

    // Calculate grid dimensions
    m_gridSize = std::min(static_cast<int>(std::sqrt(m_limit)) + 1, MAX_GRID_SIZE);

    // Initialize prime array
    m_isPrime.assign(m_limit + 1, true);
    m_isPrime[0] = m_isPrime[1] = false;

    // Create grid
    for (int i = 0; i <= m_limit && i < m_gridSize * m_gridSize; ++i) {
        int row = i / m_gridSize;
        int col = i % m_gridSize;

        double x = col * m_cellSize;
        double y = row * m_cellSize;

        // Create cell background
        QGraphicsRectItem* rectItem = m_scene->addRect(x, y, m_cellSize, m_cellSize);
        rectItem->setBrush(QBrush(getNumberColor(i, m_isPrime[i], false)));
        rectItem->setPen(QPen(Qt::black, 1));
        m_gridItems.push_back(rectItem);

        // Create number text
        QGraphicsTextItem* textItem = m_scene->addText(QString::number(i));
        textItem->setPos(x + 5, y + 5);
        textItem->setFont(QFont("Arial", 8));
        m_numberItems.push_back(textItem);
    }

    // Set scene rect
    m_scene->setSceneRect(0, 0, m_gridSize * m_cellSize, m_gridSize * m_cellSize);
}

void PrimeSieveWidget::setLimit(int limit)
{
    if (limit != m_limit) {
        m_limit = limit;
        m_limitSpinBox->setValue(limit);
        initializeGrid();
    }
}

void PrimeSieveWidget::startVisualization()
{
    if (!m_isRunning) {
        m_isRunning = true;
        m_currentIndex = 2;
        m_currentPrime = 2;
        m_startButton->setEnabled(false);
        m_stopButton->setEnabled(true);
        m_limitSpinBox->setEnabled(false);
        m_timer->start();
        m_statusLabel->setText("Running sieve visualization...");
    }
}

void PrimeSieveWidget::stopVisualization()
{
    if (m_isRunning) {
        m_isRunning = false;
        m_timer->stop();
        m_startButton->setEnabled(true);
        m_stopButton->setEnabled(false);
        m_limitSpinBox->setEnabled(true);
        m_statusLabel->setText("Visualization stopped");
    }
}

void PrimeSieveWidget::resetVisualization()
{
    stopVisualization();
    initializeGrid();
    m_progressBar->setValue(0);
    m_statusLabel->setText("Ready");
}

void PrimeSieveWidget::onTimerTimeout()
{
    if (!m_isRunning) return;

    updateVisualization();

    // Update progress
    int progress = (m_currentIndex * 100) / std::sqrt(m_limit);
    m_progressBar->setValue(std::min(progress, 100));

    // Check if we're done
    if (m_currentIndex * m_currentIndex > m_limit) {
        stopVisualization();
        m_statusLabel->setText("Visualization complete");

        // Collect primes
        std::vector<int> primes;
        for (int i = 2; i <= m_limit; ++i) {
            if (m_isPrime[i]) {
                primes.push_back(i);
            }
        }

        emit visualizationComplete(primes);
        return;
    }
}

void PrimeSieveWidget::updateVisualization()
{
    // Find next prime
    while (m_currentIndex * m_currentIndex <= m_limit && !m_isPrime[m_currentIndex]) {
        m_currentIndex++;
    }

    if (m_currentIndex * m_currentIndex <= m_limit) {
        m_currentPrime = m_currentIndex;

        // Highlight current prime
        highlightPrime(m_currentPrime);

        // Mark multiples as composite
        for (int multiple = m_currentPrime * 2; multiple <= m_limit; multiple += m_currentPrime) {
            if (m_isPrime[multiple]) {
                markComposite(multiple, m_currentPrime);
                m_isPrime[multiple] = false;
            }
        }

        m_currentIndex++;
    }
}

void PrimeSieveWidget::markComposite(int multiple, int divisor)
{
    if (multiple < m_gridItems.size()) {
        m_gridItems[multiple]->setBrush(QBrush(QColor(200, 200, 200))); // Gray for composites
        m_numberItems[multiple]->setDefaultTextColor(Qt::black);
    }
}

void PrimeSieveWidget::highlightPrime(int prime)
{
    if (prime < m_gridItems.size()) {
        m_gridItems[prime]->setBrush(QBrush(QColor(100, 200, 100))); // Green for primes
        m_numberItems[prime]->setDefaultTextColor(Qt::white);
    }
}

QColor PrimeSieveWidget::getNumberColor(int number, bool isPrime, bool isCurrent)
{
    if (isCurrent) {
        return QColor(255, 255, 100); // Yellow for current
    } else if (number < 2) {
        return QColor(150, 150, 150); // Gray for 0 and 1
    } else if (isPrime) {
        return QColor(150, 255, 150); // Light green for primes
    } else {
        return QColor(200, 200, 200); // Light gray for composites
    }
}

void PrimeSieveWidget::onStartClicked()
{
    startVisualization();
}

void PrimeSieveWidget::onStopClicked()
{
    stopVisualization();
}

void PrimeSieveWidget::onResetClicked()
{
    resetVisualization();
}

void PrimeSieveWidget::onLimitChanged(int value)
{
    setLimit(value);
}
