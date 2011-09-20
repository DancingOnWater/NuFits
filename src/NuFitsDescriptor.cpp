#include "NuFitsDescriptor.h"

#include "NufFitsHeadRecord.h"

namespace NuFitsDescriptorKeyword {
const char SimpleType[]="SIMPLE";
const char ExtensionType[] = "Extension";
const char SpecialType[] = "s";
const char Bytepix[] = "BYTEPIX";
const char NAxis[] = "NAXIS";

}


template <> void NuFitsDescriptor::setBytePix<unsigned int>(){setBytePix(0);}
template <> void NuFitsDescriptor::setBytePix<unsigned long>(){setBytePix(0);}
template <> void NuFitsDescriptor::setBytePix<unsigned short>(){setBytePix(0);}
template <> void NuFitsDescriptor::setBytePix<double>(){setBytePix(-64);}
template <> void NuFitsDescriptor::setBytePix<float>(){setBytePix(-32);}

class NuFitsDescriptorPrivate
{
public:
    NuFitsDescriptorPrivate() { clear();}
    clear();
    void interpretate(const NufFitsHeadRecord &record);
    NuFitsDescriptor::Type m_type;
    size_t m_sizeDataSequence;
    NufFitsHeadRecord headRecord;
    bool m_hasEnd;
    std::list<NufFitsHeadRecord> m_container;
static NufFitsHeadRecord endrecord;
};
NufFitsHeadRecord NuFitsDescriptorPrivate::endrecord = NufFitsHeadRecord("END", "", "");

NuFitsDescriptor::NuFitsDescriptor(): d(new NuFitsDescriptorPrivate)
{

}

NuFitsDescriptor::NuFitsDescriptor(const NuFitsDescriptor &other)
{

}

NuFitsDescriptor::~NuFitsDescriptor()
{
    delete d;
}

NuFitsDescriptor &NuFitsDescriptor::operator =(const NuFitsDescriptor &other)
{

}


bool NuFitsDescriptor::operator ==(const NuFitsDescriptor &other)
{

}

bool NuFitsDescriptor::addRecord(const std::string &string)
{

}

std::list<NufFitsHeadRecord> *NuFitsDescriptor::data()
{
    return &(d->m_container);
}

const std::list<NufFitsHeadRecord> *NuFitsDescriptor::data() const
{
    return &(d->m_container);
}

const std::vector<int> &NuFitsDescriptor::axisSize() const
{

}

long NuFitsDescriptor::axeSize(long number) const
{

}

long NuFitsDescriptor::numberDimension() const
{

}

void NuFitsDescriptor::setAxisSize(const std::vector<int> &axis)
{

}

void NuFitsDescriptor::setAxeSize(int number, int size)
{

}

void NuFitsDescriptor::setNumberDimension(int value)
{

}

void NuFitsDescriptor::setBytePix(int value)
{

}

int NuFitsDescriptor::bytepix() const
{

}

void NuFitsDescriptor::setType(NuFitsDescriptor::Type value)
{
    d->m_type = value;
    if(d->m_container.front().keyword != NuFitsDescriptorKeyword::SimpleType ||
            d->m_container.front().keyword != NuFitsDescriptorKeyword::ExtensionType ||
            d->m_container.front().keyword != NuFitsDescriptorKeyword::SpecialType){
         d->m_container.push_back(NufFitsHeadRecord());
    }

    switch (value) {
        case NuFitsDescriptor::Primary:
                d->m_container.front().keyword = NuFitsDescriptorKeyword::SimpleType;
        break; case NuFitsDescriptor::Extinsion:
                d->m_container.front().keyword = NuFitsDescriptorKeyword::ExtensionType;
        break; default:
                d->m_container.front().keyword = NuFitsDescriptorKeyword::SpecialType;
        break;
    }
    d->m_container.front().value = "T";
        d->m_container.front().comment.clear();
}

NuFitsDescriptor::Type NuFitsDescriptor::type() const
{
    return d->m_type;
}

const NufFitsHeadRecord &NuFitsDescriptor::headRecord() const
{
    return d->headRecord;
}

size_t NuFitsDescriptor::sizeDataSequence() const
{
    return d->m_sizeDataSequence;
}

bool NuFitsDescriptor::hasEnd() const
{
    return d->m_hasEnd;
}

void NuFitsDescriptor::addEnd()
{

}

bool NuFitsDescriptor::correct() const
{

}

void NuFitsDescriptor::clear()
{
    d->clear();
}


NuFitsDescriptorPrivate::clear()
{
    m_type = NuFitsDescriptor::Unset;
    m_sizeDataSequence =0;
    m_hasEnd = false;
    m_container.clear();

}

void NuFitsDescriptorPrivate::interpretate(const NufFitsHeadRecord &record)
{
    if(record.keyword == NuFitsDescriptorKeyword::SimpleType){
        m_type = NuFitsDescriptor::Primary;

    }
}
