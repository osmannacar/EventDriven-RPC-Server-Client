#include "eventreceiver.h"

#include <QMap>
#include <QMutex>

QMap<long long, EventReceiverBase*> g_eventList;
QMutex g_event_mutex;

EventReceiverBase::EventReceiverBase()
{
    ::g_eventList.insert(id(), this);
}

EventReceiverBase::~EventReceiverBase()
{
    QMutexLocker locker(&::g_event_mutex);
    ::g_eventList.remove(id());
}

long long EventReceiverBase::id(){
    return (long long )this;
}

EventReceiverBase *EventReceiverBase::getObjectFromId(long long id)
{
    return ::g_eventList.value(id);
}

void EventReceiverBase::wait(int timeout_ms){
    //if timeout_ms is -1, wait until the response
    std::unique_lock<std::mutex> l_lck(this->m_mtx);
    auto l_sec = std::chrono::milliseconds(timeout_ms);
    this->m_cv.wait_for(l_lck, l_sec, [=]()->bool {return this->m_wake_up;});
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
}

void EventReceiverBase::wakeUp()
{
    this->m_wake_up = true;
}

bool EventReceiverBase::contains(long long id)
{
    return ::g_eventList.contains(id);
}
