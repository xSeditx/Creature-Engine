#include"Core/Common.h"
#include<vector>
#include"EventSystem.h"

struct Entity;/// Temp to get this running

class Observer
{
public:
    virtual ~Observer() {}
    virtual void onNotify(const Entity& entity, Event event) = 0;
};
 
class Subject
{
private:
    std::vector<Observer*> Observers;
    uint32_t Observer_Count() { return Observers.size(); }
 
public:
    void addObserver(Observer* _observer)
    {
        Observers.emplace_back(_observer);
    }

    void removeObserver(Observer* _observer)
    {
        Observers.erase(std::find(Observers.begin(), Observers.end(), &_observer));
    }


protected:
    void Notify(const Entity& entity, Event event)
    {
        for (auto& O: Observers)
        {
            O->onNotify(entity, event);
        }
    }

};





class TestSubject : public Subject
{
public:
    void Update(Entity& entity){}
};