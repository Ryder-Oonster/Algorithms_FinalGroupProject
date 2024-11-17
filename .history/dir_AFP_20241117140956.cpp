#include <iostream>
#include <curl/curl.h>
#include <string>
#include <nlohmann/json.hpp> // Requires the JSON for Modern C++ library

using json = nlohmann::json; // Alias for convenience

// Write callback for libcurl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

int main() {
    // Coordinates for two locations (example coordinates in Massachusetts)
    std::string startLat = "42.3583";
    std::string startLon = "-71.06574721537308";
    std::string endLat = "42.3581";
    std::string endLon = "-71.0636";

    // URL for the OSRM request
    std::string url = "http://localhost:5000/route/v1/driving/" + startLon + "," + startLat + ";" + endLon + "," + endLat + "?steps=true";

    // Initialize libcurl
    CURL* curl = curl_easy_init();
    if (curl) {
        CURLcode res;
        std::string response;

        // Set curl options
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            try {
                // Parse and process the response using nlohmann::json
                json jsonData = json::parse(response);

                if (!jsonData["routes"].empty()) {
                    // Extract and print travel distance and duration
                    double distance = jsonData["routes"][0]["distance"].get<double>() / 1000.0; // Convert to km
                    double duration = jsonData["routes"][0]["duration"].get<double>() / 60.0;   // Convert to minutes
                    std::cout << "Distance: " << distance << " km\n";
                    std::cout << "Travel time: " << duration << " minutes\n";

                    // Print directions (if available)
                    if (!jsonData["routes"][0]["legs"].empty() && !jsonData["routes"][0]["legs"][0]["steps"].empty()) {
                        std::cout << "Directions:\n";
                        for (const auto& step : jsonData["routes"][0]["legs"][0]["steps"]) {
                            std::cout << "- " << step["name"].get<std::string>() << " (" << step["distance"].get<double>() << " meters)\n";
                        }
                    }
                } else {
                    std::cerr << "No routes found in response.\n";
                }
            } catch (const json::parse_error& e) {
                std::cerr << "Failed to parse JSON response: " << e.what() << std::endl;
            }
        }

        // Clean up libcurl
        curl_easy_cleanup(curl);
    }

    return 0;
}
