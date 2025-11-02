#include "NumberTheoryEngine.h"
#include <QElapsedTimer>
#include <QDebug>
#include <QtMath>
#include <algorithm>
#include <cmath>

NumberTheoryEngine::NumberTheoryEngine(QObject* parent)
    : QObject(parent)
{
}

std::unique_ptr<AlgorithmResult> NumberTheoryEngine::computeGCD(long long a, long long b)
{
    auto result = std::make_unique<AlgorithmResult>(AlgorithmType::GCD);

    QJsonObject inputParams;
    inputParams["a"] = a;
    inputParams["b"] = b;
    result->setInputParameters(inputParams);

    QElapsedTimer timer;
    timer.start();

    try {
        result->addExecutionStep("Starting GCD computation", QString("GCD(%1, %2)").arg(a).arg(b));

        long long gcdResult = gcd(a, b);

        result->addExecutionStep("GCD computation completed", QString("Result: %1").arg(gcdResult));
        result->setMainResult(gcdResult);
        result->setExecutionTime(timer.elapsed());

    } catch (const std::exception& e) {
        result->setStatus(ResultStatus::Error);
        result->setErrorMessage(QString("Computation error: %1").arg(e.what()));
    }

    return result;
}

NumberTheoryEngine::BenchmarkResult NumberTheoryEngine::benchmarkAlgorithm(std::function<void()> algorithm, const QString& name)
{
    BenchmarkResult result;
    result.algorithmName = name;

    QElapsedTimer timer;
    timer.start();

    try {
        algorithm();
        result.executionTime = timer.elapsed();
        result.operationsCount = 1; // Placeholder - could be enhanced to count actual operations
    } catch (const std::exception& e) {
        result.executionTime = -1; // Error indicator
        result.operationsCount = 0;
    }

    return result;
}

// Helper methods
void NumberTheoryEngine::addExecutionSteps(AlgorithmResult* result, const QStringList& steps)
{
    for (const QString& step : steps) {
        result->addExecutionStep(step);
    }
}

QJsonObject NumberTheoryEngine::createInputParameters(const QVariantList& params)
{
    QJsonObject inputParams;
    // Implementation for creating input parameters from variant list
    return inputParams;
}

qint64 NumberTheoryEngine::measureExecutionTime(std::function<void()> func)
{
    QElapsedTimer timer;
    timer.start();
    func();
    return timer.elapsed();
}

std::unique_ptr<AlgorithmResult> NumberTheoryEngine::computeExtendedGCD(long long a, long long b)
{
    auto result = std::make_unique<AlgorithmResult>(AlgorithmType::ExtendedGCD);

    QJsonObject inputParams;
    inputParams["a"] = a;
    inputParams["b"] = b;
    result->setInputParameters(inputParams);

    QElapsedTimer timer;
    timer.start();

    try {
        result->addExecutionStep("Starting Extended GCD computation", QString("Extended GCD(%1, %2)").arg(a).arg(b));

        ExtendedGCDResult extResult = extendedGcd(a, b);

        QVariantMap resultMap;
        resultMap["gcd"] = extResult.gcd;
        resultMap["x"] = extResult.x;
        resultMap["y"] = extResult.y;

        result->addExecutionStep("Extended GCD computation completed",
                                QString("GCD: %1, x: %2, y: %3").arg(extResult.gcd).arg(extResult.x).arg(extResult.y));
        result->setMainResult(resultMap);
        result->setExecutionTime(timer.elapsed());

    } catch (const std::exception& e) {
        result->setStatus(ResultStatus::Error);
        result->setErrorMessage(QString("Computation error: %1").arg(e.what()));
    }

    return result;
}

std::unique_ptr<AlgorithmResult> NumberTheoryEngine::computeModularInverse(long long a, long long m)
{
    auto result = std::make_unique<AlgorithmResult>(AlgorithmType::ModularInverse);

    QJsonObject inputParams;
    inputParams["a"] = a;
    inputParams["m"] = m;
    result->setInputParameters(inputParams);

    QElapsedTimer timer;
    timer.start();

    try {
        result->addExecutionStep("Starting modular inverse computation", QString("Inverse of %1 modulo %2").arg(a).arg(m));

        if (gcd(a, m) != 1) {
            result->setStatus(ResultStatus::Error);
            result->setErrorMessage("Numbers are not coprime - inverse doesn't exist");
            return result;
        }

        long long inverse = modularInverse(a, m);

        result->addExecutionStep("Modular inverse computation completed", QString("Result: %1").arg(inverse));
        result->setMainResult(inverse);
        result->setExecutionTime(timer.elapsed());

    } catch (const std::exception& e) {
        result->setStatus(ResultStatus::Error);
        result->setErrorMessage(QString("Computation error: %1").arg(e.what()));
    }

    return result;
}

std::unique_ptr<AlgorithmResult> NumberTheoryEngine::computeModularExponentiation(long long base, long long exponent, long long modulus)
{
    auto result = std::make_unique<AlgorithmResult>(AlgorithmType::ModularExponentiation);

    QJsonObject inputParams;
    inputParams["base"] = base;
    inputParams["exponent"] = exponent;
    inputParams["modulus"] = modulus;
    result->setInputParameters(inputParams);

    QElapsedTimer timer;
    timer.start();

    try {
        result->addExecutionStep("Starting modular exponentiation",
                                QString("%1^%2 mod %3").arg(base).arg(exponent).arg(modulus));

        long long modResult = modularExponentiation(base, exponent, modulus);

        result->addExecutionStep("Modular exponentiation completed", QString("Result: %1").arg(modResult));
        result->setMainResult(modResult);
        result->setExecutionTime(timer.elapsed());

    } catch (const std::exception& e) {
        result->setStatus(ResultStatus::Error);
        result->setErrorMessage(QString("Computation error: %1").arg(e.what()));
    }

    return result;
}

std::unique_ptr<AlgorithmResult> NumberTheoryEngine::testPrimality(long long n)
{
    auto result = std::make_unique<AlgorithmResult>(AlgorithmType::PrimalityTest);

    QJsonObject inputParams;
    inputParams["n"] = n;
    result->setInputParameters(inputParams);

    QElapsedTimer timer;
    timer.start();

    try {
        result->addExecutionStep("Starting primality test", QString("Testing if %1 is prime").arg(n));

        bool isPrimeResult = isPrime(n);

        result->addExecutionStep("Primality test completed", isPrimeResult ? "Number is prime" : "Number is composite");
        result->setMainResult(isPrimeResult);
        result->setExecutionTime(timer.elapsed());

    } catch (const std::exception& e) {
        result->setStatus(ResultStatus::Error);
        result->setErrorMessage(QString("Computation error: %1").arg(e.what()));
    }

    return result;
}

std::unique_ptr<AlgorithmResult> NumberTheoryEngine::generatePrimes(long long limit)
{
    auto result = std::make_unique<AlgorithmResult>(AlgorithmType::PrimeSieve);

    QJsonObject inputParams;
    inputParams["limit"] = limit;
    result->setInputParameters(inputParams);

    QElapsedTimer timer;
    timer.start();

    try {
        result->addExecutionStep("Starting prime generation", QString("Generating primes up to %1").arg(limit));

        auto primes = sieveOfEratosthenes(limit);

        result->addExecutionStep("Prime generation completed", QString("Found %1 primes").arg(primes.size()));

        QVariantList primeList;
        for (auto prime : primes) {
            primeList.append(prime);
        }

        result->setMainResult(primeList);
        result->setExecutionTime(timer.elapsed());

    } catch (const std::exception& e) {
        result->setStatus(ResultStatus::Error);
        result->setErrorMessage(QString("Computation error: %1").arg(e.what()));
    }

    return result;
}

// Core algorithm implementations (ported from existing files)

long long NumberTheoryEngine::gcd(long long a, long long b)
{
    while (b != 0) {
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

ExtendedGCDResult NumberTheoryEngine::extendedGcd(long long a, long long b)
{
    if (b == 0) {
        return {a, 1, 0};
    }

    auto result = extendedGcd(b, a % b);
    long long x = result.y;
    long long y = result.x - (a / b) * result.y;

    return {result.gcd, x, y};
}

long long NumberTheoryEngine::modularInverse(long long a, long long m)
{
    auto result = extendedGcd(a, m);
    if (result.gcd != 1) {
        throw std::runtime_error("Inverse doesn't exist");
    }
    return (result.x % m + m) % m;
}

long long NumberTheoryEngine::modularExponentiation(long long base, long long exponent, long long modulus)
{
    long long result = 1;
    base %= modulus;

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exponent /= 2;
    }

    return result;
}

bool NumberTheoryEngine::isPrime(long long n)
{
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }

    return true;
}

std::vector<long long> NumberTheoryEngine::sieveOfEratosthenes(long long limit)
{
    std::vector<bool> isPrime(limit + 1, true);
    std::vector<long long> primes;

    isPrime[0] = isPrime[1] = false;

    for (long long i = 2; i <= limit; ++i) {
        if (isPrime[i]) {
            primes.push_back(i);
            for (long long j = i * 2; j <= limit; j += i) {
                isPrime[j] = false;
            }
        }
    }

    return primes;
}

std::vector<long long> NumberTheoryEngine::segmentedSieve(long long limit)
{
    if (limit < 2) return {};

    const long long segmentSize = 100000; // Process in segments for memory efficiency
    std::vector<long long> primes;

    // First, generate small primes up to sqrt(limit) using regular sieve
    long long sqrtLimit = std::sqrt(limit) + 1;
    auto smallPrimes = sieveOfEratosthenes(sqrtLimit);
    primes.insert(primes.end(), smallPrimes.begin(), smallPrimes.end());

    // Now process segments
    std::vector<bool> isPrime(segmentSize);
    long long low = sqrtLimit + 1;
    long long high = std::min(low + segmentSize - 1, limit);

    while (low <= limit) {
        // Initialize segment as prime
        std::fill(isPrime.begin(), isPrime.end(), true);

        // Mark multiples of small primes in this segment
        for (long long prime : smallPrimes) {
            long long start = std::max(prime * prime, (low + prime - 1) / prime * prime);
            for (long long j = start; j <= high; j += prime) {
                isPrime[j - low] = false;
            }
        }

        // Collect primes from this segment
        for (long long i = std::max(2LL, low); i <= high; ++i) {
            if (isPrime[i - low]) {
                primes.push_back(i);
            }
        }

        low += segmentSize;
        high = std::min(high + segmentSize, limit);
    }

    return primes;
}

std::vector<std::pair<long long, int>> NumberTheoryEngine::primeFactorization(long long n)
{
    std::vector<std::pair<long long, int>> factors;

    // Handle factor 2
    int count = 0;
    while (n % 2 == 0) {
        count++;
        n /= 2;
    }
    if (count > 0) {
        factors.emplace_back(2, count);
    }

    // Handle odd factors
    for (long long i = 3; i * i <= n; i += 2) {
        count = 0;
        while (n % i == 0) {
            count++;
            n /= i;
        }
        if (count > 0) {
            factors.emplace_back(i, count);
        }
    }

    // Handle remaining prime factor
    if (n > 2) {
        factors.emplace_back(n, 1);
    }

    return factors;
}

long long NumberTheoryEngine::eulerTotient(long long n)
{
    long long result = n;

    for (long long i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            while (n % i == 0) {
                n /= i;
            }
            result -= result / i;
        }
    }

    if (n > 1) {
        result -= result / n;
    }

    return result;
}

long long NumberTheoryEngine::matrixExponentiationFibonacci(long long n)
{
    if (n == 0) return 0;

    std::vector<std::vector<long long>> matrix = {{1, 1}, {1, 0}};
    std::vector<std::vector<long long>> result = {{1, 0}, {0, 1}};

    while (n > 0) {
        if (n % 2 == 1) {
            // Multiply result by matrix
            long long temp[2][2] = {{0, 0}, {0, 0}};
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 2; ++j) {
                    for (int k = 0; k < 2; ++k) {
                        temp[i][j] += result[i][k] * matrix[k][j];
                    }
                }
            }
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 2; ++j) {
                    result[i][j] = temp[i][j];
                }
            }
        }

        // Square the matrix
        long long temp[2][2] = {{0, 0}, {0, 0}};
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    temp[i][j] += matrix[i][k] * matrix[k][j];
                }
            }
        }
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                matrix[i][j] = temp[i][j];
            }
        }

        n /= 2;
    }

    return result[0][1]; // F(n)
}

// Placeholder implementations for remaining algorithms
MatrixResult NumberTheoryEngine::matrixExponentiationGeneral(const std::vector<long long>& coefficients, long long n)
{
    MatrixResult result;
    result.formattedResult = QString("Matrix exponentiation for n=%1").arg(n);
    return result;
}

bool NumberTheoryEngine::isCubeFree(long long n)
{
    for (long long i = 2; i * i * i <= n; ++i) {
        if (n % (i * i * i) == 0) {
            return false;
        }
    }
    return true;
}

long long NumberTheoryEngine::divisorFunction(long long n)
{
    long long sum = 1;
    for (long long i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            long long count = 0;
            while (n % i == 0) {
                n /= i;
                count++;
            }
            sum *= (count + 1);
        }
    }
    if (n > 1) {
        sum *= 2;
    }
    return sum;
}

long long NumberTheoryEngine::lcmSum(long long n)
{
    long long sum = 0;
    for (long long i = 1; i <= n; ++i) {
        sum += n / gcd(i, n);
    }
    return sum;
}

// Async method implementations
void NumberTheoryEngine::computeGCDAsync(long long a, long long b)
{
    QtConcurrent::run([this, a, b]() {
        try {
            auto result = computeGCD(a, b);
            // Store result temporarily to avoid lifetime issues
            static std::unique_ptr<AlgorithmResult> lastResult;
            lastResult = std::move(result);
            emit computationFinished(lastResult.get());
        } catch (const std::exception& e) {
            emit computationError(QString("Async computation error: %1").arg(e.what()));
        }
    });
}

// Similar async implementations for other methods...
void NumberTheoryEngine::computeExtendedGCDAsync(long long a, long long b)
{
    QtConcurrent::run([this, a, b]() {
        try {
            auto result = computeExtendedGCD(a, b);
            static std::unique_ptr<AlgorithmResult> lastResult;
            lastResult = std::move(result);
            emit computationFinished(lastResult.get());
        } catch (const std::exception& e) {
            emit computationError(QString("Async computation error: %1").arg(e.what()));
        }
    });
}

void NumberTheoryEngine::computeModularInverseAsync(long long a, long long m)
{
    QtConcurrent::run([this, a, m]() {
        try {
            auto result = computeModularInverse(a, m);
            static std::unique_ptr<AlgorithmResult> lastResult;
            lastResult = std::move(result);
            emit computationFinished(lastResult.get());
        } catch (const std::exception& e) {
            emit computationError(QString("Async computation error: %1").arg(e.what()));
        }
    });
}

void NumberTheoryEngine::computeModularExponentiationAsync(long long base, long long exponent, long long modulus)
{
    QtConcurrent::run([this, base, exponent, modulus]() {
        try {
            auto result = computeModularExponentiation(base, exponent, modulus);
            static std::unique_ptr<AlgorithmResult> lastResult;
            lastResult = std::move(result);
            emit computationFinished(lastResult.get());
        } catch (const std::exception& e) {
            emit computationError(QString("Async computation error: %1").arg(e.what()));
        }
    });
}

void NumberTheoryEngine::testPrimalityAsync(long long n)
{
    QtConcurrent::run([this, n]() {
        try {
            auto result = testPrimality(n);
            static std::unique_ptr<AlgorithmResult> lastResult;
            lastResult = std::move(result);
            emit computationFinished(lastResult.get());
        } catch (const std::exception& e) {
            emit computationError(QString("Async computation error: %1").arg(e.what()));
        }
    });
}

void NumberTheoryEngine::generatePrimesAsync(long long limit)
{
    QtConcurrent::run([this, limit]() {
        try {
            auto result = generatePrimes(limit);
            static std::unique_ptr<AlgorithmResult> lastResult;
            lastResult = std::move(result);
            emit computationFinished(lastResult.get());
        } catch (const std::exception& e) {
            emit computationError(QString("Async computation error: %1").arg(e.what()));
        }
    });
}

void NumberTheoryEngine::computeEulerTotientAsync(long long n)
{
    QtConcurrent::run([this, n]() {
        try {
            auto result = computeEulerTotient(n);
            static std::unique_ptr<AlgorithmResult> lastResult;
            lastResult = std::move(result);
            emit computationFinished(lastResult.get());
        } catch (const std::exception& e) {
            emit computationError(QString("Async computation error: %1").arg(e.what()));
        }
    });
}

void NumberTheoryEngine::computeMatrixExponentiationAsync(const std::vector<long long>& coefficients, long long n)
{
    QtConcurrent::run([this, coefficients, n]() {
        try {
            auto result = computeMatrixExponentiation(coefficients, n);
            static std::unique_ptr<AlgorithmResult> lastResult;
            lastResult = std::move(result);
            emit computationFinished(lastResult.get());
        } catch (const std::exception& e) {
            emit computationError(QString("Async computation error: %1").arg(e.what()));
        }
    });
}

void NumberTheoryEngine::checkCubeFreeAsync(long long n)
{
    QtConcurrent::run([this, n]() {
        try {
            auto result = checkCubeFree(n);
            static std::unique_ptr<AlgorithmResult> lastResult;
            lastResult = std::move(result);
            emit computationFinished(lastResult.get());
        } catch (const std::exception& e) {
            emit computationError(QString("Async computation error: %1").arg(e.what()));
        }
    });
}

void NumberTheoryEngine::computeDivisorFunctionAsync(long long n)
{
    QtConcurrent::run([this, n]() {
        try {
            auto result = computeDivisorFunction(n);
            static std::unique_ptr<AlgorithmResult> lastResult;
            lastResult = std::move(result);
            emit computationFinished(lastResult.get());
        } catch (const std::exception& e) {
            emit computationError(QString("Async computation error: %1").arg(e.what()));
        }
    });
}

void NumberTheoryEngine::computeLCMSumAsync(long long n)
{
    QtConcurrent::run([this, n]() {
        try {
            auto result = computeLCMSum(n);
            static std::unique_ptr<AlgorithmResult> lastResult;
            lastResult = std::move(result);
            emit computationFinished(lastResult.get());
        } catch (const std::exception& e) {
            emit computationError(QString("Async computation error: %1").arg(e.what()));
        }
    });
}

// Utility method implementations
bool NumberTheoryEngine::validateInput(const QString& input, long long& output, long long min, long long max)
{
    bool ok;
    output = input.toLongLong(&ok);
    return ok && output >= min && output <= max;
}

QString NumberTheoryEngine::formatLargeNumber(long long number)
{
    QString numStr = QString::number(number);
    QString result;

    int len = numStr.length();
    for (int i = 0; i < len; ++i) {
        if (i > 0 && (len - i) % 3 == 0) {
            result += ",";
        }
        result += numStr[i];
    }

    return result;
}

QString NumberTheoryEngine::getAlgorithmDescription(AlgorithmType type)
{
    switch (type) {
        case AlgorithmType::GCD:
            return "Computes the Greatest Common Divisor of two integers using the Euclidean algorithm.";
        case AlgorithmType::ExtendedGCD:
            return "Computes GCD and the coefficients of Bézout's identity using the extended Euclidean algorithm.";
        case AlgorithmType::ModularInverse:
            return "Computes the modular multiplicative inverse using the extended Euclidean algorithm.";
        case AlgorithmType::ModularExponentiation:
            return "Computes (base^exponent) mod modulus using fast exponentiation.";
        case AlgorithmType::PrimalityTest:
            return "Tests whether a number is prime using trial division.";
        case AlgorithmType::PrimeSieve:
            return "Generates all prime numbers up to a limit using the Sieve of Eratosthenes.";
        case AlgorithmType::EulerTotient:
            return "Computes Euler's Totient function φ(n) - the count of integers up to n that are coprime to n.";
        case AlgorithmType::MatrixExponentiation:
            return "Computes linear recurrence sequences using matrix exponentiation.";
        case AlgorithmType::CubeFreeCheck:
            return "Checks if a number is cube-free (not divisible by any cube > 1).";
        case AlgorithmType::DivisorFunction:
            return "Computes the divisor function d(n) - the number of positive divisors of n.";
        case AlgorithmType::LCMSum:
            return "Computes the sum of LCM(i,n) for i from 1 to n.";
        default:
            return "Unknown algorithm.";
    }
}

// Placeholder implementations for remaining synchronous methods
std::unique_ptr<AlgorithmResult> NumberTheoryEngine::computeEulerTotient(long long n)
{
    auto result = std::make_unique<AlgorithmResult>(AlgorithmType::EulerTotient);

    QJsonObject inputParams;
    inputParams["n"] = n;
    result->setInputParameters(inputParams);

    QElapsedTimer timer;
    timer.start();

    try {
        long long totient = eulerTotient(n);
        result->setMainResult(totient);
        result->setExecutionTime(timer.elapsed());
    } catch (const std::exception& e) {
        result->setStatus(ResultStatus::Error);
        result->setErrorMessage(QString("Computation error: %1").arg(e.what()));
    }

    return result;
}

std::unique_ptr<AlgorithmResult> NumberTheoryEngine::computeMatrixExponentiation(const std::vector<long long>& coefficients, long long n)
{
    auto result = std::make_unique<AlgorithmResult>(AlgorithmType::MatrixExponentiation);

    QJsonObject inputParams;
    inputParams["n"] = n;
    result->setInputParameters(inputParams);

    QElapsedTimer timer;
    timer.start();

    try {
        long long fibResult = matrixExponentiationFibonacci(n);
        result->setMainResult(fibResult);
        result->setExecutionTime(timer.elapsed());
    } catch (const std::exception& e) {
        result->setStatus(ResultStatus::Error);
        result->setErrorMessage(QString("Computation error: %1").arg(e.what()));
    }

    return result;
}

std::unique_ptr<AlgorithmResult> NumberTheoryEngine::checkCubeFree(long long n)
{
    auto result = std::make_unique<AlgorithmResult>(AlgorithmType::CubeFreeCheck);

    QJsonObject inputParams;
    inputParams["n"] = n;
    result->setInputParameters(inputParams);

    QElapsedTimer timer;
    timer.start();

    try {
        bool cubeFree = isCubeFree(n);
        result->setMainResult(cubeFree);
        result->setExecutionTime(timer.elapsed());
    } catch (const std::exception& e) {
        result->setStatus(ResultStatus::Error);
        result->setErrorMessage(QString("Computation error: %1").arg(e.what()));
    }

    return result;
}

std::unique_ptr<AlgorithmResult> NumberTheoryEngine::computeDivisorFunction(long long n)
{
    auto result = std::make_unique<AlgorithmResult>(AlgorithmType::DivisorFunction);

    QJsonObject inputParams;
    inputParams["n"] = n;
    result->setInputParameters(inputParams);

    QElapsedTimer timer;
    timer.start();

    try {
        long long divisorSum = divisorFunction(n);
        result->setMainResult(divisorSum);
        result->setExecutionTime(timer.elapsed());
    } catch (const std::exception& e) {
        result->setStatus(ResultStatus::Error);
        result->setErrorMessage(QString("Computation error: %1").arg(e.what()));
    }

    return result;
}

std::unique_ptr<AlgorithmResult> NumberTheoryEngine::computeLCMSum(long long n)
{
    auto result = std::make_unique<AlgorithmResult>(AlgorithmType::LCMSum);

    QJsonObject inputParams;
    inputParams["n"] = n;
    result->setInputParameters(inputParams);

    QElapsedTimer timer;
    timer.start();

    try {
        long long lcmSumResult = lcmSum(n);
        result->setMainResult(lcmSumResult);
        result->setExecutionTime(timer.elapsed());
    } catch (const std::exception& e) {
        result->setStatus(ResultStatus::Error);
        result->setErrorMessage(QString("Computation error: %1").arg(e.what()));
    }

    return result;
}
