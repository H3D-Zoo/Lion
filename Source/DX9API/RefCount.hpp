#pragma once

#include <Windows.h>

class RefCount
{
public:
	RefCount() : count(1L) { }
	ULONG operator ++() { return (ULONG)InterlockedIncrement(&count); }
	ULONG operator --() { return (ULONG)InterlockedDecrement(&count); }
	ULONG Count() const { return count; }
private:
	ULONG count;
};