#include <iostream>
#include <curl/curl.h>
#include <string>

// Callback function to write received data into a std::string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out)
{
    size_t totalSize = size * nmemb;
    if (out)
    {
        out->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }
    return 0;
}

int main()
{
    CURL* curl;
    CURLcode res;
    std::string response;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl)
    {
        // Set the URL to the OSRM route API (replace the coordinates as needed)
        const char* url = "http://localhost:5000/route/v1/driving/-71.0589,42.3601;-70.9780,42.2529?overview=false";
        
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        else
        {
            // Print the response
            std::cout << "Response from OSRM: " << response << std::endl;
        }

        // Clean up
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0;
}
