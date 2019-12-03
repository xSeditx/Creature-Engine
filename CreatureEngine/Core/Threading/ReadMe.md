___
## ThreadPool Implementation
___

Namespace Core;
Namespace Threading;


___
#
___

Function Name     |  Description
------------------|---------------------
template<typename R> bool is_ready(R _future)| Nonblocking test of std::future<R> passed as the argument returning a boolean



___
# class ThreadPool
___
Public User Interface
___

Function Name | Description
--------------|--------------
static ThreadPool &get();| Returns a Single Instance of the Pool. 
template<typename F, typename ...ARGS>void Async(F&& _func, ARGS... _args);| Execute the Function passed into _func by pushing it into a Threadpool Queue. Returns an std::future object of with a return type which matches that of the called function.

Data Members | Description
-------------|-----------------------
const size_t ThreadCount; | Number of Threads Avalible for the System
std::vector<std::thread> Worker_Threads;| Vector of Worker threads which accept Jobs for Execution
std::vector<JobQueue> ThreadQueue;| Jobs waiting to be feed into the Worker Thread Vector for Execution



___
##Private Details 
The following has been removed from user interface instead choosing to only expose Threadpool::Async( Function*, Arguments...)
___
#class JobQueue
___

___

Function Name | Description
------------|-----------------
bool Try_Pop(Worker_Function &func)  | Try to aquire a function off the Queue to run
void Done() | Signal when Finished
bool pop(Worker_Function &func)| Remove an Item from the Job Queue and return it to the User
template<typename F> bool try_push(F&& func) | Attempt to push a Job onto the Queue
template<typename F> void push(F&& func) | Force a Push onto the JobQueue


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
NOTE: Copy constructors have been removed and Constructor hidden. Essentially A singleton accessed via the `get()` method
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


	





