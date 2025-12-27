#include <algorithm>
#include <optional>
#include <iostream>


template<typename K, typename V>
class OpenMap{

    //4 bytes
    enum class State {EMPTY, OCCUPIED, TOMBSTONE};
    
    //find a way around assuming K and V are default constructible
    //get rid of std::optional for Node itself
    struct Node{
        K key;
        V value;
        State state = State::EMPTY;

        //we only construct node when it is empty beforehand
        Node(K k, V v): key(k), value(v) {
            state = State::OCCUPIED;
        };
    };


private:  //need public for debugging purposes, private actually
    std::optional<Node>* data_ = nullptr;
    size_t cap_ = 5;
    size_t size_ = 0;
    static constexpr double alpha = 0.6;

public:
    OpenMap(){
        data_ = new std::optional<Node>[cap_]{};
    }

    //need to add support for tombstones
    //bloom filter wont work because you cannot take away inclusion
    // void insert(K key, V value){

    // }

    // void remove(K key, V value){



    // }


    //should be an iterator but iterator is adjacent to this ptr
    // V* find(K key){



    // }


    size_t capacity() const { return cap_; };
    size_t size() const { return size_; };


    friend std::ostream& operator<<(std::ostream& os, const OpenMap& map);



};


template<typename K, typename V>
    std::ostream& operator<<(std::ostream& os, OpenMap<K, V>& map){
        for ( size_t i{}; i < map.cap_; i++ ) {
            if ( map.data_[i].has_value() ) {
                os << "Index "<< i << " | Key/Value Pair: " << map.data_[i]->key << "/" << map.data_[i]->value << std::endl;
            } else { 
                os << "No value at index " << i << std::endl;
            }
        }
        return os;
    }