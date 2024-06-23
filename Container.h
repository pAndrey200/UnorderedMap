#pragma once
#include <iostream>
#include <initializer_list>
#include <functional>
#include "list.h"
#include "Iterator.h"
#include <exception>

template <typename keyType, typename dataType, typename Hash = std::hash<keyType>>
class UnorderedMap
{
    unsigned static const defaultCapacity = 11;
    list<keyType, dataType> *table;
	Hash hash;
	float maxLoad = 0.8;
	unsigned capacity;
	unsigned count;
	unsigned ibegin = defaultCapacity;
public:
	using iterator = MapIterator<keyType, dataType>; 

    explicit UnorderedMap(unsigned n = -1); 
	UnorderedMap(const std::initializer_list<std::pair<keyType, dataType>>& ilist);
	UnorderedMap(const UnorderedMap& h);
	UnorderedMap(UnorderedMap&& h); 
	template<class InputIt>
	UnorderedMap(InputIt first, InputIt last);
    ~UnorderedMap() { delete[] table; }
	UnorderedMap<keyType, dataType>& operator=(const UnorderedMap& h);
	UnorderedMap<keyType, dataType>& operator=(UnorderedMap&& h);
	dataType& operator[](const keyType& k);
	const dataType& operator[](const keyType& k) const; 
	dataType& at(const keyType& k);
	const dataType& at(const keyType& k) const;     
    void insert(const std::pair<keyType, dataType>& toInsert);
	template<class InputIt>
	void insert(InputIt beg, InputIt end);
	void erase(const keyType& k);
	iterator& erase(iterator& it);
	void rehash(unsigned _capacity = 0);
    void print();
	void clear();
	float getMaxLoadFactor();
	void setMaxLoad(float x);
	bool empty() const;
	std::size_t size() const;
	iterator begin() const;
	iterator end() const  { return table[capacity].getTail(); }
};


template<typename keyType, typename dataType, typename Hash>
inline void UnorderedMap<keyType, dataType, Hash>::rehash(unsigned _capacity)
{	
	if (!_capacity)
	    _capacity = capacity * 2;
	    
	UnorderedMap newhash(_capacity);
	
	newhash.maxLoad = maxLoad;
	
	for (MapIterator<keyType, dataType> it = begin(); it != end(); ++it) {
		newhash.insert(*it);
	}
	
	*this = newhash;
}

template <typename keyType, typename dataType, typename Hash>
UnorderedMap< keyType, dataType, Hash>::UnorderedMap(unsigned n)
    : hash(Hash()), count(0){
	if (n == -1) capacity = defaultCapacity;
	else capacity = n / maxLoad;
    table = new list<keyType, dataType>[capacity+1];
	table[capacity].init();
}

template<typename keyType, typename dataType, typename Hash>
inline UnorderedMap<keyType, dataType, Hash>::UnorderedMap(const std::initializer_list<std::pair<keyType, dataType>> &ilist) 
	: hash(Hash()), count(0), capacity(ilist.size() * 2)
{
	table = new list<keyType, dataType>[capacity+1];
	table[capacity].init();
	for (auto it = ilist.begin(); it != ilist.end(); ++it) {
		insert(*it);
	}
}

template<typename keyType, typename dataType, typename Hash>
inline UnorderedMap<keyType, dataType, Hash>::UnorderedMap(const UnorderedMap& h)
{
	hash = h.hash;
	count = 0;
	capacity = h.capacity;
	maxLoad = h.maxLoad;
	table = new list<keyType,dataType>[capacity+1];
	table[capacity].init();
	ibegin = h.ibegin;
	for (MapIterator<keyType, dataType> it = h.begin(); it != h.end(); it++) {
		insert(*it);
	}
}

template<typename keyType, typename dataType, typename Hash>
inline UnorderedMap<keyType, dataType, Hash>::UnorderedMap(UnorderedMap&& h)
{
	hash = h.hash;
	count = h.count;
	capacity = h.capacity;
	maxLoad = h.maxLoad;
	ibegin = h.ibegin;
	table = h.table;
	h.table = nullptr;
}

template<typename keyType, typename dataType, typename Hash>
inline UnorderedMap<keyType, dataType>& UnorderedMap<keyType, dataType, Hash>::operator=(const UnorderedMap& h)
{
	if (this == &h)
	    return *this;
	    
	if (capacity != h.capacity) {
		capacity = h.capacity;
		delete[] table;
		table = new list<keyType, dataType>[capacity + 1];
		table[capacity].init();
	}
	ibegin = h.ibegin;
	hash = h.hash;
	count = 0;
	maxLoad = h.maxLoad;
	for (MapIterator<keyType, dataType> it = h.begin(); it != h.end(); ++it) {
		insert(*it);
	}
	return *this;
}

template<typename keyType, typename dataType, typename Hash>
inline UnorderedMap<keyType, dataType>& UnorderedMap<keyType, dataType, Hash>::operator=(UnorderedMap&& h)
{
	if (this == &h)
	    return *this;
	    
	delete[] table;
	hash = h.hash;
	count = h.count;
	capacity = h.capacity;
	maxLoad = h.maxLoad;
	table = h.table;
	ibegin = h.ibegin;
	h.table = nullptr;
	return *this;
}

template<typename keyType, typename dataType, typename Hash>
inline dataType& UnorderedMap<keyType, dataType, Hash>::operator[](const keyType& k)
{
	elem<keyType, dataType> *it = table[hash(k) % capacity].getTail(); 
	while (it && it != table[hash(k) % capacity].getHead()->next) {
		if (it->data.first == k)
		    return it->data.second;
		    
		it = it->next;
	}
}

template<typename keyType, typename dataType, typename Hash>
inline const dataType& UnorderedMap<keyType, dataType, Hash>::operator[](const keyType& k) const
{
	elem<keyType, dataType>* it = table[hash(k) % capacity].getTail();
	while (it && it != table[hash(k) % capacity].getHead()->next) {
		if (it->data.first == k)
			return it->data.second;

		it = it->next;
	}
}

template<typename keyType, typename dataType, typename Hash>
inline dataType& UnorderedMap<keyType, dataType, Hash>::at(const keyType& k)
{
	elem<keyType, dataType>* it = table[hash(k) % capacity].getTail();
	while (it && it != table[hash(k) % capacity].getHead()->next) {
		if (it->data.first == k) return it->data.second;
		it = it->next;
	}
	throw std::out_of_range("key does not exist");
}

template<typename keyType, typename dataType, typename Hash>
inline const dataType& UnorderedMap<keyType, dataType, Hash>::at(const keyType& k) const
{
	elem<keyType, dataType>* it = table[hash(k) % capacity].getTail();
	while (it && it != table[hash(k) % capacity].getHead()->next) {
		if (it->data.first == k) return it->data.second;
		it = it->next;
	}
	throw std::out_of_range("key does not exist");
}

template<typename keyType, typename dataType, typename Hash>
inline void UnorderedMap<keyType, dataType, Hash>::insert(const std::pair<keyType, dataType> &toInsert)
{
	
	if (float(count) / capacity >= maxLoad) {
		//std::cout << "rehash\n";
		rehash(); 
	}
	unsigned pos = hash(toInsert.first) % capacity;
	table[pos].addItem(toInsert);
	if (ibegin > pos) ibegin = pos;
	count++;
	/*std::cout << std::endl;
	print();*/
}

template<typename keyType, typename dataType, typename Hash>
template<class InputIt>
inline void UnorderedMap<keyType, dataType, Hash>::insert(InputIt beg, InputIt end)
{
	for (; beg != end; beg++) {
		insert(*beg);
	}
}

template<typename keyType, typename dataType, typename Hash>
inline void UnorderedMap<keyType, dataType, Hash>::erase(const keyType& k)
{
	table[hash(k) % capacity].eraseItem(k);
	count--;
}

template<typename keyType, typename dataType, typename Hash>
inline MapIterator<keyType, dataType>& UnorderedMap<keyType, dataType, Hash>::erase(iterator& it)
{
	iterator it2(it++);
	table[hash((*it2).first) % capacity].eraseItem((*it2).first);
	count--;
	return it;
}

template<typename keyType, typename dataType, typename Hash>
inline void UnorderedMap<keyType, dataType, Hash>::print()
{
    for (int i = 0; i < capacity; i++) {
		std::cout << "[" << i << "]  ";
        table[i].print();
		std::cout << std::endl;
    }
}

template<typename keyType, typename dataType, typename Hash>
inline void UnorderedMap<keyType, dataType, Hash>::clear()
{
	iterator it = begin();
	while (it != end()) {
		it = erase(it);
	}
}

template<typename keyType, typename dataType, typename Hash>
inline float UnorderedMap<keyType, dataType, Hash>::getMaxLoadFactor()
{
	return maxLoad;
}

template<typename keyType, typename dataType, typename Hash>
inline void UnorderedMap<keyType, dataType, Hash>::setMaxLoad(float x)
{
	maxLoad = x;
}

template<typename keyType, typename dataType, typename Hash>
inline bool UnorderedMap<keyType, dataType, Hash>::empty() const
{
	return count == 0;
}

template<typename keyType, typename dataType, typename Hash>
inline std::size_t UnorderedMap<keyType, dataType, Hash>::size() const
{
	return count;
}

template<typename keyType, typename dataType, typename Hash>
inline MapIterator<keyType, dataType> UnorderedMap<keyType, dataType, Hash>::begin() const
{
	
	return table[ibegin].getTail();
	
}

template<typename keyType, typename dataType, typename Hash>
template<class InputIt>
inline UnorderedMap<keyType, dataType, Hash>::UnorderedMap(InputIt first, InputIt last)
	:hash(Hash()), capacity(11)
{
	table = new list<keyType, dataType>[capacity + 1];
	table[capacity].init();
	for (InputIt i = first; i != last; i++) { //!!! Эффективнее использовать ++i
		insert(std::make_pair((*i).first, (*i).second));
	}
}
