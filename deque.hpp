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
	using value_type = T;
	using reference = value_type&;
	using const_reference = const value_type&;

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
	const_iterator cbegin() const;

	iterator end();
	const_iterator cend() const;

	reverse_iterator rbegin();
	const_reverse_iterator crbegin() const;

	reverse_iterator rend();
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

template<class TContainer, class TIterator>
class _iterator_base
{
public:
	_iterator_base(TContainer*, typename TContainer::size_type);

	TIterator& operator++();
	TIterator operator++(int);
	TIterator& operator--();
	TIterator operator--(int);

	bool operator==(const TIterator&) const;
	bool operator!=(const TIterator&) const;

protected:
	TContainer* _container;
	typename TContainer::size_type _position;
};

template<class TContainer>
class _iterator 
	: public std::iterator<std::random_access_iterator_tag, typename TContainer::value_type, 
		typename TContainer::size_type, typename TContainer::pointer, typename TContainer::reference>, 
		public _iterator_base<TContainer, _iterator<TContainer>>
{
public:
	_iterator(TContainer*, typename TContainer::size_type);

	typename TContainer::reference operator*();
};

template<class TContainer>
class _const_iterator
	: public std::iterator<std::random_access_iterator_tag, typename TContainer::value_type, 
		typename TContainer::size_type, typename TContainer::const_pointer, typename TContainer::const_reference>, 
		public _iterator_base<TContainer, _const_iterator<TContainer>>
{
public:
	_const_iterator(TContainer*, typename TContainer::size_type);

	typename TContainer::const_reference operator*() const;
};

#include "deque.tpp"
