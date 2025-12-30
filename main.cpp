#include "openmap.hpp"



int main(){
    OpenMap<int, char> test{};
    test.insert(12, 'x');
    // test.insert(12, 'c');
    std::cout << test.size() << std::endl;



    

    // std::cout << test << std::endl;





    return 0;
}






