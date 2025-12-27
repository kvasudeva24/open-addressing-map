#include <algorithm>
#include <optional>


template<typename K, typename V>
class OpenMap{
    
    struct Node{
        K key;
        V value;

        Node() = default;
    }


private:
    std::optional<Node>* data_ = nullptr;
    size_t cap_ = 10;
    size_t size_ = 0;
    static constexpr double alpha = 0.6;

public:
    OpenMap(){
        data_ = new std::optional<Node>[cap_]{};
    }


    void insert(K key, V value){





    }

    void remove(K key, V value){



    }


    //should be an iterator but iterator is adjacent to this ptr
    V* find(K key){



    }


    size_t capacity() const { return cap_; };
    size_t size() const { return size_; };







};