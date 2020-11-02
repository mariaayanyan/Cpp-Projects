#pragma once
#include <cassert>
#include <cstddef>

template <typename T, class Container>
class Queue {
private:
	Container mContainer;
public:
	/*
	Constructs the underlying container with the contents of cont.
	*/
	Queue(const Container& cont = Container()) : mContainer(cont)
	{

	}

	/*
	Copy constructor. The adaptor is copy-constructed with the contents of other.mContainer.
	*/
	Queue(const Queue& other) : mContainer(other.mContainer)
	{

	}

	/*
	Copy assignment operator. Replaces the contents with a copy of the contents of other. Effectively calls mContainer = other.mContainer;
	*/
	Queue& operator=(const Queue& other)
	{
		mContainer = other.mContainer;
		return *this;
	}

	/*
	Pushes the given element value to the end of the queue. Effectively calls mContainer.push_back(value).
	*/
	void push(const T& value)
	{
		mContainer.push_back(value);
	}

	/*
	Removes an element from the front of the queue. Effectively calls mContainer.pop_front()
	*/
	void pop()
	{
		mContainer.pop_front();
	}

	/*
	Returns reference to the last element in the queue.
	This is the most recently pushed element. Effectively calls mContainer.back().
	*/
	T& back()
	{
		return mContainer.back();
	}
	const T& back() const
	{
		return mContainer.back();
	}

	/*
	Returns reference to the first element in the queue.
	This element will be the first element to be removed on a call to pop(). Effectively calls mContainer.front().
	*/
	T& front()
	{
		return mContainer.front();
	}
	const T& front() const
	{
		return mContainer.front();
	}

	/*
	Checks if the underlying container has no elements, i.e. whether mContainer.empty().
	*/
	bool empty() const
	{
		return mContainer.empty();
	}

	/*
	Returns the number of elements in the underlying container, that is, mContainer.size().
	*/
	std::size_t size() const
	{
		return mContainer.size();
	}
};