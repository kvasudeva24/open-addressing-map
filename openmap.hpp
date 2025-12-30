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


    void resize(){
        size_t new_cap = cap_ * 3;
        Node* new_data = new Node[new_cap]{};
        

        for (size_t i{}; i < cap_; i++) {
            if (data_[i].state  == State::EMPTY ) {
                continue;
            } else {
                K key = *(data_[i].key);
                V value = *(data_[i].value);
                State s = data_[i].state;

                std::size_t hash_key = std::hash<K>{}(key) % new_cap;

                //when rehasing do not need to worry about duplicates happening 

                if ( new_data[hash_key].state == State::EMPTY ) {
                    new_data[hash_key].state = s;
                    *(new_data[hash_key].key) = key;
                    *(new_data[hash_key].value) = value;
                    continue;

                } else {
                    size_t candidate = ( new_data[hash_key].state == State::TOMBSTONE ) ? hash_key : new_cap;

                    for ( size_t i{0};  i < new_cap; i++ ) {

                        size_t iter_key = ( hash_key + i ) % new_cap;

                            //as we keep probing, once we find an empty we either insert at candidate or at the empty slot
                        if ( new_data[iter_key].state == State::EMPTY ) {
                            size_t insert_idx = (candidate == new_cap) ? iter_key : candidate;
                            data_[insert_idx].state = s;
                            *(data_[insert_idx].key) = key;
                            *(data_[insert_idx].value) = value;
                            break;

                        } else {
                            //we only need to keep track of the first tombstone
                            if ( candidate == new_cap ) {
                                candidate = iter_key;
                            }
                            continue;
                        }
                    }
                }
            }
        }
        delete[] data_;
        data_ = new_data;
        cap_ = new_cap;
    }



public:
    OpenMap(){
        data_ = new Node[cap_]{};
    }

    //need to add support for tombstones
    void insert(K key, V value){

        if ( size_ >= cap_ * alpha ) { 
            // std::cout << "Triggered" << std::endl;
            resize();
        }


        std::size_t hash_key = std::hash<K>{}(key) % cap_;

        //two cases 
        //1. we hit an empty spot right away
        //2. or we dont

        if ( data_[hash_key].state == State::EMPTY ) {
            // std::cout << "Case 1 hit" << std::endl;
            data_[hash_key].state = State::OCCUPIED;
            *(data_[hash_key].key) = key;
            *(data_[hash_key].value) = value;
            size_++;
            return;
        } else {
            //if the index it should be at is a tombstone keep it as a candidate else cap_ (since cap_ is postive and non indexable)
            // std::cout << "Case 2 hit " << std::endl;

            size_t candidate = ( data_[hash_key].state == State::TOMBSTONE ) ? hash_key : cap_;

            // std::cout << candidate << std::endl;
            
            for ( size_t i{0};  i < cap_; i++ ) {

                size_t iter_key = ( hash_key + i ) % cap_;

                //we mark dead nodes by changing their state so checking just key is not good enough
                if ( *(data_[iter_key].key) == key && data_[iter_key].state == State::OCCUPIED ) {
                    *(data_[iter_key].value) = value;
                    return;
                }


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

    void remove(K key){
        std::size_t hash_key = std::hash<K>{}(key);

        for ( size_t i{0}; i < cap_; i++) {
            size_t iter_key = (hash_key + i) % cap_;
            if ( *(data_[iter_key].key) == key ) {
                data_[iter_key].state = State::TOMBSTONE;
                return;
            }
            continue;
        }
    }


    size_t capacity() const { return cap_; };
    size_t size() const { return size_; };

    friend std::ostream& operator<<(std::ostream& os, const OpenMap& map){
        for ( size_t i{}; i < map.cap_; i++){
            if ( map.data_[i].state == State::OCCUPIED ) {
                os << "Index: " << i << " | Key/Value Pair: " << *(map.data_[i].key) << "/" << *(map.data_[i].value) << std::endl;
            } else {
                os << "Index: " << i << " | No Key/Value Pair: " << std::endl;
            }
        }
        return os;
    }


};
