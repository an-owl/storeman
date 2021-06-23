#include <QStringList>
#include <QRegularExpression>
#include <QDebug>

#ifndef QUERYSEARCH_H
#define QUERYSEARCH_H


class QuerySearch
{
public:
    QuerySearch(QString SearchQuery, QString tablename);
    void sort();
    bool exec();
    void compile();
    QString table;
    QString *query = nullptr;

private:

    void checkScopeQuotes();
    void checkScope();
    void isoQuote();
    void getExclusions();

    QString addNested(int i);

    void prune();

    void cleanBreak();

    QString searchTerm;
    QString workingSearch;

    QStringList generalSearch;//general search contains all loose search patterns
    QStringList scopedSearch; //scoped search contains scope followed by search term

    QStringList allfields {"id","name","pwdhashsha512","item","qty","date","authorised","returned","pwdhashmd5","condition","comments"};

    QString exclusions = "c";
    QStringList exFields = {"comments","condition"};

    bool exclude;

};

#endif // QUERYSEARCH_H
