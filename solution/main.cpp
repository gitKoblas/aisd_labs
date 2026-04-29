#include <iostream>
#include <random>
#include <cmath>


using namespace std;

template<typename T>
struct Node {
	T data;
    Node* next;
    int key;
    Node(int key, T data): data(data), key(key), next(nullptr){}
};


template<typename T>
class hash_table {
private:
	Node<T>** bucket;
	int _size;
	int hash_func(int key) {
		return abs(key % _size);
	}
public:
    hash_table(int size): _size(size){
        bucket = new Node<T>* [_size];
        for (int i = 0; i < _size; ++i) {
            bucket[i] = nullptr;
        }
    }
    hash_table(int size, int min, int max) : _size(size) {
        bucket = new Node<T>*[_size](); 
        mt19937 gen(rand()); 
        uniform_int_distribution<int> val_dist(min, max);
        uniform_int_distribution<int> key_dist(0, 1000000);
        for (int i = 0; i < _size; ++i) {
            int key = key_dist(gen);
            T value = static_cast<T>(val_dist(gen));
            insert(key, value);
        }
    }
    hash_table(const hash_table<T> &other) : _size(other._size) {
        bucket = new Node<T>*[_size]();
        for (int i = 0; i < _size; ++i) {
            Node<T>* curr = other.bucket[i];
            while (curr) {
                insert(curr->key, curr->data);
                curr = curr->next;
            }
        }
    }
    ~hash_table(){
        for(int i=0;i<_size;++i){
            Node<T> *curr = bucket[i];
            while(curr){
                Node<T>* next = curr->next;
                delete curr;
                curr = next;
            }
        }
        delete[] bucket;
    }
    bool insert(int key, const T& value) {
        int index = hash_func(key);
        Node<T>* curr = bucket[index];
        while (curr) {
            if (curr->key == key) {
                return false;
            }
            curr = curr->next;
        }
        Node<T>* newNode = new Node<T>(key, value);
        newNode->next = bucket[index];
        bucket[index] = newNode;
        return true;
    }
    void insert_or_assign(int key, const T& value) {
        int index = hash_func(key);
        Node<T>* curr = bucket[index];
        while (curr) {
            if (curr->key == key) {
                curr->data = value;
                return;
            }
            curr = curr->next;
        }
        Node<T>* newNode = new Node<T>(key, value);
        newNode->next = bucket[index];
        bucket[index] = newNode;
        return;
    }
    hash_table<T> &operator=(const hash_table<T> &other){
        if (this == &other){
            return *this;
        }
        for(int i=0;i<_size;++i){
            Node<T> *curr = bucket[i];
            while(curr){
                Node<T>* next = curr->next;
                delete curr;
                curr = next;
            }
        }
        delete[] bucket;
        _size = other._size;
        bucket = new Node<T>*[_size]();
        for (int i = 0; i < _size; ++i) {
            Node<T>* curr = other.bucket[i];
            while (curr) {
                insert(curr->key, curr->data);
                curr = curr->next;
            }
        }
        return *this;
    }
    void print(){
        for(int i=0;i<_size;++i){
            Node<T> *curr = bucket[i];
            if (curr) {
                cout << "Bucket: " << i << endl;
                while (curr) {
                    cout << "  key: " << curr->key << "    data: " << curr->data << endl;
                    curr = curr->next;
                }
            }
        }
    }
    bool contains(T &value){
        for(int i=0;i<_size;++i){
            Node<T> *curr = bucket[i];
            while(curr){
                if(curr->data == value){
                    return true;
                }
                curr = curr->next;
            }
        }
        return false;
    }
    T* search(int key){
        int index = hash_func(key);
        Node<T>* curr = bucket[index];
        while(curr){
            if(curr -> key == key){
                return &(curr->data);
            }
            curr = curr -> next;
        }
        return nullptr;
    }
    bool erase(int key) {
        int index = hash_func(key);
        Node<T>* curr = bucket[index];
        Node<T>* prev = nullptr;
        while (curr) {
            if (curr->key == key) {
                if (prev) {
                    prev->next = curr->next;
                } else {
                    bucket[index] = curr->next;
                }
                delete curr;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }
    int count(int key){
        int index = hash_func(key);
        Node<T> *curr = bucket[index];
        int counter = 0;
        while(curr){
            ++counter;
            curr = curr -> next;
        }
        return counter;
    }
};

int main(){
    
}