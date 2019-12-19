#pragma once


#include "Core/Common.h"
#include <iostream>
#include <vector>
#include <atomic>
#include <Windows.h>

#define SLEEP_TIME 0

/*  
constexpr double myONE = 1.0;
constexpr double myINF = 1.0 / 0.0;
constexpr double myNAN = 0.0 / 0.0;
constexpr double myNEG = -0.0;
constexpr double myLIL = 1e-39;
*/
extern std::atomic<int> Function_Counter;


int TestFunctionA();
int TestFunctionB(int _param);
uint64_t TestFunctionLarge(uint64_t _p1, uint64_t _p2, uint64_t _p3, uint64_t _p4, uint64_t _p5);

float TestFunctionC(float _paramA, int _paramB);
//float SortGroup(uint32_t _input);

std::string TestFunctionD(float _paramA, int _paramB);
std::vector<uint32_t> TestFunctionE(int _paramA);
std::vector<uint32_t> TestFunctionF(int _paramA);
std::vector<uint32_t> TestFunctionG(int _paramA);
std::vector<uint32_t> TestFunctionH(int _paramA);
std::vector<uint32_t> TestFunctionI(int _paramA);
std::vector<uint32_t> TestFunctionJ(int _paramA);

void  P0R0();
float P1R1(int _param);
void  P1R0(int _param);
void  P2R0(int _param, int _param2);
int   P0R1();

uint64_t TestCompile(std::vector<std::vector<uint32_t>> _input);
uint64_t Worker_TestFunction(size_t _count);



#include<utility>
#define  Range   1000
struct TestAsyncSort
{
	 
	TestAsyncSort(uint32_t _size)
	{
		for_loop(i, _size)
		{
			MTtest.push_back(i);//rand() % _size); //-i);//  );//
			STtest.push_back(i);//rand() % _size); // rand() % );//
		}
	}
	void AsyncBubbleSort();
	void LinearBubbleSort();

	void AsyncMergeSort();
	void LinearMergeSort();


	uint64_t MTSwapSort();
	std::vector<int> MTtest;
	std::vector<int> STtest;
};

template<typename _Ty>
bool Test_Sort(std::vector<_Ty> _input)
{
	for_loop (i, _input.size()-1)
	{
		if(_input[i] > _input[i + 1])
		{
			return false;
		}
	}
	return true;
}



/* Merge Two Arrays into a single output array */
template<typename _Ty>
std::vector<_Ty> Merge(std::vector<_Ty> _A, std::vector<_Ty> _B)
{
	std::vector<_Ty> result;
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

/*Single Threaded Merge sort */
template<typename _Ty>
std::vector<_Ty> Merge_sort(std::vector<_Ty> _input)
{
	if (_input.size() <= 1)
	{
		return _input;
	}
	std::vector<_Ty> Left, Right;

	uint32_t Half = (uint32_t)(_input.size() * .5);

	std::move(_input.begin(), _input.begin() + Half, std::back_inserter(Left));
	std::move(_input.begin() + Half, _input.end(), std::back_inserter(Right));

	auto L = Merge_sort(Left);
	auto R = Merge_sort(Right);
	return Merge(L, R);
}

#include"Threadpool.h"







/* Randomize a Sorted array */
template<typename _Ty>
std::vector<_Ty> Randomize(std::vector<_Ty> _input)
{
	std::vector<_Ty> result;

	while (!_input.empty())
	{
		size_t Element = rand() % _input.size();
		result.push_back(_input[Element]);
		_input.erase(_input.begin() + Element);
	}

	return result;
}
/* Single Threaded Bubble sort */
template<typename _Ty>
std::vector<_Ty>  Bubble_sort(std::vector<_Ty> _input)
{
	int Length = _input.size();
	bool Swapped{ true };
	while (Swapped)
	{
		Swapped = false;
		for (uint32_t i{ 1 }; i < (Length); ++i)
		{
			if (_input[i - 1] > _input[i])
			{
				std::swap(_input[i - 1], _input[i]);
				Swapped = true;
			}
		}
	}

	return _input;
}






/*
==========================================================================================================================================================================
                                                           NOTES:                                                                      
==========================================================================================================================================================================

Learning C++
https://riptutorial.com/Download/cplusplus.pdf

Performance Analysis of Multithreaded Sorting Algorithms
http://www.diva-portal.org/smash/get/diva2:839729/FULLTEXT02

Programming with Threads
Parallel Sorting
https://cseweb.ucsd.edu/classes/fa13/cse160-a/Lectures/Lec02.pdf

Double Check Locking is Fixed
https://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/

Science:

Telepathic Twins
https://mobile.abc.net.au/news/2018-07-25/telepathic-communication-a-matter-of-time-hybrid-world-adelaide/10029312?pfmredir=sm&fbclid=IwAR0xOL1dGuHb0xorDmJHir-K1KEdR9LuUhgVeGKYmkEhHFAcM9INnIQBv48
*/