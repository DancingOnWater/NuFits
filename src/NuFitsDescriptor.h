#ifndef NUFITSDESCRIPTOR_H
#define NUFITSDESCRIPTOR_H
#include <list>
#include <string>
#include <vector>

class NuFitsDescriptorPrivate;
class NufFitsHeadRecord;
class NuFitsDescriptor
{

public:
    NuFitsDescriptor();
    NuFitsDescriptor(const NuFitsDescriptor &other);
    ~NuFitsDescriptor();
    NuFitsDescriptor & operator = (const NuFitsDescriptor &other);

    enum Type{Unset =0, Primary = 1, Extinsion = 2, Special = 3};

    bool operator == (const NuFitsDescriptor &other);

    bool addRecord(const std::string& string);
    bool addRecord(const NufFitsHeadRecord &  record);

    std::list<NufFitsHeadRecord> * data();
    const std::list<NufFitsHeadRecord> * data() const;
    bool update();

    const std::vector<int>& axisSize() const;
    long axeSize(long number) const;
    long numberDimension() const;



    void setBytePix(int value);
    template <typename T> void setBytePix(){ setBytePix(sizeof(T)*8);}


    void setAxeSize(int number, int size);
    void setNumberDimension(int value);

private:
   template<typename ...Arguments> void setAxeSize(int dimensionNumber, int axisSize, Arguments... args){
        setAxeSize(dimensionNumber, axisSize);
        ++dimensionNumber;
        setAxeSize(dimensionNumber, args...);
    }
public:
    void setAxisSize(const std::vector<int> &axis );

    template<typename T> void setAxisSize(const std::vector<int>  &axis)
    {
        setBytePix<T>();
        setAxisSize(axis);
    }
    template<typename T, typename ...Arguments> void setAxisSize(Arguments... args)
    {
        setBytePix<T>();
        setNumberDimension(sizeof...(args));
        setAxeSize(0, args...);
    }
    int bytepix()const;

    void setType(Type value);
    Type type() const;
    const NufFitsHeadRecord & headRecord() const;

    size_t sizeDataSequence() const;

    template<typename... T> void setBinaryTable(T && ...args)
    {
        return;
    }

    bool hasEnd()const;
    void addEnd();

    bool correct()const;

    void clear();

private:
    NuFitsDescriptorPrivate *d;
};




#endif // NUFITSDESCRIPTOR_H
