#ifndef NUFFITSKEYWORDSTRUCT_H
#define NUFFITSKEYWORDSTRUCT_H
#include <string>

class NufFitsHeadRecord{
public:
    NufFitsHeadRecord();
    NufFitsHeadRecord(const NufFitsHeadRecord &other);
    NufFitsHeadRecord(const std::string &field);
    NufFitsHeadRecord(const std::string & keyword, const std::string & value, const std::string & comment):keyword(keyword), value(value),comment(comment){}
    std::string keyword;
    std::string value;
    std::string comment;

    inline bool correct() const
    {
        if(keyword.size() >MaxNameSize)
            return false;
        int additionSymbols =0;
        additionSymbols += value.size() >0 ? 2:0;
        additionSymbols += keyword.size() >0 ? 1:0;
        return  value.size()+ keyword.size() <= StringSize - MaxNameSize -additionSymbols;
    }

    bool operator == (const NufFitsHeadRecord &other);

    NufFitsHeadRecord operator =(const NufFitsHeadRecord &other);

    std::string toString() const;
    bool fromString(const std::string &field);
    static const int StringSize = 80;
    static const int MaxNameSize = 8;
};

#endif // NUFFITSKEYWORDSTRUCT_H
