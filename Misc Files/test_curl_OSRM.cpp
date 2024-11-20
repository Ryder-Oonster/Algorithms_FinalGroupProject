#include <iostream>
#include <curl/curl.h>

int main() {
    CURL *curl;
    CURLcode res;
    
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if(curl) {
        // Set the URL for your OSRM request
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5000/route/v1/driving/-71.0892,42.3398;-70.9759,41.9903?steps=true");

        // Perform the request and store the result
        res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        // Clean up
        curl_easy_cleanup(curl);
    }
    
    // Clean up global libcurl resources
    curl_global_cleanup();
    return 0;
}
