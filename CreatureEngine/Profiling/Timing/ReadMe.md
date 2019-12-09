_namespace Timing;
___
# Timer class
___

 
Function            |     Description
--------------------|--------------------------------------------------------------------
void Start();       | Starts the Timer
uint64_t Sample();	| Returns the Amount of time passed since the Timer has been started 
uint64_t Stop();	| Stops the Timer and sets Trigger to Max so it will no longer be active 
void Reset();		| Resets the Duration to 0 and Start time to now 

 
 Data                                             | Description                                    
--------------------------------------------------|-----------------------------
std::chrono::time_point<std::chrono::steady_clock>  Start_Time;| Initial value of the Timer
std::chrono::time_point<std::chrono::steady_clock>  Duration;|  Value after the stopping of Timer. Only after calling stop is this set to avoid needly updates



___
# Scoped Timer : Derives from Timer
___
	Creates Timer object that is destroyed at the end of Scope returning the Timed value into object passed into Storage.
___


         Method             |    Description
----------------------------|-----------------------------
scoped_Timer(uint64_t* _storage)|   User passes the Timer pointer to an Object which when it goes out of scope is set showing the duration of the scope
~scoped_Timer()|  Useful for benchmarks

 Data Member |  Description
----------------------------|-----------------------------
uint64_t* Storage;        | Pointer to a 64 bit in which is passed into the constructor so when Timer goes out of scope it sets the value of the passed in Object




___
# Functional Timer : Derives from Timer
___
 Creates a Timer object in which the user passes in a given amount of Ticks dependent on resolution desired and if that many Ticks have passed at time of Update the function passed into the Update gets called
___


 Function                 |   Description
--------------------------|---------------------------------------
functional_Timer(uint64_t _trigger) noexcept |  User passes it an Interval and Update executes if amount of time has passed
template<typename _Func, typename ...ARGS>|
auto Update(_Func&& _function, ARGS&&... _args) | If Timers trigger time has passed Call _Func(ARGS...) 




      Data member   | Description
------------------|------------------
size_t Trigger| Desired Interval of which to call the function passed into the Update function






___
# Benchmarking
___

___
_namespace Profiling, Timing_
___

___
_template<size_t _SAMPLES_PER_AVERAGE>_ Profile_Timer()
___
Data Members |  Description
-------------|----------------------
uint64_t Samples;| Number of Samples to take for every Average 
uint64_t Results;| The Average value showing the over all results of the benchmark. Value is updated every _Samples_ amount of Cycles that passes
uint64_t Accumulator;| Accumulates the value over every iteration giving the sum of all recorded values
static uint64_t SampleCounter;| Tracker for counting the total number of Samples taken. Sample is taken everytime the Destructor is called
