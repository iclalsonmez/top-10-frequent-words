#include<iostream>
#include<cstdlib>
#include<string>
#include<cstdio>
using namespace std;
 

class HashNode
{
    public:
        string key;
    	int value;
        HashNode* next;
        HashNode(string key, int value)
        {
            this->key = key;
            this->value = value;
            this->next = NULL;
        }
};
 

class HashMap
{
    public:
        HashNode** htable;
        int size; 
        int TABLE_SIZE = 10001;
        HashMap(int SIZE)
        {
            TABLE_SIZE = SIZE; 
            htable = new HashNode*[TABLE_SIZE];
            for (int i = 0; i < TABLE_SIZE; i++){
                htable[i] = NULL;
            }
            size = 0; 
        }
        ~HashMap()
        {
            for (int i = 0; i < TABLE_SIZE; ++i)
	        {
                HashNode* entry = htable[i];
                while (entry != NULL)
                {
                    HashNode* prev = entry;
                    entry = entry->next;
                    delete prev;
                }
            }
            delete[] htable;
        }
       
        int HashFunc(string key) 
        {
            std::hash<std::string> hasher; 
            return hasher(key) % TABLE_SIZE; 
        }
 
        void insert(string key, int value)
        {
            if (this -> search(key)) {
                update(key, value); 
                return; 
            }
            int hash_val = HashFunc(key);
            HashNode* prev = NULL;
            HashNode* entry = htable[hash_val];
            while (entry != NULL)
            {
                prev = entry;
                entry = entry->next;
            }
            if (entry == NULL)
            {
                entry = new HashNode(key, value);
                if (prev == NULL)
	            {
                    htable[hash_val] = entry;
                }
                else
                {
                    prev->next = entry;
                }
            }
            else
            {
                entry->value = value;
            }
            size += 1; 
        }
        
        void update(string key, int value) {
            if (! (this -> search(key))) {
                std::cout << "Key is not in the hashmap, can't update" << std::endl; 
                return;
            }

            int hash_val = HashFunc(key); 
            HashNode* prev = NULL; 
            HashNode* entry = htable[hash_val];
            while ( (entry -> key) != key)
            {
                prev = entry;
                entry = entry->next;
            }
            entry -> value = value; 
        }
        
        int search(string key)
        {
            int hash_val = HashFunc(key);
            HashNode* entry = htable[hash_val];
            while (entry != NULL)
    	    {
                if (entry->key == key)
	            {
                    return 1; 
                }
                entry = entry->next;
            }
            return 0; 
        }
        
        int retrieve(string key) 
        {
            int hash_val = HashFunc(key); 
            HashNode* entry = htable[hash_val]; 
            while (entry != NULL) {
                if (entry -> key == key) {
                    return entry -> value; 
                }
                entry = entry -> next; 
            }
            return -1; 
        }


        HashNode** getAllEntries() {
            HashNode** entries = new HashNode*[size]; 
            int curr_ind = 0; 
            for (int i = 0; i < TABLE_SIZE; i++) {
                HashNode* entry = htable[i]; 
                while (entry != NULL) {
                    entries[curr_ind] = entry; 
                    curr_ind += 1; 
                    entry = entry -> next; 
                }
            }
            return entries; 
        }
};