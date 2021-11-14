#ifndef XML_HIGHLIGHTER_H
#define XML_HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class XML_Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    XML_Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat attributeFormat;
    QTextCharFormat valueFormat;
    QTextCharFormat commentFormat;

};



#endif // XML_HIGHLIGHTER_H
