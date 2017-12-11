#include "deque.hpp"

template<typename T>
Deque<T>::Deque()
	: _impl(new T[DEFAULT_SIZE]), _capacity(DEFAULT_SIZE), _size(0), _start(0)
{

}

template<typename T>
Deque<T>::Deque(const Deque<T>& other)
	: _impl(new T[other._capacity]), _capacity(other._capacity), _size(other._size), _start(other._start)
{
	std::memcpy(_impl, other._impl, _capacity*sizeof(T));
}

template<typename T>
bool Deque<T>::empty() const
{
	return _size == 0;
}

template<typename T>
typename Deque<T>::size_type Deque<T>::size() const
{
	return _size;
}

template<typename T>
typename Deque<T>::size_type Deque<T>::capacity() const
{
	return _capacity;
}

template<typename T>
void Deque<T>::clear()
{
	if (_impl != nullptr) 
		delete[] _impl;

	_capacity = DEFAULT_SIZE;
	_impl = new T[_capacity];
	_size = 0;
	_start = 0;
}

template<typename T>
void Deque<T>::reserve(typename Deque<T>::size_type amount)
{
	if (amount <= _capacity) 
		return;

	_resize(amount);
}

template<typename T>
typename Deque<T>::reference Deque<T>::operator[](Deque<T>::difference_type index)
{
	Deque<T>::size_type tmp = _start + index;
	if (tmp >= _capacity)
		tmp -= _capacity;
	return _impl[tmp];
}

template<typename T>
typename Deque<T>::const_reference Deque<T>::operator[](Deque<T>::difference_type index) const
{
	Deque<T>::size_type tmp = _start + index;
	if (tmp >= _capacity) 
		tmp -= _capacity;
	return _impl[tmp];
}

template<typename T>
Deque<T>& Deque<T>::operator=(const Deque<T>& other)
{
	if (this == &other) 
		return *this;
	_capacity = other._capacity;
	if (_impl != nullptr) 
		delete[] _impl;
	_impl = new T[_capacity];
	std::memcpy(_impl, other._impl, _capacity*sizeof(T));
	_size = other._size;
	_start = other._start;
}

template<typename T>
void Deque<T>::push_back(Deque<T>::const_reference value)
{
	operator[](_size) = value;
	++_size;
	_normalize();
}

template<typename T>
void Deque<T>::pop_back()
{
	//behaviour on empty containers is undefined, just as it is in STL
	--_size;
	_normalize();
}

template<typename T>
void Deque<T>::push_front(Deque<T>::const_reference value)
{
	if (_start == 0) 
		_start = _capacity - 1;
	else 
		_start--;

	++_size;
	_impl[_start] = value;
	_normalize();
}

template<typename T>
void Deque<T>::pop_front()
{
	//behaviour on empty containers is undefined, just as it is in STL
	++_start;
	if (_start >= _capacity) _start = 0;
	--_size;
	_normalize();
}

template<typename T>
typename Deque<T>::reference Deque<T>::back()
{
	return operator[](_size - 1);
}

template<typename T>
typename Deque<T>::const_reference Deque<T>::back() const
{
	return operator[](_size - 1);
}

template<typename T>
typename Deque<T>::reference Deque<T>::front()
{
	return _impl[_start];
}

template<typename T>
typename Deque<T>::const_reference Deque<T>::front() const
{
	return _impl[_start];
}

template<typename T>
typename Deque<T>::iterator Deque<T>::begin()
{
	return Deque<T>::iterator(this, 0);
}

template<typename T>
typename Deque<T>::const_iterator Deque<T>::begin() const
{
	return Deque<T>::const_iterator(this, 0);
}

template<typename T>
typename Deque<T>::const_iterator Deque<T>::cbegin() const
{
	return Deque<T>::const_iterator(this, 0);
}

template<typename T>
typename Deque<T>::iterator Deque<T>::end()
{
	return Deque<T>::iterator(this, _size);
}

template<typename T>
typename Deque<T>::const_iterator Deque<T>::end() const
{
	return Deque<T>::const_iterator(this, _size);
}

template<typename T>
typename Deque<T>::const_iterator Deque<T>::cend() const
{
	return Deque<T>::const_iterator(this, _size);
}


template<typename T>
typename Deque<T>::reverse_iterator Deque<T>::rbegin()
{
	return 
		std::reverse_iterator<Deque<T>::iterator>(Deque<T>::iterator(this, _size));
}

template<typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::rbegin() const
{
	return 
		std::reverse_iterator<Deque<T>::const_iterator>(Deque<T>::const_iterator(this, _size));
}

template<typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::crbegin() const
{
	return 
		std::reverse_iterator<Deque<T>::const_iterator>(Deque<T>::const_iterator(this, _size));
}

template<typename T>
typename Deque<T>::reverse_iterator Deque<T>::rend()
{
	return 
		std::reverse_iterator<Deque<T>::iterator>(Deque<T>::iterator(this, 0));
}

template<typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::rend() const
{
	return 
		std::reverse_iterator<Deque<T>::const_iterator>(Deque<T>::const_iterator(this, 0));
}

template<typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::crend() const
{
	return 
		std::reverse_iterator<Deque<T>::const_iterator>(Deque<T>::const_iterator(this, 0));
}

template<typename T>
Deque<T>::~Deque()
{
	delete[] _impl;
}

template<typename T>
void Deque<T>::_normalize()
{
	//poential place for optimizations and bugs
	if (_size >= _capacity) 
		_resize(RESIZE_COEFF*_capacity);
	else if (RESIZE_COEFF*RESIZE_COEFF*_size <= _capacity) 
		_resize(RESIZE_COEFF*_size);
}

template<typename T>
void Deque<T>::_resize(Deque<T>::size_type capacity)
{
	if (capacity < DEFAULT_SIZE) 
		capacity = DEFAULT_SIZE;
	
	//if capacity < _size, behaviour is undefined
	T* tmp = new T[capacity];
	if (_size <= _capacity - _start)
	{
		std::memcpy(tmp, _impl + _start, _size*sizeof(T));
	}
	else
	{
		std::memcpy(tmp, _impl + _start, (_capacity - _start)*sizeof(T));
		std::memcpy(tmp + _capacity - _start, _impl, (_size - (_capacity - _start))*sizeof(T));
	}

	std::swap(tmp, _impl);

	_capacity = capacity;
	_start = 0;
	delete[] tmp;
}

// ==================== ITERATORS =======================

template<typename TContainerPtr>
_iterator_base<TContainerPtr>::_iterator_base(TContainerPtr ptr, difference_type position)
	: _position(position), _container(ptr)
{

}

template<typename TContainerPtr>
_iterator_base<TContainerPtr>& _iterator_base<TContainerPtr>::operator++()
{
	_position++;
	return *this;
}

template<typename TContainerPtr>
_iterator_base<TContainerPtr> _iterator_base<TContainerPtr>::operator++(int)
{
	own_type copy = *this;
	_position++;
	return copy;
}

template<typename TContainerPtr>
_iterator_base<TContainerPtr>& _iterator_base<TContainerPtr>::operator--()
{
	_position--;
	return *this;
}

template<typename TContainerPtr>
_iterator_base<TContainerPtr> _iterator_base<TContainerPtr>::operator--(int)
{
	own_type copy = *this;
	_position--;
	return copy;
}

template<typename TContainerPtr>
_iterator_base<TContainerPtr>& _iterator_base<TContainerPtr>::operator+=(difference_type dist)
{
	_position += dist;
	return *this;
}

template<typename TContainerPtr>
_iterator_base<TContainerPtr>& _iterator_base<TContainerPtr>::operator-=(difference_type dist)
{
	_position -= dist;
	return *this;
}

template<typename TContainerPtr>
bool _iterator_base<TContainerPtr>::operator==(const own_type& other) const
{
	//lots of stuff can go wrong here, 
	//but it's an undefined behavior in stl too, so why should I care?
	return _position == other._position;
}

template<typename TContainerPtr>
bool _iterator_base<TContainerPtr>::operator!=(const own_type& other) const
{
	return !operator==(other);
}

template<typename TContainerPtr>
_iterator_base<TContainerPtr> operator+(
	const _iterator_base<TContainerPtr>& iter, 
	typename std::iterator_traits<_iterator_base<TContainerPtr>>::difference_type dist)
{
	_iterator_base<TContainerPtr> copy = iter;
	copy += dist;
	return copy;
}

template<typename TContainerPtr>
_iterator_base<TContainerPtr> operator+(
	typename std::iterator_traits<_iterator_base<TContainerPtr>>::difference_type dist,
	const _iterator_base<TContainerPtr>& iter)
{
	return iter + dist;
}

template<typename TContainerPtr>
_iterator_base<TContainerPtr> operator-(
	const _iterator_base<TContainerPtr>& iter, 
	typename std::iterator_traits<_iterator_base<TContainerPtr>>::difference_type dist)
{
	_iterator_base<TContainerPtr> copy = iter;
	copy -= dist;
	return copy;
}

template<typename TContainerPtr>
typename std::iterator_traits<_iterator_base<TContainerPtr>>::difference_type 
	operator-(
		const _iterator_base<TContainerPtr>& first, 
		const _iterator_base<TContainerPtr>& second)
{
	return first._position - second._position;
}

template<typename TContainerPtr>
bool operator<(
	const _iterator_base<TContainerPtr>& first,
	const _iterator_base<TContainerPtr>& second)
{
	return first - second < 0;
}

template<typename TContainerPtr>
typename std::iterator_traits<_iterator_base<TContainerPtr>>::reference 
	_iterator_base<TContainerPtr>::operator*()
{
	return _container->operator[](_position);
}

template<class TContainerPtr>
typename std::iterator_traits<_iterator_base<TContainerPtr>>::pointer 
	_iterator_base<TContainerPtr>::operator->()
{
	return &(_container->operator[](_position));
}
