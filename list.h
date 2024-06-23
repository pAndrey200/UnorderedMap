#pragma once

template <typename keyType, typename dataType>
struct list;

template <typename keyType, typename dataType>
struct elem
{
	std::pair<keyType, dataType> data;
	elem<keyType, dataType>* next;
	list<keyType, dataType>* th;
};

template <typename keyType, typename dataType>
class list
{
private:
	elem<keyType, dataType>* head, * tail;
	int count;

public:
	list() : head(nullptr), tail(nullptr), count(0){}
	
	~list() {
		while (tail) {
			elem<keyType, dataType>* node = tail;
			tail = tail->next;
			delete node;
		}
	}
	
	void addItem(const std::pair<keyType, dataType>& value) { 
		elem<keyType, dataType>* node = new elem<keyType, dataType>;
		node->data = value;
		node->th = this;
		if (count == 0){
			node->next = nullptr;
			head = tail = node;

		}
		else{
			node->next = nullptr;
			head->next = node;
			head = node;
		}
		count++;
	}

	void eraseItem(const keyType& k) {
		elem<keyType, dataType>* cur = tail;
		
		while (cur->data.first != k)
			cur = cur->next;
			
		elem<keyType, dataType>* cur2 = tail;
		
		if (cur == nullptr)
		    throw std::out_of_range("key does not exist");
		
		if (cur != tail) {
			while (cur2->next != cur)
				cur2 = cur2->next;
			cur2->next = cur->next;
			delete cur;
		} else {
			if (tail != head) {
				tail = tail->next;
				delete cur;
			} else {
				delete tail;
				tail = head = nullptr;
			}
		}
		
		count--;
	}

	void print() {
		if (count != 0)
		{
			elem<keyType, dataType>* node = tail;
			std::cout << "( ";
			while (node->next != nullptr)
			{
				std::cout << node->data.first << " " << node->data.second << ", ";
				node = node->next;
			}

			std::cout << node->data.first << " " << node->data.second << " )";
		}
	}
	
	elem<keyType, dataType>* getTail() { 
		return tail; 
	} 

	elem<keyType, dataType>* getHead() { 
		return head; 
	}
	
	void init() {
		elem<keyType, dataType>* node = new elem<keyType, dataType>;
		node->th = this;
		node->next = nullptr;
		head = tail = node;
		count = -1;
	}
	
	bool isEmpty() const {
		return count == 0;
	}
};

