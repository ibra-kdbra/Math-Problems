#include "AlgorithmResult.h"
#include <QJsonArray>

AlgorithmResult::AlgorithmResult(AlgorithmType type)
    : m_algorithmType(type)
    , m_status(ResultStatus::Success)
    , m_executionTime(0)
    , m_startTime(QDateTime::currentDateTime())
{
}

void AlgorithmResult::setStatus(ResultStatus status)
{
    m_status = status;
}

void AlgorithmResult::setMainResult(const QVariant& result)
{
    m_mainResult = result;
}

void AlgorithmResult::setErrorMessage(const QString& message)
{
    m_errorMessage = message;
    if (!message.isEmpty()) {
        m_status = ResultStatus::Error;
    }
}

void AlgorithmResult::setExecutionTime(qint64 milliseconds)
{
    m_executionTime = milliseconds;
}

void AlgorithmResult::addExecutionStep(const QString& description, const QString& result)
{
    ExecutionStep step;
    step.description = description;
    step.result = result;
    step.timestamp = QDateTime::currentMSecsSinceEpoch();
    m_executionSteps.append(step);
}

void AlgorithmResult::setInputParameters(const QJsonObject& params)
{
    m_inputParameters = params;
}

AlgorithmType AlgorithmResult::getAlgorithmType() const
{
    return m_algorithmType;
}

ResultStatus AlgorithmResult::getStatus() const
{
    return m_status;
}

QVariant AlgorithmResult::getMainResult() const
{
    return m_mainResult;
}

QString AlgorithmResult::getErrorMessage() const
{
    return m_errorMessage;
}

qint64 AlgorithmResult::getExecutionTime() const
{
    return m_executionTime;
}

QVector<ExecutionStep> AlgorithmResult::getExecutionSteps() const
{
    return m_executionSteps;
}

QJsonObject AlgorithmResult::getInputParameters() const
{
    return m_inputParameters;
}

QString AlgorithmResult::getAlgorithmName() const
{
    return algorithmTypeToString(m_algorithmType);
}

QString AlgorithmResult::getStatusString() const
{
    return resultStatusToString(m_status);
}

QString AlgorithmResult::formatResult() const
{
    if (m_status != ResultStatus::Success) {
        return QString("Error: %1").arg(m_errorMessage);
    }

    QString result;
    switch (m_algorithmType) {
        case AlgorithmType::GCD:
            result = QString("GCD = %1").arg(m_mainResult.toLongLong());
            break;
        case AlgorithmType::ExtendedGCD:
            if (m_mainResult.type() == QVariant::Map) {
                auto map = m_mainResult.toMap();
                result = QString("GCD = %1, x = %2, y = %3")
                        .arg(map["gcd"].toLongLong())
                        .arg(map["x"].toLongLong())
                        .arg(map["y"].toLongLong());
            }
            break;
        case AlgorithmType::ModularInverse:
            result = QString("Inverse = %1").arg(m_mainResult.toLongLong());
            break;
        case AlgorithmType::ModularExponentiation:
            result = QString("Result = %1").arg(m_mainResult.toString());
            break;
        case AlgorithmType::PrimalityTest:
            result = m_mainResult.toBool() ? "Prime" : "Composite";
            break;
        case AlgorithmType::PrimeSieve:
            if (m_mainResult.type() == QVariant::List) {
                auto list = m_mainResult.toList();
                result = QString("Found %1 primes").arg(list.size());
            }
            break;
        case AlgorithmType::EulerTotient:
            result = QString("φ(n) = %1").arg(m_mainResult.toLongLong());
            break;
        case AlgorithmType::MatrixExponentiation:
            result = QString("Result = %1").arg(m_mainResult.toString());
            break;
        case AlgorithmType::CubeFreeCheck:
            result = m_mainResult.toBool() ? "Cube Free" : "Not Cube Free";
            break;
        case AlgorithmType::DivisorFunction:
            result = QString("Sum = %1").arg(m_mainResult.toString());
            break;
        case AlgorithmType::LCMSum:
            result = QString("Sum = %1").arg(m_mainResult.toString());
            break;
        default:
            result = m_mainResult.toString();
    }

    return result;
}

QJsonObject AlgorithmResult::toJson() const
{
    QJsonObject json;
    json["algorithm"] = algorithmTypeToString(m_algorithmType);
    json["status"] = resultStatusToString(m_status);
    json["executionTime"] = m_executionTime;
    json["timestamp"] = m_startTime.toString(Qt::ISODate);

    if (m_status == ResultStatus::Success) {
        json["result"] = QJsonValue::fromVariant(m_mainResult);
    } else {
        json["error"] = m_errorMessage;
    }

    json["input"] = m_inputParameters;

    QJsonArray stepsArray;
    for (const auto& step : m_executionSteps) {
        QJsonObject stepObj;
        stepObj["description"] = step.description;
        stepObj["result"] = step.result;
        stepObj["timestamp"] = step.timestamp;
        stepsArray.append(stepObj);
    }
    json["steps"] = stepsArray;

    return json;
}

QString AlgorithmResult::toFormattedString() const
{
    QString output = QString("=== %1 ===\n").arg(getAlgorithmName());
    output += QString("Status: %1\n").arg(getStatusString());
    output += QString("Execution Time: %1 ms\n").arg(m_executionTime);

    if (!m_inputParameters.isEmpty()) {
        output += "Input Parameters:\n";
        for (auto it = m_inputParameters.begin(); it != m_inputParameters.end(); ++it) {
            output += QString("  %1: %2\n").arg(it.key(), it.value().toString());
        }
    }

    output += "\nResult: " + formatResult() + "\n";

    if (!m_executionSteps.isEmpty()) {
        output += "\nExecution Steps:\n";
        for (int i = 0; i < m_executionSteps.size(); ++i) {
            const auto& step = m_executionSteps[i];
            output += QString("%1. %2").arg(i + 1).arg(step.description);
            if (!step.result.isEmpty()) {
                output += QString(" → %1").arg(step.result);
            }
            output += "\n";
        }
    }

    return output;
}

QString AlgorithmResult::algorithmTypeToString(AlgorithmType type)
{
    switch (type) {
        case AlgorithmType::GCD: return "Greatest Common Divisor";
        case AlgorithmType::ExtendedGCD: return "Extended Euclidean Algorithm";
        case AlgorithmType::ModularInverse: return "Modular Inverse";
        case AlgorithmType::ModularExponentiation: return "Modular Exponentiation";
        case AlgorithmType::PrimalityTest: return "Primality Test";
        case AlgorithmType::PrimeSieve: return "Prime Sieve";
        case AlgorithmType::EulerTotient: return "Euler's Totient Function";
        case AlgorithmType::MatrixExponentiation: return "Matrix Exponentiation";
        case AlgorithmType::CubeFreeCheck: return "Cube-Free Number Check";
        case AlgorithmType::DivisorFunction: return "Divisor Function";
        case AlgorithmType::LCMSum: return "LCM Sum";
        default: return "Unknown Algorithm";
    }
}

QString AlgorithmResult::resultStatusToString(ResultStatus status)
{
    switch (status) {
        case ResultStatus::Success: return "Success";
        case ResultStatus::Error: return "Error";
        case ResultStatus::Timeout: return "Timeout";
        case ResultStatus::InvalidInput: return "Invalid Input";
        default: return "Unknown";
    }
}
