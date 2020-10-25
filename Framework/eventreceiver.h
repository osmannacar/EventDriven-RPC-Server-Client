#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H
#include <thread>
#include <chrono>
#include <tuple>

#include <iostream>
#include <string>
#include "Serilization/templatereaderswriters.h"
#include <condition_variable>

//// helper function to print a tuple of any size

//template<typename Tuple, std::size_t N>
//struct TuplePrinter {
//    static void print(const Tuple& t)
//    {
//        std::cout << std::get<(N-1)>(t) << std::endl;
//        TuplePrinter<Tuple, N-1>::print(t);
//    }
//};

//template<typename Tuple>
//struct TuplePrinter<Tuple, 1> {
//    static void print(const Tuple& t)
//    {
//        std::cout << std::get<0>(t) << std::endl;
//    }
//};

//template<typename... Args>
//void print(const std::tuple<Args...>& t)
//{
//    TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
//}
//// end helper function



class EventReceiverBase
{

    std::mutex m_mtx;
    std::condition_variable m_cv;
    bool m_wake_up = false;
public:
    EventReceiverBase();
    virtual ~EventReceiverBase();
    long long id();
    static EventReceiverBase *getObjectFromId(long long id);
    void wait(int timeout_ms);
    void wakeUp();
    static bool contains(long long id);
    virtual bool readFromNetwork(TemplateReadersWriters *_readerwriter) = 0;
};



template<typename ...T> class EventReceiver : public EventReceiverBase{
    std::tuple<T...> _args;
public:
    void set(const T &...args){
        this->_args = std::tuple<T...>(args...);
    }
    template<std::size_t i>
    typename std::tuple_element<i, std::tuple<T...>>::type &get(){
        return std::get<i>(this->_args);
    }

    static EventReceiver<T...> *getObjectFromAddress(long long id){
        if(!EventReceiverBase::contains(id))
            return nullptr;
        return  reinterpret_cast<EventReceiver<T...>*>(id);
    }

    virtual bool readFromNetwork(TemplateReadersWriters *_readerwriter) override{
        bool flag = _readerwriter->unMarshallTupple(this->_args);
        return  flag;

    }
};


#endif // EVENTRECEIVER_H
