#include <QString>
#include <QtTest>

#include <NufFitsHeadRecord.h>
class NuFitsHeadReacordTest : public QObject
{
    Q_OBJECT

public:
    NuFitsHeadReacordTest();

private Q_SLOTS:
    void ordianryParse();
    void parseWithEmptyValue();
    void parseWithEmptyComment();
    void parseWithEmptyValueAndComment();
    void formString();
};

NuFitsHeadReacordTest::NuFitsHeadReacordTest()
{
}

void NuFitsHeadReacordTest::ordianryParse()
{
    NufFitsHeadRecord record;
    record.fromString("BITPIX  =                   16 / number of bits per data pixel                  ");
    QVERIFY(record.keyword == "BITPIX");
    QVERIFY(record.value=="16");
    QVERIFY(record.comment =="number of bits per data pixel");
}

void NuFitsHeadReacordTest::parseWithEmptyValue()
{
    NufFitsHeadRecord record;
    record.fromString("BITPIX  =                   16 / number of bits per data pixel                  ");
    record.fromString("BITPIX  =   / number of bits per data pixel ");
    QVERIFY(record.keyword == "BITPIX");
    QVERIFY(record.value=="");
    QVERIFY(record.comment == "number of bits per data pixel");
}

void NuFitsHeadReacordTest::parseWithEmptyComment()
{
    NufFitsHeadRecord record;
    record.fromString("BITPIX  =                   16 / number of bits per data pixel                  ");
    record.fromString("BITPIX  =                   16 ");
    QVERIFY(record.keyword == "BITPIX");
    QVERIFY(record.value=="16");
    QVERIFY(record.comment == "");
}

void NuFitsHeadReacordTest::parseWithEmptyValueAndComment()
{
    NufFitsHeadRecord record;
    record.fromString("BITPIX  =                   16 / number of bits per data pixel                  ");
    record.fromString("END     ");
    QVERIFY(record.keyword == "END");
    QVERIFY(record.value=="");
    QVERIFY(record.comment == "");
}

void NuFitsHeadReacordTest::formString()
{
    NufFitsHeadRecord record("BITPIX", "16", "number of bits per data pixel");
    auto str = record.toString();
    QVERIFY(str.length() == NufFitsHeadRecord::StringSize);
    QVERIFY(str =="BITPIX  = 16/number of bits per data pixel                                      ");
    record.value="";
    str = record.toString();
    QVERIFY(str =="BITPIX  /number of bits per data pixel                                          ");
    record.comment = "";
    str = record.toString();
    QVERIFY(str =="BITPIX                                                                          ");
}

QTEST_APPLESS_MAIN(NuFitsHeadReacordTest)

#include "tst_NuFitsHeadReacordTest.moc"
