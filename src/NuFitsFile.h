#pragma once

#include <string>
#include <unordered_map>
#include <vector>

/**
\class NuFitsFile

\brief Предназначен для работы с FITS-файлами

При открытрии Fits-файла на чтение тут же считывает primary header и обеспечивает доступ к чтению данных
(точно также должно делать при перходе с Unit-а на unit, но пока этого нет)

Запись fits-файла идет схожим образом: header формируется отдельно, а потом записывается единым куском,
после чего открыватеся доступ на потоковый ввод данных
  
\fn NuFitsFile::NuFitsFile
Конструктор
\fn NuFitsFile~NuFitsFile
закрыть файл и уничтожить объект

\fn NuFitsFile::setFileName
Задать имя файла, которое следует открыть. (Пока не вызван open можно работать с предыдущим фалом)
\fn NuFitsFile::fileName
Вернуть имя файла, открытого в данный момент, или который пытались в последний раз.
\fn NuFitsFile::open(OpenMode mode)
\reimp
Открыть файл в режиме \a mode
\fn NuFitsFile::open(const QString &fileName, OpenMode mode)
Открыть файл \a fileName в режиме \a mode

\fn NuFitsFile::readAll(char *data)
Прочесть структурный блок до конца
\fn NuFitsFile::atEnd
Возвращает \a true, если файл прочитан полностью
\fn NuFitsFile::close()
Закрыть файл.

\fn NuFitsFile::bytesAvailable()
Возвращает сколько еще осталось байт в структурном блоке

\fn NuFitsFile::setAxeSize(int number, int size)
Задать размер \a szie оси \a number
\fn NuFitsFile::setAxisSize
Задать размеры по осям. Размерность файла становиться размерностью массива
\fn NuFitsFile::setNumberDimension(int value)
Задать размерность Fits-файла

**/

class NuFitsDescriptor;
class NuFitsFilePrivate;
class NuFitsFile
{
public:
    enum Error{NoError, ///< Ошибок нет
                   DoesntExist, ///< Файл не существует
                   UncorrectOpenMode, ///< Невозможно открыть в выбранном режиме
                   CantRead, ///< Чтение невозможно
                   CantWrite ///< Запись невозможна
                  };
    enum OpenMode{
        ReadOnly = 0x1,
        WriteOnly = 0x10,
        ReadWrite = ReadOnly | WriteOnly
    };
public:
    explicit NuFitsFile();
    virtual ~NuFitsFile();
    void setFileName(const std::string &name);
    const std::string &fileName();

    bool open(OpenMode mode);
    bool open(const std::string &fileName, OpenMode mode);
    OpenMode openMode();



    int64_t readAll(char *data);
    template<typename T> inline  int64_t readAll(std::vector<T> *dest){ dest->resize(bytesAvailable()/sizeof(T)); return readAll( reinterpret_cast<char*>(dest->data())); }


    bool atEnd();
    void close();
    bool isOpen();
    int64_t bytesAvailable() const;

    NuFitsDescriptor * descriptor();
    const NuFitsDescriptor * descriptor() const;

    void setDescriptor(NuFitsDescriptor * value);

    int64_t read(char *data, int64_t maxlen);
    int64_t write(const char *data, int64_t len);
protected:
    int64_t readData(char *data, int64_t maxlen);
    int64_t writeData(const char *data, int64_t len);
private:
    NuFitsFilePrivate *d;
};
