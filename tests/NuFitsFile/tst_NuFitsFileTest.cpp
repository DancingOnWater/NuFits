#include <QString>
#include <QtTest>

#include <NuFitsFile.h>
#include <NuFitsDescriptor.h>
#include <vector>
#include <string>
#include <QDir>
class NuFitsFileTest : public QObject
{
    Q_OBJECT
private:
    std::string  validFileName = std::string(TEST_MATERIAL_PATH) + std::string("/test.fts");
public:
    NuFitsFileTest();

private Q_SLOTS:
    void openFile();
    void readPrimaryData();
    void writePrimaryData();
};

NuFitsFileTest::NuFitsFileTest()
{
}

void NuFitsFileTest::openFile()
{
    NuFitsFile fitsFile;
    QVERIFY2(! fitsFile.isOpen(), "New file is already open");
    if( ! fitsFile.open(validFileName, NuFitsFile::ReadOnly))
        QFAIL("Can't open correct file.");

    QVERIFY2( ! fitsFile.isOpen(), "We already open file, but method isOpen return false");
    QVERIFY2( ! fitsFile.atEnd(), "We already open file, but method atEnd return true");
}

void NuFitsFileTest::readPrimaryData()
{
    std::vector<int> buffer;
    NuFitsFile fitsFile;
    fitsFile.open(validFileName, NuFitsFile::ReadOnly);

    QVERIFY2( fitsFile.bytesAvailable() == -1, "After opening incorrect determine aviable bytes");
    QVERIFY2( ! fitsFile.atEnd(), "Already correct open file at end");
    buffer.resize(10);
    auto oldAviableSize = fitsFile.bytesAvailable();
    int readBytes = fitsFile.read( reinterpret_cast<char*>(buffer.data()), buffer.size());
    QVERIFY2( readBytes == buffer.size(), "Can't read part of data" );
    QVERIFY2( fitsFile.bytesAvailable() + buffer.size() == oldAviableSize, "We read too mach" );
    readBytes = fitsFile.readAll(&buffer);
    QVERIFY2(  readBytes == buffer.size(), "Can't read all data" );
    QVERIFY2( fitsFile.bytesAvailable() ==0, "After read all buffer isn't empty" );
}

void NuFitsFileTest::writePrimaryData()
{
    std::vector<char> buffer;
    NuFitsFile readFile, writeFile;
    readFile.open(validFileName, NuFitsFile::ReadOnly);
    buffer.resize(readFile.bytesAvailable());
    readFile.readAll(&buffer);

    std::string destFile = std::string(TEST_MATERIAL_PATH)+std::string("/dest.fts");
    QDir destpath;
    destpath.rmpath(QString(destFile.c_str()));
    QVERIFY2 (writeFile.open(destFile, NuFitsFile::WriteOnly) ==true, "Can't open file to write" );
    *writeFile.descriptor() =*readFile.descriptor();

    QVERIFY2(writeFile.write(buffer.data(), buffer.size()) == buffer.size(), "Write less bytes that in buffer");
    writeFile.close();
    writeFile.descriptor()->clear();
    writeFile.open(NuFitsFile::ReadOnly);
    QVERIFY2 ( *writeFile.descriptor() == *readFile.descriptor(), "Write descriptor doesn't correct" );
    std::vector<char> dataFromWriteFile;
    writeFile.readAll(&dataFromWriteFile);
    QVERIFY2(buffer == dataFromWriteFile, "in Write file no");
    char smallbuffer;
    QVERIFY2(writeFile.read(&smallbuffer, 1)== 0, "File doesn't empty");
    QVERIFY2(writeFile.atEnd(), "File doesn't empty");
}



QTEST_APPLESS_MAIN(NuFitsFileTest)

#include "tst_NuFitsFileTest.moc"
