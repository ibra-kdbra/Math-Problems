#ifndef PERFORMANCECHARTWIDGET_H
#define PERFORMANCECHARTWIDGET_H

#include <QWidget>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QChart>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QProgressBar>
#include <QTableWidget>
#include <vector>
#include <QElapsedTimer>

struct PerformanceData {
    QString algorithmName;
    qint64 executionTime;
    long long inputSize;
    QString status;
};

class PerformanceChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PerformanceChartWidget(QWidget* parent = nullptr);
    ~PerformanceChartWidget() = default;

public slots:
    void runBenchmark();
    void clearResults();

signals:
    void benchmarkComplete(const QVector<PerformanceData>& results);

private:
    void setupUI();
    void setupChart();
    void updateChart();
    void updateTable();

    // Benchmark methods
    PerformanceData benchmarkGCD(long long n);
    PerformanceData benchmarkPrimality(long long n);
    PerformanceData benchmarkPrimeGeneration(long long limit);
    PerformanceData benchmarkEulerTotient(long long n);
    PerformanceData benchmarkMatrixExponentiation(long long n);

    QChartView* m_chartView;
    QChart* m_chart;
    QBarSeries* m_barSeries;
    QTableWidget* m_resultsTable;

    QLabel* m_statusLabel;
    QPushButton* m_runButton;
    QPushButton* m_clearButton;
    QComboBox* m_algorithmCombo;
    QSpinBox* m_inputSizeSpin;
    QProgressBar* m_progressBar;

    QVector<PerformanceData> m_performanceData;

    static constexpr int MAX_BENCHMARK_SIZE = 1000000;
};

#endif // PERFORMANCECHARTWIDGET_H
