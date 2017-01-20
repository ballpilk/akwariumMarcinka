#ifndef TIMEREVENTQUEUE_H
#define TIMEREVENTQUEUE_H

class TimerEventListener
{
public:
    virtual long unsigned onTime()=0;
    virtual ~TimerEventListener(){}
};
struct EventElement
{
    EventElement(TimerEventListener* obj, unsigned long ptime)
    {
        callback=obj;
        time = ptime;
    }
    EventElement()
    :callback(0),time(0)
    {}
    TimerEventListener* callback;
    unsigned long time;
private:

};
#define LISTMAXLEN 20
class List
{
public:
    void insert(const EventElement& elemToInsert)
    {
        unsigned short count(0);
        while(count < eventCount && eventList[(currentHead+count)%LISTMAXLEN].time < elemToInsert.time )
        {
            count++;
        }

        if(eventCount < LISTMAXLEN-1)
            eventCount++;
        else
            return;//GAAAAAAAAAA! we have too many elements
        EventElement tmp, tmp2 = elemToInsert;
        while(count < eventCount)//insert and move the rest by one place
        {                        //one element (last one - newer initalized) gets dropped
            tmp = eventList[(currentHead+count)%LISTMAXLEN];
            eventList[(currentHead+count)%LISTMAXLEN] = tmp2;
            count++;
            tmp2 = tmp;
        }
    }

    void popFront(long unsigned ptime)
    {
        int count(0);
        while(eventCount > 0 && eventList[currentHead].time <  ptime && count++<LISTMAXLEN)
        {
            unsigned int adjustment(0);
            if(eventList[currentHead].callback != 0 )
            {
                adjustment = eventList[currentHead].callback->onTime();
                eventList[currentHead].time = ptime + adjustment;
            }
            else{
                Serial.println("GAAAAAAAAAAAAAAA nullptr");
                break;
            }
            if(adjustment==0)
            {
                moveRight();
            }
            else
            {
                reSort();
            }
        }
    }
    ~List(){}
    List()
    : eventCount(0), currentHead(0)
    {}
private:
    void swap(unsigned short first, unsigned short second)
    {
        EventElement tmp = eventList[second];
        eventList[second] = eventList[first];
        eventList[first] = tmp;
    }
    void reSort(unsigned short from=0)
    {
        unsigned short count(from);
        while(count+1 < eventCount &&
              eventList[(count+currentHead)%LISTMAXLEN].time > eventList[(count+1+currentHead)%LISTMAXLEN].time)
        {
            swap((count+currentHead)%LISTMAXLEN, (count+1+currentHead)%LISTMAXLEN);
        }
    }
    void moveRight()
    {
        currentHead = (currentHead+1)%LISTMAXLEN;
        eventCount--;
    }
    unsigned short currentHead;
    unsigned short eventCount;
    EventElement eventList[LISTMAXLEN];
};
class TimerQueue
{
public:
    void registerTimerEvent(TimerEventListener* obj, unsigned long time)
    {
        _eventList.insert(EventElement(obj, micros()+time));
    }
    void periodicTimerTick(unsigned long time = 0)
    {
        _eventList.popFront(time==0?micros():time);
    }
private:
    List _eventList;
};

#endif // TIMEREVENTQUEUE_H
