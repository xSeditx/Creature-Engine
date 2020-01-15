#include"TestFunctions.h"
#include<type_traits>

#pragma warning( disable : 4244 )

#define Worker_Print(x) //std::cout << x << " : "
std::atomic<int> Function_Counter{ 0 };

/* unoptimized code section */
OPTIMIZATION_OFF()

void TestFunction(int _count, int _output)
{
	int result{ 0 };
	for (int i{ 0 }; i < _count; ++i)
	{
		result++;
		result += std::pow(result, _output);
	}
	std::cout << "PARAM:" << _output << "Finished Work: " << result << "\n";
}
OPTIMIZATION_ON()


OPTIMIZATION_OFF()
int TestFunctionA()
{
	Worker_Print("FunctionA");
	Sleep(SLEEP_TIME);

	++Function_Counter;
	return 11;
}
OPTIMIZATION_ON()


OPTIMIZATION_OFF()
int TestFunctionB(int _param)
{
	int result;
	Worker_Print("FunctionB: " << _param);
	//result.set_value(10);
	result = _param * 2;
	Sleep(SLEEP_TIME);
	++Function_Counter;
	return result;
}
OPTIMIZATION_ON()

OPTIMIZATION_OFF()
float TestFunctionC(float _paramA, int _paramB)
{
	Worker_Print("FunctionC: " << _paramA << " : " << _paramB);
	//Print("FuncC Param is " << _paramB << " : ");

	Sleep(SLEEP_TIME);
	++Function_Counter;
	return _paramA * _paramB * (rand() % 1000);
}
OPTIMIZATION_ON()

OPTIMIZATION_OFF()
std::string TestFunctionD(float _paramA, int _paramB)
{
	Worker_Print("FunctionD " << _paramA << " : " << _paramB);
	Sleep(SLEEP_TIME);
	++Function_Counter;
	return "String: " + std::to_string(_paramA) + " : " + std::to_string(_paramB);
}
OPTIMIZATION_ON()

OPTIMIZATION_OFF()
std::vector<uint32_t> TestFunctionE(int _paramA)
{
	std::vector<uint32_t> SomethingAllocated;

	uint64_t result{ 0 };
	for (int i{ 0 }; i < _paramA; ++i)
	{
		//	std::cout << "E: " << i*i ;
		result += static_cast<int64_t>(i * i);
		SomethingAllocated.push_back(result);
	}
	Sleep(SLEEP_TIME);
	++Function_Counter;
	return SomethingAllocated;
}
OPTIMIZATION_ON()

OPTIMIZATION_OFF()
std::vector<uint32_t> TestFunctionF(int _paramA)
{
	std::vector<uint32_t> SomethingAllocated;
	uint64_t result{ 0 };
	for (int i{ 0 }; i < _paramA; ++i)
	{
		double A = sqrt(i*i);
		//		std::cout << "F: " << A ;
		result += static_cast<int64_t>(_paramA * i + A);
		SomethingAllocated.push_back(result);
	}
	Sleep(SLEEP_TIME);
	++Function_Counter;
	return SomethingAllocated;
}
OPTIMIZATION_ON()


OPTIMIZATION_OFF()
std::vector<uint32_t> TestFunctionG(int _paramA)
{
	std::vector<uint32_t> SomethingAllocated;
	uint64_t result{ 0 };
	int B = rand() % _paramA;
	int C{ 0 };

	for (int i{ 0 }; i < _paramA; ++i)
	{
		double A = sqrt(i*i);
		//		std::cout << "G: " << A ;
		result += static_cast<int64_t>((_paramA * i) / pow(A, i));
		SomethingAllocated.push_back(result);
	}
	Sleep(SLEEP_TIME);
	++Function_Counter;
	return SomethingAllocated;
}
OPTIMIZATION_ON()

OPTIMIZATION_OFF()
std::vector<uint32_t> TestFunctionH(int _paramA)
{
	std::vector<uint32_t> SomethingAllocated;
	double A{ 0 };
	uint64_t result{ 0 };
	int B = rand() % _paramA;
	int C{ 0 };

	for (int i{ 0 }; i < _paramA; ++i)
	{
		A = sqrt(i*i);
		Worker_Print("H: " << A);
		result += i * A;
		SomethingAllocated.push_back(result);
	}
	Sleep(SLEEP_TIME);
	++Function_Counter;
	return SomethingAllocated;
}
OPTIMIZATION_ON()

OPTIMIZATION_OFF()
std::vector<uint32_t> TestFunctionI(int _paramA)
{
	std::vector<uint32_t> SomethingAllocated;
	uint64_t A{ 0 };

	int B = rand() % _paramA;
	int C{ 0 };

	for (int i{ 0 }; i < _paramA; ++i)
	{
		A = pow(_paramA, i);
		if (!((i + 1) % (B + 1)))C += A;
		Worker_Print("I: " << A);
		uint32_t result = A * C;
		SomethingAllocated.push_back(result);
	}
	Sleep(SLEEP_TIME);
	++Function_Counter;
	return SomethingAllocated;
}
OPTIMIZATION_ON()

OPTIMIZATION_OFF()
std::vector<uint32_t> TestFunctionJ(int _paramA)
{
	std::vector<uint32_t> SomethingAllocated;
	float A = 0;
	int B = rand() % _paramA;
	int C{ 0 };
	for (int i{ 0 }; i < _paramA; ++i)
	{
		A = pow(i, _paramA) / i;
		if (A == B)C = i;
		Worker_Print("J: " << A << ":");
		uint32_t result = A * C;
		SomethingAllocated.push_back(result);
	}
	Sleep(SLEEP_TIME);
	++Function_Counter;
	return SomethingAllocated;
}
OPTIMIZATION_ON()

OPTIMIZATION_OFF()
uint64_t Worker_TestFunction(size_t _count)
{
	int B = rand() % _count;
	int C{ 0 };
	for (uint32_t i{ 0 }; i < _count; ++i)
	{
		std::cout << i << ":";
		if (!((i + 1) % (B + 1)))C++;
	}
	return _count * C;
}
OPTIMIZATION_ON()

OPTIMIZATION_OFF()
uint64_t TestCompile(std::vector<std::vector<uint32_t>> _input)
{
	uint64_t result{ 0 };
	for (auto& Y : _input)
	{
		for (auto& X : Y)
		{
			result += X;
		}
	}
	return result;
}
OPTIMIZATION_ON()
OPTIMIZATION_OFF()

uint64_t TestFunctionLarge(uint64_t _p1, uint64_t _p2, uint64_t _p3, uint64_t _p4, uint64_t _p5)
{
	uint64_t result = _p1 + _p2 + _p3 + _p4 + _p5;
	Print(result);
	return result;
}

OPTIMIZATION_ON()

void  P0R0()
{
	std::cout << "P0R0" << "\n";
}
float P1R1(int _param)
{
	std::cout << "P1R1: " << _param << "\n";
	return static_cast<float>(_param * _param);
}
void  P1R0(int _param)
{
	std::cout << "P1R0:" << _param << "\n";
}
void  P2R0(int _param, int _param2)
{
	std::cout << "P1R0:" << _param << ":" << _param2 << "\n";
}
int   P0R1()
{
	std::cout << "P0R1" << "\n";
	return 42;
}



#include"Threadpool.h"
int TestFunctionHG()
{
	return 20;
}
int SortGroup(std::vector<int>& _input)
{
    bool Swapped{ true };
    while (Swapped)
    {
    	Swapped = false;
    	for (uint32_t i{ 1 }; i < (_input.size() - 1); ++i)
    	{
    	    if (_input[i - 1] > _input[i])
    	    {
    	    	std::swap(_input[i - 1], _input[i]);
    	    	Swapped = true;
    	    }
    	}
    }
	return (float)0.0f;
}
int SortGroupAsync(int *_input, int _start, int _size)
{
	bool Swapped{ true };
	while (Swapped)
	{
		Swapped = false;
		for (int i{ _start + 1 }; i < ((_start+ _size) - 1); ++i)
		{
			if (_input[i - 1] > _input[i])
			{
				std::swap(_input[i - 1], _input[i]);
				Swapped = true;
			}
		}
	}
	return (float)0.0f;
}
void TestAsyncSort::LinearBubbleSort()
{
 	SortGroup(STtest);
}


void TestAsyncSort::AsyncBubbleSort()//TestFunctionC, 3.14159f, 123);//
{
 	uint16_t GroupCount = std::thread::hardware_concurrency();
	uint32_t Groupsize = (uint32_t)MTtest.size() / (uint32_t)GroupCount;// Only dealing with perfectly divisible Counts for now
	int Elm{ 0 };

	std::vector<std::vector<int>> Temps;
	std::vector<std::future<int>> Results;
	for_loop(j, GroupCount)
	{
		Temps.push_back(std::vector<int>());
		for_loop(i, Groupsize)
		{
			Temps[j].push_back(MTtest[Elm]);
 			++Elm;
		}	
	 	//Results.push_back(Core::Threading::ThreadPool::get().Async(SortGroup, MTtest));
		//Results.push_back(Core::Threading::ThreadPool::get().Async(SortGroupAsync, (int*)&MTtest[0],(int) (Groupsize * j), (int)Groupsize)); // (int*)&Temps[j][0]
		//Results.push_back(std::async(SortGroupAsync, (int*)&MTtest[0],  (Groupsize * j),  Groupsize));
	}


	for (auto &A : Results)
	{// Syncs up the threads by waiting till they all complete before moving forward. 
		A.get(); // Acts like a Fence basically 
	}
}


OPTIMIZATION_ON()

OPTIMIZATION_OFF()
void TestAsyncSort::LinearMergeSort()
{
	STtest = Randomize(STtest);
	STtest = Merge_sort(STtest);
}
OPTIMIZATION_ON()

OPTIMIZATION_OFF()

OPTIMIZATION_ON()

OPTIMIZATION_OFF()
/* Merge Two Arrays into a single output array */
std::vector<int> AMerge(std::vector<int> _A, std::vector<int> _B)
{
	std::vector<int> result;
	int a{ 0 }, b{ 0 };

	while (a < _A.size() && b < _B.size())
	{
		if (_A[a] <= _B[b])
		{
			result.push_back(_A[a++]);
		}
		else
		{
			result.push_back(_B[b++]);
		}
	}
	while (a++ < _A.size())
	{
		result.push_back(_A[a - 1]);
	}
	while (b++ < _B.size())
	{
		result.push_back(_B[b - 1]);
	}
	return result;
}
OPTIMIZATION_ON()

OPTIMIZATION_OFF()
/*Multithreaded Threaded Merge sort */
std::vector<int> MTMerge_sort(std::vector<int> _input)
{
    if (_input.size() <= 1)
    {
    	return _input;
    }
    std::vector<int> Left, Right;
    
    uint32_t Half = (uint32_t)(_input.size() * .5);
    std::move(_input.begin()       , _input.begin() + Half, std::back_inserter(Left));
    std::move(_input.begin() + Half, _input.end()         , std::back_inserter(Right));
    
	// This needs to Run the Function, Store this Location... Then jump back here after the child returns
    auto L = Core::Threading::ThreadPool::get().Async(MTMerge_sort, (std::vector<int>)Left);// 
	auto Lv = L.get(); /// Maybe it would be better if I Invoke the Continuation in the get() as that would ensure 
    auto R = Core::Threading::ThreadPool::get().Async(MTMerge_sort, (std::vector<int>)Right);// (std::vector<int>)
	Print(" Waiting on MergeSort Left, size: " << _input.size());
	Print(" Waiting on MergeSort Right, size: " << _input.size());
	auto Rv = R.get(); /// That specific future call has returned already
    
	return AMerge(Lv, Rv);
}
OPTIMIZATION_ON()

OPTIMIZATION_OFF()
void TestAsyncSort::AsyncMergeSort()
{
	MTtest = Randomize(MTtest);
	MTtest = MTMerge_sort(MTtest);
}

OPTIMIZATION_ON()

OPTIMIZATION_OFF()

/*Multithreaded Threaded Merge sort */
std::vector<int> STDMerge_sort(std::vector<int> _input)
{
	if (_input.size() <= 1)
	{
		return _input;
	}
	std::vector<int> Left, Right;

	uint32_t Half = (uint32_t)(_input.size() * .5);
	std::move(_input.begin(), _input.begin() + Half, std::back_inserter(Left));
	std::move(_input.begin() + Half, _input.end(), std::back_inserter(Right));

	auto L = std::async(STDMerge_sort, Left);//
	auto R = std::async(STDMerge_sort, Right);// (std::vector<int>)
	auto Lv = L.get();
	auto Rv = R.get();

	return AMerge(Lv, Rv);
}
OPTIMIZATION_ON()

OPTIMIZATION_OFF()
void TestAsyncSort::StdMergeSort()
{
	STDtest = Randomize(STDtest);
	STDtest = STDMerge_sort(STDtest);
}
OPTIMIZATION_ON()

OPTIMIZATION_OFF()

	//ThreadMTMerge_sort(MTtest,std::move( prom));fut = prom.get_future();

//auto L =std::async(MTMerge_sort, Left); //
//auto L = std::thread(MTMerge_sort, (std::vector<int>)Left);
//auto R =std::async(MTMerge_sort, Right);//
//auto R = std::thread(MTMerge_sort, (std::vector<int>)Right);





//bool Try_Swap(std::vector<int>& _input, int _i)
//{
//	for_loop(j, _input.size())
//	{
//		if (Mutexs[j].try_lock)
//		{
//			if (_input[_i] >_input[j])
//			{
//				std::swap(_input[_i], _input[j]);
//				return true;
//			}
//		}
//	}
//	return false;
//}

/*
Multithreaded Sorting Algorithms
http://www.diva-portal.org/smash/get/diva2:839729/FULLTEXT02
*/
//
std::vector<std::mutex*> Mutexs;
//
 int SortG(std::vector<int>& _input)
 {
 	bool Sorted{ false };
 	while (!Sorted)
 	{
 		
 		for_loop(i, _input.size())
 		{
			Sorted = false;
 			if (Mutexs[i]->try_lock())
 			{
				Mutexs[i]->lock();
 				for_loop(j, _input.size())
 				{
 					if (i == j) continue;
 					if (Mutexs[j]->try_lock())
 					{
						Mutexs[j]->lock();
 						if(_input[i] < _input[j])
 						{
 							std::swap(_input[i] , _input[j]);
 							Sorted = true;
 							
						//	Mutexs[i]->unlock();
 						//	continue;
 						}
						Mutexs[j]->unlock();
 					}
 				}
 				Mutexs[i]->unlock();
 			}
 		}
 	}
	return 50;
 }
 uint64_t TestAsyncSort::MTSwapSort()
 {
	 MTtest = Randomize(MTtest);

	 int Groupcount = 8;// Core::Threading::ThreadPool::Number_of_Threads;// Number in the Threadpool
 
 	for_loop (i, MTtest.size())
 	{
 		Mutexs.push_back(new std::mutex());
 	}
 
	std::vector<std::future<int>> Futs;
	//std::vector<Future<int>> Futs;
	for(int g = 0; g <  Groupcount; ++g)
 	{
// 		Futs.push_back(Core::Threading::ThreadPool::get().Async(SortG, MTtest));
 	}
 
 	for (auto &F : Futs)
 	{
 		F.get();
 	}
	assert(Test_Sort(MTtest));
 	return 12345678910;
}
//
//
 
//	int Groupsize = MTtest.size() / Groupcount;// Number of elements per group
//	int Left_over = MTtest.size() % Groupcount;// Remainder that is nondivisible

///bool Swapped{ false };
///int i{ 0 };
///while(!Swapped)
///{
///	Swapped = false;
///	for_loop (i, _input.size())
///	{
///		int j{ 0 };
///		while(!Mutexs[i+(j++)].try_lock())
///		{ 
///		
///		}
///
///
///		if (Mutexs[i].try_lock())
///		{
///			int k{ 0 };
///
///			while (!Mutexs[i + (k++)].try_lock())
///			{
///				if (_input[i] < _input[i + k])
///				{
///					std::swap(_input[i], _input[i + k]);
///					Swapped = true;
///				}
///			}
///		}
///	}
///}
/// DOES NOT WORK
/*Multithreaded Threaded Merge sort 
void ThreadMTMerge_sort(std::vector<int> _input, std::promise<std::vector<int>> _result)
{
	if (_input.size() <= 1)
	{
		_result.set_value(_input);
		return;
	}
	std::vector<int> Left, Right;

	uint32_t Half = (uint32_t)(_input.size() * .5);

	std::move(_input.begin(), _input.begin() + Half, std::back_inserter(Left));
	std::move(_input.begin() + Half, _input.end(), std::back_inserter(Right));

	std::promise<std::vector<int>> L;
	std::thread(ThreadMTMerge_sort, &Left, &L);

	std::promise<std::vector<int>> R;
	std::thread(ThreadMTMerge_sort, &Right, &R);

	auto Lv = L.get_future();
	auto Rv = R.get_future();
	_result.set_value(AMerge(Lv.get(), Rv.get()));
	return;
}*/







//std::promise<std::vector<_Ty>> L;// = std::async(MTMerge_sort, Left);
//std::promise<std::vector<_Ty>> R;// = std::async(MTMerge_sort, Right);


//  std::vector<int> Merge(std::vector<int> _left, std::vector<int> _right)
//  {
//  	std::vector<int> result;
//  	std::deque<int> resultQ;
//  
//  	while (_left.size() && _right.size())
//  	{
//  		if (_left.back() <= _right.back())
//  		{
//  			result.push_back(_left.back());
//  			resultQ.push_front(_left.back());
//  			_left.pop_back();
//  		}
//  		else
//  		{
//  			result.push_back(_right.back());
//  			resultQ.push_front(_right.back());
//  			_right.pop_back();
//  		}
//  	}
//  	/* Comsume Remainders */
//  	while (_left.size())
//  	{
//  		result.push_back(_left.back());	
//  		resultQ.push_front(_left.back());
//  
//  		_left.pop_back();
//  	}
//  	while (_right.size())
//  	{
//  		result.push_back(_right.back());
//  		resultQ.push_front(_right.back());
//  		_right.pop_back();
//  	}
//  
//  	return result;
//  }
//  std::vector<int> Merge_Sort(std::vector<int> _input)
//  {
//  	if (_input.size() <= 1)
//  	{
//  		return _input;
//  	}
//  	int Half = (int)std::floor(_input.size() * .5);
//  
//  	std::vector<int> Left;
//  	std::vector<int> Right;
//  	std::move(_input.begin()       , _input.begin() + Half , std::back_inserter( Left));
//  	std::move(_input.begin() + Half, _input.end()          , std::back_inserter(Right));
//  
//  	auto L = Merge_Sort(Left);
//  	auto R = Merge_Sort(Right);
//  
//  	return Merge(L, R);
//  }
//i{ 0 },
//	int length = 0;
//	_lsize > _rsize ? length = _lsize : length = _rsize;
//	int Order[] = { 0,1,2,3,4,5,6,7,8 };
//bool Swapped{ true };
//while (Swapped)
//{// Do a Bubble sort on the Order of the Groups
//	Swapped = false;
//	for (uint32_t i{ 1 }; i < (GroupCount - 1); ++i)
//	{
//		if (Temps[i - 1][0] > Temps[i][0])
//		{// This should account for Last being greater than first but Eh...
//			std::swap(Order[i - 1], Order[i]);
//			Swapped = true;
//		}
//	}
//}

//Elm = 0;
//for_loop(j, GroupCount)
//{
//	for_loop(i, Groupsize)
//	{
//		MTtest[Elm] = Temps[Order[j]][i];
//		Elm++;
//	}
//}

//uint64_t counter = Fut.size();
//while (counter)
//{
//	for (auto& F : Fut)
//	{
//		if (!is_ready(F))
//		{
//			continue;
//		}
//		result += (uint64_t)F.get();
//		--counter;
//	}
//}
//
//uint32_t Length = Range;
//bool Swapped{ true };
//while (Swapped) 
//{
//	Swapped = false;
//	for (uint32_t i{ 1 }; i < (Range - 1); ++i)
//	{
//		if (STtest[i - 1] > STtest[i])
//		{
//			std::swap(STtest[i - 1], STtest[i]);
//			Swapped = true;
//		}
//	}
//} 
////procedure bubbleSort(A : list of sortable items)
//n = length(A)
//repeat
//swapped = false
//for i = 1 to n - 1 inclusive do
///* if this pair is out of order */
//if A[i - 1] > A[i] then
///* swap them and remember something changed */
//swap(A[i - 1], A[i])
//swapped = true
//end if
//end for
//until not swapped
//end procedure
////


//
//
//
//
//
//
//if (int remains = (Groupsize % 8) != 0)
//{
//	for_loop(i, remains)
//	{
//		//Group[7].push_back(MTtest[MTtest.size()])
//	}
//}
//
//
//
//int SG(uint32_t input)
//{
//	//bool Swapped{ true };
//	//while (Swapped)
//	//{
//	//	Swapped = false;
//	//	for (uint32_t i{ 1 }; i < (Range - 1); ++i)
//	//	{
//	//	//  if (_input[i - 1] > _input[i])
//	//	//  {
//	//	//  	std::swap(_input[i - 1], _input[i]);
//	//	//  	Swapped = true;
//	//	//  }
//	//	}
//	//}
//	return 50;
//}
//
//int Tfunc(int p)
//{
//	return 745;
//}
//void H()
//{
//	uint32_t Temps;
//	std::vector<uint32_t> Te;
//	Te.push_back(10);
//	Te.push_back(30);
//	Te.push_back(40);
//	Temps = 10;
//	//auto Res = std::invoke_result_t<decltype(SG), decltype(19)>();
////	auto R = std::invoke_result<decltype(SG), decltype(19)>(SG, 19);
//	//auto Res = std::result_of(SortGroup( Temps));
//	Core::Threading::ThreadPool::get().Async(SG, (uint32_t)19);
//	//Core::Threading::ThreadPool::get().Async(TestFunctionC, 3.14159f, 123);
//}



/*

std::vector<int> Merge(int*_left, int _lsize, int *_right, int _rsize)
{
	std::vector<int> result;
	int l{ 0 }, r{ 0 };
	while (l > _lsize && r < _rsize)
	{
		if (_left[l] <= _right[r])
		{
			result.push_back(_left[l++]);
		}
		else
		{
			result.push_back(_right[r++]);
		}

	}

while (l > _lsize)
{
	result.push_back(_left[l++]);
}
while (r < _rsize)
{
	result.push_back(_right[r++]);
}
return result;
}
std::vector<int> Merge_Sort(int *_input, int _start, int _size)
{
	if (_size <= 1)
	{
		std::vector<int> result;
		result.push_back(_input[0]);
		return result;
	}
	_size = (int)(_size * .5);
	auto L = Merge_Sort(_input, _start, _size);
	auto R = Merge_Sort(_input, _start + _size, _size);
	return Merge((int*)&L[0], _size, (int*)&R[0], _size);
}

*/