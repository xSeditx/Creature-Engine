#pragma once


#include<queue>

#include<string>
//#include<cstdint>
#include<iostream>
#include<Windows.h>
#include<windowsx.h>
#include<unordered_map>

#include"Core/Common.h"

using MsgType = uint32_t;
using Event = MSG;

// Event& make_msg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



/* Creates a Listener Object to Handle whatever message is assigned to it
Syntax:   Listener(void(*func)(Event))   */
struct CREATURE_API Listener
{
public:
    Listener(void(*func)(Event)) noexcept
        :
        _handler(func)
    {}
    void SetHandler(void(*func)(Event)) noexcept
    {
        _handler = func;
    }
    inline void Handler(Event msg) noexcept
    {
        _handler(msg);
    };
private:
    void(*_handler)(Event msg);
};


struct CREATURE_API EventHandler
{
public:

    NO_COPY_OR_ASSIGNMENT(EventHandler);
    EventHandler() = default;

    void PollEvents() noexcept;
    void PostMSG(Event msg) noexcept;
    bool PeekMSG(Event& msg) noexcept;
    bool PeekMSG(Event& msg, unsigned int rangemin, unsigned int rangemax, int handlingflags) noexcept;

    void Dispatch(Event msg) noexcept;

    void RegisterListener(MsgType msg, Listener& handler) noexcept;
    void RemoveListener(MsgType msg, Listener& handler) noexcept;

    EventHandler(EventHandler&& _other) noexcept
        :// Allows Application::Window to move
        Messages(std::move(_other.Messages)),
        ListenerMap(std::move(_other.ListenerMap))
    {
        _other.ListenerMap.clear();
    }
    EventHandler& operator = (EventHandler  &&_other) noexcept
    {/// Should likely be Swapping here?
        return _other;//*this;
    }
private:
    std::queue<Event> Messages;
    std::unordered_map<MsgType, std::vector<Listener*>> ListenerMap;
};


//7/21/2020 Fixed this WARNING
//   WARN_ME("FIX THIS SHIT, IDK WHY MY BRAIN IS SHITTING ITSELF RIGHT NOW BUT ASSIGNMENT MOVE SHOULD NOT WORK");
//EventHandler(EventHandler&& _other) noexcept
//    :
//    Messages (std::move(_other.Messages)),
//    ListenerMap(std::move( _other.ListenerMap))
//{
//    _other.ListenerMap.clear();
//}
//EventHandler& operator = (EventHandler  &&_other) noexcept
//{/// Should likely be Swapping here?
//    return _other;//*this;
//}
//    EventHandler(EventHandler const&) = delete; // and prevents Copies from being made
//    EventHandler operator = (EventHandler const&) = delete;

