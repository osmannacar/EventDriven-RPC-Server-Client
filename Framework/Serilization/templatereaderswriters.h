#ifndef READWRITEPARAMETERS_H
#define READWRITEPARAMETERS_H
#include <Serilization/abstractreader.h>
#include <Serilization/abstractwriter.h>
//variadic reader writer
class TemplateReadersWriters
{
    AbstractReader *_reader;
    AbstractWriter *_writer;
    template<typename T> bool readParameters(T &parameter){
        return this->_reader->read(parameter);
    }
    template<typename T, typename ...TParameters> bool readParameters(T &parameter, TParameters &...parameters){
        return (this->_reader->read(parameter) && this->readParameters(parameters...));
    }
    template<typename T> bool writeParameters(const T &parameter){
        return this->_writer->write(parameter);
    }
    template<typename T, typename ...TParameters> bool writeParameters(const T &parameter, const TParameters &...parameters){
        return (this->_writer->write(parameter) && this->writeParameters(parameters...));
    }
    template<typename Tuple, std::size_t N>
    struct TupleReader {
        static bool readTupleParameters(AbstractReader *reader, Tuple& t)
        {
            return (TupleReader<Tuple, N-1>::readTupleParameters(reader, t) && reader->read(std::get<(N-1)>(t)));
        }
    };

    template<typename Tuple>
    struct TupleReader<Tuple, 1> {
        static bool readTupleParameters(AbstractReader *reader, Tuple& t)
        {
            return reader->read(std::get<0>(t));
        }
    };

public:

    TemplateReadersWriters(AbstractWriter *writer, AbstractReader *reader){
        this->_writer = writer;
        this->_reader = reader;
    }

    template<typename ...TParameters> inline bool marshall(const TParameters &...parameters){
        return this->writeParameters(parameters...);
    }

    template<typename ...TParameters> inline bool unMarshall(TParameters &...parameters){
        return this->readParameters(parameters...);
    }

    template<typename... Args> bool inline unMarshallTupple(std::tuple<Args...>& t)
    {
        return TupleReader<decltype(t), sizeof...(Args)>::readTupleParameters(this->_reader, t);
    }

};

#endif // READWRITEPARAMETERS_H
