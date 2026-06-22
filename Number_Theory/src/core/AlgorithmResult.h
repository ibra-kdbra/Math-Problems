#ifndef ALGORITHMRESULT_H
#define ALGORITHMRESULT_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVector>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>

enum class AlgorithmType {
    GCD,
    ExtendedGCD,
    ModularInverse,
    ModularExponentiation,
    PrimalityTest,
    PrimeSieve,
    EulerTotient,
    MatrixExponentiation,
    CubeFreeCheck,
    DivisorFunction,
    LCMSum
};

enum class ResultStatus {
    Success,
    Error,
    Timeout,
    InvalidInput
};

struct ExecutionStep {
    Q_GADGET
    Q_PROPERTY(QString description MEMBER description)
    Q_PROPERTY(QString result MEMBER result)
    Q_PROPERTY(qint64 timestamp MEMBER timestamp)

public:
    QString description;
    QString result;
    qint64 timestamp;
};

class AlgorithmResult
{
public:
    AlgorithmResult(AlgorithmType type = AlgorithmType::GCD);
    ~AlgorithmResult() = default;

    // Setters
    void setStatus(ResultStatus status);
    void setMainResult(const QVariant& result);
    void setErrorMessage(const QString& message);
    void setExecutionTime(qint64 milliseconds);
    void addExecutionStep(const QString& description, const QString& result = QString());
    void setInputParameters(const QJsonObject& params);

    // Getters
    Q_INVOKABLE AlgorithmType getAlgorithmType() const;
    Q_INVOKABLE ResultStatus getStatus() const;
    Q_INVOKABLE QVariant getMainResult() const;
    Q_INVOKABLE QString getErrorMessage() const;
    Q_INVOKABLE qint64 getExecutionTime() const;
    Q_INVOKABLE QVector<ExecutionStep> getExecutionSteps() const;
    Q_INVOKABLE QJsonObject getInputParameters() const;

    // Utility methods
    QString getAlgorithmName() const;
    QString getStatusString() const;
    QString formatResult() const;
    QJsonObject toJson() const;
    QString toFormattedString() const;

    // Static utility methods
    static QString algorithmTypeToString(AlgorithmType type);
    static QString resultStatusToString(ResultStatus status);

private:
    AlgorithmType m_algorithmType;
    ResultStatus m_status;
    QVariant m_mainResult;
    QString m_errorMessage;
    qint64 m_executionTime;
    QVector<ExecutionStep> m_executionSteps;
    QJsonObject m_inputParameters;
    QDateTime m_startTime;
};

#endif // ALGORITHMRESULT_H
