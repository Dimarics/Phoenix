// pidcontroller.h
#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

#include <QObject>
#include <QTimer>

class PIDController : public QObject
{
    Q_OBJECT

public:
    explicit PIDController(QObject *parent = nullptr);

    // Настройка параметров PID
    void setParameters(double kp, double ki, double kd);

    // Настройка диапазонов
    void setOutputLimits(double min, double max);
    void setIntegralLimit(double max);

    // Установка целевого значения
    void setSetpoint(double setpoint);

    // Основная функция обновления
    double update(double processVariable, double dt);

    // Сброс регулятора
    void reset();

private:
    // Параметры PID
    double m_kp;
    double m_ki;
    double m_kd;

    // Переменные состояния
    double m_integral;
    double m_previousError;

    // Ограничения
    double m_outputMin;
    double m_outputMax;
    double m_integralMax;

    // Целевое значение
    double m_setpoint;

    // Время последнего обновления
    qint64 m_lastUpdateTime;
};

#endif // PIDCONTROLLER_H
