#pragma once
/*======================================================================= 
                     # Event System Module  #                          

     Handles the Acceptance and Dispatching                 
  Of Events Registered by calling the Listener Object assigned to the 
  given message

 =======================================================================*/


/*************************************************************************/
/*                       This file is part of:                           */
/*                      Unknown_Engine ENGINE                            */
/*              https://github.com/xSeditx/Unknown-GameEngine            */
/*************************************************************************/
/* Copyright (c) 2019 Sedit                                              */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*=======================================================================*/

#include"FrontEnd\GUI\EventSystem.h"
#include"FrontEnd\Input.h"


namespace Unknown_Engine
{
	namespace HID
	{

		Event::Event()
			:
			ID(0),
			WinID(0),
			Time(0)
		{
			memset(&data, 0, 16);
		}
		Event::Event(MsgType msg)
			:
			ID(msg),
			WinID(0),
			Time(0)
		{
			memset(&data, 0, 16);
		}

		EventSystem::EventSystem()
		{
		}
		EventSystem & EventSystem::Instance()
		{
			static EventSystem instance;
			return instance;
		}
		void EventSystem::PostMSG(Event msg)
		{
			Messages.push(msg);
		}
		void EventSystem::RegisterListener(MsgType msg, Listener & handler)
		{
			ListenerMap[msg].push_back(&handler);
		}
		void EventSystem::RemoveListener(MsgType msg, Listener &handler)
		{
			ListenerMap[msg].erase(std::find(ListenerMap[msg].begin(), ListenerMap[msg].end(), &handler));
		}


		void EventSystem::Dispatch(Event msg)
		{
			for (auto& Callback : ListenerMap[msg.ID])
			{
				Callback->Handler(msg);
			}
		}
		bool EventSystem::PeekMSG(Event &_msg, unsigned int rangemin, unsigned int rangemax, int handlingflags)
		{// None Blocking pulls a message from the Message Queue and returns a reference to it in _msg
			if (Messages.size())
			{
				_msg = Event(Messages.front());
				if (_msg.ID > rangemin && _msg.ID < rangemax)
				{
					switch (handlingflags)
					{
					case 0://REMOVE_MESSAGE:
					{// ONLY USE THIS FOR NOW
						Messages.pop();
						return true;
					}break;
					case 1://KEEP_MESSAGE:
					{
						__debugbreak();
					}break;
					case 2://REPOST_MESSAGE:
					{/// THIS WILL CURRENTLY BE A PROBLEM DO NOT USE IT
						Messages.pop();
						PostMSG(_msg);
						__debugbreak();
					}break;

					}
				}
			}
			return false;//Messages.size() > 0;
		}
		void EventSystem::PollEvents()
		{
			__debugbreak(); // Change this to an SDL event loop to poll the native events and handle them properly
			//InputManager::PollNativeEvents();
		}

	}// End of HID namespace
}// End of Unknown_Engine Namespace
