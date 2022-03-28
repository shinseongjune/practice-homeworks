#include <iostream>
#include <vector>

using namespace std;

/// <summary>
/// 질문1. initializer_list를 이용한 생성자
/// 질문2. T가 int가 아닐 경우 int 2개를 매개변수로 가지는 생성자(std::vector는 T의 생성자에 2번째 int를 매개변수로 넣음)
/// </summary>

namespace Test {
	template <typename T>
	class Vector {
		T* arr = NULL;
		int size = 0;
		int capacity = 0;
		
	public:
		Vector() { }
		Vector(int _size, T _value = T()) {
			size = _size;
			capacity = _size;
			arr = new T[capacity];
			for (int i = 0; i < size; i++) {
				arr[i] = _value;
			}
		}
		Vector(Vector& v) {
			size = v.size;
			capacity = v.capacity;
			arr = new T[v.capacity];
			for (int i = 0; i < v.capacity; i++) {
				arr[i] = v[i];
			}
		}

		~Vector() {
			if (arr != NULL) delete[] arr;
		}

		T operator[](int i) {
			return arr[i];
		}

		Vector& operator=(Vector& v) {
			if (arr != NULL) delete[] arr;
			size = v.size;
			capacity = v.capacity;
			arr = new T[v.capacity];
			for (int i = 0; i < v.capacity; i++) {
				arr[i] = v[i];
			}
			return *this;
		}

		bool operator==(Vector& v) {
			if (size != v.size) return false;
			for (int i = 0; i < size; i++) {
				if (arr[i] != v[i]) return false;
			}
			return true;
		}

		bool operator!=(Vector& v) {
			return !(this == v);
		}

		bool operator<(Vector& v) {
			bool isThisShortOrEqual = size <= v.size;
			int shortest = isThisShortOrEqual ? size : v.size;
			for (int i = 0; i < shortest; i++) {
				if (arr[i] > v[i]) return false;
				if (arr[i] < v[i]) return true;
			}
			if (!isThisShortOrEqual) {
				return false;
			}
			return true;
		}

		bool operator<=(Vector& v) {
			return this == v || this < v;
		}
		
		bool operator>(Vector& v) {
			return !(this < v);
		}

		bool operator>=(Vector& v) {
			return this == v || !(this < v);
		}

		void assign(int count, T value) {
			if (arr != NULL) delete[] arr;
			size = count;
			capacity = count;
			arr = new T[capacity];
			for (int i = 0; i < capacity; i++) {
				arr[i] = value;
			}
		}

		T at(int i) {
			try {
				if (size == 0 || i < 0 || i >= size) {
					throw i;
				}
				return arr[i];
			}
			catch (int i) {
				cout << "Array Out of Bounds Exception!! :" << i << endl;
				return NULL;
			}
		}

		T front() {
			if (size != 0) return arr[0];
		}

		T back() {
			if (size != 0) return arr[size - 1];
		}

		void push_back(const T value) {
			if (++size > capacity) {
				capacity *= 2;
				T* temp = new T[capacity];
				for (int i = 0; i < size - 1; i++) {
					temp[i] = arr[i];
				}
				if (arr != NULL) delete[] arr;
				arr = temp;
			}
			arr[size - 1] = value;
		}

		void pop_back() {
			if (size > 0) size--;
			if (size == 0) {
				if (arr != NULL) {
					delete[] arr;
				}
				if (capacity != 0) arr = new T[capacity];
			}
			else {
				if (capacity != 0) {
					T* temp = new T[capacity];
					for (int i = 0; i < capacity; i++) {
						temp[i] = arr[i];
					}
					if (arr != NULL) delete[] arr;
					arr = temp;
				}
			}
		}

		void clear() {
			size = 0;
			capacity = 0;
			if (arr != NULL) delete[] arr;
		}

		bool empty() {
			if (size == 0) return true;
			return false;
		}

		int getSize() {
			return size;
		}

		int getCapacity() {
			return capacity;
		}

		void shrink_to_fit() {
			if (size == capacity) return;
			if (size != 0) {
				T* temp = new T[size];
				for (int i = 0; i < size; i++) {
					temp[i] = arr[i];
				}
				if (arr != NULL) delete[] arr;
				arr = temp;
			}
			capacity = size;
		}

		//max_size(), reserve(int i), begin(), end(), rbegin(), rend(),
		//insert(iterator it, value), emplace(iterator it, value), emplace_back(),
		//erase(iterator it), erase(iterator start, iterator end), resize(int i), swap(Vector& v)
	};
}