#include <string>

// replace all occurrences of a smaller string with another
std::string findAndReplaceAll(const std::string& data, std::string toSearch, std::string replaceStr){
    std::string output = data;
    size_t pos = output.find(toSearch); // Get the first occurrence
    // Repeat till end is reached:
    while(pos != std::string::npos){
        output.replace(pos, toSearch.size(), replaceStr); // Replace this occurrence of Sub String
        pos =output.find(toSearch, pos + replaceStr.size()); // Get the next occurrence from the current position
    }
    return output;
}