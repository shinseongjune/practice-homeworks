#include <iostream>

/// <summary>
/// operator[]
/// begin()
/// end()
/// front()
/// back()
/// push_back(T&)
/// pop_back()
/// resize(int i)
/// swap(vector& v)
/// capacity()
/// size()
/// empty()
/// clear()
/// </summary>

namespace newVec {
	template <typename T>
	class vector {
	private:
		T[] arr;
		int size;
		int capacity;
	public:
		vector() {
			arr = new T[0];
			size = 0;
			capacity = 0;
		}

		vector(int capacity) {
			arr = new T[capacity];
			size = 0;
			this->capacity = capacity;
		}

		vector(const vector& v) {
			arr = new T[v.capacity];
			size = v.size;
			capacity = v.capacity;

			for (int i = 0; i < size; i++) {
				arr[i] = v[i];
			}
		}

		~vector() {
			delete[] arr;
		}

		vector& operator=(const vector& v) {
			arr = v.arr;
			size = v.size;
			capacity = v.capacity;
			return *this;
		}

		bool operator==(const vector& v) {
			if (size != v.size) return false;
			for (int i = 0; i < size; i++) {
				if (arr[i] != v[i]) return false;
			}
			return true;
		}

		bool operator!=(const vector& v) {
			return !(*this == v);
		}

		T& operator[](int i) {
			return arr[i];
		}

		T* begin() {
			return arr;
		}

		T* end() {
			return arr + size;
		}

		T& front() {
			return arr[0];
		}

		T& back() {
			return arr[size - 1];
		}

		void push_back(T t) {
			if (size == capacity) {
				capacity *= 1.5;
			}
			T* temp = new T[capacity];
			for (int i = 0; i < size; i++) {
				temp[i] = arr[i];
			}
			delete[] arr;
			arr = temp;
			arr[size++] = t;
		}

		void pop_back() {
			if (size == 0) return;
			size -= 1;
		}

		void resize(int size, int num) {
			T* temp = new T[size];
			temp->size = size;
			if (this->size > size) {
				for (int i = 0; i < size; i++) {
					temp[i] = arr[i];
				}
			}
			else {
				for (int i = 0; i < this->size; i++) {
					temp[i] = arr[i];
				}
				for (int i = this->size; i < size; i++) {
					temp[i] = numl
				}
			}
			delete[] arr;
			arr = temp;
			this->size = size;
		}

		void swap(const vector& v) {
			vector* temp = new vector(v);
			v = *this;
			*this = *temp;
		}

		int capacity() {
			return capacity;
		}

		int size() {
			return size;
		}

		bool empty() {
			return size == 0;
		}

		void clear() {
			if (arr != NULL) delete[] arr;
			size = 0;
			capacity = 0;
		}
	};
}