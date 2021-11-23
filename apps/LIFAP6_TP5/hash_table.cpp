#include "hash_table.h"

HashTable::HashTable(uint m, const hashfunc &h,const rehashfunc& rh)
    : max_size(m) , hash(h) , rehash(rh)
{
    values.resize(m);
    keys.resize(m,nullptr);
}

void HashTable::insert(key k , value v)
{
    uint h = compute_hash(k);
    uint nb_tries = 0;
    while (keys[h] != nullptr){
        if (*keys[h] == k)
            return;
        h = compute_rehash(h,nb_tries);
        nb_tries++;
    }
    keys[h] = new key(k);
    values[h] = v;
}

value *HashTable::search(key k)
{
    uint h = compute_hash(k);
    uint nb_tries = 0;
    while (nb_tries < max_size){
        if (keys[h] != nullptr)
            if (*(keys[h]) == k)
                return &values[h];
        h = compute_rehash(h,nb_tries);
        nb_tries++;
    }
    return nullptr;
}

void HashTable::log_content() const
{
    for (uint k = 0;k<max_size;k++){
        if (keys[k] != nullptr)
            std::cout << "at index " << k << " " << *keys[k] << ":" << values[k] << std::endl;
    }
}

std::vector<bool> HashTable::book_table() const
{
    std::vector<bool> hist(max_size);
    for (uint k = 0;k<max_size;k++)
        hist[k] = keys[k] != nullptr;
    return hist;
}


