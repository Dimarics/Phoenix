#include "scriptbackend.h"
#include "cpphighlighter.h"
#include "pythonhighlighter.h"
#include "appbackend.h"
#include <QTextCursor>
#include <QDir>

#if QT_CONFIG(process) && !defined(Q_OS_ANDROID)

const QString ScriptBackend::location = AppBackend::tempLocation() + "script/";
#ifdef Q_OS_WINDOWS
//const QString ScriptBackend::terminal = "cmd.exe";
const QString ScriptBackend::compiler = "g++";//"D:/Programs/Qt/6.7.2/mingw_64/bin/g++.exe";
const QString ScriptBackend::exec = location + "exec.exe";
const QString ScriptBackend::interpreter = "python";//"script/Python/python.exe";
#elif defined(Q_OS_LINUX)
const QString ScriptBackend::compiler = "g++";
const QString ScriptBackend::interpreter = "python3";
#endif

ScriptBackend::ScriptBackend(QObject *parent) : QObject (parent),
    m_language(None),
    m_syntaxHighlighter(nullptr),
    m_process(new QProcess)
{
    // Терминал
    connect(m_process, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error) {
        //stop();
        qDebug() << error;
        emit finished();
    });
    connect(m_process, &QProcess::started, this, []
    {
        //qDebug() << "Started";
    });
    connect(m_process, &QProcess::finished, this, [this](int exitCode, QProcess::ExitStatus exitStatus)
    {
        qDebug() << exitCode << exitStatus;
        if (exitCode == 0 && exitStatus == QProcess::NormalExit) {
            if (m_process->program() == compiler) {
                m_process->start(exec);
            } else if (m_process->program() == exec || m_process->program() == interpreter) {
                emit finished();
            }
        }
    });
    connect(m_process, &QProcess::readyReadStandardOutput, this, [this]
    {
        //qDebug() << "output:" << m_process->readAllStandardOutput();
        QString input = m_process->readAllStandardOutput();
        if (m_process->program() == exec || m_process->program() == interpreter) {
            //m_currentCommand = m_process->readAll().trimmed().split(' ');
            //if (!m_currentCommand.isEmpty()) scriptHandler(m_currentCommand);
            input.remove('\n').remove('\r');
            emit messageReceived(input);
        }
        input.replace('\n', "<br>").remove('\r');
        //m_terminalDocument->textDocument()->setHtml(m_terminalDocument->textDocument()->toHtml() + input);
        m_terminalDocument->textDocument()->setHtml(m_terminalDocument->textDocument()->toHtml() + input);
    });
    connect(m_process, &QProcess::readyReadStandardError, this, [this]
    {
        QString input = "<font color=\"red\">" + m_process->readAllStandardError() + "</font>";
        //QString input = m_process->readAllStandardError().trimmed().replace('\n', "<br>");
        input.replace('\n', "<br>").remove('\r');
        QString text = m_terminalDocument->textDocument()->toHtml();
        m_terminalDocument->textDocument()->setHtml(text + input);
        emit finished();
    });
}

ScriptBackend::~ScriptBackend()
{
    if (m_process->state() == QProcess::Running) {
        m_process->kill();
        connect(m_process, &QProcess::finished, m_process, &QObject::deleteLater);
    } else {
        m_process->deleteLater();
    }
}

void ScriptBackend::start()
{
    if (m_process->state() != QProcess::Running) {
        QByteArray header;
        QFile hLib, hLibCopy, script;
        switch (m_language) {
        case Cpp:
            header = "#include \"hobots.h\"";
            hLib.setFileName(":/script/hobots.h");
            hLibCopy.setFileName(location + "hobots.h");
            script.setFileName(location + "script.cpp");
            m_process->setArguments({ script.fileName(), "-o", exec });
            m_process->setProgram(compiler);
            break;
        case Python:
            header = "from hobots import *";
            hLib.setFileName(":/script/hobots.py");
            hLibCopy.setFileName(location + "hobots.py");
            script.setFileName(location + "script.py");
            m_process->setArguments({ script.fileName() });
            m_process->setProgram(interpreter);
            break;
        default:
            qDebug() << "Неизвестный язык";
            return;
        }

        // создание папки для хранения временных файлов
        if (!QDir(location).exists()) QDir().mkpath(location);

        // копирование заголовочного файла
        if (!hLib.open(QIODevice::ReadOnly) || !hLibCopy.open(QIODevice::WriteOnly)) {
            //ui->toolBar->setStarted(false);
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
        script.write(m_textDocument->textDocument()->toPlainText().toUtf8());
        script.close();

        /*m_process->start("cmd.exe");
        m_process->waitForStarted();
        m_process->write("sdds\n");*/
        m_process->start();
    } else { // resume

    }
}

void ScriptBackend::stop()
{
    if (m_process->state() != QProcess::NotRunning) {
        m_process->kill();
    }
}

void ScriptBackend::next()
{
    if ((m_process->program() == exec || m_process->program() == interpreter) && m_process->state() == QProcess::Running)
        m_process->write("\n");
}

#else
ScriptBackend::ScriptBackend(QObject *parent) : QObject (parent),
    m_language(None),
    m_syntaxHighlighter(nullptr)
{
}

ScriptBackend::~ScriptBackend()
{
}

void ScriptBackend::start()
{
}

void ScriptBackend::stop()
{
}

void ScriptBackend::next()
{
}
#endif

void ScriptBackend::setLanguage(Language language)
{
    if (language == m_language || !m_textDocument) return;
    QSyntaxHighlighter *newHighlighter = nullptr;
    switch (language) {
    case Cpp: newHighlighter = new CppHighlighter(m_textDocument->textDocument()); break;
    case Python: newHighlighter = new PythonHighlighter(m_textDocument->textDocument()); break;
    default: return; break;
    }
    m_language = language;
    if (m_syntaxHighlighter) m_syntaxHighlighter->deleteLater();
    m_syntaxHighlighter = newHighlighter;
    emit languageChanged();
}

void ScriptBackend::setTextDocument(QQuickTextDocument *textDocument)
{
    if (textDocument == m_textDocument) return;
    m_textDocument = textDocument;
    emit textDocumentChanged();
}

void ScriptBackend::setTerminalDocument(QQuickTextDocument *terminalDocument)
{
    if (terminalDocument == m_terminalDocument) return;
    m_terminalDocument = terminalDocument;
    emit terminalDocumentChanged();
}
