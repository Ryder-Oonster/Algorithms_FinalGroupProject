#include <iostream>
#include <curl/curl.h>
#include <string>
#include <nlohmann/json.hpp>
#include <vector>
#include <cmath> // For rounding

using json = nlohmann::json;

// Function to round a double to 4 decimal places
std::string roundToFourDecimalPlaces(double value) {
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%.4f", value);
    return std::string(buffer);
}

// Structure to hold city data
struct City {
    std::string name;
    double latitude;
    double longitude;
};

// List of cities
std::vector<City> cities = {
    {"Greenfield", 42.587493343031454, -72.60071874795115},
    {"Orange", 42.589571762835696, -72.31194049863984},
    {"Leominster", 42.525681053586894, -71.75951732590133},
    {"Littleton", 42.53701123089329, -71.5117022182466},
    {"Lowell", 42.63368358841284, -71.31656248393247},
    {"Laurence", 42.706764119806984, -71.16538161999169},
    {"Springfield", 42.10211327762007, -72.59097830516824},
    {"Worcester", 42.261482511170584, -71.81462395786178},
    {"Burlington", 42.50434567719397, -71.19657572305924},
    {"Boston", 42.35940546138307, -71.06437392437775},
    {"Newton", 42.33625714988035, -71.20887626376805},
    {"Salem", 42.5191086954799, -70.8946169075994},
    {"Marlborough", 42.347132179759605, -71.55409605823336},
    {"Norwood", 42.19475437369707, -71.19868678842211},
    {"Quincy", 42.25356128427269, -71.00588685015889},
    {"Mansfield", 42.03393144946192, -71.21848965909258},
    {"Attleborough", 41.89196106474753, -70.61974389884898},
    {"Providence", 41.94444377201998, -71.28999567657799}
};

// Write callback for libcurl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

int main() {
    CURL* curl = curl_easy_init();

    if (!curl) {
        std::cerr << "Failed to initialize libcurl\n";
        return 1;
    }

    for (size_t i = 0; i < cities.size(); ++i) {
        for (size_t j = i + 1; j < cities.size(); ++j) {
            // Prepare coordinates
            std::string startLat = roundToFourDecimalPlaces(cities[i].latitude);
            std::string startLon = roundToFourDecimalPlaces(cities[i].longitude);
            std::string endLat = roundToFourDecimalPlaces(cities[j].latitude);
            std::string endLon = roundToFourDecimalPlaces(cities[j].longitude);

            // URL for the OSRM request
            std::string url = "http://localhost:5000/route/v1/driving/" + startLon + "," + startLat + ";" + endLon + "," + endLat + "?overview=false";

            CURLcode res;
            std::string response;

            // Set curl options
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            // Perform the request
            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            } else {
                try {
                    // Parse and process the response using nlohmann::json
                    json jsonData = json::parse(response);

                    if (!jsonData["routes"].empty()) {
                        // Extract travel distance and duration
                        double distance = jsonData["routes"][0]["distance"].get<double>() / 1000.0; // Convert to km
                        double duration = jsonData["routes"][0]["duration"].get<double>() / 60.0;   // Convert to minutes
                        std::cout << "From " << cities[i].name << " to " << cities[j].name << ":\n";
                        std::cout << "  Distance: " << distance << " km\n";
                        std::cout << "  Travel time: " << duration << " minutes\n";
                    } else {
                        std::cerr << "No routes found in response from " << cities[i].name << " to " << cities[j].name << ".\n";
                    }
                } catch (const json::parse_error& e) {
                    std::cerr << "Failed to parse JSON response from " << cities[i].name << " to " << cities[j].name << ": " << e.what() << std::endl;
                }
            }
        }
    }

    // Clean up libcurl
    curl_easy_cleanup(curl);

    return 0;
}