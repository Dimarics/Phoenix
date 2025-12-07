#ifndef CPPHIGHLIGHTER_H
#define CPPHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class CppHighlighter : public QSyntaxHighlighter {

public:
    CppHighlighter(QTextDocument *parent);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> m_highlightingRules;
    QTextCharFormat m_commentFormat;
};

#endif // CPPHIGHLIGHTER_H
