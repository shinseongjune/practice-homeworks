#include <iostream>
#include <list>
#include <assert.h>

using namespace std;

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

			Node(T value) {
				data = value;
				prev = NULL;
				next = NULL;
				isEnd = false;
			}

			void setEndNode() {
				isEnd = true;
			}

			void operator==(Node& other) {
				return data == other.data && prev == other.prev && next == other.next && isEnd == other.isEnd ? true : false;
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
			bool isReverse = false;

			iterator() { 
				node = NULL;
			}

			iterator(iterator& other) {
				node = other.node;
				isReverse = other.isReverse;
			}
			
			iterator& operator++() {
				if (node->isEnd) return *this;
				if (isReverse) {
					if (node->prev != NULL) {
						node = node->prev;
					}
				}
				else {
					if (node->next != NULL) {
						node = node->next;
					}
				}
				return *this;
			}

			iterator& operator--() {
				if (isReverse) {
					if (node->next->isEnd) return *this;
					if (node->next != NULL) {
						node = node->next;
					}
				}
				else {
					if (node->prev->isEnd) return *this;
					if (node->prev != NULL) {
						node = node->prev;
					}
				}
				return *this;
			}

			iterator operator++(int) {
				iterator temp = *this;
				if (node->isEnd) return temp;
				if (isReverse) {
					if (node->prev != NULL) {
						node = node->prev;
					}
				}
				else {
					if (node->next != NULL) {
						node = node->next;
					}
				}
				return temp;
			}

			iterator operator--(int) {
				iterator temp = *this;
				if (isReverse) {
					if (node->next->isEnd) return temp;
					if (node->next != NULL) {
						node = node->next;
					}
				}
				else {
					if (node->prev->isEnd) return *this;
					if (node->prev != NULL) {
						node = node->prev;
					}
				}
				return temp;
			}

			bool operator==(iterator other) {
				return (node == other.node);
			}

			bool operator!=(iterator other) {
				return !(*this == other);
			}

			T operator*() {
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
			Node* h = new Node(value);
			head = h;
			for (int i = 0; i < num - 1; i++) {
				Node* temp = new Node(value);
				h->next = temp;
				temp->prev = h;
				h = temp;
			}
			tail = h;
			end = new Node();
			end->setEndNode();
			tail->next = end;
			end->prev = tail;
			end->next = head;
			head->prev = end;
		}

		iterator& beginIterator() {
			iterator* it = new iterator();
			it->node = head;
			return *it;
		}

		iterator& endIterator() {
			iterator* it = new iterator();
			it->node = end;
			return *it;
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
				temp->data = *it;
				h->next = temp;
				temp->prev = h;
				h = temp;
			}
			tail = h;
			end = new Node();
			end->setEndNode();
			tail->next = end;
			end->prev = tail;
			end->next = head;
			head->prev = end;
		}

		bool operator==(List& other) {
			if (size != other.size) return false;
			iterator thisIt = beginIterator();
			iterator otherIt = other.beginIterator();
			for (; thisIt != endIterator(); thisIt++, otherIt++) {
				if (*thisIt != *otherIt) return false;
			}
			return true;
		}

		bool operator!=(List& other) {
			return !(*this == other);
		}

		bool operator<(List& other) {
			if (*this == other) return false;
			iterator thisIt = beginIterator();
			iterator otherIt = other.beginIterator();
			while (thisIt != endIterator() && otherIt != other.endIterator()) {
				if (*thisIt != *otherIt) {
					return *thisIt < *otherIt ? true : false;
				}
				thisIt++;
				otherIt++;
			}
			if (thisIt == endIterator()) {
				return true;
			}
			else {
				return false;
			}
		}

		bool operator<=(List& other) {
			return (*this < other || *this == other);
		}

		bool operator>(List& other) {
			return !(*this <= other);
		}

		bool operator>=(List& other) {
			return (*this > other || *this == other);
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
			assert(num >= 0);
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
				Node* temp = new Node(value);
				h->next = temp;
				temp->prev = h;
				h = temp;
			}
			tail = h;
			end = new Node();
			end->setEndNode();
			tail->next = end;
			end->prev = tail;
			end->next = head;
			head->prev = end;
		}

		void push_back(T value) {
			if (size == 0) {
				size = 1;
				Node* h = new Node(value);
				head = h;
				tail = h;
				tail->next = end;
				end->prev = tail;
				end->next = head;
				head->prev = end;
			}
			else {
				size++;
				Node* h = new Node(value);
				tail->next = h;
				h->prev = tail;
				tail = h;
				tail->next = end;
				end->prev = tail;
				end->next = head;
				head->prev = end;
			}
		}

		void push_front(T value) {
			if (size == 0) {
				size = 1;
				Node* h = new Node();
				h->data = value;
				head = h;
				tail = h;
				tail->next = end;
				end->prev = tail;
				end->next = head;
				head->prev = end;
			}
			else {
				size++;
				Node* h = new Node();
				h->data = value;
				head->prev = h;
				h->next = head;
				head = h;
				end->next = head;
				head->prev = end;
			}
		}

		void pop_back() {
			if (size == 0) {
				return;
			}
			else {
				Node* t = tail->prev;
				t->next = end;
				end->prev = t;
				delete tail;
				tail = t;
				size--;
			}
		}

		void pop_front() {
			if (size == 0) {
				return;
			}
			else {
				Node* h = head->next;
				h->prev = NULL;
				delete head;
				head = h;
				end->next = head;
				head->prev = end;
				size--;
			}
		}

		iterator& rbegin() {
			iterator* it = new iterator();
			it->node = tail;
			it->isReverse = true;
			return *it;
		}

		iterator& rend() {
			iterator* it = new iterator();
			it->node = end;
			it->isReverse = true;
			return *it;
		}

		void insert(iterator& it, T value) {
			if (size == 0) {
				push_back(value);
			}
			else {
				if (it.isReverse) {
					iterator tempIt(it);
					Node* n = new Node(value);
					n->prev = tempIt.node;
					tempIt--;
					n->next = tempIt.node;
					tempIt.node->prev = n;
					tempIt++;
					tempIt++;
					tempIt.node->next = n;
				}
				else {
					iterator tempIt(it);
					Node* n = new Node(value);
					n->next = tempIt.node;
					tempIt--;
					n->prev = tempIt.node;
					tempIt.node->next = n;
					tempIt++;
					tempIt++;
					tempIt.node->prev = n;
				}
				head = end->next;
				tail = end->prev;
				it--;
				size++;
			}
		}

		void erase(iterator& it) {
			if (it.node->isEnd) return;
			if (it.isReverse) {
				iterator tempIt(it);
				tempIt--;
				tempIt.node->prev = it.node->prev;
				delete it.node;
				it.node = tempIt.node->prev;
				it.node->next = tempIt.node;
			}
			else {
				iterator tempIt(it);
				tempIt--;
				tempIt.node->next = it.node->next;
				delete it.node;
				it.node = tempIt.node->next;
				it.node->prev = tempIt.node;
			}
			head = end->next;
			tail = end->prev;
			size--;
		}

		void remove(T value) {
			for (iterator iter = beginIterator(); iter != endIterator();) {
				if (*iter == value) {
					iterator tempIter = iter;
					iter++;
					tempIter.node->prev->next = iter.node;
					iter.node->prev = tempIter.node->prev;
					delete tempIter.node;
					size--;
				}
				else {
					iter++;
				}
			}
			head = end->next;
			tail = end->prev;
		}

		void swap(List& other) {
			List<T> temp;
			temp.head = other.head;
			temp.tail = other.tail;
			temp.end = other.end;
			temp.size = other.size;

			other.head = head;
			other.tail = tail;
			other.end = end;
			other.size = size;

			head = temp.head;
			tail = temp.tail;
			end = temp.end;
			size = temp.size;
		}

		void reverse() {
			for (iterator iter = beginIterator(); iter != endIterator();) {
				iterator tempIter(iter);
				iter++;
				Node* n = tempIter.node->prev;
				tempIter.node->prev = tempIter.node->next;
				tempIter.node->next = n;
			}
			Node* h = head;
			head = tail;
			tail = h;
			end->prev = tail;
			end->next = head;
		}

		void splice(iterator& it, List& lt) {
			if (it.isReverse) return;
			if (lt.size == 0) return;
			if (size == 0) {
				List<T> tempList(lt);
				head = tempList.head;
				tail = tempList.tail;
				delete end;
				end = tempList.end;
			}
			else if (it.node == head) {
				List<T> tempList(lt);
				tempList.head->prev = end;
				end->next = tempList.head;
				tempList.tail->next = head;
				head->prev = tempList.tail;
				head = tempList.head;
				delete tempList.end;
			}
			else if (it.node == end) {
				List<T> tempList(lt);
				tail->next = tempList.head;
				tempList.head->prev = tail;
				tempList.tail->next = end;
				end->prev = tempList.tail;
				tail = tempList.tail;
				it.node = tempList.head;
				delete tempList.end;
			}
			else {
				List<T> tempList(lt);
				iterator tempIt(it);
				tempIt--;
				tempIt.node->next = tempList.head;
				tempList.head->prev = tempIt.node;
				tempList.tail->next = it.node;
				it.node->prev = tempList.tail;
				it.node = tempList.head;
				delete tempList.end;
			}
			size += lt.size;
		}

		void unique() {
			if (size == 0) return;
			T value = head->data;;
			for (iterator iter = beginIterator(); iter != endIterator();) {
				iter++;
				if (*iter == value) {
					iterator tempIter(iter);
					iter++;
					tempIter.node->prev->next = iter.node;
					iter.node->prev = tempIter.node->prev;
					delete tempIter.node;
					iter--;
					size--;
				}
				else {
					value = *iter;
				}
			}
		}
		private:
		void swapNode(iterator& it1, iterator& it2) {
			Node* temp = it1.node->prev;
			it1.node->prev = it2.node->prev;
			it2.node->prev->next = it1.node;
			it2.node->prev = temp;
			temp->next = it2.node;

			temp = it1.node->next;
			it1.node->next = it2.node->next;
			it2.node->next->prev = it1.node;
			it2.node->next = temp;
			temp->prev = it2.node;

			iterator tempIt(it1);
			it1.node = it2.node;
			it2.node = tempIt.node;
		}
		public:
		void sort() {
			if (size <= 1) return;
			int n = size - 1;

			iterator it1;
			iterator it2;
			while (n > 0) {
				it1.node = end->next;
				it2.node = end->next;
				it2++;
				for (int i = 0; i < n; i++) {
					if (*it1 > *it2) {
						swapNode(it1, it2);
					}
					it1++;
					it2++;
				}
				head = end->next;
				tail = end->prev;
				n--;
			}
		}

		void sort(int (*func)(T, T)) {
			if (size <= 1) return;
			int n = size - 1;

			iterator it1;
			iterator it2;
			while (n > 0) {
				it1.node = end->next;
				it2.node = end->next;
				it2++;
				for (int i = 0; i < n; i++) {
					if (func(*it1, *it2) > 0) {
						swapNode(it1, it2);
					}
					it1++;
					it2++;
				}
				head = end->next;
				tail = end->prev;
				n--;
			}
		}

		void merge(List lt) {
			splice(beginIterator(), lt);
			sort();
		}

		void merge(List lt, int (*func)(T, T)) {
			splice(beginIterator(), lt);
			sort(func);
		}

		void remove_if(bool (*func)(T)) {
			if (size == 0) return;
			iterator it = beginIterator();
			for (; it != endIterator();) {
				it++;
				if (func(*it)) {
					iterator tempIt(it);
					it++;
					tempIt.node->prev->next = it.node;
					it.node->prev = tempIt.node->prev;
					delete tempIt.node;
					it--;
					size--;
				}
			}
		}
	};
}

int testFunc(int a, int b) {
	if (a < b) {
		return 1;
	}
	else if (a > b) {
		return -1;
	}
	else {
		return 0;
	}
}

bool testPre(int value) {
	if (value > 8) {
		return true;
	}
	else {
		return false;
	}
}

using namespace std;
using namespace Test;
void main() {
	cout << "============== main() start ==============" << endl;
	cout << endl;
	List<int> lt1;
	lt1.assign(5, 10);
	List<int> lt2 = lt1;
	List<int> lt3(10, 2);

	if (lt1.empty()) {
		cout << "비었습니다." << endl;
	}
	else {
		cout << "원소 추가 전 lt1 size:" << lt1.getSize() << endl;
	}

	lt1.push_back(6);
	lt1.push_back(7);
	lt1.push_front(8);
	lt1.push_front(9);

	cout << "원소 추가 후 lt1 size:" << lt1.getSize() << endl;
	cout << endl;
	cout << "원소 제거 전 lt3 size:" << lt3.getSize() << endl;

	lt3.pop_back();
	lt3.pop_back();
	lt3.pop_front();
	lt3.pop_front();

	cout << "원소 제거 후 lt3 size:" << lt3.getSize() << endl;
	cout << endl;

	auto iter = lt1.beginIterator();
	iter++;
	lt1.insert(iter, 300);

	cout << "추가된 원소 :" << *iter << endl;
	iter++;
	lt1.erase(iter);
	lt2.swap(lt3);

	cout << "swap 후 lt2 size:" << lt2.getSize() << endl;
	cout << "swap 후 lt3 size:" << lt3.getSize() << endl;

	cout << endl;
	cout << "---lt1의 원소들 ---" << endl;
	for (auto i = lt1.beginIterator(); i != lt1.endIterator(); i++) {
		cout << *i << " ";
	}
	cout << endl;
	cout << endl;
	lt1.reverse();
	cout << "---뒤집힌 lt1의 원소들 ---" << endl;
	for (auto i = lt1.beginIterator(); i != lt1.endIterator(); i++) {
		cout << *i << " ";
	}
	cout << endl;
	cout << endl;

	lt1.splice(lt1.beginIterator(), lt2);
	lt1.unique();
	cout << "---병합 후 unique처리를 한 lt1의 원소들 ---" << endl;
	for (auto i = lt1.beginIterator(); i != lt1.endIterator(); i++) {
		cout << *i << " ";
	}
	cout << endl;
	cout << endl;

	lt1.sort(testFunc);
	cout << "---내림차순 정렬한 lt1의 원소들 ---" << endl;
	for (auto i = lt1.beginIterator(); i != lt1.endIterator(); i++) {
		cout << *i << " ";
	}
	cout << endl;
	cout << endl;

	lt1.merge(lt3);
	cout << "---병합 후 오름차순 정렬한 lt1의 원소들 ---" << endl;
	for (auto i = lt1.beginIterator(); i != lt1.endIterator(); i++) {
		cout << *i << " ";
	}
	cout << endl;
	cout << endl;

	lt1.remove(10);
	cout << "---10을 삭제한 lt1의 원소들 ---" << endl;
	for (auto i = lt1.beginIterator(); i != lt1.endIterator(); i++) {
		cout << *i << " ";
	}
	cout << endl;
	cout << endl;

	lt1.remove_if(testPre);
	cout << "---8보다 큰 수를 삭제한 lt1의 원소들 ---" << endl;
	for (auto i = lt1.beginIterator(); i != lt1.endIterator(); i++) {
		cout << *i << " ";
	}
	cout << endl;
	cout << endl;

	cout << "============== main() end ==============" << endl;
}