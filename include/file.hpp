#include <fstream>
#include <filesystem>
#include <unistd.h>

std::string fileToString(std::string address){
	std::ifstream inputFile(address, std::ios::binary);
    std::string fileStr;

    std::istreambuf_iterator<char> inputIt(inputFile), emptyInputIt;
    std::back_insert_iterator<std::string> stringInsert(fileStr);

    copy(inputIt, emptyInputIt, stringInsert);
    inputFile.close();
    return fileStr;
}

std::string currentFolderPath(){
    std::stringstream s;
    s << std::filesystem::current_path();
    std::string out = s.str();
    if('\"' == *out.begin()){ // to handle outside quotation marks which current_path() returns in windows os
        out = out.substr(1, out.size() - 2);
    }
    return out;
}

// // no need to this function:
// string upperFolderPath(int mode = 1){ // uses currentFilePath()
//     return currentFolderPath(mode).substr(0, currentFolderPath(mode).find_last_of("\\/"));
// }


