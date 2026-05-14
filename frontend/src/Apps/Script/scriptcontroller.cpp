#include "scriptcontroller.h"
#include "cpphighlighter.h"
#include "pythonhighlighter.h"
#include <QTextCursor>
#include <QDir>

ScriptController::ScriptController(QObject *parent) : QObject (parent),
    m_reached(false),
    m_language(None),
    m_syntaxHighlighter(nullptr),
    m_scriptWorker(new ScriptWorker(this))
{
    // Терминал
    connect(m_scriptWorker, &ScriptWorker::finished, this, [this] {
        setState(NotRunning);
    });
    connect(m_scriptWorker, &ScriptWorker::commandReceived, this, &ScriptController::commandReceived);
    connect(m_scriptWorker, &ScriptWorker::dataReceived, this, [this](const QString &data) {
        QString input(data);
        input.replace('\n', "<br>").remove('\r');
        m_terminalDocument->textDocument()->setHtml(m_terminalDocument->textDocument()->toHtml() + input);
    });
    connect(m_scriptWorker, &ScriptWorker::errorReceived, this, [this](const QString &error) {
        QString input = "<font color=\"red\">" + error + "</font>";
        input.replace('\n', "<br>").remove('\r');
        QString text = m_terminalDocument->textDocument()->toHtml();
        m_terminalDocument->textDocument()->setHtml(text + input);
    });
}

void ScriptController::setReached(bool reached)
{
    if (m_reached == reached) return;
    m_reached = reached;
    emit reachedChanged();
}

void ScriptController::start() {
    if (m_state == NotRunning) {
        setState(Running);
        m_scriptWorker->start(m_language, m_textDocument->textDocument()->toPlainText().toUtf8());
    }
}

void ScriptController::pause()
{
    if (m_state == Running) {
        setState(Paused);
    }
}

void ScriptController::resume()
{
    if (m_state == Paused) {
        setState(Running);
        if (m_reached) m_scriptWorker->next();
    }
}

void ScriptController::stop() {
    if (m_state != NotRunning) {
        setState(NotRunning);
        m_scriptWorker->stop();
    }
}

void ScriptController::next() { m_scriptWorker->next(); }

void ScriptController::setLanguage(Language language)
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

void ScriptController::setState(State state)
{
    if (m_state == state) return;
    m_state = state;
    emit stateChanged();
}

void ScriptController::setTextDocument(QQuickTextDocument *textDocument)
{
    if (textDocument == m_textDocument) return;
    m_textDocument = textDocument;
    emit textDocumentChanged();
}

void ScriptController::setTerminalDocument(QQuickTextDocument *terminalDocument)
{
    if (terminalDocument == m_terminalDocument) return;
    m_terminalDocument = terminalDocument;
    emit terminalDocumentChanged();
}
