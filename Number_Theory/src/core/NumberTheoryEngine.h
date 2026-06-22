#ifndef NUMBERTHEORYENGINE_H
#define NUMBERTHEORYENGINE_H

#include "AlgorithmResult.h"
#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include <functional>
#include <memory>

// Forward declarations for algorithm result types
struct ExtendedGCDResult {
    long long gcd;
    long long x;
    long long y;
};

struct MatrixResult {
    std::vector<std::vector<long long>> matrix;
    QString formattedResult;
};

class NumberTheoryEngine : public QObject
{
    Q_OBJECT

public:
    explicit NumberTheoryEngine(QObject* parent = nullptr);
    ~NumberTheoryEngine() = default;

    // Synchronous algorithm execution
    std::unique_ptr<AlgorithmResult> computeGCD(long long a, long long b);
    std::unique_ptr<AlgorithmResult> computeExtendedGCD(long long a, long long b);
    std::unique_ptr<AlgorithmResult> computeModularInverse(long long a, long long m);
    std::unique_ptr<AlgorithmResult> computeModularExponentiation(long long base, long long exponent, long long modulus);
    std::unique_ptr<AlgorithmResult> testPrimality(long long n);
    std::unique_ptr<AlgorithmResult> generatePrimes(long long limit);
    std::unique_ptr<AlgorithmResult> computeEulerTotient(long long n);
    std::unique_ptr<AlgorithmResult> computeMatrixExponentiation(const std::vector<long long>& coefficients, long long n);
    std::unique_ptr<AlgorithmResult> checkCubeFree(long long n);
    std::unique_ptr<AlgorithmResult> computeDivisorFunction(long long n);
    std::unique_ptr<AlgorithmResult> computeLCMSum(long long n);

    // Asynchronous execution with signals
    Q_INVOKABLE void computeGCDAsync(long long a, long long b);
    Q_INVOKABLE void computeExtendedGCDAsync(long long a, long long b);
    Q_INVOKABLE void computeModularInverseAsync(long long a, long long m);
    Q_INVOKABLE void computeModularExponentiationAsync(long long base, long long exponent, long long modulus);
    Q_INVOKABLE void testPrimalityAsync(long long n);
    Q_INVOKABLE void generatePrimesAsync(long long limit);
    Q_INVOKABLE void computeEulerTotientAsync(long long n);
    Q_INVOKABLE void computeMatrixExponentiationAsync(const std::vector<long long>& coefficients, long long n);
    Q_INVOKABLE void checkCubeFreeAsync(long long n);
    Q_INVOKABLE void computeDivisorFunctionAsync(long long n);
    Q_INVOKABLE void computeLCMSumAsync(long long n);

    // Utility methods
    bool validateInput(const QString& input, long long& output, long long min = LLONG_MIN, long long max = LLONG_MAX);
    QString formatLargeNumber(long long number);
    QString getAlgorithmDescription(AlgorithmType type);

signals:
    void computationFinished(const QString& algorithmName, const QString& status, qint64 executionTime,
                           const QJsonObject& inputParams, const QVariant& mainResult,
                           const QVector<ExecutionStep>& steps, int algorithmType);
    void computationError(const QString& error);
    void progressUpdated(int percentage, const QString& message);

private:
    // Core algorithm implementations (ported from existing C++ files)
    long long gcd(long long a, long long b);
    ExtendedGCDResult extendedGcd(long long a, long long b);
    long long modularInverse(long long a, long long m);
    long long modularExponentiation(long long base, long long exponent, long long modulus);
    bool isPrime(long long n);
    std::vector<long long> sieveOfEratosthenes(long long limit);
    std::vector<long long> segmentedSieve(long long limit);
    std::vector<std::pair<long long, int>> primeFactorization(long long n);
    long long eulerTotient(long long n);
    long long matrixExponentiationFibonacci(long long n);
    MatrixResult matrixExponentiationGeneral(const std::vector<long long>& coefficients, long long n);
    bool isCubeFree(long long n);
    long long divisorFunction(long long n);
    long long lcmSum(long long n);

    // Performance benchmarking
    struct BenchmarkResult {
        qint64 executionTime;
        long long operationsCount;
        QString algorithmName;
    };
    BenchmarkResult benchmarkAlgorithm(std::function<void()> algorithm, const QString& name);

    // Helper methods
    void addExecutionSteps(AlgorithmResult* result, const QStringList& steps);
    QJsonObject createInputParameters(const QVariantList& params);
    qint64 measureExecutionTime(std::function<void()> func);

    // Constants
    static constexpr long long MAX_PRIME_LIMIT = 10000000; // 10 million
    static constexpr long long MAX_MATRIX_SIZE = 1000;
    static constexpr int TIMEOUT_SECONDS = 30;
};

#endif // NUMBERTHEORYENGINE_H
