#pragma once
#include <string>
#include "DX9Include.h"

class RefCount
{
public:
	RefCount();

	int operator ++() { count += 1; return count; }
	int operator --() { count -= 1; return count; }
	bool OnlyOne() { return count == 1; }

private:
	int count;
};

template<typename T> struct PointerReleaser { void operator()(T* pointer) { pointer->Release(); } };
template<typename T> struct PointerDeleter { void operator()(T* pointer) { delete pointer; } };
template<typename T> struct ArrayDeleter { void operator()(T* pointer) { delete[] pointer; } };

template<typename Pointer, typename Killer>
class AutoKillPtr
{
	Pointer* pointer;
	AutoKillPtr(const AutoKillPtr&);
public:
	AutoKillPtr() : pointer(nullptr) { }

	AutoKillPtr(Pointer* ptr) : pointer(ptr) {}

	AutoKillPtr(AutoKillPtr&& other) : pointer(other.pointer)
	{
		other.pointer = nullptr;
	}

	Pointer** operator&()
	{
		return &pointer;
	}

	AutoKillPtr& operator=(Pointer* ptr)
	{
		if (ptr != pointer)
			Release();
		pointer = ptr;
		return *this;
	}

	AutoKillPtr& operator=(AutoKillPtr&& other)
	{
		if (this != &other)
		{
			if (pointer != other.pointer)
			{
				Release();
			}
			pointer = other.pointer;
			other.pointer = nullptr;
		}
		return *this;
	}

	bool IsNullPtr() const { return pointer == nullptr; }

	bool IsNotNullPtr() const { return !IsNullPtr(); }

	~AutoKillPtr() { Release(); }

	void Release()
	{
		if (pointer != nullptr)
		{
			Killer k;
			k(pointer);
			pointer = nullptr;
		}
	}

	operator Pointer* &() { return pointer; }
	operator Pointer* () const { return pointer; }
	Pointer* operator->() { return pointer; }
	const Pointer* operator->() const { return pointer; }

};

template<typename T> using AutoR = AutoKillPtr<T, PointerReleaser<T>>;
template<typename T> using AutoD = AutoKillPtr<T, PointerDeleter<T>>;
template<typename T> using AutoA = AutoKillPtr<T, ArrayDeleter<T>>;

const int RTFormatCount = 2;
const int DSFormatCount = 3;
const int MSFormatCount = 6;
const int TexFormatCount = 12;
const int IndexLengthCount = 6;
const int IndexFormatCount = 6;
const int LockOptionCount = 3;
extern D3DFORMAT s_RTFormats[RTFormatCount];
extern D3DFORMAT s_DSFormats[DSFormatCount];
extern D3DFORMAT s_TextureFormats[TexFormatCount];
extern D3DFORMAT s_IndexFormats[IndexFormatCount];
extern unsigned int s_IndexLengths[IndexLengthCount];
extern unsigned int s_lockOptions[LockOptionCount];
extern D3DMULTISAMPLE_TYPE s_sampleTypes[MSFormatCount];

bool IsLocalFileExist(const std::string & fileName);
