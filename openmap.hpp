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
        std::optional<K> key;
        std::optional<V> value;
        State state = State::EMPTY;

        Node() = default;
    };


private:  //need public for debugging purposes, private actually
    Node* data_ = nullptr;
    size_t cap_ = 5;
    size_t size_ = 0;
    static constexpr double alpha = 0.6;

public:
    OpenMap(){
        data_ = new Node[cap_]{};
    }

    //need to add support for tombstones
    void insert(K key, V value){
        std::size_t hash_key = std::hash<K>{}(key) % cap_;

        //two cases 
        //1. we hit an empty spot right away
        //2. or we dont

        if ( data_[hash_key].state == State::EMPTY ) {
            data_[hash_key].state = State::OCCUPIED;
            *(data_[hash_key].key) = key;
            *(data_[hash_key].value) = value;
            size_++;
            return;
        } else {
            //if the index it should be at is a tombstone keep it as a candidate else cap_ (since cap_ is postive and non indexable)
            size_t candidate = ( data_[hash_key].state == State::TOMBSTONE ) ? hash_key : cap_;
            
            for ( size_t i{1};  i < cap_; i++ ) {
                if ( *(data_[hash_key].key) == key ) {
                    *(data_[hash_key].value) = value;
                    return;
                }

                size_t iter_key = ( hash_key + i ) % cap_;

                //as we keep probing, once we find an empty we either insert at candidate or at the empty slot
                if ( data_[iter_key].state == State::EMPTY ) {
                    size_t insert_idx = (candidate == cap_) ? iter_key : candidate;
                    data_[insert_idx].state = State::OCCUPIED;
                    *(data_[insert_idx].key) = key;
                    *(data_[insert_idx].value) = value;
                    size_++;
                    return;
                } else {
                    //we only need to keep track of the first tombstone
                    if ( candidate == cap_ ) {
                        candidate = iter_key;
                    }
                    continue;
                }
            }
            //edge case where the whole map had no empty slots only tombstones or occupied so we insert here since we return early everywhere else
            data_[candidate].state = State::OCCUPIED;
            *(data_[candidate].key) = key;
            *(data_[candidate].value) = value;
            size_++;
            return;
        }

    }

    // void remove(K key, V value){



    // }


    //should be an iterator but iterator is adjacent to this ptr
    // V* find(K key){



    // }


    size_t capacity() const { return cap_; };
    size_t size() const { return size_; };


    friend std::ostream& operator<<(std::ostream& os, const OpenMap& map);



};


// template <typename K, typename V>
// std::ostream& operator<<(std::ostream& os, const OpenMap<K,V>& map){
//     for ( size_t i{}; i < map.cap_; i++){
//         if ( map.data_[i].state == State::OCCUPIED ) {
//             os << "Index: " << i << " | Key/Value Pair: " << *(map.data_[i].key) << "/" << *(map.data_[i].value) << std::endl;
//         } else {
//             os << "Index: " << i << " | No Key/Value Pair: " << std::endl;
//         }
//     }
//     return os;
// }