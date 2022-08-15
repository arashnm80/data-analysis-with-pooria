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
    return std::filesystem::current_path();
}

// // no need to this function:
// string upperFolderPath(int mode = 1){ // uses currentFilePath()
//     return currentFolderPath(mode).substr(0, currentFolderPath(mode).find_last_of("\\/"));
// }


