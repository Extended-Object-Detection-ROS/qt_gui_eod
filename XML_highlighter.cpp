#include "XML_highlighter.h"

XML_Highlighter::XML_Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::black);
    keywordFormat.setFontWeight(QFont::Bold);

    attributeFormat.setForeground(Qt::darkGreen);

    valueFormat.setForeground(Qt::red);

    commentFormat.setForeground(Qt::gray);

    rule.pattern = QRegularExpression(QStringLiteral("<[?\\s]*[/]?[\\s]*([^\\n][^>]*)(?=[\\s/>])"));
    rule.format = keywordFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\\w+(?=\\=)"));
    rule.format = attributeFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\"[^\\n\"]+\"(?=[?\\s/>])"));
    rule.format = valueFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("<!--[^\\n]*-->"));
    rule.format = commentFormat;
    highlightingRules.append(rule);


}

void XML_Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
