//https://github.com/Mrkol/hw_Deque.git
#include <iterator>
#include <cstring>
#include <cstddef>


#pragma once

template<class TContainer, class TIterator>
class _iterator_base;
template<class TContainer>
class _iterator;
template<class TContainer>
class _const_iterator;

template<typename T>
class Deque
{
public:
	using size_type = std::size_t;
	using dist_type = std::ptrdiff_t;

	using value_type = T;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;

	using iterator = _iterator<Deque<T>>;
	using const_iterator = _const_iterator<Deque<T>>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	Deque();
	Deque(const Deque<T>&);

	bool empty() const;
	size_type size() const;
	size_type capacity() const;

	void clear();
	void reserve(size_type);

	reference operator[](size_type);
	const_reference operator[](size_type) const;

	Deque<T>& operator=(const Deque<T>&);

	void push_back(const_reference);
	void pop_back();

	void push_front(const_reference);
	void pop_front();

	reference back();
	const_reference back() const;

	reference front();
	const_reference front() const;

	iterator begin();
	const_iterator begin() const;
	const_iterator cbegin() const;

	iterator end();
	const_iterator end() const;
	const_iterator cend() const;

	reverse_iterator rbegin();
	const_reverse_iterator rbegin() const;
	const_reverse_iterator crbegin() const;

	reverse_iterator rend();
	const_reverse_iterator rend() const;
	const_reverse_iterator crend() const;

	~Deque();

private:
	void _normalize();
	void _resize(size_type);

	T* _impl;
	std::size_t _capacity;
	std::size_t _size;
	std::size_t _start;

	static const std::size_t RESIZE_COEFF = 2;
	static const std::size_t DEFAULT_SIZE = 1;
};




////////// ITERATORS //////////


template<class TContainer, class TIterator>
TIterator operator+(const _iterator_base<TContainer, TIterator>&, 
	typename TContainer::dist_type);

template<class TContainer, class TIterator>
TIterator operator+(typename TContainer::dist_type,
	const _iterator_base<TContainer, TIterator>&);

template<class TContainer, class TIterator>
TIterator operator-(const _iterator_base<TContainer, TIterator>&, 
	typename TContainer::dist_type);

template<class TContainer, class TIterator>
class _iterator_base
{
public:
	_iterator_base(const _iterator_base<TContainer, TIterator>&) = default;

	TIterator& operator++();
	TIterator operator++(int);
	TIterator& operator--();
	TIterator operator--(int);

	//can't really use the proper TIterator::difference_type cuz of dependent base class
	TIterator& operator+=(typename TContainer::dist_type);
	TIterator& operator-=(typename TContainer::dist_type);

	friend TIterator operator+<>(const _iterator_base<TContainer, TIterator>&, 
		typename TContainer::dist_type);
	friend TIterator operator+<>(typename TContainer::dist_type,
		const _iterator_base<TContainer, TIterator>&);
	friend TIterator operator-<>(const _iterator_base<TContainer, TIterator>&, 
		typename TContainer::dist_type);

	bool operator==(const TIterator&) const;
	bool operator!=(const TIterator&) const;

	_iterator_base& operator=(const _iterator_base&) = default;

protected:
	_iterator_base(typename TContainer::size_type);
	TIterator* _to_derived();
	const TIterator* _to_derived() const;

	typename TContainer::size_type _position;
};


template<class TContainer>
class _iterator 
	: public std::iterator<std::random_access_iterator_tag, typename TContainer::value_type, 
		typename TContainer::dist_type, typename TContainer::pointer, typename TContainer::reference>, 
		public _iterator_base<TContainer, _iterator<TContainer>>
{
public:
	_iterator(TContainer*, typename TContainer::size_type);
	_iterator(const _iterator<TContainer>&) = default;

	_iterator& operator=(const _iterator&) = default;
	typename TContainer::reference operator*();

private:
	TContainer* _container;
};

template<class TContainer>
class _const_iterator
	: public std::iterator<std::random_access_iterator_tag, typename TContainer::value_type, 
		typename TContainer::dist_type, typename TContainer::const_pointer, typename TContainer::const_reference>, 
		public _iterator_base<TContainer, _const_iterator<TContainer>>
{
public:
	_const_iterator(const TContainer*, typename TContainer::size_type);
	_const_iterator(const _iterator<TContainer>&);
	_const_iterator(const _const_iterator<TContainer>&) = default;

	_const_iterator& operator=(const _const_iterator&) = default;
	typename TContainer::const_reference operator*() const;

private:
	const TContainer* _container;
};


template<typename T>
Deque<T>::Deque()
{
	_capacity = DEFAULT_SIZE;
	_impl = new T[_capacity];
	_size = 0;
	_start = 0;
}

template<typename T>
Deque<T>::Deque(const Deque<T>& other)
{
	_capacity = other._capacity;
	_impl = new T[_capacity];
	std::memcpy(_impl, other._impl, _capacity*sizeof(T));
	_size = other._size;
	_start = other._start;
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
	if (_impl != nullptr) delete[] _impl;
	_capacity = DEFAULT_SIZE;
	_impl = new T[_capacity];
	_size = 0;
	_start = 0;
}

template<typename T>
void Deque<T>::reserve(typename Deque<T>::size_type amount)
{
	if (amount <= _capacity) return;
	_resize(amount);
}

template<typename T>
typename Deque<T>::reference Deque<T>::operator[](Deque<T>::size_type index)
{
	return _impl[(_start + index) % _capacity];
}

template<typename T>
typename Deque<T>::const_reference Deque<T>::operator[](Deque<T>::size_type index) const
{
	return _impl[(_start + index) % _capacity];
}

template<typename T>
Deque<T>& Deque<T>::operator=(const Deque<T>& other)
{
	_capacity = other._capacity;
	if (_impl != nullptr) delete[] _impl;
	_impl = new T[_capacity];
	std::memcpy(_impl, other._impl, _capacity*sizeof(T));
	_size = other._size;
	_start = other._start;
}

template<typename T>
void Deque<T>::push_back(Deque<T>::const_reference value)
{
	_impl[(_start + _size)%_capacity] = value;
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
	if (_start == 0) _start = _capacity - 1;
	else _start--;
	++_size;
	_impl[_start] = value;
	_normalize();
}

template<typename T>
void Deque<T>::pop_front()
{
	//behaviour on empty containers is undefined, just as it is in STL
	++_start;
	_start %= _capacity;
	--_size;
	_normalize();
}

template<typename T>
typename Deque<T>::reference Deque<T>::back()
{
	return _impl[(_start + _size - 1) % _capacity];
}

template<typename T>
typename Deque<T>::const_reference Deque<T>::back() const
{
	return _impl[(_start + _size - 1) % _capacity];
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
		std::reverse_iterator<Deque<T>::iterator>(Deque<T>::iterator(this, 0));
}

template<typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::rbegin() const
{
	return 
		std::reverse_iterator<Deque<T>::const_iterator>(Deque<T>::const_iterator(this, 0));
}

template<typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::crbegin() const
{
	return 
		std::reverse_iterator<Deque<T>::const_iterator>(Deque<T>::const_iterator(this, 0));
}

template<typename T>
typename Deque<T>::reverse_iterator Deque<T>::rend()
{
	return 
		std::reverse_iterator<Deque<T>::iterator>(Deque<T>::iterator(this, _size));
}

template<typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::rend() const
{
	return 
		std::reverse_iterator<Deque<T>::const_iterator>(Deque<T>::const_iterator(this, _size));
}

template<typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::crend() const
{
	return 
		std::reverse_iterator<Deque<T>::const_iterator>(Deque<T>::const_iterator(this, _size));
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
	if (RESIZE_COEFF*RESIZE_COEFF*_size <= _capacity) _resize(RESIZE_COEFF*_size);
	else if (_size >= _capacity) _resize(RESIZE_COEFF*_capacity);
}

template<typename T>
void Deque<T>::_resize(Deque<T>::size_type capacity)
{
	if (capacity < DEFAULT_SIZE) capacity = DEFAULT_SIZE;
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


template<class TContainer, class TIterator>
_iterator_base<TContainer, TIterator>::_iterator_base(typename TContainer::size_type position)
	: _position(position)
{

}

template<class TContainer, class TIterator>
TIterator& _iterator_base<TContainer, TIterator>::operator++()
{
	_position++;
	return *_to_derived();
}

template<class TContainer, class TIterator>
TIterator _iterator_base<TContainer, TIterator>::operator++(int)
{
	TIterator copy = *_to_derived();
	_position++;
	return copy;
}

template<class TContainer, class TIterator>
TIterator& _iterator_base<TContainer, TIterator>::operator--()
{
	_position--;
	return *_to_derived();
}

template<class TContainer, class TIterator>
TIterator _iterator_base<TContainer, TIterator>::operator--(int)
{
	TIterator copy = *_to_derived();
	_position--;
	return copy;
}

template<class TContainer, class TIterator>
TIterator& _iterator_base<TContainer, TIterator>::operator+=(typename TContainer::dist_type dist)
{
	_position += dist;
	return *_to_derived();
}

template<class TContainer, class TIterator>
TIterator& _iterator_base<TContainer, TIterator>::operator-=(typename TContainer::dist_type dist)
{
	_position -= dist;
	return *_to_derived();
}

template<class TContainer, class TIterator>
bool _iterator_base<TContainer, TIterator>::operator==(const TIterator& other) const
{
	//lots of stuff can go wrong here, 
	//but it's an undefined behavior in stl too, so why should I care?
	return _position == other._position;
}

template<class TContainer, class TIterator>
bool _iterator_base<TContainer, TIterator>::operator!=(const TIterator& other) const
{
	return !operator==(other);
}

template<class TContainer, class TIterator>
TIterator operator+(const _iterator_base<TContainer, TIterator>& iter, 
	typename TContainer::dist_type dist)
{
	TIterator copy = *iter._to_derived();
	copy += dist;
	return copy;
}

template<class TContainer, class TIterator>
TIterator operator+(typename TContainer::dist_type dist,
	const _iterator_base<TContainer, TIterator>& iter)
{
	return iter + dist;
}

template<class TContainer, class TIterator>
TIterator operator-(const _iterator_base<TContainer, TIterator>& iter, 
	typename TContainer::dist_type dist)
{
	TIterator copy = *iter._to_derived();
	copy -= dist;
	return copy;
}

template<class TContainer, class TIterator>
TIterator* _iterator_base<TContainer, TIterator>::_to_derived()
{
	return static_cast<TIterator*>(this);
}

template<class TContainer, class TIterator>
const TIterator* _iterator_base<TContainer, TIterator>::_to_derived() const
{
	return static_cast<const TIterator*>(this);
}

template<class TContainer>
_iterator<TContainer>::_iterator(TContainer* container, 
	typename TContainer::size_type position)
	: _iterator_base<TContainer, _iterator<TContainer>>(position),
	_container(container)
{

}

template<class TContainer>
typename TContainer::reference _iterator<TContainer>::operator*()
{
	//doesn't compile without "this". Weird.
	return this->_container->operator[](this->_position);
}

template<class TContainer>
_const_iterator<TContainer>::_const_iterator(const TContainer* container, 
	typename TContainer::size_type position)
	: _iterator_base<TContainer, _const_iterator<TContainer>>(position),
	_container(container)
{

}

template<class TContainer>
_const_iterator<TContainer>::_const_iterator(const _iterator<TContainer>& other)
	: _iterator_base<TContainer, _const_iterator<TContainer>>(other)
{

}

template<class TContainer>
typename TContainer::const_reference _const_iterator<TContainer>::operator*() const
{
	return this->_container->operator[](this->_position);
}