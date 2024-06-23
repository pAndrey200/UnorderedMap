#pragma once
#include "list.h"
#include "Container.h"

template<typename keyType, typename dataType>
class MapIterator                             
{
private:
    void next()
    {
        if (ptr && ptr->next)
            ptr = ptr->next;
        else {
            do
                lptr++;
            while (lptr->isEmpty());
            ptr = lptr->getTail();

        }
    }
public:
    elem<keyType, dataType>* ptr;
    list<keyType, dataType>* lptr;

    MapIterator(elem<keyType, dataType>* _ptr = nullptr)
        :ptr(_ptr)
    {
        if (ptr)
            lptr = ptr->th;
        else
            lptr = nullptr;
        
    }

    MapIterator(const MapIterator& i)
        :ptr(i.ptr), lptr(ptr->th)
    {}

    

    MapIterator& operator=(const MapIterator& i)
    {
        if (this == &i) 
            return *this;
        ptr = i.ptr;
        lptr = i.lptr;
        return *this;
    }

    std::pair<keyType, dataType>& operator*() const
    {
        return ptr->data;
    }

    std::pair<keyType, dataType>* operator->() const
    {
        return &(ptr->data);
    }

    MapIterator& operator++()
    {
        next();
        return *this;
    }

    MapIterator operator++(int)
    {
        MapIterator tmp(*this);
        next();
        return tmp;
    }

    bool isEmpty() {
        return ptr;
    }
};

template<typename keyType, typename dataType>
bool operator==(const MapIterator<keyType, dataType>& x, const MapIterator<keyType, dataType>& y)
{
    return x.ptr == y.ptr;
}

template<typename keyType, typename dataType>
bool operator!=(const MapIterator<keyType, dataType>& x, const MapIterator<keyType, dataType>& y)
{
    return x.ptr != y.ptr;
}