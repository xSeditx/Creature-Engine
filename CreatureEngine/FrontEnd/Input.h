#pragma once
/*=========================================================================
              # Input System Module  #         
     Defines the various Input Structures as well as Abstracting the
Event Types incase a different Window and Input managing system is desired
 in the future
=========================================================================*/


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


#define   UNKOWN_FIRSTEVENT			                SDL_FIRSTEVENT
#define   UNKOWN_QUIT					            SDL_QUIT
#define   UNKOWN_APP_TERMINATING		            SDL_APP_TERMINATING
#define   UNKOWN_APP_LOWMEMORY			            SDL_APP_LOWMEMORY
#define   UNKOWN_APP_WILLENTERBACKGROUND            SDL_APP_WILLENTERBACKGROUND
#define   UNKOWN_APP_DIDENTERBACKGROUND	            SDL_APP_DIDENTERBACKGROUND
#define   UNKOWN_APP_WILLENTERFOREGROUND            SDL_APP_WILLENTERFOREGROUND
#define   UNKOWN_APP_DIDENTERFOREGROUND	            SDL_APP_DIDENTERFOREGROUND
#define   UNKOWN_WINDOWEVENT			            SDL_WINDOWEVENT
#define   UNKOWN_SYSWMEVENT				            SDL_SYSWMEVENT
#define   UNKOWN_KEYDOWN	                        SDL_KEYDOWN
#define   UNKOWN_KEYUP					            SDL_KEYUP
#define   UNKOWN_TEXTEDITING			            SDL_TEXTEDITING
#define   UNKOWN_TEXTINPUT				            SDL_TEXTINPUT
#define   UNKOWN_KEYMAPCHANGED			            SDL_KEYMAPCHANGED
#define   UNKOWN_MOUSEMOTION			            SDL_MOUSEMOTION
#define   UNKOWN_MOUSEBUTTONDOWN		            SDL_MOUSEBUTTONDOWN
#define   UNKOWN_MOUSEBUTTONUP			            SDL_MOUSEBUTTONUP
#define   UNKOWN_MOUSEWHEEL				            SDL_MOUSEWHEEL
#define   UNKOWN_JOYAXISMOTION			            SDL_JOYAXISMOTION
#define   UNKOWN_JOYBALLMOTION			            SDL_JOYBALLMOTION
#define   UNKOWN_JOYHATMOTION			            SDL_JOYHATMOTION
#define   UNKOWN_JOYBUTTONDOWN			            SDL_JOYBUTTONDOWN
#define   UNKOWN_JOYBUTTONUP			            SDL_JOYBUTTONUP
#define   UNKOWN_JOYDEVICEADDED			            SDL_JOYDEVICEADDED
#define   UNKOWN_JOYDEVICEREMOVED		            SDL_JOYDEVICEREMOVED
#define   UNKOWN_CONTROLLERAXISMOTION	            SDL_CONTROLLERAXISMOTION
#define   UNKOWN_CONTROLLERBUTTONDOWN	            SDL_CONTROLLERBUTTONDOWN
#define   UNKOWN_CONTROLLERBUTTONUP		            SDL_CONTROLLERBUTTONUP
#define   UNKOWN_CONTROLLERDEVICEADDED	            SDL_CONTROLLERDEVICEADDED
#define   UNKOWN_CONTROLLERDEVICEREMOVED            SDL_CONTROLLERDEVICEREMOVED
#define   UNKOWN_CONTROLLERDEVICEREMAPPED           SDL_CONTROLLERDEVICEREMAPPED
#define   UNKOWN_FINGERDOWN				            SDL_FINGERDOWN
#define   UNKOWN_FINGERUP				            SDL_FINGERUP
#define   UNKOWN_FINGERMOTION			            SDL_FINGERMOTION
#define   UNKOWN_DOLLARGESTURE			            SDL_DOLLARGESTURE
#define   UNKOWN_DOLLARRECORD			            SDL_DOLLARRECORD
#define   UNKOWN_MULTIGESTURE			            SDL_MULTIGESTURE
#define   UNKOWN_CLIPBOARDUPDATE		            SDL_CLIPBOARDUPDATE
#define   UNKOWN_DROPFILE				            SDL_DROPFILE
#define   UNKOWN_DROPTEXT				            SDL_DROPTEXT
#define   UNKOWN_DROPBEGIN				            SDL_DROPBEGIN
#define   UNKOWN_DROPCOMPLETE			            SDL_DROPCOMPLETE
#define   UNKOWN_AUDIODEVICEADDED		            SDL_AUDIODEVICEADDED
#define   UNKOWN_AUDIODEVICEREMOVED		            SDL_AUDIODEVICEREMOVED
#define   UNKOWN_RENDER_TARGETS_RESET	            SDL_RENDER_TARGETS_RESET
#define   UNKOWN_RENDER_DEVICE_RESET	            SDL_RENDER_DEVICE_RESET
#define   UNKOWN_USEREVENT				            SDL_USEREVENT
#define   UNKOWN_LASTEVENT                          SDL_LASTEVENT



/*
SDL event                                                      iOS                            Android             WinRT
===================================================================================================================================
SDL_APP_TERMINATING                                applicationWillTerminate()            |     onDestroy()     |   Exiting()
The application is being terminated by the OS.											 |					   |
																						 |					   |
																						 |					   |
SDL_APP_LOWMEMORY                                  applicationDidReceiveMemoryWarning()  |     onLowMemory()   |
The application is low on memory, free memory if possible.								 |					   |
																						 |					   |
																						 |					   |
SDL_APP_WILLENTERBACKGROUND                        applicationWillResignActive()         |     onPause()       |   Suspending()
The application is about to enter the background.										 |					   |
																						 |					   |
																						 |					   |
																						 |					   |
SDL_APP_DIDENTERBACKGROUND                         applicationDidEnterBackground()       |     onPause()       |   Suspending()                                                                 //
The application did enter the background and may not get CPU for some time.				 |					   |
     																					 |					   |
  																						 |					   |
  																						 |					   |
																						 |					   |
SDL_APP_WILLENTERFOREGROUND                        applicationWillEnterForeground()      |     onResume()      |    Resuming()                                                                       //
The application is about to enter the foreground.										 |					   |
          																				 |					   |
   																						 |					   |
  																						 |					   |
																						 |					   |
SDL_APP_DIDENTERFOREGROUND                          applicationDidBecomeActive()         |     onResume()      |    Resuming()       
The application is now interactive.														 |					   |
                																		 |					   |
*/