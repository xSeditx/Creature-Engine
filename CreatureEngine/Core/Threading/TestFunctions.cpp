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
    	for (uint32_t i{ 1 }; i < (Range - 1); ++i)
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
}void TestAsyncSort::LinearBubbleSort()
{
 	SortGroup(STtest);
}


void TestAsyncSort::AsyncBubbleSort()//TestFunctionC, 3.14159f, 123);//
{
 	uint16_t GroupCount = std::thread::hardware_concurrency();
	int Groupsize = MTtest.size() / GroupCount;// Only dealing with perfectly divisible Counts for now
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

void TestAsyncSort::LinearMergeSort()
{
	MTtest = Merge_sort(MTtest);
}
















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