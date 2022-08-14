// https://github.com/arashnm80/data-analysis-with-pooria.git

#include "headers.hpp"
#include "variables.hpp"

using namespace std;

void test();
void initializeVariables();
void getHeaders(string& firstLine);
void logic(double** arr, int logicNumber);
void printSummary();

int main(){
    // optional function to change some variables
    initializeVariables();

    // create double array
    double** arr = new double*[maxRows];
    for(int i{0}; i < maxRows; i++){
        arr[i] = new double[maxColumns];
    }
    
    // open input file stream
    ifstream input(filesPath + inputFileName);
    if(input.fail()){
        cerr << "input failed" << endl;
    }

    // read everything from input
    string line;
    getline(input, line);
    columns = 0;
    getHeaders(line); // calculate csv headers and columns count based on first line
    string item;
    rows = 0;
    while(getline(input, line)){
        currentColumn = 0;
        item.clear();
        for(auto &ch : line){
            if(',' == ch){
                arr[rows][currentColumn] = stod(item);
                currentColumn++;
                item.clear();
            }else if('\r' != ch){
                item += ch;
            }
        }
        arr[rows][currentColumn] = stod(item); // for last item in row which doesn't have comma
        rows++;
    }
    
    // close input file stream
    input.close();

    // try our logics
    cout << endl << "which logic you want to try?" << endl
            << "1. intertwined transactions" << endl
            << "2. separate transactions" << endl << endl;
    cin >> logicNumber;
    cin.ignore();
    logic(arr, logicNumber);

    // delete double array
    for(int i{0}; i < maxRows; i++){
        delete[] arr[i];
    }
    delete[] arr;

    cout << "\ndone. press enter to exit...";
    cin.ignore();
}

void test(){
    cout << "date " << dateCsvColumn << endl;
    cout << "time " << timeCsvColumn << endl;
    cout << "open " << openCsvColumn << endl;
    cout << "high " << highCsvColumn << endl;
    cout << "low " << lowCsvColumn << endl;
    cout << "close " << closeCsvColumn << endl;
    cout << "volume " << volumeCsvColumn << endl;
}

void initializeVariables(){
    int i;
    cout << "do you want to change any variables?" << endl
            << "1. yes" << endl
            << "2. no" << endl << endl;
    cin >> i;
    if(1 == i){
        cout << "enter value for variables below:" << endl;
        cout << "N: ";
        cin >> N;
        cout << "upMargin: ";
        cin >> upMargin;
        cout << "downMargin: ";
        cin >> downMargin;
    }

    cout << endl;
    cout << "N = " << N << endl;
    cout << "upMargin = " << upMargin << endl;
    cout << "downMargin = " << downMargin << endl;

    ofstream summary;
    summary.open(filesPath + summaryFileName);
    summary << "N"
        << ',' << "upMarginPercentage"
        << ',' << "downMarginPercentage"
        << ',' << "logicNumber"
        << ',' << "percentageAverage"
        << ',' << "transactionsCount"
        << ',' << "logicScore"
        << endl;
    summary.close();
}

void getHeaders(string& firstLine){
    for(auto &ch : firstLine){
        if(',' == ch){
            columns++;
        }else if('\r' != ch){
            csvHeaders[columns] += ch;
        }
    }
    columns++;

    for(int i{0}; i < columns; i++){
        if("index" == csvHeaders[i]){
            indexCsvColumn = i;
        }else if("date" == csvHeaders[i]){
            dateCsvColumn = i;
        }else if("time" == csvHeaders[i]){
            timeCsvColumn = i;
        }else if("open" == csvHeaders[i]){
            openCsvColumn = i;
        }else if("high" == csvHeaders[i]){
            highCsvColumn = i;
        }else if("low" == csvHeaders[i]){
            lowCsvColumn = i;
        }else if("close" == csvHeaders[i]){
            closeCsvColumn = i;
        }else if("volume" == csvHeaders[i]){
            volumeCsvColumn = i;
        }else{
            cerr << "error on getHeaders()" << endl;
        }
    }
}

void logic(double** arr, int logicNumber){
    // precalculations
    upMarginPercentage = static_cast<int>((upMargin * 100) + 0.5);
    downMarginPercentage = static_cast<int>((downMargin * 100) + 0.5);

    // open output file streams
    ofstream output;
    output.open(filesPath + outputFileName);
    string resultFileName = resultsPath + to_string(N)
            + '_' + to_string(upMarginPercentage)
            + '_' + to_string(downMarginPercentage)
            + '_' + to_string(logicNumber);
    ofstream result;
    result.open(resultFileName);

    // check this logic for all items
    percentageSum = 0;
    transactionsCount = 0;
    result << "status,enter row,exit row,enter price,exit price,percentage" << endl;
    for(int row{N}; row < rows; row++){
        double maxHigh = arr[row - N][highCsvColumn];
        double minLow = arr[row - N][lowCsvColumn];
        for(int i{row - N}; i < row; i++){
            if(arr[i][highCsvColumn] > maxHigh){
                maxHigh = arr[i][highCsvColumn];
            }
            if(arr[i][lowCsvColumn] < minLow){
                minLow = arr[i][lowCsvColumn];
            }
        }
        if(arr[row][closeCsvColumn] > maxHigh){ // start transaction here
            bool exitFlag = false;
            for(int i = row + 1; i < rows; i++){ // check for transaction's end condition
                if(arr[i][closeCsvColumn] > maxHigh * (1 + upMargin) 
                    && arr[i][closeCsvColumn] > arr[row][closeCsvColumn]){
                        exitFlag = true;
                }else if(arr[i][closeCsvColumn] < minLow * (1 - downMargin)
                    && arr[i][closeCsvColumn] < arr[row][closeCsvColumn]){
                        exitFlag = true;
                }
                if(exitFlag){
                    double percentage = ((arr[i][closeCsvColumn] / arr[row][closeCsvColumn]) - 1) * 100;
                    result << (percentage > 0 ? '+' : '-')
                            << ',' << row // transaction's enter index
                            << ',' << i // transaction's exit index
                            << ',' << arr[row][closeCsvColumn]
                            << ',' << arr[i][closeCsvColumn]
                            << ',' << percentage
                            << endl;
                    percentageSum += percentage;
                    transactionsCount++;                 
                    if(2 == logicNumber){
                        row = i + 1;
                    }
                    break;
                }
            }
        }
    }
    
    // print summary to console:
    printSummary();
    
    // copy input to output with a little or no change
    output << "index";
    for(int i{0}; i < columns; i++){
        output << ',' << csvHeaders[i];
    }
    output << endl;
    for(int row{0}; row < rows; row++){
        for(int column{0}; column < columns; column++){
            if(0 == column){
                output << row;
            }
            output << ',' << arr[row][column];
        }
        output << endl;
    }

    // close output file streams
    output.close();
    result.close();
}

void printSummary(){
    ofstream summary;
    summary.open(filesPath + summaryFileName, ios::app);

    percentageAverage = percentageSum / transactionsCount;
    logicScore = percentageAverage * transactionsCount;
    summary << N
            << ',' << upMarginPercentage
            << ',' << downMarginPercentage
            << ',' << logicNumber
            << ',' << percentageAverage
            << ',' << transactionsCount
            << ',' << logicScore
            << endl;

    summary.close();
}