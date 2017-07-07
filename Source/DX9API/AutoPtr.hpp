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
			other.pointer = nullptr;
		}
		return *this;
	}

	bool IsNullPtr() const { return pointer == nullptr; }

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

template<typename T> class AutoR : public AutoKillPtr<T, PointerReleaser<T>> {};
template<typename T> class AutoD : public AutoKillPtr<T, PointerDeleter<T>> {};
template<typename T> class AutoA : public AutoKillPtr<T, ArrayDeleter<T>> {};
