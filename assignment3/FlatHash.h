#ifndef FLATHASH_H
#define FLATHASH_H

#include <iostream>

// Flag(hint) for overflow handling
enum overflow_handle {
  LINEAR_PROBING = 0,
  QUADRATIC_PROBING
};

class FlatHash
{
private:
  unsigned int* hashtable;
  unsigned int const TOMBSTONE = 1234567;
  // Variable for overflow handling
  enum overflow_handle flag;
  // Loading factor
  float alpha;
  // Size of hash table
  unsigned int table_size;
  // Nums of keys
  unsigned int num_of_keys;

public:
  FlatHash(enum overflow_handle _flag, float _alpha);

  ~FlatHash();

  unsigned int hashFunction(const unsigned int key) { return key % table_size; }

  unsigned int getTableSize() { return table_size; }

  unsigned int getNumofKeys() { return num_of_keys; }

  // Return time cost
  int insert(const unsigned int key);

  // Remove function in lecture. Return time cost
  int remove(const unsigned int key);

  // Return time cost
  int search(const unsigned int key);

  // Delete tombstones
  void clearTombstones();

  void print();

protected:
  unsigned int *getTable() { return hashtable; }
};

FlatHash::FlatHash(enum overflow_handle _flag, float _alpha)
{
  // Initial table size is 1000 
  table_size = 1000;
  num_of_keys = 0;
  flag = _flag;
  alpha = _alpha;
  
  // Write your code
  hashtable = new unsigned int[table_size];
  for (size_t i = 0; i < table_size; i++)
    hashtable[i] = 0;
}

FlatHash::~FlatHash()
{
  // Write your code
  delete[] hashtable;
}

int FlatHash::insert(const unsigned int key)
{
  // You have to implement two overflow handling by using flag
  // Write your code
  int time_cost = 0;
  bool success = false;

  for (size_t j = 0; j < table_size; j++) {
    size_t index = (hashFunction(key) + (flag == LINEAR_PROBING ? j : j*j)) % table_size;
    time_cost++;
    if (hashtable[index] == key) {
      time_cost = -time_cost;
      success = true;
      break;
    }
    if (hashtable[index] == 0 || hashtable[index] == TOMBSTONE) {
      hashtable[index] = key;
      num_of_keys++;
      success = true;
      break;
    }
  }

  if (!success && flag == QUADRATIC_PROBING) {
    for (size_t j = 0; j < table_size; j++) {
      size_t index = (hashFunction(key) + j) % table_size;
      time_cost++;
      if (hashtable[index] == key) {
        time_cost = -time_cost;
        success = true;
        break;
      }
      if (hashtable[index] == 0 || hashtable[index] == TOMBSTONE) {
        hashtable[index] = key;
        num_of_keys++;
        success = true;
        break;
      }
    }
  }

  if (float(num_of_keys) / table_size >= alpha) {
    // resize
    unsigned int *new_hashtable = new unsigned int[table_size * 2];
    for (size_t i = 0; i < table_size * 2; i++)
      new_hashtable[i] = 0;
    
    table_size += table_size;

    for (size_t i = 0; i < table_size / 2; i++) {
      if (hashtable[i] != 0 && hashtable[i] != TOMBSTONE) {
        const unsigned int key = hashtable[i];
        bool success = false;

        for (size_t j = 0; j < table_size; j++) {
          size_t index = (hashFunction(key) + (flag == LINEAR_PROBING ? j : j*j)) % table_size;

          if (new_hashtable[index] == 0 || new_hashtable[index] == TOMBSTONE) {
            new_hashtable[index] = key;
            success = true;
            break;
          }
        }

        if (!success && flag == QUADRATIC_PROBING) {
          for (size_t j = 0; j < table_size; j++) {
            size_t index = (hashFunction(key) + j) % table_size;

            if (new_hashtable[index] == 0 || new_hashtable[index] == TOMBSTONE) {
              new_hashtable[index] = key;
              success = true;
              break;
            }
          }
        }
      }
    }
    delete[] hashtable;
    hashtable = new_hashtable;
  }

  return time_cost;
}

int FlatHash::remove(const unsigned int key)
{
  // Write your code
  int time_cost = 0;
  if (flag == LINEAR_PROBING) {
    size_t index = table_size;
    for (size_t j = 0; j < table_size; j++) {
      size_t i = (hashFunction(key) + j) % table_size;
      time_cost++;
      if (hashtable[i] == key) {
        index = i;
        break;
      }
      if (hashtable[i] == 0)
        break;
    }
    if (index != table_size) {
      hashtable[index] = 0;
      num_of_keys--;
      // shift
      size_t next_index = (index + 1) % table_size;
      while (hashtable[next_index]) {
        if (hashFunction(hashtable[next_index]) == next_index)
          break;
        
        hashtable[index] = hashtable[next_index];
        hashtable[next_index] = 0;

        index = (index + 1) % table_size;
        next_index = (index + 1) % table_size;
      }
    }
    else
      time_cost = -time_cost;
  }
  else {
    size_t index = table_size;
    bool quit = false;
    for (size_t j = 0; j < table_size; j++) {
      size_t i = (hashFunction(key) + j*j) % table_size;
      time_cost++;
      if (hashtable[i] == key) {
        index = i;
        break;
      }
      if (hashtable[i] == 0) {
        quit = true;
        break;
      }
    }
    if (!quit && index == table_size) {
      for (size_t j = 0; j < table_size; j++) {
        size_t i = (hashFunction(key) + j) % table_size;
        time_cost++;
        if (hashtable[i] == key) {
          index = i;
          break;
        }
        if (hashtable[i] == 0)
          break;
      }
    }
    if (index != table_size) {
      num_of_keys--;
      hashtable[index] = TOMBSTONE;
    }
    else
      time_cost = -time_cost;
  }

  return time_cost;
}

int FlatHash::search(const unsigned int key)
{
  // Write your code
  int time_cost = 0;
  if (flag == LINEAR_PROBING) {
    size_t index = table_size;
    for (size_t j = 0; j < table_size; j++) {
      size_t i = (hashFunction(key) + j) % table_size;
      time_cost++;
      if (hashtable[i] == key) {
        index = i;
        break;
      }
      if (hashtable[i] == 0)
        break;
    }
    if (index == table_size)
      time_cost = -time_cost;
  }
  else {
    size_t index = table_size;
    bool quit = false;
    for (size_t j = 0; j < table_size; j++) {
      size_t i = (hashFunction(key) + j*j) % table_size;
      time_cost++;
      if (hashtable[i] == key) {
        index = i;
        break;
      }
      if (hashtable[i] == 0) {
        quit = true;
        break;
      }
    }
    if (!quit && index == table_size) {
      for (size_t j = 0; j < table_size; j++) {
        size_t i = (hashFunction(key) + j) % table_size;
        time_cost++;
        if (hashtable[i] == key) {
          index = i;
          break;
        }
        if (hashtable[i] == 0)
          break;
      }
    }
    if (index == table_size)
      time_cost = -time_cost;
  }

  return time_cost;

}

void FlatHash::clearTombstones()
{
  // Write your code
  if (flag == LINEAR_PROBING)
    return;

  unsigned int *new_hashtable = new unsigned int[table_size];
  for (size_t i = 0; i < table_size; i++)
    new_hashtable[i] = 0;
  for (size_t i = 0; i < table_size; i++) {
    if (hashtable[i] != 0 && hashtable[i] != TOMBSTONE) {
      const unsigned int key = hashtable[i];
      bool success = false;

      for (size_t j = 0; j < table_size; j++) {
        size_t index = (hashFunction(key) + (flag == LINEAR_PROBING ? j : j*j)) % table_size;
        
        if (new_hashtable[index] == 0 || new_hashtable[index] == TOMBSTONE) {
          new_hashtable[index] = key;
          success = true;
          break;
        }
      }

      if (!success && flag == QUADRATIC_PROBING) {
        for (size_t j = 0; j < table_size; j++) {
          size_t index = (hashFunction(key) + j) % table_size;

          if (new_hashtable[index] == 0 || new_hashtable[index] == TOMBSTONE) {
            new_hashtable[index] = key;
            success = true;
            break;
          }
        }
      }
    }
  }
  delete[] hashtable;
  hashtable = new_hashtable;
}

void FlatHash::print()
{
  // Print valid key pair - (index1:key1,index2:key2)
  // Give **NO** space between each character
  // e.g., (1:3,3:7,5:1)
  std::cout << "(";

  // Write your code
  bool start = true;
  for (size_t i = 0; i < table_size; i++) {
    if (hashtable[i] != 0 && hashtable[i] != TOMBSTONE) {
      if (!start)
        std::cout << ',';
      std::cout << i << ':' << hashtable[i];
      start = false;
    }
  }

  std::cout << ")" << std::endl;
}

#endif
