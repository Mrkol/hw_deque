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
typename TContainer::dist_type operator-(const _iterator_base<TContainer, TIterator>& first, 
	const _iterator_base<TContainer, TIterator>& second)
{
	return first._position - second._position;
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
typename TContainer::pointer _iterator<TContainer>::operator->()
{
	return &(this->_container->operator[](this->_position));
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

template<class TContainer>
typename TContainer::const_pointer _const_iterator<TContainer>::operator->() const
{
	return &(this->_container->operator[](this->_position));
}