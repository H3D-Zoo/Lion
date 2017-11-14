#pragma once

template<typename T> struct PointerReleaser { void operator()(T* pointer) { pointer->Release(); } };
template<typename T> struct PointerDeleter { void operator()(T* pointer) { delete pointer; } };
template<typename T> struct ArrayDeleter { void operator()(T* pointer) { delete[] pointer; } };

template<typename Pointer, typename Killer>
class AutoKillPtr
{
protected:
	Pointer* pointer;
	AutoKillPtr(const AutoKillPtr&);
	AutoKillPtr& operator=(AutoKillPtr&);
public:
	AutoKillPtr() : pointer(NULL) { }

	AutoKillPtr(Pointer* ptr) : pointer(ptr) {}

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

	AutoKillPtr& GetFrom(AutoKillPtr& other)
	{
		if (this != &other)
		{
			if (pointer != other.pointer)
			{
				Release();
			}
			pointer = other.pointer;
			other.pointer = NULL;
		}
		return *this;
	}

	bool IsNullPtr() const { return pointer == NULL; }

	bool IsNotNullPtr() const { return !IsNullPtr(); }

	~AutoKillPtr() { Release(); }

	void Release()
	{
		if (pointer != NULL)
		{
			Killer k;
			k(pointer);
			pointer = NULL;
		}
	}

	operator Pointer* &() { return pointer; }
	operator Pointer* () const { return pointer; }
	Pointer* operator->() { return pointer; }
	const Pointer* operator->() const { return pointer; }

};

template<typename T> class AutoR : public AutoKillPtr<T, PointerReleaser<T>> 
{
public:
	AutoR() { }
	AutoR(T* ptr) : AutoKillPtr(ptr) {}
	AutoR& operator=(T* ptr) { AutoKillPtr::operator=(ptr); return *this; }
};

template<typename T> class AutoD : public AutoKillPtr<T, PointerDeleter<T>>
{
public:
	AutoD() { }
	AutoD(T* ptr) : AutoKillPtr(ptr) {}
	AutoD& operator=(T* ptr) { AutoKillPtr::operator=(ptr); return *this; }
};
template<typename T> class AutoA : public AutoKillPtr<T, ArrayDeleter<T>> 
{
public:
	AutoA() { }
	AutoA(T* ptr) : AutoKillPtr(ptr) {}
	AutoA& operator=(T* ptr) { AutoKillPtr::operator=(ptr); return *this; }
};

