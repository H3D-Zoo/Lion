#pragma once

#include <Windows.h>

class RefCount
{
public:
	RefCount() : count(1L) { }
	LONG operator ++() { return InterlockedIncrement(&count); }
	LONG operator --() { return InterlockedDecrement(&count); }
	LONG Count() const { return count; }
private:
	volatile LONG count;
};