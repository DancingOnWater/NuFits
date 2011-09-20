#include "NuFitsFile.h"
#include <math.h>
#include <stdio.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <list>
#include <fstream>

#include <NuFitsDescriptor.h>
#include <NufFitsHeadRecord.h>

using namespace std;
class NuFitsFilePrivate
{
    friend class NuFitsFile;
private:
    NuFitsFilePrivate();

private:
    bool parsePrimaryHeader(const list<string> &fields);
    bool readExtinsionHeader();
    void nextStructRead();

private:
    enum ReadingStatus{DontRead =0, PrimaryDataRead=1, ExtensionDataRead=2, SpecialDataread =3, EndRead=4};
    enum WritingStatus{ DontWrite =0,
                        PrimaryHeaderWrite = 1,
                        PrimaryDataWrite = 2,
                        ExtensionHeaderWrite =3,
                        ExtensionDataWrite = 4,
                        SpecialFieldWrite =5,
                        EndWrite = 6
                        };

private:
    fstream fptr;
    string currentFileName;
    string newFileName;
    ReadingStatus currentReadingStatus;
    WritingStatus currentWritingStatus;

    NuFitsDescriptor *descriptor;
    int64_t unreadDataBytes;
    bool isEnd;
    NuFitsFile::OpenMode openmode;
    bool isOpen;
private:

    bool headerAlreadyWrite;

private:
    void writeHeader();

    int skipFreeSpaceToData(int totalSpace, int alreadyRead);
    bool getHeaderFields();
    int readData(char *dest, int length);


    NuFitsFile::Error error;
private:
    static const int headerMinLength = 2880;
    string *removeAllSpaces(string *buffer);
    void fillZero(string *str);
};

NuFitsFilePrivate::NuFitsFilePrivate()
{
    isEnd=true;
    currentReadingStatus = DontRead;
    currentWritingStatus = DontWrite;

    headerAlreadyWrite = false;
    isOpen = false;
    descriptor = new NuFitsDescriptor();
}

NuFitsFile::NuFitsFile()
{
    d = new NuFitsFilePrivate();
}

NuFitsFile::~NuFitsFile()
{
    this->close();
    delete d;
}

void NuFitsFile::setFileName(const string &name)
{
    d->newFileName = name;
}

bool NuFitsFile::open(OpenMode mode)
{
    d->headerAlreadyWrite = false;
	this->close();
    d->currentFileName = d->newFileName;

    switch (mode)
	{
        break;case WriteOnly:
            d->fptr.open(d->currentFileName, ios::binary | ios::in);
        break; case ReadWrite:
            d->fptr.open(d->currentFileName, ios::binary | ios::in | ios::out);
        break; case ReadOnly:
            d->fptr.open(d->currentFileName, ios::binary | ios::out);
		break; default:
			return false;
		break;
	}
    if( ! d->fptr.is_open() )
        return false;
    d->isEnd=false;
    d->currentReadingStatus = NuFitsFilePrivate::DontRead;
    d->currentWritingStatus = NuFitsFilePrivate::DontWrite;
    //Иницилизация параметров чтения и записи
    if( mode != WriteOnly)
        d->nextStructRead();
   return true;
}

bool NuFitsFile::open(const string &fileName, OpenMode mode)
{
    setFileName(fileName);
    return open(mode);
}

NuFitsFile::OpenMode NuFitsFile::openMode()
{
    return d->openmode;
}

int64_t NuFitsFile::bytesAvailable() const
{
    return d->unreadDataBytes;
}

NuFitsDescriptor *NuFitsFile::descriptor()
{

}

const NuFitsDescriptor *NuFitsFile::descriptor() const
{

}

void NuFitsFile::setDescriptor(NuFitsDescriptor *value)
{

}


bool NuFitsFilePrivate::parsePrimaryHeader(const list<string> &fields)
{ /*
    string tmpkey(8, '0');
    string tmpvalue(80, '0');
    string tmpComment(80, '0');
    auto fields_iterator = fields.begin();
	///Парсим объязательные параметры////////////////////////
    if(fields.size() < 4)return false;


    headerFieldPars(*fields_iterator, &tmpkey, &tmpvalue, &tmpComment);
        if( *removeAllSpaces(&tmpkey)!=string("SIMPLE")) return false;
        fillZero(&tmpkey); fillZero(&tmpvalue); fillZero(&tmpComment);
    ++fields_iterator;
    headerFieldPars(*fields_iterator, &tmpkey, &tmpvalue, &tmpComment);
        readFields.bytepix = stoi(tmpvalue)/8;
        if( *removeAllSpaces(&tmpkey)!=string("BITPIX")) return false;//проверка корректности следованию формата
        fillZero(&tmpkey); fillZero(&tmpvalue); fillZero(&tmpComment);
    ++fields_iterator;
    headerFieldPars(*fields_iterator, &tmpkey, &tmpvalue, &tmpComment);
        readFields.naxis = stoi(tmpvalue);
        if(*removeAllSpaces(&tmpkey)!=string("NAXIS")) return false;
        fillZero(&tmpkey); fillZero(&tmpvalue); fillZero(&tmpComment);
            readFields.axisSize.resize(readFields.naxis);
			unreadDataBytes=1;
    ++fields_iterator;
    for(int i=0; i < readFields.naxis; i++, ++fields_iterator)
    {
        headerFieldPars( *fields_iterator,&tmpkey, &tmpvalue, &tmpComment);
        readFields.axisSize[i]=  stoi(tmpvalue);
        if( *removeAllSpaces(&tmpkey)!=string("NAXIS")+to_string(i+1)) return false;
        fillZero(&tmpkey); fillZero(&tmpvalue); fillZero(&tmpComment);
        unreadDataBytes = unreadDataBytes*readFields.axisSize[i];
    }
        unreadDataBytes=unreadDataBytes*readFields.bytepix;
        sizeReadingData=unreadDataBytes;
    //Парсим остальные слова в header
    for(++fields_iterator; fields_iterator !=fields.end(); ++fields_iterator)
    {

        tmpkey = fields_iterator->substr(0, 8);
        if(fields_iterator->at(8)=='=' && fields_iterator->at(9) == ' '){
            auto substr = fields_iterator->substr(10, fields_iterator->find('/',10)-10);
            readFields.addition.insert(make_pair(tmpkey, substr));
        }
        else
             readFields.addition.insert(make_pair(tmpkey, string()));
    }
    readFields.addition.erase(string("EXTEND  "));
    return true;
    */
}

void NuFitsFilePrivate::fillZero(string *str){
    str->replace(0, str->length(),"0");
}

string * NuFitsFilePrivate::removeAllSpaces(string * buffer)
{
    auto index = buffer->find(" ");
    while(index < buffer->length()){
        buffer->erase(index, 1);
        index = buffer->find(" ", index);
    }
    return buffer;
}

int64_t NuFitsFile::readAll(char *data)
{
    return readData(data, d->unreadDataBytes);
}

void NuFitsFilePrivate::nextStructRead()
{
    auto reactionForUncorrect =[&](){
        descriptor->clear();
        isEnd=true;
        currentReadingStatus=EndRead;
        return;
    };
    if(currentReadingStatus == EndRead)
        return;
    currentReadingStatus = static_cast<ReadingStatus>(currentReadingStatus+1);
    if(currentReadingStatus == EndRead)
        return;

    if(currentReadingStatus == SpecialDataread){
        unreadDataBytes = headerMinLength;
        return;
    }
    if(! getHeaderFields())
        reactionForUncorrect();

    if(currentReadingStatus == PrimaryDataRead)
    {
        descriptor->setType(NuFitsDescriptor::Primary);
        if( ! descriptor->correct())
            reactionForUncorrect();
        unreadDataBytes = descriptor->sizeDataSequence();
        if(skipFreeSpaceToData(headerMinLength, NufFitsHeadRecord::StringSize*descriptor->data()->size()) < headerMinLength){
            error = NuFitsFile::CantRead;
            reactionForUncorrect();
        }
	}
    else if(currentReadingStatus == ExtensionDataRead)//В противном случаем остается только special fields
    {
        descriptor->setType(NuFitsDescriptor::Extinsion);
        if( ! descriptor->correct())
            reactionForUncorrect();
        unreadDataBytes = descriptor->sizeDataSequence();
	}
}

void NuFitsFilePrivate::writeHeader()
{
    headerAlreadyWrite = true;

    int bytesPass;
    if(descriptor->type() == NuFitsDescriptor::Unset){

    }
    //Body

    if(! descriptor->hasEnd()){
        fptr <<"END"<<string(NufFitsHeadRecord::StringSize -3, ' ');
    }
    for(int countBytes = bytesPass+81; countBytes <= headerMinLength; ++countBytes)
        fptr<<' ';
}

int NuFitsFilePrivate::skipFreeSpaceToData(int totalSpace, int alreadyRead)
{
    int i;
    char tmp;
    try{
        for(i=alreadyRead; i<totalSpace; i++)
            fptr >>tmp;
    }catch(...){
        /*
        if(feof(fptr)) return -258;
        if(ferror(fptr)) return -1;
        */
        return -256;
    }

    return i;
}

bool NuFitsFilePrivate::getHeaderFields()
{
    if( fptr.is_open()){
        error = NuFitsFile::CantRead;
        return false;
    }
    string tmpbuffer(80, '0');


    do{
    try{
        //fptr.read(tmpbuffer.c_str(), tmpbuffer.size());
    }catch(...){
        return false;
    }
        if( ! descriptor->addRecord(tmpbuffer))
            return false;
    }while( ! descriptor->hasEnd());
    return true;
}

int NuFitsFilePrivate::readData(char * dest, int length)
{
    /*
    char buffer;

    int count_bytes;
    for(count_bytes=0; count_bytes < length; count_bytes+=bytepix)
    {
        for(int value_counter=1; value_counter <=bytepix; ++value_counter)
        {
            try{
            fptr >> buffer;
            }catch(...){
//                if(feof(fptr)) return count_bytes;
//                if(ferror(fptr)) return -1;
            }
            dest[bytepix-value_counter] = buffer;
        }
        dest+=bytepix;
    }

    return count_bytes;
    */
}

bool NuFitsFile::atEnd()
{
    if(!isOpen()) return false;
    return d->isEnd;
}

int64_t NuFitsFile::readData(char *data, int64_t maxlen)
{
    int64_t realRead=0;
    if( (openMode() & ReadOnly) !=1)
        return 0;

	if(maxlen<1) return 0;
    if(maxlen > d->unreadDataBytes) maxlen = d->unreadDataBytes;

    realRead = d->readData(data, maxlen);
    d->unreadDataBytes-=realRead;

    if(d->unreadDataBytes==0)
	{
        if(realRead != maxlen) d->isEnd=true;
            d->nextStructRead();
	}
	return realRead;
}

int64_t NuFitsFile::writeData(const char *data, int64_t len)
{
    if(! d->headerAlreadyWrite)
        d->writeHeader();

    /*
    string buffer;
    int bytepix = d->writeFields.bytepix;
        buffer.resize(bytepix);
    for(int counter=0; counter<len; counter+=bytepix)
    {
        const char *src = data+counter;
        for(int count_bytes=0; count_bytes<bytepix; count_bytes++)
           buffer[count_bytes]= src[bytepix-count_bytes-1];
        try{
            d->fptr<< buffer;
        }catch(...){
            return -1;
        }
    }
    d->fptr.flush();
    return len;
    */
}

void NuFitsFile::close()
{
    if(!isOpen()) return;
    d->fptr.close();
    d->unreadDataBytes=0;
    d->isEnd=true;
}

bool NuFitsFile::isOpen()
{
    return d->fptr.is_open();
}

int64_t NuFitsFile::read(char *data, int64_t maxlen)
{
    return  readData(data, maxlen);
}

int64_t NuFitsFile::write(const char *data, int64_t len)
{
    return writeData(data, len);
}

const string& NuFitsFile::fileName()
{
    return d->currentFileName;
}
