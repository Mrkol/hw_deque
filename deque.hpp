//https://github.com/Mrkol/hw_Deque.git

#ifndef DEQUE_HPP
#define DEQUE_HPP

#include <iterator>
#include <cstring>
#include <cstddef>


template<typename TContainerPtr>
class _iterator_base;

template<typename T>
class Deque
{
public:
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	using value_type = T;
	using reference = value_type&;
	using pointer = value_type*;

	using const_value_type = const T;
	using const_reference = const value_type&;
	using const_pointer = const value_type*;

	using iterator = _iterator_base<Deque<T>*>;
	using const_iterator = _iterator_base<const Deque<T>*>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	Deque();
	Deque(const Deque<T>&);

	bool empty() const;
	size_type size() const;
	size_type capacity() const;

	void clear();
	void reserve(size_type);

	reference operator[](difference_type);
	const_reference operator[](difference_type) const;

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
	reference _get(difference_type) const;

	T* _impl;
	size_type _capacity;
	size_type _size;
	size_type _start;

	static const size_type RESIZE_COEFF = 2;
	static const size_type DEFAULT_SIZE = 8;
};




////////// ITERATORS //////////


template<typename TContainerPtr>
_iterator_base<TContainerPtr> operator+(
	const _iterator_base<TContainerPtr>&, 
	typename std::iterator_traits<_iterator_base<TContainerPtr>>::difference_type);

template<typename TContainerPtr>
_iterator_base<TContainerPtr> operator+(
	typename std::iterator_traits<_iterator_base<TContainerPtr>>::difference_type,
	const _iterator_base<TContainerPtr>&);

template<typename TContainerPtr>
_iterator_base<TContainerPtr> operator-(
	const _iterator_base<TContainerPtr>&, 
	typename std::iterator_traits<_iterator_base<TContainerPtr>>::difference_type);

template<typename TContainerPtr>
typename std::iterator_traits<_iterator_base<TContainerPtr>>::difference_type 
	operator-(
		const _iterator_base<TContainerPtr>&, 
		const _iterator_base<TContainerPtr>&);

template<typename TContainerPtr>
bool operator<(
	const _iterator_base<TContainerPtr>&,
	const _iterator_base<TContainerPtr>&);

namespace std
{
	template<typename T>
	class iterator_traits<_iterator_base<Deque<T>*>>
	{
	public:
		using difference_type = typename Deque<T>::difference_type;
		using value_type = typename Deque<T>::value_type;
		using pointer = typename Deque<T>::pointer;
		using reference = typename Deque<T>::reference;
		using iterator_category = std::random_access_iterator_tag;
	};

	template<typename T>
	class iterator_traits<_iterator_base<const Deque<T>*>>
	{
	public:
		using difference_type = typename Deque<T>::difference_type;
		using value_type = typename Deque<T>::const_value_type;
		using pointer = typename Deque<T>::const_pointer;
		using reference = typename Deque<T>::const_reference;
		using iterator_category = std::random_access_iterator_tag;
	};
}

template<typename TContainerPtr>
class _iterator_base 
	: public std::iterator<
		typename std::iterator_traits<_iterator_base<TContainerPtr>>::value_type, 
		typename std::iterator_traits<_iterator_base<TContainerPtr>>::iterator_category,
		typename std::iterator_traits<_iterator_base<TContainerPtr>>::difference_type, 
		typename std::iterator_traits<_iterator_base<TContainerPtr>>::pointer, 
		typename std::iterator_traits<_iterator_base<TContainerPtr>>::reference>
{
public:
	using value_type = typename std::iterator_traits<_iterator_base<TContainerPtr>>::value_type;
	using difference_type = typename std::iterator_traits<_iterator_base<TContainerPtr>>::difference_type;
	using pointer = typename std::iterator_traits<_iterator_base<TContainerPtr>>::pointer;
	using reference = typename std::iterator_traits<_iterator_base<TContainerPtr>>::reference;
	using own_type = _iterator_base<TContainerPtr>;

	_iterator_base(TContainerPtr, difference_type);

	_iterator_base(const _iterator_base<TContainerPtr>&) = default;

	_iterator_base& operator++();
	_iterator_base operator++(int);
	_iterator_base& operator--();
	_iterator_base operator--(int);

	_iterator_base& operator+=(difference_type);
	_iterator_base& operator-=(difference_type);

	friend _iterator_base operator+<>(const own_type&, difference_type);

	friend _iterator_base operator+<>(difference_type, const own_type&);

	friend _iterator_base operator-<>(const own_type&, difference_type);
	
	friend difference_type operator-<>(const own_type&, const own_type&);

	bool operator==(const own_type&) const;
	bool operator!=(const own_type&) const;

	_iterator_base& operator=(const _iterator_base&) = default;

	reference operator*();
	pointer operator->();

private:
	difference_type _position;

	TContainerPtr _container;
};

#include "deque.tpp"

#endif //DEQUE_HPP
