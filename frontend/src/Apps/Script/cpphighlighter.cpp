#include "cpphighlighter.h"

CppHighlighter::CppHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
    // Ключевые слова C++
    QStringList keywordPatterns = {
        "char", "class", "const", "double",
        "enum", "explicit", "friend", "inline",
        "int", "long", "namespace", "operator",
        "private", "protected", "public", "short",
        "signals", "signed", "slots", "static",
        "struct", "template", "typedef", "typename",
        "union", "unsigned", "virtual", "void",
        "volatile", "bool", "break", "case",
        "catch", "continue", "default", "delete",
        "do", "else", "false", "for", "goto",
        "if", "new", "return", "sizeof", "switch",
        "this", "throw", "true", "try", "while",
        "int8_t", "int16_t", "int32_t", "int64_t",
        "uint8_t", "uint16_t", "uint32_t", "uint64_t"
    };

    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkMagenta);
    //keywordFormat.setFontWeight(QFont::Bold);
    for (const QString &pattern : keywordPatterns) {
        HighlightingRule rule;
        rule.pattern = QRegularExpression("\\b" + pattern + "\\b");
        rule.format = keywordFormat;
        m_highlightingRules << rule;
    }

    // Числа
    QTextCharFormat numberFormat;
    numberFormat.setForeground(Qt::darkCyan);
    //numberFormat.setFontWeight(QFont::DemiBold);
    HighlightingRule numberRule;
    numberRule.pattern = QRegularExpression("\\b\\d+(\\.\\d+)?\\b");
    numberRule.format = numberFormat;
    m_highlightingRules << numberRule;

    // Функции
    QTextCharFormat functionFormat;
    //functionFormat.setFontItalic(true);
    functionFormat.setForeground(QColor(210, 85, 0));
    HighlightingRule functionRule;
    functionRule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    functionRule.format = functionFormat;
    m_highlightingRules << functionRule;

    // Классы
    QTextCharFormat classFormat;
    classFormat.setForeground(QColor(210, 85, 0));
    classFormat.setFontWeight(QFont::Bold);
    HighlightingRule classRule;
    classRule.pattern = QRegularExpression("\\b[A-ZА-ЯЁ][A-Za-zА-Яа-яЁё0-9_]*\\b");
    classRule.format = classFormat;
    m_highlightingRules << classRule;

    // Директивы препроцессора
    QTextCharFormat preprocessorFormat;
    preprocessorFormat.setForeground(Qt::darkMagenta);
    HighlightingRule preprocessorRule;
    preprocessorRule.pattern = QRegularExpression("^\\s*#\\s*(?:include|define|ifdef|ifndef|if|else|elif|endif|undef|pragma|error|warning|line)\\b");
    preprocessorRule.format = preprocessorFormat;
    m_highlightingRules << preprocessorRule;

    // Строки в кавычках
    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkGreen);
    HighlightingRule quotationRule;
    quotationRule.pattern = QRegularExpression("\".*\"");
    quotationRule.format = quotationFormat;
    m_highlightingRules << quotationRule;

    // Комментарии
    m_commentFormat.setForeground(Qt::darkGray);
    HighlightingRule singleLineCommentRule;
    singleLineCommentRule.pattern = QRegularExpression("//[^\n]*");
    singleLineCommentRule.format = m_commentFormat;
    m_highlightingRules << singleLineCommentRule;
}

void CppHighlighter::highlightBlock(const QString &text) {
    for (const HighlightingRule &rule : m_highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    // Многострочные комментарии /* */
    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf("/*");

    while (startIndex >= 0) {
        QRegularExpressionMatch match = QRegularExpression("\\*/").match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, m_commentFormat);
        startIndex = text.indexOf("/*", startIndex + commentLength);
    }
}
