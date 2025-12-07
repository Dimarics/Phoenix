#ifndef SCRIPTBACKEND_H
#define SCRIPTBACKEND_H

#include <QObject>
#include <QQmlEngine>
#include <QQuickTextDocument>
#include <QSyntaxHighlighter>

#if QT_CONFIG(process) && !defined(Q_OS_ANDROID)
#include <QProcess>

class ScriptBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(Language language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QQuickTextDocument *textDocument READ textDocument WRITE setTextDocument NOTIFY textDocumentChanged)
    Q_PROPERTY(QQuickTextDocument *terminalDocument READ terminalDocument WRITE setTerminalDocument NOTIFY terminalDocumentChanged)
public:
    enum Language { None, Cpp, Python };
    Q_ENUM(Language)
    ScriptBackend(QObject *parent = nullptr);
    ~ScriptBackend();
    inline Language language() const { return m_language; };
    inline QQuickTextDocument *textDocument() const { return m_textDocument; };
    inline QQuickTextDocument *terminalDocument() const { return m_terminalDocument; };
    void setLanguage(Language language);
    void setTextDocument(QQuickTextDocument *textDocument);
    void setTerminalDocument(QQuickTextDocument *terminalDocument);
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void next();

private:
    //static const QString terminal;
    static const QString location;
    static const QString compiler;
    static const QString exec;
    static const QString interpreter;
    Language m_language;
    QSyntaxHighlighter *m_syntaxHighlighter;
    QQuickTextDocument *m_textDocument;
    QQuickTextDocument *m_terminalDocument;
    QProcess *m_process;

signals:
    void languageChanged();
    void textDocumentChanged();
    void terminalDocumentChanged();
    void messageReceived(const QString &message);
    void finished();
};

#else
class ScriptBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(Language language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QQuickTextDocument *textDocument READ textDocument WRITE setTextDocument NOTIFY textDocumentChanged)
    Q_PROPERTY(QQuickTextDocument *terminalDocument READ terminalDocument WRITE setTerminalDocument NOTIFY terminalDocumentChanged)
public:
    enum Language { None, Cpp, Python };
    Q_ENUM(Language)
    ScriptBackend(QObject *parent = nullptr);
    ~ScriptBackend();
    inline Language language() const { return m_language; };
    inline QQuickTextDocument *textDocument() const { return m_textDocument; };
    inline QQuickTextDocument *terminalDocument() const { return m_terminalDocument; };
    void setLanguage(Language language);
    void setTextDocument(QQuickTextDocument *textDocument);
    void setTerminalDocument(QQuickTextDocument *terminalDocument);
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void next();

private:
    Language m_language;
    QSyntaxHighlighter *m_syntaxHighlighter;
    QQuickTextDocument *m_textDocument;
    QQuickTextDocument *m_terminalDocument;

signals:
    void languageChanged();
    void textDocumentChanged();
    void terminalDocumentChanged();
    void messageReceived(const QString &message);
    void finished();
};
#endif

#endif // SCRIPTBACKEND_H
