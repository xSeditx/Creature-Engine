___
## ThreadPool Implementation
___


___
### Program Roughly follows the order which the Methods are place in our Threadpool.cpp                                                                                                                              
___
#### INITIALIZERS:
Handles Initializing the Threadpool Instance, Creating the Threads and Running a Scheduler on the Thread
___

*Step 1)* get() is called which calls a Static instance of Threadpool:: 												   <br>
*Step 2)* This in turn makes the one and only call to the Constructor and thanks to C++ 14 Standards singletons are		   <br>
           Initialized in a thread safe manner																			   <br>
*Step 3)* Constructor Creates an Array of N threads each containing a Lambda which calls the Run(int _i) Function          <br>                                                                                                       
___                                                                                          
#### SUBMITTERS:
Responsible for Accepting Functions into our Threadpool, managing created Task and Returning a Future value to the user.      
Distrubuted work into our system fairly
___

*Step 1)a:* Async(Function, Args...) is called, Returns an std::future<Func_return_type> to the user.                       <br> 
*Step 2)a:* Async Wraps the Function and its Arguments into an new asyncTask(Function, Args...); 				            <br> 
       *b:* Try_Push is called with a Pointer to the base class of asyncTask : Wrapper_Base 					            <br> 
*Step 3)a:* Try_Push attemps to Add Function to our JobQueue. If we can not Lock it we return False; 			            <br> 
                else we push the Function pointer into the Queue and Notify the Condition Variable that it can	            <br> 
                stop waiting it now has work to do																            <br> 
___
#### EXECUTORS:
Handles Running the Threadpool, Keeps tabs on which Queues have work in them and Runs Functions as they are pushed in 
___

*Step 1)* Run Cycles infinitely constantly going over all the threads in the Thread pool looking for Work.                   <br>     
*Step 2)* Try_Pop(Wrapper_Base*& _func) is in a For Loop which goes over all the Queues looking for work.					 <br> 
              If it finds it it Returns a pointer to Work through _func.													 <br> 
        																													 <br> 
*Step 3)* <p>If it fails to find anything in the other threads Pop(Wrapper_Base *& _func) is called 							 <br> 
              Pop checks to make sure Quit has not been called and checks his own Queue for work. If there is no			 <br> 
              Work Pop waits on a Conditional lock to conserve CPU power while there is not Jobs</p>							 <br> 
			                                                                                                                 <br> 
*Step 4)* If Queue is Empty and Quit Signal has been given Pop returns False to Run and Run breaks out of 					 <br> 
              Its Infinite Loop when Pop fails to return a function which triggers the Shut down Sequence of the 			 <br> 
              ThreadPool
___
#### DESTRUCTORS:  
Handles the Closing of threads and Deletion of any Allocated resources.
___

*Step 1)* Done is called which Locks a Mutex and sets the is_Done flag letting the Pool know its closing                      <br>                                                     
              Condition Variable is also set to Notify all and wake them up so they can Initialize the shut down              <br>                                                      
              process                                                                 										  <br> 
*Step 2)* Destructor Called which Cycles over the Threadpools Done functions to double check and then                         <br>                                           
*Step 3)* Joins all the Threads for our Threadpool and closes down.                                                           <br>         
___                                                                             
                                                                             



Namespace Core; <br> 
Namespace Threading; <br> 


___
#
___

Function Name      |  Description
:------------------|:---------------------
`template<typename R> bool is_ready(R _future)`| Nonblocking test of std::future<R> passed as the argument returning a boolean



___
# class ThreadPool
___
Public User Interface
___

Function Name  | Description
:--------------|:--------------
`static ThreadPool &get();`| Returns a Single Instance of the Pool. 
`template<typename F, typename ...ARGS>` | 
`void Async(F&& _func, ARGS... _args);`| Execute the Function passed into _func by pushing it into a Threadpool Queue. Returns an std::future object of with a return type which matches that of the called function.

Data Members  | Description
:-------------|:-----------------------
`const size_t ThreadCount; `| Number of Threads Avalible for the System 
`std::vector<std::thread> Worker_Threads;`|   Vector of Worker threads which accept Jobs for Execution 
`std::vector<JobQueue> ThreadQueue;`| Jobs waiting to be feed into the Worker Thread Vector for Execution 



___
## Private Details 
The following has been removed from user interface instead choosing to only expose Threadpool::Async( Function*, Arguments...)
___
# class JobQueue
___

___

Function Name | Description
------------|-----------------
`bool Try_Pop(Worker_Function &func)`  | Try to aquire a function off the Queue to run
`void Done()` | Signal when Finished
`bool pop(Worker_Function &func)`| Remove an Item from the Job Queue and return it to the User
`template<typename F> bool try_push(F&& func)` | Attempt to push a Job onto the Queue
`template<typename F> void push(F&& func)` | Force a Push onto the JobQueue


Data Members | Description
-------------|-----------------
`std::deque< Worker_Function> TaskQueue;`|
`std::condition_variable is_Ready; `|
`std::mutex QueueMutex;` |
`bool is_Done;` |

___



___
# ThreadPool
___
*NOTE:* Copy constructors have been removed and Constructor hidden. Essentially A singleton accessed via the `get()` method
___

Function Name | Description
--------------|--------------
`void Run(unsigned int _i);` | Starts Specific Thread in the Pool

Data Members | Description
-------------|-----------------------
`const size_t ThreadCount; `| Number of Threads Avalible for the System
`std::vector<std::thread> Worker_Threads;`| Vector of Worker threads which accept Jobs for Execution
`std::vector<JobQueue> ThreadQueue;`| Jobs waiting to be feed into the Worker Thread Vector for Execution
`std::atomic<unsigned int> Index;`|
___


___		
 Wrapper for functions sent into our Threadpool.<br>
 An std::deque<Wrapper_Base*> _func Object accepts functions from Threadpool::Async(func, args...) and wraps them to pass pointers to the heap allocated heap
___
# Wrapper_Base
___
*NOTE:* Polymorphic Base class to allow ThreadQueue to accept any function signature
___



Function Name     |  Description
------------------|---------------------
`virtual ~Wrapper_Base()`|
`virtual void Invoke() = 0;`|
`void operator()();`|

Data Members | Description
-------------|-----------------------
`asyncStatus Status`| Current status of the function passed in. Mainly for Debug information but will tell the current status of any item passed into the Threadpool

   Types       | Possible Values
---------------|--------------------
`enum asyncStatus`|	Empty, Valid, Waiting, Busy, Submitted, Ready, Aquired
___




___
# asyncTask
___
 Wrapper for functions sent into our Threadpool. An std::deque<Wrapper_Base*> _func Object accepts functions from Threadpool::Async(func, args...) and wraps them to pass pointers to the heap allocated heap
___		

template<*typename _Func*, *typename ...ARGS*> <br>
struct asyncTask: public Wrapper_Base <br>




Function Name     |  Description
------------------|---------------------
`asyncTask(_Func&& _function, ARGS&&... _args) noexcept`| Function and Argument to wrapped up to Asyncronously run in the Threadpool. 
`virtual ~asyncTask()` | Virtual destructor to ensure complete deletion of allocated object
`asyncTask(asyncTask&& _other) noexcept`| Perfect forwarding move constructor
`asyncTask& operator=(asyncTask&& _other) noexcept`| Perfect forwarding Assignment constructor
`virtual void Invoke()  override` |
`std::future<type> get_future()` | Return an std::future of type Function return type passed into asyncTask c_Tor
`bool is_ready()`| Check to see if the Function has returned (Deprecated 12/2/2019: R: I believe this is unable to be aquired with the API change to the Threadpool interface. with asyncTask hidden from the user checking its status is strictly debug information )
.|.
`asyncTask(const asyncTask&)` | Deleted;
`asyncTask& operator=(const asyncTask& _other)` | Deleted;
`asyncTask(asyncTask&&)` | Deleted;
`asyncTask& operator=(asyncTask&& _other)` | Deleted;


Data Members | Description
-------------|-----------------------
`Fptr Function`| Function pointer of the function passed into the Templated c_Tor
`std::tuple<ARGS...> Arguments;`| Tuple of the arguments passed into the Template
`std::promise<type> ReturnValue;`| A promise of the return type of the Function passed in
`type get()` | Blocking return of the results of the function call

   Types       | Description
---------------|--------------------
`type` | Return type of the Passed function
`Fptr` | Function pointer to the passed Function
`ARGS` | Argument types for Function Call

	





