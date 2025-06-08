#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

//ADD YOUR API KEY HERE
string youtubeApiKey = "";

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

string makeRequest(const string& url) {
    CURL* curl = curl_easy_init();
    string response;

    if (curl) {
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

    string apiURL = "https://www.googleapis.com/youtube/v3/search?part=snippet&type=video&maxResults=5&q=" + encodedQuery + "&key=" + youtubeApiKey;
    string result = makeRequest(apiURL);

    json data = json::parse(result, nullptr, false);
    if (data.is_discarded() || data.contains("error")) {
        cerr << "Error parsing YouTube API response.\n";
        return;
    }

    vector<string> videoIds;

    cout << "\n🔎 YouTube Search Results:\n";
    int index = 1;

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

void calculate(const string& expression) {
    try {
        vector<double> numbers;
        vector<char> operators;
        string currentNum;
        
        for (char c : expression) {
            if (isdigit(c) || c == '.') {
                currentNum += c;
            } else if (c != ' ') {
                if (!currentNum.empty()) {
                    numbers.push_back(stod(currentNum));
                    currentNum.clear();
                }
                operators.push_back(c);
            }
        }
        if (!currentNum.empty()) {
            numbers.push_back(stod(currentNum));
        }
        
        if (numbers.empty()) {
            cout << "Error: Invalid expression\n";
            return;
        }
        
        for (size_t i = 0; i < operators.size(); i++) {
            if (operators[i] == '*' || operators[i] == '/') {
                double result;
                if (operators[i] == '*') {
                    result = numbers[i] * numbers[i + 1];
                } else {
                    if (numbers[i + 1] == 0) {
                        cout << "Error: Division by zero\n";
                        return;
                    }
                    result = numbers[i] / numbers[i + 1];
                }
                numbers[i] = result;
                numbers.erase(numbers.begin() + i + 1);
                operators.erase(operators.begin() + i);
                i--;
            }
        }
        
        double finalResult = numbers[0];
        for (size_t i = 0; i < operators.size(); i++) {
            if (operators[i] == '+') {
                finalResult += numbers[i + 1];
            } else if (operators[i] == '-') {
                finalResult -= numbers[i + 1];
            }
        }
        
        cout << "Result: " << finalResult << endl;
    } catch (const exception& e) {
        cout << "Error: Invalid expression\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: ./exe -yt <search term> or ./exe -gg <search term> or ./exe -calc <expression>\n";
        return 1;
    }

    string flag = argv[1];
    string query;
    for (int i = 2; i < argc; ++i) {
        query += argv[i];
        if (i < argc - 1) query += " ";
    }

    if (flag == "-yt") {
        searchYouTube(query);
    } else if (flag == "-gg") {
        searchGoogle(query);
    } else if (flag == "-calc") {
        calculate(query);
    } else {
        cout << "Unknown flag. Use -yt for YouTube, -gg for Google search, or -calc for calculations.\n";
    }

    return 0;
}
