#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <random>
#include <cmath>
#include <memory>
#include <vector>

template <typename T>
struct Node
{
    T data;
    int key;
    std::unique_ptr<Node<T>> next;

    Node(int key, T data) : data(data), key(key), next(nullptr) {}
};

template <typename T>
class hash_table
{
private:
    std::vector<std::unique_ptr<Node<T>>> bucket;
    int _size;

    int hash_func(int key) const
    {
        return std::abs(key % _size);
    }

public:
    explicit hash_table(int size) : bucket(size), _size(size) {}

    hash_table(int size, int min, int max) : bucket(size), _size(size)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> val_dist(min, max);
        std::uniform_int_distribution<int> key_dist(0, 1000000);

        for (int i = 0; i < _size; ++i)
        {
            int key = key_dist(gen);
            T value = static_cast<T>(val_dist(gen));
            insert(key, value);
        }
    }

    hash_table(const hash_table<T> &other) : bucket(other._size), _size(other._size)
    {
        for (int i = 0; i < _size; ++i)
        {
            Node<T> *curr = other.bucket[i].get();
            while (curr)
            {
                insert(curr->key, curr->data);
                curr = curr->next.get();
            }
        }
    }

    hash_table<T> &operator=(const hash_table<T> &other)
    {
        if (this == &other)
            return *this;

        _size = other._size;
        bucket.assign(_size, nullptr);

        for (int i = 0; i < _size; ++i)
        {
            Node<T> *curr = other.bucket[i].get();
            while (curr)
            {
                insert(curr->key, curr->data);
                curr = curr->next.get();
            }
        }
        return *this;
    }

    bool insert(int key, const T &value)
    {
        int index = hash_func(key);
        Node<T> *curr = bucket[index].get();
        while (curr)
        {
            if (curr->key == key)
                return false;
            curr = curr->next.get();
        }
        auto newNode = std::make_unique<Node<T>>(key, value);
        newNode->next = std::move(bucket[index]);
        bucket[index] = std::move(newNode);
        return true;
    }

    void insert_or_assign(int key, const T &value)
    {
        int index = hash_func(key);
        Node<T> *curr = bucket[index].get();
        while (curr)
        {
            if (curr->key == key)
            {
                curr->data = value;
                return;
            }
            curr = curr->next.get();
        }
        auto newNode = std::make_unique<Node<T>>(key, value);
        newNode->next = std::move(bucket[index]);
        bucket[index] = std::move(newNode);
    }

    void print() const
    {
        for (int i = 0; i < _size; ++i)
        {
            Node<T> *curr = bucket[i].get();
            if (curr)
            {
                std::cout << "Bucket: " << i << std::endl;
                while (curr)
                {
                    std::cout << "  key: " << curr->key << "    data: " << curr->data << std::endl;
                    curr = curr->next.get();
                }
            }
        }
    }

    bool contains(const T &value) const
    {
        for (int i = 0; i < _size; ++i)
        {
            Node<T> *curr = bucket[i].get();
            while (curr)
            {
                if (curr->data == value)
                    return true;
                curr = curr->next.get();
            }
        }
        return false;
    }

    T *search(int key) const
    {
        int index = hash_func(key);
        Node<T> *curr = bucket[index].get();
        while (curr)
        {
            if (curr->key == key)
                return &(curr->data);
            curr = curr->next.get();
        }
        return nullptr;
    }

    bool erase(int key)
    {
        int index = hash_func(key);
        Node<T> *curr = bucket[index].get();
        Node<T> *prev = nullptr;

        while (curr)
        {
            if (curr->key == key)
            {
                if (prev)
                {
                    prev->next = std::move(curr->next);
                }
                else
                {
                    bucket[index] = std::move(curr->next);
                }
                return true;
            }
            prev = curr;
            curr = curr->next.get();
        }
        return false;
    }

    int count(int index_or_key, bool is_index = true) const
    {
        int idx = is_index ? index_or_key : hash_func(index_or_key);
        if (idx < 0 || idx >= _size)
            return 0;

        Node<T> *curr = bucket[idx].get();
        int counter = 0;
        while (curr)
        {
            ++counter;
            curr = curr->next.get();
        }
        return counter;
    }
    std::vector<std::pair<int, T>> items() const
    {
        std::vector<std::pair<int, T>> out;
        for (int i = 0; i < _size; ++i)
        {
            Node<T> *curr = bucket[i].get();
            while (curr)
            {
                out.push_back({curr->key, curr->data});
                curr = curr->next.get();
            }
        }
        return out;
    }

    int get_size() const { return _size; }
};

template <typename T>
void random_input(int num, hash_table<T> &hash)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 10000);
    for (int i = 0; i < num; ++i)
    {
        int key = dist(gen);
        hash.insert(key, static_cast<T>(i));
    }
}

template <typename T>
int count_collisions(hash_table<T> &hash)
{
    int collisions = 0;
    for (int i = 0; i < hash.get_size(); ++i)
    {
        int c = hash.count(i, true);
        if (c > 1)
            collisions += (c - 1);
    }
    return collisions;
}

#endif
