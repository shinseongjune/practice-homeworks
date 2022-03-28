#include <iostream>
#include <list>

namespace Test {
	template <typename T>
	class List {
		class Node {
		public:
			T data;
			Node* prev;
			Node* next;
			bool isEnd;

			Node() {
				data = NULL;
				prev = NULL;
				next = NULL;
				isEnd = false;
			}

			void setEndNode() {
				isEnd = true;
			}
		};

		Node* head;
		Node* tail;
		Node* end;
		int size;
	public:
		class iterator {
		public:
			Node* node;

			iterator() { 
				node = NULL;
			}

			iterator(iterator& other) {
				node = other.node;
			}
			
			iterator& operator++() {
				if (node->next != NULL) {
					node = node->next;
				}
				return *this;
			}

			iterator& operator--() {
				if (node->prev != NULL) {
					node = node->prev;
				}
				return *this;
			}

			iterator operator++(int) {
				iterator temp = *this;
				if (node->next != NULL) {
					node = node->next;
				}
				return temp;
			}

			iterator operator--(int) {
				iterator temp = *this;
				if (node->prev != NULL) {
					node = node->prev;
				}
				return temp;
			}

			bool operator==(iterator other) {
				return (node == other.node);
			}

			bool operator!=(iterator other) {
				return !(this == other);
			}

			T value() {
				return node->data;
			}
		};

		List() {
			size = 0;
			head = NULL;
			tail = NULL;
			end = new Node();
			end->setEndNode();
		}

		List(int num, T value = T()) {
			if (num == 0) {
				size = 0;
				head = NULL;
				tail = NULL;
				end = new Node();
				end->setEndNode();
				return;
			}
			size = num;
			Node* h = new Node();
			h->data = value;
			head = h;
			for (int i = 0; i < num - 1; i++) {
				Node* temp = new Node();
				temp->data = value;
				h->next = temp;
				h = temp;
			}
			tail = h;
			end = new Node();
			end->setEndNode();
			tail->next = end;
		}

		iterator& beginIterator() {
			iterator it;
			it.node = head;
			return it;
		}

		iterator& endIterator() {
			iterator it;
			it.node = end;
			return it;
		}

		List(List& other) {
			if (other.size == 0) {
				size = 0;
				head = NULL;
				tail = NULL;
				end = new Node();
				end->setEndNode();
				return;
			}
			size = other.size;
			Node* h = new Node();
			h->data = other.head->data;
			head = h;
			iterator it = other.beginIterator();
			for (int i = 0; i < size - 1; i++) {
				it++;
				Node* temp = new Node();
				temp->data = it.value();
				h->next = temp;
				h = temp;
			}
			tail = h;
			end = new Node();
			end->setEndNode();
			tail->next = end;
		}

		int getSize() {
			return size;
		}

		T front() {
			return head->data;
		}

		T back() {
			return tail->data;
		}

		bool empty() {
			if (size == 0) return true;
			return false;
		}

		void assign(int num, T value = T()) {
			if (size != 0) {
				iterator it = beginIterator();
				for (int i = 0; i < size; i++) {
					delete it++.node;
				}
			}
			if (num == 0) {
				size = 0;
				head = NULL;
				tail = NULL;
				end = new Node();
				end->setEndNode();
				return;
			}
			size = num;
			Node* h = new Node();
			h->data = value;
			head = h;
			for (int i = 0; i < num - 1; i++) {
				Node* temp = new Node();
				temp->data = value;
				h->next = temp;
				h = temp;
			}
			tail = h;
			end = new Node();
			end->setEndNode();
			tail->next = end;
		}
	};
}

using namespace std;
using namespace Test;
void main() {
	
	List<int> l2(3, 5);
	List<int> l3 = l2;
	List<int> l4;
	l4.assign(3, 5);
	l4.assign(2, 3);
}