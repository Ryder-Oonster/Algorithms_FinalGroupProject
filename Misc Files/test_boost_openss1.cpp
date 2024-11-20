#include <iostream>

// OpenSSL includes
#include <openssl/ssl.h>
#include <openssl/err.h>

// Boost includes
#include <boost/version.hpp>
#include <boost/lexical_cast.hpp>

int main() {
    // Test OpenSSL initialization
    SSL_library_init();
    SSL_load_error_strings();
    const SSL_METHOD* method = TLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);

    if (ctx == nullptr) {
        std::cerr << "OpenSSL initialization failed!" << std::endl;
        ERR_print_errors_fp(stderr);
        return 1;
    } else {
        std::cout << "OpenSSL initialized successfully." << std::endl;
    }
    SSL_CTX_free(ctx);

    // Test Boost functionality
    std::cout << "Boost version: " << BOOST_LIB_VERSION << std::endl;

    try {
        int value = boost::lexical_cast<int>("12345");
        std::cout << "Boost lexical_cast conversion: " << value << std::endl;
    } catch (const boost::bad_lexical_cast& e) {
        std::cerr << "Boost lexical_cast failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
