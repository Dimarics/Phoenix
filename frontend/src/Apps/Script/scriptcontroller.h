#ifndef SCRIPTCONTROLLER_H
#define SCRIPTCONTROLLER_H

#include "scriptworker.h"
#include <QQmlEngine>
#include <QQuickTextDocument>
#include <QSyntaxHighlighter>

class ScriptController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool reached READ reached WRITE setReached NOTIFY reachedChanged FINAL)
    Q_PROPERTY(State state READ state NOTIFY stateChanged FINAL)
    Q_PROPERTY(Language language READ language WRITE setLanguage NOTIFY languageChanged FINAL)
    Q_PROPERTY(QQuickTextDocument *textDocument READ textDocument WRITE setTextDocument NOTIFY textDocumentChanged FINAL)
    Q_PROPERTY(QQuickTextDocument *terminalDocument READ terminalDocument WRITE setTerminalDocument NOTIFY terminalDocumentChanged FINAL)

public:
    enum State: quint8 { NotRunning, Running, Paused };
    enum Language: quint8 { None, Cpp, Python };
    Q_ENUM(Language)
    ScriptController(QObject *parent = nullptr);
    inline bool reached() const { return m_reached; }
    inline State state() const { return m_state; }
    inline Language language() const { return m_language; };
    inline QQuickTextDocument *textDocument() const { return m_textDocument; };
    inline QQuickTextDocument *terminalDocument() const { return m_terminalDocument; };
    void setReached(bool reached);
    void setState(State state);
    void setLanguage(Language language);
    void setTextDocument(QQuickTextDocument *textDocument);
    void setTerminalDocument(QQuickTextDocument *terminalDocument);
    Q_INVOKABLE void start();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void resume();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void next();

private:
    bool m_reached;
    State m_state;
    Language m_language;
    QSyntaxHighlighter *m_syntaxHighlighter;
    QQuickTextDocument *m_textDocument;
    QQuickTextDocument *m_terminalDocument;
    ScriptWorker *m_scriptWorker;

signals:
    void reachedChanged();
    void stateChanged();
    void languageChanged();
    void textDocumentChanged();
    void terminalDocumentChanged();
    void commandReceived(const QString &command);
};

#endif // SCRIPTCONTROLLER_H
