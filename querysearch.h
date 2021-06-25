/*

    soremant is designed to replace oper managemnt of stores in the NZCF
    Copyright (C) 20201  Charles Kern-Smith

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

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
