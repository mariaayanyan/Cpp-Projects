#pragma once
#include <cassert>
#include <cstddef>

template <typename T, class Container>
class Stack {
private:
	Container mContainer;
public:
	/*
	Constructs the underlying container with the contents of cont.
	*/
	Stack(const Container& cont = Container()) : mContainer(cont)
	{

	}

	/*
	Copy constructor. The adaptor is copy-constructed with the contents of other.mContainer.
	*/	
	Stack(const Stack& other) : mContainer(other.mContainer)
	{

	}

	/*
	Copy assignment operator. Replaces the contents with a copy of the contents of other. Effectively calls mContainer = other.mContainer;
	*/
	Stack& operator=(const Stack& other)
	{
		mContainer = other.mContainer;
		return *this;
	}


	/*
	Pushes the given element value to the top of the stack. Effectively calls mContainer.push_back(value).
	*/
	void push(const T& value)
	{
		mContainer.push_back(value);
	}

	/*
	Removes the top element from the stack. Effectively calls mContainer.pop_back().
	*/
	void pop()
	{
		mContainer.pop_back();
	}

	/*
	Returns reference to the top element in the stack. This is the most recently pushed element.
	This element will be removed on a call to pop(). Effectively calls mContainer.back().
	*/
	T& top()
	{
		return mContainer.back();
	}
	const T& top() const
	{
		return mContainer.back();
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