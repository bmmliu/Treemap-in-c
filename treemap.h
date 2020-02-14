#ifndef TREEMAP_H_
#define TREEMAP_H_

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <utility>


template <typename K, typename V>
class Treemap {
public:
    //
    // @@@ The class's public API below should _not_ be modified @@@
    //

    // * Capacity
    // Returns number of key-value mappings in map --O(1)
    size_t Size();
    // Returns true if map is empty --O(1)
    bool Empty();

    // * Modifiers
    // Insert @key in map --O(log N) on average
    void Insert(const K &key, const V &value);
    // Remove @key from map --O(log N) on average
    void Remove(const K &key);

    // * Lookup
    // Return value corresponding to @key --O(log N) on average
    const V& Get(const K &key);

    // Return greatest key less than or equal to @key --O(log N) on average
    const K& FloorKey(const K &key);
    // Return least key greater than or equal to @key --O(log N) on average
    const K& CeilKey(const K &key);

    // Return whether @key is found in map --O(log N) on average
    bool ContainsKey(const K& key);
    // Return whether @value is found in map --O(N)
    bool ContainsValue(const V& value);

    // Return max key in map --O(log N) on average
    const K& MaxKey();
    // Return min key in map --O(log N) on average
    const K& MinKey();

private:
 struct Node{
   K key;
   V value;
   std::unique_ptr<Node> left;
   std::unique_ptr<Node> right;
 };
  size_t cur_size = 0;
 std::unique_ptr<Node> root;
 Node* MinKey(Node *n);
 void Insert(std::unique_ptr<Node> &n, const K &key, const V &value);
 void Remove(std::unique_ptr<Node> &n, const K &key);
 void ContainsValue(Node *n, const V &value, unsigned int &y_n);
 const K& FloorKey(Node *n, const K &key);
 const K& CeilKey(Node *n, const K &key);
    // @@@ The class's internal members below can be modified @@@
    //

    // Private member variables
    // ...To be completed...

    // Private constants
    // ...To be completed (if any)...

    // Private methods
    // ...To be completed (if any)...
};


//
// Your implementation of the class should be located below
//

// ...To be completed...

template <typename K, typename V>
size_t Treemap<K, V>::Size() {
  return cur_size;
}

template <typename K, typename V>
bool Treemap<K, V>::Empty() {
  if(cur_size == 0){
    return true;
  } else{
    return false;
  }
}

template <typename K, typename V>
void Treemap<K, V>::Insert(const K &key, const V &value) {
  Insert(root, key, value);
  cur_size++;
}

template <typename K, typename V>
void Treemap<K, V>::Insert(std::unique_ptr<Node> &n, const K &key, const V &value) {
  if (!n)
    n = std::unique_ptr<Node>(new Node{key, value});
  else if (key < n->key)
    Insert(n->left, key, value);
  else if (key > n->key)
    Insert(n->right, key, value);
  else
    throw std::range_error("Duplicate key");
}

template <typename K, typename V>
void Treemap<K, V>::Remove(const K &key) {
  if(cur_size == 0){
    throw std::invalid_argument("Empty tree");
  }
  Remove(root, key);
  cur_size--;
}

template <typename K, typename V>
void Treemap<K, V>::Remove(std::unique_ptr<Node> &n, const K &key) {
  // Key not found
  if (!n) return;

  if (key < n->key) {
    Remove(n->left, key);
  } else if (key > n->key) {
    Remove(n->right, key);
  } else {
    // Found node
    if (n->left && n->right) {
      // Two children: replace with min node in right subtree
      n->key = MinKey(n->right.get())->key;
      n->value = Get(n->key);
      Remove(n->right, n->key);
    } else {
      // Replace with only child or with nullptr
      n = std::move((n->left) ? n->left : n->right);
    }
  }
}

template <typename K, typename V>
const V& Treemap<K, V>::Get(const K &key) {
  Node *n = root.get();
  while (n) {
    if (key == n->key)
      return n->value;

    if (key < n->key)
      n = n->left.get();
    else
      n = n->right.get();
  }
  throw std::underflow_error("Invalid key");
}

template <typename K, typename V>
const K& Treemap<K, V>::FloorKey(const K &key) {
  if(key < MinKey()){
    throw std::underflow_error("Invalid key");
  }
  return FloorKey(root.get(), key);
}

template <typename K, typename V>
const K& Treemap<K, V>::FloorKey(Node *n, const K &key){
  if(Empty()){
    throw std::invalid_argument("Empty tree");
  }
  static K floor;
  static K y_n = -1;
  if(!n){
    return y_n;
  }


  if(n->key == key){
    floor = n->key;
  } else if(n->key < key){
    floor = FloorKey(n->right.get(), key);
    if(floor == y_n){
      floor = n->key;
    }
  } else{
    floor = FloorKey(n->left.get(), key);
  }

  return floor;
}


template <typename K, typename V>
const K&  Treemap<K, V>::CeilKey(const K &key){
  if(key > MaxKey()){
    throw std::underflow_error("Invalid key");
  }
  return CeilKey(root.get(), key);
}

template <typename K, typename V>
const K&  Treemap<K, V>::CeilKey(Node *n, const K &key){

  static K ceil;
  static K y_n = -1;

  if(!n){
    return y_n;
  }


  if(n->key == key){
    ceil = n->key;
  } else if(n->key < key){
    ceil = CeilKey(n->right.get(), key);
  } else{
    ceil = CeilKey(n->left.get(), key);
    if(ceil == y_n){
      ceil = n->key;
    }
  }
  return ceil;

}

template <typename K, typename V>
bool Treemap<K, V>::ContainsKey(const K& key){
  Node *n = root.get();
  if(Empty()){
    throw std::invalid_argument("Empty tree");
  }
  while (n) {
    if (key == n->key)
      return true;

    if (key < n->key)
      n = n->left.get();
    else
      n = n->right.get();
  }

  return false;

}


template <typename K, typename V>
bool Treemap<K, V>::ContainsValue(const V& value){
  if(Empty()){
    throw std::invalid_argument("Empty tree");
  }
  unsigned int y_n = 0;
  ContainsValue(root.get(), value, y_n);
  if (y_n > 0){
    return true;
  } else {
    return false;
  }
}

template <typename K, typename V>
void Treemap<K, V>::ContainsValue(Node *n, const V& value, unsigned int& y_n){
  if(!n) return;

  if(n->value == value) {
    y_n++;
  }
  ContainsValue(n->left.get(), value, y_n);
  ContainsValue(n->right.get(), value, y_n);
}

template <typename K, typename V>
const K& Treemap<K, V>::MaxKey(){
  if(Empty()){
    throw std::invalid_argument("Empty tree");
  }
  Node *n = root.get();
  while (n->right)
    n = n->right.get();
  return n->key;
}

template <typename K, typename V>
const K& Treemap<K, V>::MinKey(){
  if(Empty()){
    throw std::invalid_argument("Empty tree");
  }
  return MinKey(root.get())->key;
}

template <typename K, typename V>
typename Treemap<K, V>::Node* Treemap<K, V>::MinKey(Node *n) {
  if (n->left)
    return MinKey(n->left.get());
  else
    return n;
}

#endif  // TREEMAP_H_