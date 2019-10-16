
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
