___
## ThreadPool Implementation
___


___
### Program Roughly follows the order which the Methods are place in our Threadpool.cpp                                                                                                                              
___
#### INITIALIZERS:
Handles Initializing the Threadpool Instance, Creating the Threads and Running a Scheduler on the Thread
___

**Step 1) get() is called which calls a Static instance of Threadpool:: 
**Step 2) This in turn makes the one and only call to the Constructor and thanks to C++ 14 Standards singletons are
        Initialized in a thread safe manner
**Step 3) Constructor Creates an Array of N threads each containing a Lambda which calls the Run(int _i) Function                                                                                                                     
___                                                                                          
#### SUBMITTERS:
Responsible for Accepting Functions into our Threadpool, managing created Task and Returning a Future value to the user. Distrubuted work into our system fairly
___

**Step 1)a: Async(Function, Args...) is called, Returns an std::future<Func_return_type> to the user.
**Step 2)a: Async Wraps the Function and its Arguments into an new asyncTask(Function, Args...); 
         b: Try_Push is called with a Pointer to the base class of asyncTask : Wrapper_Base 
**Step 3)a: Try_Push attemps to Add Function to our JobQueue. If we can not Lock it we return False; 
                else we push the Function pointer into the Queue and Notify the Condition Variable that it can
                stop waiting it now has work to do
___
#### EXECUTORS:
Handles Running the Threadpool while active, Keeps tabs on which Queues have work in them and Runs Functions stored in the Queues when their is Work
___

**Step 1) Run Cycles infinitely constantly going over all the threads in the Thread pool looking for Work. 
**Step 2) Try_Pop(Wrapper_Base*& _func) is in a For Loop which goes over all the Queues looking for work.
              If it finds it it Returns a pointer to Work through _func.
        
**Step 3) If it fails to find anything in the other threads Pop(Wrapper_Base*& _func) is called 
              Pop checks to make sure Quit has not been called and checks his own Queue for work. If there is no
              Work Pop waits on a Conditional lock to conserve CPU power while there is not Jobs
**Step 4) If Queue is Empty and Quit Signal has been given Pop returns False to Run and Run breaks out of 
              Its Infinite Loop when Pop fails to return a function which triggers the Shut down Sequence of the 
              ThreadPool
___
#### DESTRUCTORS:  
Handles the Closing of threads and Deletion of any Allocated resources.
___

**Step 1) Done is called which Locks a Mutex and sets the is_Done flag letting the Pool know its closing                                                                             
              Condition Variable is also set to Notify all and wake them up so they can Initialize the shut down                                                                      
              process                                                                 
**Step 2) Destructor Called which Cycles over the Threadpools Done functions to double check and then                                                                      
**Step 3) Joins all the Threads for our Threadpool and closes down.                                                                      
___                                                                             
                                                                             



Namespace Core;
Namespace Threading;


___
#
___

Function Name     |  Description
:------------------|:---------------------
`template<typename R> bool is_ready(R _future)`| Nonblocking test of std::future<R> passed as the argument returning a boolean



___
# class ThreadPool
___
Public User Interface
___

Function Name | Description
:--------------|:--------------
static ThreadPool &get();| Returns a Single Instance of the Pool. 
template<typename F, typename ...ARGS>void Async(F&& _func, ARGS... _args);| Execute the Function passed into _func by pushing it into a Threadpool Queue. Returns an std::future object of with a return type which matches that of the called function.

Data Members | Description
:-------------|:-----------------------
##### const size_t ThreadCount; `| Number of Threads Avalible for the System
##### std::vector<std::thread> Worker_Threads;|   Vector of Worker threads which accept Jobs for Execution
##### std::vector<JobQueue> ThreadQueue;`| Jobs waiting to be feed into the Worker Thread Vector for Execution



___
##Private Details 
The following has been removed from user interface instead choosing to only expose Threadpool::Async( Function*, Arguments...)
___
#class JobQueue
___

___

Function Name | Description
------------|-----------------
##### bool Try_Pop(Worker_Function &func)  | Try to aquire a function off the Queue to run
##### void Done() | Signal when Finished
##### bool pop(Worker_Function &func)| Remove an Item from the Job Queue and return it to the User
##### template<typename F> bool try_push(F&& func) | Attempt to push a Job onto the Queue
##### template<typename F> void push(F&& func) | Force a Push onto the JobQueue


Data Members | Description
-------------|-----------------
std::deque< Worker_Function> TaskQueue;|
std::condition_variable is_Ready; |
std::mutex QueueMutex; |
bool is_Done; |

___



___
# class ThreadPool
___
*NOTE:* Copy constructors have been removed and Constructor hidden. Essentially A singleton accessed via the `get()` method
___

Function Name | Description
--------------|--------------
static ThreadPool &get();| Returns a Single Instance of the Pool. 
void Run(unsigned int _i); | Starts Specific Thread in the Pool

Data Members | Description
-------------|-----------------------
const size_t ThreadCount; | Number of Threads Avalible for the System
std::vector<std::thread> Worker_Threads;| Vector of Worker threads which accept Jobs for Execution
std::vector<JobQueue> ThreadQueue;| Jobs waiting to be feed into the Worker Thread Vector for Execution
std::atomic<unsigned int> Index;|
___


___		
 Wrapper for functions sent into our Threadpool. An std::deque<Wrapper_Base*> _func Object accepts functions from Threadpool::Async(func, args...) and wraps them to pass pointers to the heap allocated heap
___
# struct Wrapper_Base
___
NOTE: Polymorphic Base class to allow ThreadQueue to accept any function signature
___



Function Name     |  Description
------------------|---------------------
virtual ~Wrapper_Base()|
virtual void Invoke() = 0;|
void operator()();|

Data Members | Description
-------------|-----------------------
asyncStatus Status| Current status of the function passed in. Mainly for Debug information but will tell the current status of any item passed into the Threadpool

   Types
---------------|--------------------
enum asyncStatus|	Empty, Valid, Waiting, Busy, Submitted, Ready, Aquired
___




___
#asyncTask
___
 Wrapper for functions sent into our Threadpool. An std::deque<Wrapper_Base*> _func Object accepts functions from Threadpool::Async(func, args...) and wraps them to pass pointers to the heap allocated heap
___			
template<typename _Func, typename ...ARGS>|
struct asyncTask: public Wrapper_Base |




Function Name     |  Description
------------------|---------------------
asyncTask(_Func&& _function, ARGS&&... _args) noexcept| Function and Argument to wrapped up to Asyncronously run in the Threadpool. 
virtual ~asyncTask() | Virtual destructor to ensure complete deletion of allocated object
asyncTask(asyncTask&& _other) noexcept| Perfect forwarding move constructor
asyncTask& operator=(asyncTask&& _other) noexcept| Perfect forwarding Assignment constructor
virtual void Invoke()  override |
std::future<type> get_future() | Return an std::future of type Function return type passed into asyncTask c_Tor
bool is_ready()| Check to see if the Function has returned (Deprecated 12/2/2019: R: I believe this is unable to be aquired with the API change to the Threadpool interface. with asyncTask hidden from the user checking its status is strictly debug information )
.|.
asyncTask(const asyncTask&) | Deleted;
asyncTask& operator=(const asyncTask& _other) | Deleted;


Data Members | Description
-------------|-----------------------
Fptr Function| Function pointer of the function passed into the Templated c_Tor
std::tuple<ARGS...> Arguments;| Tuple of the arguments passed into the Template
std::promise<type> ReturnValue;| A promise of the return type of the Function passed in
type get() | Blocking return of the results of the function call

   Types
---------------|--------------------
type | Return type of the Passed function
reference_type | Reference of Type type
Fptr          | Function pointer to the passed Function


	





