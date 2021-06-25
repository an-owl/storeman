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

#include "querysearch.h"

//this will get all in quotation marks (["'])(?:(?=(\\?))\2.)*?\1
//this will get scope::"followed by quote" (\S+)\s*::\s*"(.*?)"

/*test search
!c name::owl item::"froot loops" "milk pant" u-235 egg
*/

QuerySearch::QuerySearch(QString SearchQuery,QString tablename)
{
    searchTerm = SearchQuery;
    workingSearch = SearchQuery;
    table = tablename;
}



bool QuerySearch::exec()
{
    sort();

    compile();

    return true;
}

void QuerySearch::sort()
//sorts search data into relevant places
{
    checkScopeQuotes();
    checkScope();
    isoQuote();
    getExclusions();

    prune();
    cleanBreak();
}

void QuerySearch::checkScopeQuotes()
//gets all scopes followed by a quote
{                                // e    e     e   e      e    e= escape "\"
    QRegularExpression scopequote("(\\S+)\\s*::\\s*\"(.*?)\"");
    QRegularExpressionMatch match = scopequote.match(workingSearch);
    if (match.hasMatch()){
        scopedSearch.append(match.capturedTexts());
        qDebug() << "scoped quotes" << scopedSearch;
        workingSearch.remove(scopequote);
        qDebug() << "workingSearch" << workingSearch;
    }
}


void QuerySearch::checkScope()
//gets all scopes without quotes
{
    QRegularExpression reg("(\\S+)::(\\S+)");
    QRegularExpressionMatch match = reg.match(workingSearch);
    if (match.hasMatch()){
        scopedSearch.append(match.capturedTexts());
        qDebug() << "scoped quotes" << scopedSearch;
        workingSearch.remove(reg);
        qDebug() << "workingSearch" << workingSearch;
    }

}

void QuerySearch::isoQuote()
//isolates quotes to protect them form whitespace breakup
{                         //e      e
    QRegularExpression reg("\"(.*?)\"");
    QRegularExpressionMatch match = reg.match(workingSearch);
    if (match.hasMatch()){
        generalSearch.append(match.capturedTexts());
        qDebug() << "scoped quotes" << generalSearch;
        workingSearch.remove(reg);
        qDebug() << "workingSearch" << workingSearch;
    }
}

void QuerySearch::getExclusions()
//gets exclusions
//if adapted change this
{                                 //e
    QRegularExpression reg("(?<=[!])\\w+");
    QRegularExpressionMatch match = reg.match(workingSearch);
    if (match.hasMatch()){
        if (match.captured(0) == exclusions)
            exclude = true;
        reg.setPattern("^(\\S*)");
            workingSearch.remove(reg);
    }
}

void QuerySearch::cleanBreak()
{
    workingSearch.remove('"');
    generalSearch.append(workingSearch.split(' ',Qt::SkipEmptyParts));

}

void QuerySearch::prune()
//trims excess strings from search lists
{
    for(int i = 0; i < scopedSearch.length(); i++, i++)
        scopedSearch.removeAt(i);

    for(int i = 0; i < generalSearch.length(); i++)
        generalSearch.removeAt(i);
}

QString QuerySearch::addNested(int i = 0)
//recursively creates nested SQL queries
//works instead of lots of sequential ANDs or ORs

//if last SELECT FROM $table
//basically if not last recursively create new query where $table is previous query
{
    QString que;
    if (exclude)
        for (int i = 0 ; i < exFields.size(); i++)
            allfields.removeAll(exFields.at(i));
    que.append("(SELECT * FROM ");
    if (i == (generalSearch.length()-1))
        que.append(table);
    else
        que.append(addNested(i+1));
    que.append(" WHERE ");
    for (int j = 0; j < allfields.size();j++)
    {
        que.append(allfields.at(j));
        que.append(" LIKE \"%");
        que.append(generalSearch.at(i));
        que.append("%\" OR ");
    }
    QRegularExpression reg("\\sOR\\s*$");
    que.remove(reg);
    que.append(")"); //;)
    qDebug() << "\n" << que;
    return que;

}

void QuerySearch::compile()
//compiles search data int sql query
{
    if (query != nullptr)
        delete query;
    query = new QString;


    query->append("SELECT id,name,item,qty,date,authorised,returned,hidden FROM ");
    if (generalSearch.isEmpty())
        query->append(table);
    else
        query->append(addNested());


    if (!scopedSearch.isEmpty()){
    query->append(" WHERE ");

        for(int i = 0; i <= (scopedSearch.size() / 2) ; i = i+2)
        //inserts scoped search paramaters to query
        {
            query->append(scopedSearch.at(i));
            query->append("=\"");
            query->append(scopedSearch.at(i+1));
            query->append("\"");
            query->append(" AND ");
        }

        QRegularExpression reg("\\sAND\\s*$");
        query->remove(reg);
    }
    query->append(";");
    qDebug() << "\n" << *query;
}




































