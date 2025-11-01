#ifndef FACTORIZATIONTREEWIDGET_H
#define FACTORIZATIONTREEWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <vector>
#include <queue>

struct FactorNode {
    long long value;
    long long factor;
    QGraphicsEllipseItem* ellipse;
    QGraphicsTextItem* text;
    FactorNode* left;
    FactorNode* right;

    FactorNode(long long val, long long fac = 1)
        : value(val), factor(fac), ellipse(nullptr), text(nullptr), left(nullptr), right(nullptr) {}
};

class FactorizationTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FactorizationTreeWidget(QWidget* parent = nullptr);
    ~FactorizationTreeWidget();

    void setNumber(long long number);
    void buildFactorizationTree();
    void clearTree();

signals:
    void factorizationComplete(const std::vector<std::pair<long long, int>>& factors);

private:
    void setupUI();
    void drawTree();
    void drawNode(FactorNode* node, int x, int y, int level);
    void positionNodes(FactorNode* node, int& x, int y, int levelSpacing);
    int calculateTreeWidth(FactorNode* node);
    void cleanupTree(FactorNode* node);

    QGraphicsView* m_graphicsView;
    QGraphicsScene* m_scene;

    QLabel* m_numberLabel;
    QLineEdit* m_numberInput;
    QPushButton* m_factorizeButton;
    QPushButton* m_clearButton;

    FactorNode* m_root;
    long long m_currentNumber;

    static constexpr int NODE_RADIUS = 30;
    static constexpr int LEVEL_HEIGHT = 80;
    static constexpr int HORIZONTAL_SPACING = 60;
};

#endif // FACTORIZATIONTREEWIDGET_H
