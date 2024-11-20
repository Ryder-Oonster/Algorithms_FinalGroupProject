#include <iostream>
#include <boost/version.hpp>
#include <boost/lexical_cast.hpp>

int main() {
    std::cout << "Boost version: " << BOOST_LIB_VERSION << std::endl;

    try {
        int value = boost::lexical_cast<int>("123");
        std::cout << "Boost lexical_cast conversion: " << value << std::endl;
    } catch (const boost::bad_lexical_cast& e) {
        std::cerr << "Boost lexical_cast failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
