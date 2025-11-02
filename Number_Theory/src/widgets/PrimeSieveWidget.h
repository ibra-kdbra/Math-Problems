#ifndef PRIMESIEVEWIDGET_H
#define PRIMESIEVEWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QProgressBar>
#include <vector>

class PrimeSieveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PrimeSieveWidget(QWidget* parent = nullptr);
    ~PrimeSieveWidget() = default;

    void setLimit(int limit);
    void startVisualization();
    void stopVisualization();
    void resetVisualization();

signals:
    void visualizationComplete(const std::vector<int>& primes);

private slots:
    void onTimerTimeout();
    void onStartClicked();
    void onStopClicked();
    void onResetClicked();
    void onLimitChanged(int value);

private:
    void setupUI();
    void initializeGrid();
    void updateVisualization();
    void markComposite(int multiple, int divisor);
    void highlightPrime(int prime);
    QColor getNumberColor(int number, bool isPrime, bool isCurrent);

    QGraphicsView* m_graphicsView;
    QGraphicsScene* m_scene;
    QTimer* m_timer;

    QLabel* m_statusLabel;
    QPushButton* m_startButton;
    QPushButton* m_stopButton;
    QPushButton* m_resetButton;
    QSpinBox* m_limitSpinBox;
    QProgressBar* m_progressBar;

    std::vector<bool> m_isPrime;
    std::vector<QGraphicsRectItem*> m_gridItems;
    std::vector<QGraphicsTextItem*> m_numberItems;

    int m_limit;
    int m_currentIndex;
    int m_currentPrime;
    bool m_isRunning;
    int m_gridSize;
    double m_cellSize;

    static constexpr int MAX_GRID_SIZE = 50; // Maximum grid dimension for visualization
    static constexpr int TIMER_INTERVAL = 100; // milliseconds
};

#endif // PRIMESIEVEWIDGET_H
