#include "FactorizationTreeWidget.h"
#include <QFont>
#include <QBrush>
#include <QPen>
#include <cmath>
#include <algorithm>

FactorizationTreeWidget::FactorizationTreeWidget(QWidget* parent)
    : QWidget(parent)
    , m_graphicsView(nullptr)
    , m_scene(nullptr)
    , m_root(nullptr)
    , m_currentNumber(0)
{
    setupUI();
}

FactorizationTreeWidget::~FactorizationTreeWidget()
{
    clearTree();
}

void FactorizationTreeWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Control panel
    QHBoxLayout* controlLayout = new QHBoxLayout;

    m_numberLabel = new QLabel("Number to factorize:");
    m_numberInput = new QLineEdit;
    m_numberInput->setText("60");
    m_numberInput->setMaximumWidth(100);

    m_factorizeButton = new QPushButton("Factorize");
    m_clearButton = new QPushButton("Clear");

    controlLayout->addWidget(m_numberLabel);
    controlLayout->addWidget(m_numberInput);
    controlLayout->addWidget(m_factorizeButton);
    controlLayout->addWidget(m_clearButton);
    controlLayout->addStretch();

    mainLayout->addLayout(controlLayout);

    // Graphics view
    m_graphicsView = new QGraphicsView;
    m_scene = new QGraphicsScene(this);
    m_graphicsView->setScene(m_scene);
    m_graphicsView->setMinimumHeight(400);
    m_graphicsView->setRenderHint(QPainter::Antialiasing);

    mainLayout->addWidget(m_graphicsView);

    // Connect signals
    connect(m_factorizeButton, &QPushButton::clicked, this, &FactorizationTreeWidget::buildFactorizationTree);
    connect(m_clearButton, &QPushButton::clicked, this, &FactorizationTreeWidget::clearTree);
}

void FactorizationTreeWidget::setNumber(long long number)
{
    m_currentNumber = number;
    m_numberInput->setText(QString::number(number));
}

void FactorizationTreeWidget::buildFactorizationTree()
{
    bool ok;
    long long number = m_numberInput->text().toLongLong(&ok);

    if (!ok || number < 2) {
        // Invalid input
        return;
    }

    m_currentNumber = number;
    clearTree();

    // Build the factorization tree
    m_root = buildFactorTree(number);

    // Draw the tree
    drawTree();

    // Emit completion signal with prime factors
    std::vector<std::pair<long long, int>> factors;
    collectPrimeFactors(m_root, factors);
    emit factorizationComplete(factors);
}

FactorNode* FactorizationTreeWidget::buildFactorTree(long long n)
{
    if (n <= 1) return nullptr;

    // Check if n is prime
    bool isPrime = true;
    for (long long i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            isPrime = false;
            break;
        }
    }

    if (isPrime) {
        return new FactorNode(n, n); // Leaf node with prime factor
    }

    // Find smallest prime factor
    long long factor = 2;
    while (n % factor != 0) {
        factor++;
    }

    long long quotient = n / factor;

    FactorNode* node = new FactorNode(n, factor);
    node->left = new FactorNode(factor, factor); // Prime factor
    node->right = buildFactorTree(quotient); // Continue with quotient

    return node;
}

void FactorizationTreeWidget::collectPrimeFactors(FactorNode* node, std::vector<std::pair<long long, int>>& factors)
{
    if (!node) return;

    if (!node->left && !node->right) {
        // Leaf node - count this prime factor
        auto it = std::find_if(factors.begin(), factors.end(),
                              [node](const std::pair<long long, int>& p) {
                                  return p.first == node->factor;
                              });

        if (it != factors.end()) {
            it->second++;
        } else {
            factors.emplace_back(node->factor, 1);
        }
    } else {
        collectPrimeFactors(node->left, factors);
        collectPrimeFactors(node->right, factors);
    }
}

void FactorizationTreeWidget::drawTree()
{
    if (!m_root) return;

    m_scene->clear();

    // Calculate positions and draw
    int startX = 400; // Center of scene
    int startY = 50;

    drawNode(m_root, startX, startY, 0);
}

void FactorizationTreeWidget::drawNode(FactorNode* node, int x, int y, int level)
{
    if (!node) return;

    // Create ellipse for node
    node->ellipse = m_scene->addEllipse(x - NODE_RADIUS, y - NODE_RADIUS,
                                       NODE_RADIUS * 2, NODE_RADIUS * 2);
    node->ellipse->setBrush(QBrush(QColor(100, 150, 255)));
    node->ellipse->setPen(QPen(Qt::black, 2));

    // Create text for node value
    node->text = m_scene->addText(QString::number(node->value));
    node->text->setFont(QFont("Arial", 10, QFont::Bold));
    node->text->setDefaultTextColor(Qt::white);

    // Center text in ellipse
    QRectF ellipseRect = node->ellipse->rect();
    QRectF textRect = node->text->boundingRect();
    node->text->setPos(x - textRect.width()/2, y - textRect.height()/2);

    // Draw factor label if not a leaf
    if (node->left && node->right) {
        QGraphicsTextItem* factorText = m_scene->addText(QString("÷%1").arg(node->factor));
        factorText->setFont(QFont("Arial", 8));
        factorText->setDefaultTextColor(Qt::black);
        factorText->setPos(x - 20, y + NODE_RADIUS + 5);
    }

    // Draw connections to children
    if (node->left) {
        int childY = y + LEVEL_HEIGHT;
        int leftX = x - HORIZONTAL_SPACING * (1 << (3 - level)); // Exponential spacing decrease

        // Draw line to left child
        m_scene->addLine(x, y + NODE_RADIUS, leftX, childY - NODE_RADIUS,
                       QPen(Qt::black, 2));

        drawNode(node->left, leftX, childY, level + 1);
    }

    if (node->right) {
        int childY = y + LEVEL_HEIGHT;
        int rightX = x + HORIZONTAL_SPACING * (1 << (3 - level));

        // Draw line to right child
        m_scene->addLine(x, y + NODE_RADIUS, rightX, childY - NODE_RADIUS,
                       QPen(Qt::black, 2));

        drawNode(node->right, rightX, childY, level + 1);
    }
}

void FactorizationTreeWidget::clearTree()
{
    if (m_root) {
        cleanupTree(m_root);
        m_root = nullptr;
    }
    m_scene->clear();
}

void FactorizationTreeWidget::cleanupTree(FactorNode* node)
{
    if (!node) return;

    cleanupTree(node->left);
    cleanupTree(node->right);

    delete node;
}
