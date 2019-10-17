___
## ThreadPool Implementation
___

NEED TO CREATE A NAMESPACE

___
#class JobQueue
___

*Data Types
>typedef void(*Worker_Function)();

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
template<typename F>void Async(F&& _func);| Execute the Function passed into _func by pushing it into a Threadpool Queue.

Data Members | Description
-------------|-----------------------
const size_t ThreadCount; | Number of Threads Avalible for the System
std::vector<std::thread> Worker_Threads;| Vector of Worker threads which accept Jobs for Execution
std::vector<JobQueue> ThreadQueue;| Jobs waiting to be feed into the Worker Thread Vector for Execution
std::atomic<unsigned int> Index;|
