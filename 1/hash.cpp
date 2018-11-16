// ECE 365 DSA2
// Jing Jiang Project 1 Spellcheck

#include <iostream>
#include "hash.h"

using namespace std;

// init hashTable
hashTable::hashTable(int size) {
	filled = 0;
	capacity = getPrime(size);
	data.resize(capacity);
	hashItem emptyItem;

	for(int i = 0; i < capacity; i++) {
	data[i].isOccupied = false;
	data[i].isDeleted = false;
	}
}

int hashTable::insert(const std::string &key, void *pv) {
	int position = hash(key);
	int p = -1;

	while(data[position].isOccupied) {
		if(data[position].key == key) {
			if(!data[position].isDeleted) {
				return 1;
			} else{
				data[position].isDeleted = false;
				return 0;
			}
		} else{
			if(data[position].isDeleted && p == -1) {
				p = position;
			}
		}

		if(position == capacity - 1) {
			position = 0;
		} else{
			position++;
		}
	}
	if(p != -1) {
		position = p;
	}
	data[position].isOccupied = true;
	filled++;
	data[position].key = key;

	if((2*filled) > capacity) {
		if(!rehash())
			return 2;
	}

	return 0;
}

bool hashTable::contains(const std::string &key){
	if (this->findPos(key)>= 0){
		return true;
	} else {
		return false;
	}
}

void* hashTable::getPointer(const std::string &key, bool *b ){
	int position = findPos(key);
	if (position >= 0) {
		if(b != nullptr)
			*b=true;
		return data.at(position).pv;
	} else {
		if (b != nullptr)
			*b = false;
		return nullptr;
	}
}

int hashTable::setPointer(const std::string &key, void *pv){
	int position = findPos(key);
	if (position >= 0) {
		data.at(position).pv=pv;
		return 0;
	}else
		return 1;
}

bool hashTable::remove(const std::string &key){
	int position = findPos(key);
		if (position >= 0) {
			(data.at (position)).isDeleted = true;
			return true;
		} else {
			return false;
		}
}

int hashTable::hash(const std::string& key) {
	unsigned int hash = 0xAAAAAAAA;

	for(size_t i = 0; i < key.length(); i++) {
		hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ key[i] * (hash >> 3)) :
								(~((hash << 11) + (key[i] ^ (hash >> 5))));
	}

	return hash %capacity;
}

bool hashTable::rehash() {
	int prevCap = capacity;
	capacity = getPrime(prevCap * 2);
	if(capacity == 0)
		return false;

	vector<hashItem> copy = data;
	data.resize(capacity);
	if(data.size() != capacity)
		return false;

	for(int i = 0; i < capacity; i++) {
		hashItem &temp = data.at(i);
		temp.key = "";
		temp.isOccupied = false;
		temp.isDeleted = false;
	}

	filled = 0;
	for(int i = 0; i < prevCap; i++) {
		hashItem temp2 = copy.at(i);

		if(temp2.isOccupied && !temp2.isDeleted) {
		insert(temp2.key);
		}
	}

	return true;
}

// choose a prime number for size
unsigned int hashTable::getPrime(int size) {
	static unsigned int hashPrime[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289,
		24593, 49157, 98317, 196613, 393241, 786433,
		1572869, 3145739, 6291469, 12582917, 25165843,
		50331653, 100663319, 201326611, 402653189,
		805306457, 1610612741};
	int i = 0;
	while(hashPrime[i] < size) {
		i++;
	}

	return hashPrime[i];
}

// find position of a key
int hashTable::findPos(const std::string &key) {
	int position = hash(key);

	while(data[position].isOccupied) {
		if(data[position].key == key) {
			if(!data[position].isDeleted)
				return position;
			else
				return -1;
		} else
			position++;
	}
	return -1;
}
