#ifndef SCRIPTWORKER_H
#define SCRIPTWORKER_H

#include <QObject>
#include <QWebSocket>

#if QT_CONFIG(process) && !defined(Q_OS_ANDROID)

#include <QProcess>

class ScriptWorker : public QObject
{
    Q_OBJECT
public:
    enum MSG_CLIENT_ID: quint8 {
        MSG_CLIENT_SCRIPT_START = 1,
        MSG_CLIENT_SCRIPT_STOP,
        MSG_CLIENT_SCRIPT_NEXT
    };
    enum MSG_SERVER_ID: quint8 {
        MSG_SERVER_SCRIPT_COMMAND_RECEIVED = 1,
        MSG_SERVER_SCRIPT_DATA_RECEIVED,
        MSG_SERVER_SCRIPT_ERROR_RECEIVED,
        MSG_SERVER_SCRIPT_FINISHED
    };
    enum State: quint8 { NotRunning, Running, Paused };
    enum Language: quint8 { None, Cpp, Python };
    ScriptWorker(QObject *parent = nullptr);
    ~ScriptWorker();
    void start(quint8 language, const QByteArray &document);
    void stop();
    void next();

public slots:
    void messageHandler(const QByteArray &msg);

private:
    //Language m_language;
    QString m_location;
    QString m_compiler;
    QString m_exec;
    QString m_interpreter;
    QProcess *m_process;

signals:
    void sendMessage(const QByteArray &msg);
    void commandReceived(const QString &command);
    void dataReceived(const QString &data);
    void errorReceived(const QString &error);
    void finished();
};

#else

class ScriptWorker : public QObject
{
    Q_OBJECT
public:
    enum MSG_CLIENT_ID: quint8 {
        MSG_CLIENT_SCRIPT_START = 1,
        MSG_CLIENT_SCRIPT_STOP,
        MSG_CLIENT_SCRIPT_NEXT
    };
    enum MSG_SERVER_ID: quint8 {
        MSG_SERVER_SCRIPT_COMMAND_RECEIVED = 1,
        MSG_SERVER_SCRIPT_DATA_RECEIVED,
        MSG_SERVER_SCRIPT_ERROR_RECEIVED,
        MSG_SERVER_SCRIPT_FINISHED
    };
    enum Language: quint8 { None, Cpp, Python };
    ScriptWorker(QObject *parent = nullptr);
    void start(quint8 language, const QByteArray &document);
    void stop();
    void next();

private:
    QWebSocket *m_webSocket;

signals:
    void commandReceived(const QString &command);
    void dataReceived(const QString &data);
    void errorReceived(const QString &error);
    void finished();
};

#endif

#endif // SCRIPTWORKER_H
