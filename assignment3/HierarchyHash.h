#ifndef HIERARCHYHASH_H
#define HIERARCHYHASH_H

#include <iostream>
#include "FlatHash.h"

class HierarchyHash
{
private:
  unsigned int** hashtable;
  unsigned int const TOMBSTONE = 1234567;
  // Variable for overflow handling
  enum overflow_handle flag;
  // Loading factor
  float alpha;
  // Size(range) of full hash table. Initially 1000
  unsigned int table_size;
  // Size of subhash table. Fixed by 100
  unsigned int sub_table_size;
  // Nums of keys
  unsigned int num_of_keys;


public:
  HierarchyHash(enum overflow_handle _flag, float _alpha);

  ~HierarchyHash();

  unsigned int hashFunction(const unsigned int key) { return key % table_size; }

  unsigned int getTableSize() { return table_size; }

  unsigned int getNumofKeys() { return num_of_keys; }

  // Return the size of allocated sub hash table
  unsigned int getAllocatedSize();

  // Return time cost
  int insert(const unsigned int key);

  // Return time cost
  int remove(const unsigned int key);

  // Return time cost
  int search(const unsigned int key);

  // Delete tombstones
  void clearTombstones();

  void print();
};

HierarchyHash::HierarchyHash(enum overflow_handle _flag, float _alpha)
{
  // Initial map size is 1000
  table_size = 1000;
  // Table size is fixed to 100
  sub_table_size = 100;
  num_of_keys = 0;
  flag = _flag;
  alpha = _alpha;

  // Write your code
  hashtable = new unsigned int *[table_size / sub_table_size];
  for (size_t i = 0; i < table_size / sub_table_size; i++)
    hashtable[i] = NULL;
}

HierarchyHash::~HierarchyHash()
{
  // Write your code
  for (size_t i = 0; i < table_size / sub_table_size; i++)
    if (hashtable[i] != NULL)
      delete[] hashtable[i];
  delete[] hashtable;
}

unsigned int HierarchyHash::getAllocatedSize()
{
  // Write your code
  unsigned int sum = 0;
  for (size_t i = 0; i < table_size / sub_table_size; i++)
    if (hashtable[i] != NULL)
      sum += sub_table_size;
  return sum;
}

int HierarchyHash::insert(const unsigned int key)
{
  // Write your code
  int time_cost = 0;
  bool success = false;

  for (size_t j = 0; j < table_size; j++) {
    size_t index = (hashFunction(key) + (flag == LINEAR_PROBING ? j : j*j)) % table_size;
    time_cost++;

    if (hashtable[index / sub_table_size] == NULL) {
      hashtable[index / sub_table_size] = new unsigned int[sub_table_size];
      for (size_t i = 0; i < sub_table_size; i++)
        hashtable[index / sub_table_size][i] = 0;
      hashtable[index / sub_table_size][index % sub_table_size] = key;
      num_of_keys++;
      success = true;
      break;
    }

    unsigned int &cur = hashtable[index / sub_table_size][index % sub_table_size];

    if (cur == key) {
      time_cost = -time_cost;
      success = true;
      break;
    }
    if (cur == 0 || cur == TOMBSTONE) {
      cur = key;
      num_of_keys++;
      success = true;
      break;
    }
  }

  if (!success && flag == QUADRATIC_PROBING) {
    for (size_t j = 0; j < table_size; j++) {
      size_t index = (hashFunction(key) + j) % table_size;
      time_cost++;

      if (hashtable[index / sub_table_size] == NULL) {
        hashtable[index / sub_table_size] = new unsigned int[sub_table_size];
        for (size_t i = 0; i < sub_table_size; i++)
          hashtable[index / sub_table_size][i] = 0;
        hashtable[index / sub_table_size][index % sub_table_size] = key;
        num_of_keys++;
        success = true;
        break;
      }

      unsigned int &cur = hashtable[index / sub_table_size][index % sub_table_size];

      if (cur == key) {
        time_cost = -time_cost;
        success = true;
        break;
      }
      if (cur == 0 || cur == TOMBSTONE) {
        cur = key;
        num_of_keys++;
        success = true;
        break;
      }
    }
  }
  
  if (float(num_of_keys) / table_size >= alpha) {
    // resize
    unsigned int **new_hashtable = new unsigned int *[table_size / sub_table_size * 2];
    for (size_t i = 0; i < table_size / sub_table_size * 2; i++)
      new_hashtable[i] = NULL;

    table_size += table_size;

    for (size_t i = 0; i < table_size / 2; i++) {
      if (hashtable[i / sub_table_size] == NULL)
        continue;
      unsigned int &cur = hashtable[i / sub_table_size][i % sub_table_size];
      if (cur != 0 && cur != TOMBSTONE) {
        const unsigned int key = cur;
        bool success = false;

        for (size_t j = 0; j < table_size; j++) {
          size_t index = (hashFunction(key) + (flag == LINEAR_PROBING ? j : j*j)) % table_size;

          if (new_hashtable[index / sub_table_size] == NULL) {
            new_hashtable[index / sub_table_size] = new unsigned int[sub_table_size];
            for (size_t i = 0; i < sub_table_size; i++)
              new_hashtable[index / sub_table_size][i] = 0;
          }

          unsigned int &load = new_hashtable[index / sub_table_size][index % sub_table_size];
          
          if (load == 0 || load == TOMBSTONE) {
            load = key;
            success = true;
            break;
          }
        }

        if (!success && flag == QUADRATIC_PROBING) {
          for (size_t j = 0; j < table_size; j++) {
            size_t index = (hashFunction(key) + j) % table_size;

            if (new_hashtable[index / sub_table_size] == NULL) {
              new_hashtable[index / sub_table_size] = new unsigned int[sub_table_size];
              for (size_t i = 0; i < sub_table_size; i++)
                new_hashtable[index / sub_table_size][i] = 0;
            }

            unsigned int &load = new_hashtable[index / sub_table_size][index % sub_table_size];

            if (load == 0 || load == TOMBSTONE) {
              load = key;
              success = true;
              break;
            }
          }
        }
      }
    }
    
    for (size_t i = 0; i < table_size / sub_table_size / 2; i++)
      if (hashtable[i] != NULL)
        delete[] hashtable[i];
    delete[] hashtable;
    hashtable = new_hashtable;
  }

  return time_cost;
}

int HierarchyHash::remove(const unsigned int key)
{
  // Write your code
  int time_cost = 0;
  if (flag == LINEAR_PROBING) {
    size_t index = table_size;
    for (size_t j = 0; j < table_size; j++) {
      size_t i = (hashFunction(key) + j) % table_size;
      time_cost++;

      if (hashtable[i / sub_table_size] == NULL)
        break;

      unsigned int &cur = hashtable[i / sub_table_size][i % sub_table_size];

      if (cur == key) {
        index = i;
        break;
      }
      if (cur == 0)
        break;
    }

    if (index != table_size) {
      unsigned int &cur = hashtable[index / sub_table_size][index % sub_table_size];
      cur = 0;
      num_of_keys--;
      // shift
      size_t next_index = (index + 1) % table_size;
      while (hashtable[next_index / sub_table_size] && hashtable[next_index / sub_table_size][next_index % sub_table_size]) {
        unsigned int &next = hashtable[next_index / sub_table_size][next_index % sub_table_size];
        if (hashFunction(next) == next_index)
          break;

        cur = next;
        next = 0;

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

      if (hashtable[i / sub_table_size] == NULL) {
        quit = true;
        break;
      }

      unsigned int &cur = hashtable[i / sub_table_size][i % sub_table_size];

      if (cur == key) {
        index = i;
        break;
      }
      if (cur == 0) {
        quit = true;
        break;
      }
    }
    if (!quit && index == table_size) {
      for (size_t j = 0; j < table_size; j++) {
        size_t i = (hashFunction(key) + j) % table_size;
        time_cost++;

        if (hashtable[i / sub_table_size] == NULL)
          break;

        unsigned int &cur = hashtable[i / sub_table_size][i % sub_table_size];

        if (cur == key) {
          index = i;
          break;
        }
        if (cur == 0)
          break;
      }
    }
    
    if (index != table_size) {
      num_of_keys--;
      hashtable[index / sub_table_size][index & sub_table_size] = TOMBSTONE;
    }
    else
      time_cost = -time_cost;
  }

  return time_cost;
}

int HierarchyHash::search(const unsigned int key)
{
  // Write your code
  int time_cost = 0;
  if (flag == LINEAR_PROBING) {
    size_t index = table_size;
    for (size_t j = 0; j < table_size; j++) {
      size_t i = (hashFunction(key) + j) % table_size;
      time_cost++;

      if (hashtable[i / sub_table_size] == NULL)
        break;

      unsigned int &cur = hashtable[i / sub_table_size][i % sub_table_size];

      if (cur == key) {
        index = i;
        break;
      }
      if (cur == 0)
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

      if (hashtable[i / sub_table_size] == NULL) {
        quit = true;
        break;
      }

      unsigned int &cur = hashtable[i / sub_table_size][i % sub_table_size];

      if (cur == key) {
        index = i;
        break;
      }
      if (cur == 0) {
        quit = true;
        break;
      }
    }
    if (!quit && index == table_size) {
      for (size_t j = 0; j < table_size; j++) {
        size_t i = (hashFunction(key) + j) % table_size;
        time_cost++;

        if (hashtable[i / sub_table_size] == NULL)
          break;

        unsigned int &cur = hashtable[i / sub_table_size][i % sub_table_size];

        if (cur == key) {
          index = i;
          break;
        }
        if (cur == 0)
          break;
      }
    }

    if (index == table_size)
      time_cost = -time_cost;
  }

  return time_cost;
}

void HierarchyHash::clearTombstones()
{
  // Write your code
  if (flag == LINEAR_PROBING)
    return;

  unsigned int **new_hashtable = new unsigned int *[table_size / sub_table_size];
  for (size_t i = 0; i < table_size / sub_table_size; i++)
    new_hashtable[i] = NULL;
  for (size_t i = 0; i < table_size; i++) {
    if (hashtable[i / sub_table_size])
      continue;
    unsigned int &cur = hashtable[i / sub_table_size][i % sub_table_size];
    if (cur != 0 && cur != TOMBSTONE) {
      const unsigned int key = cur;
      bool success = false;

      for (size_t j = 0; j < table_size; j++) {
        size_t index = (hashFunction(key) + (flag == LINEAR_PROBING ? j : j*j)) % table_size;

        if (new_hashtable[index / sub_table_size]) {
          new_hashtable[index / sub_table_size] = new unsigned int[sub_table_size];
          for (size_t i = 0; i < sub_table_size; i++)
            new_hashtable[index / sub_table_size][i] = 0;
        }

        unsigned int &load = new_hashtable[index / sub_table_size][index % sub_table_size];
        
        if (load == 0 || load == TOMBSTONE) {
          load = key;
          success = true;
          break;
        }
      }

      if (!success && flag == QUADRATIC_PROBING) {
        for (size_t j = 0; j < table_size; j++) {
          size_t index = (hashFunction(key) + j) % table_size;

          if (new_hashtable[index / sub_table_size]) {
            new_hashtable[index / sub_table_size] = new unsigned int[sub_table_size];
            for (size_t i = 0; i < sub_table_size; i++)
              new_hashtable[index / sub_table_size][i] = 0;
          }

          unsigned int &load = new_hashtable[index / sub_table_size][index % sub_table_size];

          if (load == 0 || load == TOMBSTONE) {
            load = key;
            success = true;
            break;
          }
        }
      }
    }
  }
  
  for (size_t i = 0; i < table_size / sub_table_size; i++)
    if (hashtable[i] != NULL)
      delete[] hashtable[i];
  delete[] hashtable;
  hashtable = new_hashtable;
}

void HierarchyHash::print()
{
  // Print valid key pair for each sub hash table - subtable_id:(index1:key1,index2:key2)
  // Seperate each sub table by endl
  // Give **NO** space between each character
  // e.g., 0:(1:3,3:7,5:1)
  //       1:(101:2,192:10)
  //       9:(902:90,938:82)

  // Exceptionally, keep this code only for the case there is no key in the table
  if( getNumofKeys() == 0){
    std::cout << "()" << std::endl;
    return;
  }

  // Write your code
  for (size_t i = 0; i < table_size / sub_table_size; i++) {
    if (hashtable[i] != NULL) {
      unsigned int sum = 0;
      for (size_t j = 0; j < sub_table_size; j++)
        if (hashtable[i][j] != 0 && hashtable[i][j] != TOMBSTONE)
          sum++;
      if (sum) {
        std::cout << i << ":(";
        bool start = true;
        for (size_t j = 0; j < sub_table_size; j++) {
          if (hashtable[i][j] != 0 && hashtable[i][j] != TOMBSTONE) {
            if (!start)
              std::cout << ',';
            start = false;
            std::cout << i * sub_table_size + j << ':' << hashtable[i][j];
          }
        }
        std::cout << ")" << std::endl;
      }
    }
  }
}

#endif
