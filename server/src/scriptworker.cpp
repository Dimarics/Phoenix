#include "scriptworker.h"
#include <QStandardPaths>
#include <QDir>

ScriptWorker::ScriptWorker(QObject *parent) : QObject (parent),
    m_process(new QProcess)
{
    m_location = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/Phoenix/script/";
#ifdef Q_OS_WINDOWS
    m_compiler = "g++";
    m_interpreter = "python";
    m_exec = m_location + "exec";
#elif defined(Q_OS_LINUX)
    m_compiler = "g++";
    m_interpreter = "python3";
    m_exec = m_location + "exec";
#endif
    // Терминал
    connect(m_process, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error) {
        qDebug() << "script error occurred:" << error;
        emit finished();
    });
    connect(m_process, &QProcess::started, this, [] {
        //qDebug() << "Started";
    });
    connect(m_process, &QProcess::finished, this, [this](int exitCode, QProcess::ExitStatus exitStatus) {
        qDebug() << exitCode << exitStatus;
        if (exitCode == 0 && exitStatus == QProcess::NormalExit) {
            if (m_process->program() == m_compiler) {
                m_process->start(m_exec);
            } else if (m_process->program() == m_exec || m_process->program() == m_interpreter) {
                emit finished();
            }
        }
    });
    connect(m_process, &QProcess::readyReadStandardOutput, this, [this] {
        QString input(m_process->readAllStandardOutput());
        if (m_process->program() == m_exec || m_process->program() == m_interpreter) {
            input.remove('\n').remove('\r');
            emit commandReceived(input);
        }
        emit dataReceived(input);
    });
    connect(m_process, &QProcess::readyReadStandardError, this, [this] {
        emit errorReceived(m_process->readAllStandardError());
        emit finished();
    });

    /*
     * void finished();
     * void commandReceived(const QString &command);
     * void dataReceived(const QString &data);
     * void errorReceived(const QString &error);
     */
    connect(this, &ScriptWorker::commandReceived, this, [this](const QString &command) {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << MSG_SERVER_SCRIPT_COMMAND_RECEIVED << command;
        emit sendMessage(data);
    });
    connect(this, &ScriptWorker::dataReceived, this, [this](const QString &input) {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << MSG_SERVER_SCRIPT_DATA_RECEIVED << input;
        emit sendMessage(data);
        //qDebug() << "script data received:" << input;
    });
    connect(this, &ScriptWorker::errorReceived, this, [this](const QString &error) {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << MSG_SERVER_SCRIPT_ERROR_RECEIVED << error;
        emit sendMessage(data);
        //qDebug() << "script error received:" << error;
    });
    connect(this, &ScriptWorker::finished, this, [this] {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << MSG_SERVER_SCRIPT_FINISHED;
        emit sendMessage(data);
        //qDebug() << "script finished";
    });
}

ScriptWorker::~ScriptWorker()
{
    if (m_process->state() == QProcess::Running) {
        m_process->kill();
        connect(m_process, &QProcess::finished, m_process, &QObject::deleteLater);
    } else {
        m_process->deleteLater();
    }
}

void ScriptWorker::start(quint8 language, const QByteArray &document)
{
    if (m_process->state() != QProcess::Running) {
        QByteArray header;
        QFile hLib, hLibCopy, script;
        switch (language) {
        case Cpp:
            header = "#include \"phoenix.h\"";
            hLib.setFileName(":/script/phoenix.h");
            hLibCopy.setFileName(m_location + "phoenix.h");
            script.setFileName(m_location + "script.cpp");
            m_process->setArguments({ script.fileName(), "-o", m_exec });
            m_process->setProgram(m_compiler);
            break;
        case Python:
            header = "from phoenix import *";
            hLib.setFileName(":/script/phoenix.py");
            hLibCopy.setFileName(m_location + "phoenix.py");
            script.setFileName(m_location + "script.py");
            m_process->setArguments({ script.fileName() });
            m_process->setProgram(m_interpreter);
            break;
        default:
            qDebug() << "Неизвестный язык";
            return;
        }

        // создание папки для хранения временных файлов
        if (!QDir(m_location).exists()) QDir().mkpath(m_location);

        // копирование заголовочного файла
        if (!hLib.open(QIODevice::ReadOnly) || !hLibCopy.open(QIODevice::WriteOnly)) {
            return;
        }
        hLibCopy.write(hLib.readAll());
        hLib.close();
        hLibCopy.close();

        // создание файла скрипта
        if (!script.open(QIODevice::WriteOnly)) {
            //ui->toolBar->setStarted(false);
            return;
        }
        script.write(header + '\n');
        script.write(document);
        script.close();

        m_process->start();
    } else {
        /*
        m_process->kill();
        connect(m_process, &QProcess::finished, this, [this, language, document] {
            start(language, document);
        });
        */
    }
}

void ScriptWorker::stop()
{
    if (m_process->state() != QProcess::NotRunning) {
        m_process->kill();
    }
}

void ScriptWorker::next()
{
    if ((m_process->program() == m_exec || m_process->program() == m_interpreter) && m_process->state() == QProcess::Running)
        m_process->write("\n");
}

void ScriptWorker::messageHandler(const QByteArray &msg)
{
    quint8 msg_id;
    QDataStream msg_data(msg);
    msg_data >> msg_id;
    //qDebug() << "msg id:" << msg_id;
    switch(msg_id) {
    case MSG_CLIENT_SCRIPT_START: {
        quint8 language;
        QByteArray document;
        msg_data >> language >> document;
        start(language, document);
        qDebug() << "script language:" << language;
    }   break;
    case MSG_CLIENT_SCRIPT_STOP:
        stop();
        qDebug() << "script stop";
        break;
    case MSG_CLIENT_SCRIPT_NEXT:
        next();
        qDebug() << "script next";
        break;
    }
}
