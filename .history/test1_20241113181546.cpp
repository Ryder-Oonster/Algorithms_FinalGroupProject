#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>

int main() {
    // Initialize the OpenSSL library
    SSL_library_init();
    SSL_load_error_strings();

    // Create an SSL context
    const SSL_METHOD* method = TLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);

    if (ctx == nullptr) {
        // If context creation failed, print errors
        ERR_print_errors_fp(stderr);
        return 1;
    }

    std::cout << "OpenSSL initialized successfully.\n";

    // Free the SSL context
    SSL_CTX_free(ctx);
    return 0;
}
