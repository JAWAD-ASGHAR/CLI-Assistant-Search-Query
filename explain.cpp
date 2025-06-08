#include <iostream>
#include <string>
// vector is a dynamic array that can change size
#include <vector>
// using cstdlib to open the browser and use system function
#include <cstdlib>
// using curl library to make the request
// Client URL Request Library
//https://curl.se/libcurl/
#include <curl/curl.h>
// using nlohmann/json to parse the json data
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

// Youtube API Key
// https://console.cloud.google.com/
string apiKey = "AIzaSyDr2bi-ieVXl6XjosJnAY6HSFsB_LalWDY";

// callback function to write the response to the string
// REFERENCE:
// https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html

// *contents is the pointer to the raw data from the server in bytes usually
// size is the size of the data in bytes. In our case we are handing text data so should be 1 byte
// nmemb is the number of members in the data.
// output is the pointer to the string that was passed to be used to store the response

// This function can run multiple times because http requests are chunked in data packets.
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    // typecasting data as character to store to string
    // using append function instead of + operator in curl lib
    // We appending raw binary data instead of string. That's why append(ptr, length) is used instead of append(ptr).
    output->append((char*)contents, totalSize);
    // libcurl expects you to return the number of bytes you received. 
    // If you return a different number, it thinks something went wrong.
    return totalSize;
}

// request function to make the request to the youtube api
// REFERENCE:
// https://youtu.be/daA-KBKfJ_o?si=SL8p5nhhp7yGyG3H
// https://curl.se/libcurl/c/curl_easy_init.html
// https://curl.se/libcurl/c/curl_easy_setopt.html
// https://curl.se/libcurl/c/curl_easy_perform.html
// https://curl.se/libcurl/c/curl_easy_cleanup.html
string makeRequest(const string& url) {
    CURL* curl = curl_easy_init();
    string response;

    if (curl) {
        // c_str function converts std::string to a C-style string, which libcurl needs being a c lang lib.
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "CURL error: " << curl_easy_strerror(res) << endl;
        }
        curl_easy_cleanup(curl);
    } else {
        cerr << "Failed to initialize CURL\n";
    }

    return response;
}

// REFERENCE:
// Open in default browser
// https://stackoverflow.com/questions/142508/how-do-i-check-os-with-a-preprocessor-directive
// https://www.geeksforgeeks.org/how-to-detect-operating-system-through-a-c-program/
// system is a C library function. It expects a C-style string specifically a char* thats why we use c_str function.
void openInBrowser(const string& url) {
#if defined(_WIN32)
    system(("start " + url).c_str());
#elif defined(__APPLE__)
    system(("open " + url).c_str());
#else
    system(("xdg-open " + url).c_str());
#endif
}

void searchYouTube(const string& query) {
    string encodedQuery = query;
    for (char& ch : encodedQuery) if (ch == ' ') ch = '+';

    // REFERENCE:
    // https://developers.google.com/youtube/v3/getting-started
    // https://developers.google.com/youtube/v3/docs/search/list

    // Url for the youtube api search -> https://www.googleapis.com/youtube/v3/search
    // all parameters follow key=value pairs
    // multiple parameters are separated by & 
    // part=snippet tells the api to return the snippet of the video like matadata
    // type=video tells the api to return only videos
    // maxResults=5 tells the api to return only 5 videos
    // q= tells the api to search for the query
    // key= tells the api to use the api key

    string apiURL = "https://www.googleapis.com/youtube/v3/search?part=snippet&type=video&maxResults=5&q=" + encodedQuery + "&key=" + apiKey;
    string result = makeRequest(apiURL);

    json data = json::parse(result, nullptr, false);
    if (data.is_discarded() || data.contains("error")) {
        cerr << "Error parsing YouTube API response.\n";
        return;
    }

    // vector is a dynamic array that can change size
    // https://www.w3schools.com/cpp/cpp_vectors.asp
    vector<string> videoIds;

    cout << "\n🔎 YouTube Search Results:\n";
    int index = 1;

    // auto is a keyword that tells the compiler to get the type of the variable from the right side of the assignment
    // https://www.geeksforgeeks.org/type-inference-in-c-auto-and-decltype/
    for (auto& item : data["items"]) {
        string title = item["snippet"]["title"];
        string channel = item["snippet"]["channelTitle"];
        string videoId = item["id"]["videoId"];

        cout << index << ". " << title << " [" << channel << "]\n";
        videoIds.push_back(videoId);
        index++;
    }

    if (videoIds.empty()) {
        cout << "No videos found.\n";
        return;
    }

    cout << "\nSelect a video [1-" << videoIds.size() << "]: ";
    int choice;
    cin >> choice;

    if (choice >= 1 && choice <= videoIds.size()) {
        string url = "https://www.youtube.com/watch?v=" + videoIds[choice - 1];
        openInBrowser(url);
    } else {
        cout << "Invalid choice.\n";
    }
}

void searchGoogle(const string& query) {
    string encodedQuery = query;
    for (char& ch : encodedQuery) if (ch == ' ') ch = '+';
    string url = "https://www.google.com/search?q=" + encodedQuery;
    openInBrowser(url);
}

int main(int argc, char* argv[]) {
    // argc is the number of arguments passed to the program - arg count
    // argv is an array of strings that contains the arguments passed to the program - arg vector
    // https://www.geeksforgeeks.org/command-line-arguments-in-c-cpp/
    if (argc < 3) {
        cout << "Usage: SQ -yt <search term> or SQ -gg <search term>\n";
        return 1;
    }

    // flag is the first argument passed to the program
    // query is the rest of the arguments passed to the program
    string flag = argv[1];
    string query;
    for (int i = 2; i < argc; ++i) {
        query += argv[i];
        // if the current index is not the last index then add a space to the query
        if (i < argc - 1) query += " ";
    }

    if (flag == "-yt") {
        searchYouTube(query);
    } else if (flag == "-gg") {
        searchGoogle(query);
    } else {
        cout << "Unknown flag. Use -yt for YouTube or -gg for Google search.\n";
    }

    return 0;
}