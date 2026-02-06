// pidcontroller.cpp
#include "pidcontroller.h"
#include <QDateTime>
#include <QDebug>
#include <algorithm>

PIDController::PIDController(QObject *parent)
    : QObject(parent)
    , m_kp(1.0)
    , m_ki(0.0)
    , m_kd(0.0)
    , m_integral(0.0)
    , m_previousError(0.0)
    , m_outputMin(-100.0)
    , m_outputMax(100.0)
    , m_integralMax(100.0)
    , m_setpoint(0.0)
    , m_lastUpdateTime(QDateTime::currentMSecsSinceEpoch())
{
}

void PIDController::setParameters(double kp, double ki, double kd)
{
    m_kp = kp;
    m_ki = ki;
    m_kd = kd;
}

void PIDController::setOutputLimits(double min, double max)
{
    m_outputMin = min;
    m_outputMax = max;
}

void PIDController::setIntegralLimit(double max)
{
    m_integralMax = max;
}

void PIDController::setSetpoint(double setpoint)
{
    m_setpoint = setpoint;
}

double PIDController::update(double processVariable, double dt)
{
    if (dt <= 0) {
        dt = 0.001; // минимальный шаг времени
    }

    // Вычисление ошибки
    double error = m_setpoint - processVariable;

    // Пропорциональная составляющая
    double proportional = m_kp * error;

    // Интегральная составляющая
    m_integral += error * dt;

    // Антивинд ап интеграла
    if (m_integral > m_integralMax) m_integral = m_integralMax;
    if (m_integral < -m_integralMax) m_integral = -m_integralMax;

    double integral = m_ki * m_integral;

    // Дифференциальная составляющая
    double derivative = 0;
    if (dt > 0) {
        derivative = m_kd * (error - m_previousError) / dt;
    }

    // Суммируем все составляющие
    double output = proportional + integral + derivative;

    // Ограничиваем выходное значение
    output = std::clamp(output, m_outputMin, m_outputMax);

    // Сохраняем ошибку для следующей итерации
    m_previousError = error;

    return output;
}

void PIDController::reset()
{
    m_integral = 0.0;
    m_previousError = 0.0;
    m_lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
}
