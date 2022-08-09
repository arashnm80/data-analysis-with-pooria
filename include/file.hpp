// last edit: 2022-08-05

#include <fstream>
#include <filesystem>
#include <unistd.h>

// using std::filesystem::current_path;

// using namespace std;

std::string fileToString(std::string address){
	std::ifstream inputFile(address, std::ios::binary);
    std::string fileStr;

    std::istreambuf_iterator<char> inputIt(inputFile), emptyInputIt;
    std::back_insert_iterator<std::string> stringInsert(fileStr);

    copy(inputIt, emptyInputIt, stringInsert);
    inputFile.close();
    return fileStr;
}

std::string currentFolderPath(int mode = 1){ // help source: https://www.delftstack.com/howto/cpp/get-current-directory-cpp/
    switch(mode){
        case 1: // directory without double backslashes and outside double quotation marks
            char directory[256];
            getcwd(directory, 256);
            // if(directory[0] >= 'a' && directory[0] <= 'z'){
            //     directory[0] -= 32; // make windows drive letter uppercase (optional)
            // }
            return std::string(directory);
            break;
        // TO-DO: edit and complete section 2
        // case 2: // directory with double backslashes and outside double quotation marks
        //     return current_path();
        //     break;
        default:
            std::cout << "error on currentFolderPath()" << std::endl;
            return std::string("error on currentFolderPath()");
            break;
    }
}

// // no need to this function:
// string upperFolderPath(int mode = 1){ // uses currentFilePath()
//     return currentFolderPath(mode).substr(0, currentFolderPath(mode).find_last_of("\\/"));
// }


