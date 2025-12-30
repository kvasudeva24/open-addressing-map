#include "openmap.hpp"



int main(){
    OpenMap<int, char> test{};
    test.insert(12, 'x');
    test.insert(13, 'y');
    std::cout << test << std::endl;
    test.insert(14, 'q');
    std::cout << test.size() << std::endl;
    test.insert(15, 'x');
    // test.insert(16, 'z');
    // test.remove(12);


    // // test.remove(12);
    // test.insert(12, 'v');
    std::cout << test << std::endl;



    

    // std::cout << test << std::endl;





    return 0;
}






