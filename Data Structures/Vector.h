#include <cstddef>

template <typename T>
class Vector {
private:
	std::size_t mSize;
	std::size_t mCapacity;
	T* mArr; 
public:
	/*
	Default constructor. Constructs an empty container
	*/
	Vector();

	/*
	Constructs the container with count copies of elements with default value.
	*/
	Vector(std::size_t count);

	/*
	Copy constructor. Constructs the container with the copy of the contents of other.
	*/
	Vector(const Vector& vector);

	/*
	Returns a reference to the element at specified location pos.
	If user tries to access element out of bounds throw assertion.
	*/
	T& operator[](std::size_t pos);
	const T& operator[](std::size_t pos) const;

	/*
	Replaces the contents with a copy of the contents of other.
	If capacity of target (*this) is smaller than number of elements in other, deallocate memory and allocate new one with others capacity.
	*/
	Vector<T>& operator=(const Vector<T>& other);
	
	/*
	Checks if the contents of vectors are equal, that is, they have the same number of elements and
	each element in current object compares equal with the element in other at the same position.
	*/
	bool operator==(const Vector<T>& other);

	/*
	Increase the capacity of the vector to a value that's greater or equal to new_capacity.
	If new_capacity is greater than the current capacity(), new storage is allocated, otherwise the method does nothing.
	reserve() does not change the size of the vector.
	*/
	void reserve(std::size_t new_capacity);

	/*
	Appends the given element value to the end of the container.
	*/
	void push_back(const T& elem);

	/*
	Resizes the container to contain count elements.
	If the current size is greater than count, the container is reduced to its first count elements.
	If the current size is less than count, additional default-inserted elements are appended.
	*/
	void resize(std::size_t count);

	/*
	Removes the last element of the container.
	If user tries to call pop_back on an empty container throw assertion.
	*/
	void pop_back();

	/*
	Erases all elements from the container. After this call, size() returns zero.
	*/
	void clear();

	/*
	Returns reference to the last element in the container.
	If user tries to call back on an empty container throw assertion.
	*/
	T& back();
	const T& back() const;

	/*
	Checks if the container has no elements.
	*/
	bool empty() const;

	/*
	Returns the number of elements in the container.
	*/
	std::size_t size() const;

	/*
	Returns the number of elements that the container has currently allocated space for.
	*/
	std::size_t capacity() const;

	/*
	Destructs the vector. The destructors of the elements are called and the used storage is deallocated.
	*/
	~Vector();
};

// Vector implementation

#include <cassert>

template <typename T>
Vector<T>::Vector() : mSize(0), mCapacity(0), mArr(nullptr)
{

}

/*
Constructs the container with count copies of elements with default value.
*/
template <typename T>
Vector<T>::Vector(std::size_t count) : mSize(count), mCapacity(count), mArr(new T[count]{})
{

}

/*
Copy constructor. Constructs the container with the copy of the contents of other.
*/

template <typename T>
Vector<T>::Vector(const Vector<T>& vector) : mSize(vector.mSize), mCapacity(vector.mCapacity), mArr(new T[vector.mCapacity])
{
	for (std::size_t i = 0; i < mSize; ++i)
		mArr[i] = vector.mArr[i];
}

/*
Returns a reference to the element at specified location pos.
If user tries to access element out of bounds throw assertion.
*/
template <typename T>
T& Vector<T>::operator[](std::size_t pos)
{
	assert(pos >= 0 && pos < mSize);
	return mArr[pos];
}

template <typename T>
const T& Vector<T>::operator[](std::size_t pos) const
{
	assert(pos >= 0 && pos < mSize);
	return mArr[pos];
}

/*
Replaces the contents with a copy of the contents of other.
If capacity of target (*this) is smaller than number of elements in other, deallocate memory and allocate new one with others capacity.
*/
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	if (this == &other)
		return *this;

	if (mCapacity < other.mCapacity)
		reserve(other.mCapacity);

	for (std::size_t i = 0; i < other.mSize; ++i)
		mArr[i] = other.mArr[i];

	mSize = other.mSize;

	return *this;
}

/*
Checks if the contents of vectors are equal, that is, they have the same number of elements and
each element in current object compares equal with the element in other at the same position.
*/
template <typename T>
bool Vector<T>::operator==(const Vector<T>& other)
{
	if (this == &other)
		return true;

	if (mSize != other.mSize)
		return false;

	for (std::size_t i = 0; i < mSize; ++i)
		if (mArr[i] != other.mArr[i])
			return false;
	return true;
}

/*
Increase the capacity of the vector to a value that's greater or equal to new_capacity.
If new_capacity is greater than the current capacity(), new storage is allocated, otherwise the method does nothing.
reserve() does not change the size of the vector.
*/
template <typename T>
void Vector<T>::reserve(std::size_t new_capacity)
{
	if (new_capacity <= mCapacity)
		return;

	T* newArr = new T[new_capacity]{};
	mCapacity = new_capacity;

	for (std::size_t i = 0; i < mSize; ++i)
		newArr[i] = mArr[i];

	delete[] mArr;
	mArr = newArr;
}

/*
Appends the given element value to the end of the container.
*/
template <typename T>
void Vector<T>::push_back(const T& elem)
{
	if (mSize < mCapacity)
	{
		mArr[mSize++] = elem;
		return;
	}

	if (mCapacity == 0)
		reserve(1);
	else
		reserve(mCapacity * 2);
	mArr[mSize++] = elem;
}

/*
Resizes the container to contain count elements.
If the current size is greater than count, the container is reduced to its first count elements.
If the current size is less than count, additional default-inserted elements are appended.
*/
template <typename T>
void Vector<T>::resize(std::size_t count)
{
	if (count > mCapacity)
		reserve(count);

	for (std::size_t i = mSize; i < count; ++i)
		mArr[i] = T();
	mSize = count;
}

/*
Removes the last element of the container.
If user tries to call pop_back on an empty container throw assertion.
*/
template <typename T>
void Vector<T>::pop_back()
{
	assert(mSize);
	--mSize;
}

/*
Erases all elements from the container. After this call, size() returns zero.
*/
template <typename T>
void Vector<T>::clear()
{
	mSize = 0;
}

/*
Returns reference to the last element in the container.
If user tries to call back on an empty container throw assertion.
*/
template <typename T>
T& Vector<T>::back()
{
	assert(mSize);
	return mArr[mSize - 1];
}

template <typename T>
const T& Vector<T>::back() const
{
	assert(mSize);
	return mArr[mSize - 1];
}

/*
Checks if the container has no elements.
*/
template <typename T>
bool Vector<T>::empty() const
{
	return mSize == 0;
}

/*
Returns the number of elements in the container.
*/
template <typename T>
std::size_t Vector<T>::size() const
{
	return mSize;
}

/*
Returns the number of elements that the container has currently allocated space for.
*/
template <typename T>
std::size_t Vector<T>::capacity() const
{
	return mCapacity;
}

/*
Destructs the vector. The destructors of the elements are called and the used storage is deallocated.
*/
template <typename T>
Vector<T>::~Vector()
{
	delete[] mArr;
}
