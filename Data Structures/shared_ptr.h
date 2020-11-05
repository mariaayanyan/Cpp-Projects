#pragma once
#include <cstddef>
template <typename T>
class shared_ptr
{
private:
	T* mPtr;
	std::size_t* mCount;

public:
	/*
	constructs new shared_ptr
	*/
	shared_ptr(T* ptr) : mPtr(ptr), mCount(new std::size_t(1))
	{

	}

	/*
	copy constructs new shared_ptr
	*/
	shared_ptr(const shared_ptr& other) : mPtr(other.mPtr), mCount(other.mCount)
	{
		++(*mCount);
	}

	/*
	assigns the shared_ptr
	*/
	shared_ptr& operator=(const shared_ptr& other)
	{
		--(*mCount);
		if (*mCount == 0)
			delete mPtr;

		mPtr = other.mPtr;
		mCount = other.mCount;
		++(*mCount);
		return *this;
	}

	/*
	returns the number of shared_ptr objects referring to the same managed object
	*/
	std::size_t use_count() const
	{
		return *mCount;
	}

	/*
	dereferences the stored pointer
	*/
	T& operator*() const
	{
		return *mPtr;
	}
	
	/*
	returns the stored pointer
	*/
	T* operator->() const
	{
		return mPtr;
	}

	/*
	destructs the owned object if no more shared_ptrs link to it
	*/
	~shared_ptr()
	{
		--(*mCount);
		if (*mCount == 0)
			delete mPtr;
	}
};