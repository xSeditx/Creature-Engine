#include"EventSystem.h"


#include"Application.h"
void EventHandler::PollEvents() noexcept
{
    MSG msg;
    while (PeekMessage(&msg, Application::getWindow().g_Handle(), 0, 0, PM_REMOVE))
    {
        //		 Application::Window::EventHandler::get().PostMSG(msg);
        Application::getWindow().Messenger().PostMSG(msg);
        //		 Print("Message" << msg.message);
        DispatchMessage(&msg);
    }
}
void EventHandler::Dispatch(Event msg) noexcept
{

    for (auto& Callback : ListenerMap[msg.message])
    {
        Callback->Handler(msg);
    }
}
///==================================================================================================================

void EventHandler::PostMSG(Event msg) noexcept
{
    Messages.push(msg);
}
void EventHandler::RegisterListener(MsgType msg, Listener& handler) noexcept
{
    ListenerMap[msg].push_back(&handler);
}
void EventHandler::RemoveListener(MsgType msg, Listener& handler) noexcept
{
    ListenerMap[msg].erase(std::find(ListenerMap[msg].begin(), ListenerMap[msg].end(), &handler));
}
bool EventHandler::PeekMSG(Event& msg, unsigned int rangemin, unsigned int rangemax, int handlingflags) noexcept
{
    if (Messages.size())
    {
        msg = Event(Messages.front());
        if (msg.message > rangemin && msg.message < rangemax)
        {
            switch (handlingflags)
            {
            case 0:
            {// ONLY USE THIS FOR NOW
                Messages.pop();
                return true;
            }break;
            case 2:
            {
                __debugbreak();
            }break;
            case 3:
            {/// THIS WILL CURRENTLY BE A PROBLEM DO NOT USE IT
                Messages.pop();
                PostMSG(msg);
                __debugbreak();
            }break;

            }
        }
    }
    return false;//Messages.size() > 0;
}
bool EventHandler::PeekMSG(Event& msg) noexcept
{

    if (Messages.size() > 0)
    {
        msg = Event(Messages.front());
        Messages.pop();
        return true;
    }
    return false;
}


Event& make_msg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Event result;
    result.hwnd = hwnd;
    result.message = uMsg;
    result.wParam = wParam;
    result.lParam = lParam;

    POINT p;
    GetCursorPos(&p);
    result.pt = p;

    result.time = GetTickCount();
    return result;
}
