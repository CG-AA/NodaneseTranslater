#include <sstream>
#include <map>
#include <nlohmann/json.hpp>
#include <fstream>
#include <algorithm>
#include <vector>
#include <regex>

using json = nlohmann::json;

json parseToJSON(std::string input, std::vector<std::string> keys) {
    json j;
    for (int i = 0; i < keys.size(); i++) {
        size_t start = input.find(keys[i]);
        if (start != std::string::npos) {
            start += keys[i].size();
            size_t end = (i < keys.size() - 1) ? input.find(keys[i + 1]) : std::string::npos;
            std::string value = (end != std::string::npos) ? input.substr(start, end - start) : input.substr(start);
            value.erase(remove_if(value.begin(), value.end(), isspace), value.end());
            value = value.substr(value.find_first_not_of("=")); // remove leading '='
            j[keys[i]] = value;
        }
    }
    return j;
}

int main() {
    std::ifstream file("info.txt");
    std::string input((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    std::vector<std::string> keys = {"ID", "Title", "Tags", "Discription", "Input Format", "Output Format", "TL", "ML", "Visible", "Share Submission", "Type", "TestData", "Source", "Solution", "Answer"};
    json j = parseToJSON(input, keys);

    // Handle input/output samples separately
    std::regex in_out_regex("(Input Sample \\d+|Output Sample \\d+)");
    std::sregex_iterator it(input.begin(), input.end(), in_out_regex);
    std::sregex_iterator reg_end;
    for (; it != reg_end; ++it) {
        std::string key = it->str();
        size_t start = input.find(key) + key.size();
        size_t end = input.find("=", start);
        std::string value = input.substr(start, end - start);
        value.erase(remove_if(value.begin(), value.end(), isspace), value.end());
        value = value.substr(value.find_first_not_of("=")); // remove leading '='
        j[key] = value;
    }

    std::ofstream out("info.json");
    out << j.dump(4);
    out.close();
    return 0;
}