#include <QString>
#include <QtTest>

#include <NuFitsDescriptor.h>
#include <NufFitsHeadRecord.h>
#include <vector>
class NuFitsDescriptorTest : public QObject
{
    Q_OBJECT

public:
    NuFitsDescriptorTest();

private Q_SLOTS:
    void defualtValue();
    void addPrimaryHead();
    void setAxisSyzeVariants();
    void ortogonizePropertyCheck();
};

NuFitsDescriptorTest::NuFitsDescriptorTest()
{
}

void NuFitsDescriptorTest::defualtValue()
{
    NuFitsDescriptor descriptor;
    QVERIFY2(descriptor.type() == NuFitsDescriptor::Unset, "Default value isn't unset");
    QVERIFY2(descriptor.data()->size() ==0, "Default container isn't empty");
    QVERIFY2(descriptor.sizeDataSequence() ==0, "Default container isn't empty");
}

void NuFitsDescriptorTest::addPrimaryHead()
{

    std::vector<NufFitsHeadRecord> container = {NufFitsHeadRecord("SIMPLE  =                    T / file does conform to FITS standard             "),
                           NufFitsHeadRecord("BITPIX  =                   16 / number of bits per data pixel                  "),
                           NufFitsHeadRecord("NAXIS   =                    2 / number of data axes                            "),
                           NufFitsHeadRecord("NAXIS2  =                 1032 / length of data axis 2                          "),
                           NufFitsHeadRecord("END")
                          };

    NuFitsDescriptor descriptor;
   // descriptor.setContainer(container);

    QVERIFY2(descriptor.type() == NuFitsDescriptor::Primary, "");
    QVERIFY2(descriptor.hasEnd(), "Wrong has end");
    QVERIFY2(descriptor.correct(), "Correct header parse incorrect");
}

void NuFitsDescriptorTest::setAxisSyzeVariants()
{
    std::vector<NufFitsHeadRecord> container={NufFitsHeadRecord("BITPIX  = 16"), NufFitsHeadRecord("NAXIS   = 2"), NufFitsHeadRecord("NAXIS1  = 1392"), NufFitsHeadRecord("NAXIS2  = 1032")};

    NuFitsDescriptor descriptor;
    //descriptor.setAxeSize();
}

void NuFitsDescriptorTest::ortogonizePropertyCheck()
{
    std::vector<NufFitsHeadRecord> container={NufFitsHeadRecord("SIMPLE  = T"), NufFitsHeadRecord("BITPIX  = 16"), NufFitsHeadRecord("NAXIS   = 2"),
                                              NufFitsHeadRecord("NAXIS1  = 1392"), NufFitsHeadRecord("NAXIS2  = 1032"),NufFitsHeadRecord("END")};

    NuFitsDescriptor descriptor;
    descriptor.addEnd();
    descriptor.setAxisSize<short>(1392,1032);
    descriptor.setType(NuFitsDescriptor::Primary);
    descriptor.setBytePix(16);
    //QVERIFY( *descriptor.data() == container);
    descriptor.clear();
}



QTEST_APPLESS_MAIN(NuFitsDescriptorTest)

#include "tst_NuFitsDescriptorTest.moc"
