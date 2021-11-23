#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <iostream>

using uint = unsigned int;

using key = uint;
using value = int;

#include <vector>
#include <functional>

using hashfunc = std::function<uint(key)>;
using rehashfunc = std::function<uint(uint,uint)>;

using histogram = std::vector<bool>;

class HashTable{
public:
    HashTable(uint m,const hashfunc& h,const rehashfunc& rh);

    void insert(key,value);
    value* search(key);

    void log_content() const;

    histogram book_table() const;

    inline void reset_values(){
        values.clear();
        keys.clear();
        values.resize(max_size);
        keys.resize(max_size,nullptr);
    }


private:

    inline uint compute_hash(key k){
        return hash(k)%max_size;
    }
    inline uint compute_rehash(uint h,uint nb_tries){
        return rehash(h,nb_tries)%max_size;
    }

    uint max_size;
    std::vector<key*> keys;
    std::vector<value> values;
    hashfunc hash;
    rehashfunc rehash;

};

#endif // HASH_TABLE_H
