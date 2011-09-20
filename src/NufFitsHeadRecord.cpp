#include "NufFitsHeadRecord.h"
#include <iostream>
namespace NufFitsHeadRecordPriavte {

    void eraseWhiteSpace(std::string *recordPart)
    {
        int i;
        for(i =0; i!= recordPart->length() && recordPart->at(i) == ' '; ++i);
        recordPart->erase(0, i);
        for(i = recordPart->length()-1; i!= -1  && recordPart->at(i) == ' '; --i);
        recordPart->erase(i+1, recordPart->length()-1);
    }
}


NufFitsHeadRecord::NufFitsHeadRecord()
{

}

NufFitsHeadRecord::NufFitsHeadRecord(const NufFitsHeadRecord &other)
{
    keyword = other.keyword;
    value = other.value;
    comment = other.comment;
}

NufFitsHeadRecord::NufFitsHeadRecord(const std::string &field)
{
    fromString(field);
}

NufFitsHeadRecord NufFitsHeadRecord::operator =(const NufFitsHeadRecord &other)
{
    keyword = other.keyword;
    value = other.value;
    comment = other.comment;
}

std::string NufFitsHeadRecord::toString() const
{
    std::string str(NufFitsHeadRecord::StringSize, ' ');
    str.replace(0, keyword.size(),keyword);
    int lastnumber = MaxNameSize;
    if(value.size() != 0){
        str[MaxNameSize]='=';
        str.replace(MaxNameSize+2, value.size(), value);
        lastnumber += 2+value.size();
    }
    if(comment.size() >0){
        str[lastnumber]='/';
        str.replace(lastnumber+1, comment.size(), comment);

    }
    return str;
}

bool NufFitsHeadRecord::fromString(const std::string &field)
{
    if (field.size() < NufFitsHeadRecord::MaxNameSize)
        return false;
    keyword = field.substr(0, MaxNameSize);
    NufFitsHeadRecordPriavte::eraseWhiteSpace(&keyword);
    if(field.length() <= MaxNameSize + 2){
        value.clear();
        comment.clear();
        return true;
    }
    int endNumber=MaxNameSize-1;
    if((field.at(MaxNameSize)=='=') && (field.at(MaxNameSize+1)==' '))
    {
        endNumber = field.find('/', MaxNameSize+2);
        value = field.substr(MaxNameSize+2, endNumber-MaxNameSize-2);
        NufFitsHeadRecordPriavte::eraseWhiteSpace(&value);
    }
    else
        keyword.clear();

    if(endNumber < field.length()-1){
        comment = field.substr(endNumber+1);
        NufFitsHeadRecordPriavte::eraseWhiteSpace(&comment);
    }
    else
        comment.clear();

    return true;
}


