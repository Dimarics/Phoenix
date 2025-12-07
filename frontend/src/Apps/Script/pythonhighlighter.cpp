#include "pythonhighlighter.h"


PythonHighlighter::PythonHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
    // Настройка форматов
    //keywordFormat.setForeground(QColor(136, 120, 195));  // Фиолетовый
    keywordFormat.setForeground(Qt::darkMagenta);
    //keywordFormat.setFontWeight(QFont::Bold);

    selfFormat.setForeground(QColor(203, 75, 22));  // Оранжевый
    decoratorFormat.setForeground(QColor(137, 89, 168));  // Пурпурный

    //commentFormat.setForeground(QColor(106, 153, 85));  // Зеленый
    commentFormat.setForeground(Qt::darkGray);
    //stringFormat.setForeground(QColor(42, 161, 152));  // Бирюзовый
    stringFormat.setForeground(Qt::darkGreen);
    //numberFormat.setForeground(QColor(181, 137, 0));  // Желтый
    numberFormat.setForeground(Qt::darkCyan);
    classFormat.setForeground(QColor(220, 50, 47));  // Красный
    defFormat.setForeground(QColor(38, 139, 210));  // Синий

    // Ключевые слова Python
    QStringList keywordPatterns = {
        "False", "None", "True", "and", "as",
        "assert", "break", "class", "continue",
        "def", "del", "elif", "else", "except",
        "finally", "for", "from", "global", "if",
        "import", "in", "is", "lambda", "nonlocal",
        "not", "or", "pass", "raise", "return",
        "try", "while", "with", "yield"
    };

    for (const QString &pattern : keywordPatterns) {
        HighlightingRule rule;
        rule.pattern = QRegularExpression("\\b" + pattern + "\\b");
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Числа
    HighlightingRule numberRule;
    numberRule.pattern = QRegularExpression("\\b\\d+(\\.\\d+)?\\b");
    numberRule.format = numberFormat;
    highlightingRules.append(numberRule);

    // self
    HighlightingRule selfRule;
    selfRule.pattern = QRegularExpression("\\bself\\b");
    selfRule.format = selfFormat;
    highlightingRules.append(selfRule);

    // Имена классов
    HighlightingRule classRule;
    classRule.pattern = QRegularExpression("\\bclass\\s+(\\w+)");
    classRule.format = classFormat;
    highlightingRules.append(classRule);

    // Имена функций
    HighlightingRule defRule;
    defRule.pattern = QRegularExpression("\\bdef\\s+(\\w+)");
    defRule.format = defFormat;
    highlightingRules.append(defRule);

    // Декораторы @
    HighlightingRule decoratorRule;
    decoratorRule.pattern = QRegularExpression("@\\w+");
    decoratorRule.format = decoratorFormat;
    highlightingRules.append(decoratorRule);

    // Строки (одинарные и двойные кавычки, тройные кавычки)
    HighlightingRule stringRule;
    stringRule.pattern = QRegularExpression("\"\"\".*\"\"\"|'''.*'''|\".*\"|'.*'");
    stringRule.format = stringFormat;
    highlightingRules.append(stringRule);

    // Комментарии #
    HighlightingRule commentRule;
    commentRule.pattern = QRegularExpression("#[^\n]*");
    commentRule.format = commentFormat;
    highlightingRules.append(commentRule);
}

void PythonHighlighter::highlightBlock(const QString &text) {
    for (const HighlightingRule &rule : highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
