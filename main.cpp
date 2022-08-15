#include "headers.hpp"
#include "variables.hpp"

using namespace std;

void test();
void initialize();
void getHeaders(string& firstLine);
void logic(double** arr, int logicNumber);
void printSummary();

int main(){
    // start timer
    auto start = chrono::high_resolution_clock::now();

    // optional function to change some variables
    initialize();

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

    // try our experiments
    for(auto & a : N_arr){
        for(auto & b : upMarginPercentage_arr){
            for(auto & c : downMarginPercentage_arr){
                for(auto & d : logicNumber_arr){
                    N = a;
                    upMarginPercentage = b;
                    downMarginPercentage = c;
                    logicNumber = d;
                    logic(arr, logicNumber);
                }
            }
        }
    }

    // delete double array
    for(int i{0}; i < maxRows; i++){
        delete[] arr[i];
    }
    delete[] arr;

    // stop timer
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    cout << "duration: " << duration.count() << " seconds.\n";
}

void test(){
    cout << filesystem::current_path() << endl;
}

void initialize(){
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
    upMargin = static_cast<double>(upMarginPercentage) / 100;
    downMargin = static_cast<double>(downMarginPercentage) / 100;

    // open output file streams
    ofstream output;
    output.open(filesPath + outputFileName);
    string resultFileName = resultsPath + "result"
            + '_' + to_string(N)
            + '_' + to_string(upMarginPercentage)
            + '_' + to_string(downMarginPercentage)
            + '_' + to_string(logicNumber)
            + ".csv";
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
                            << '\n';
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
            << '\n';

    summary.close();

    cout << N
            << '_' << upMarginPercentage
            << '_' << downMarginPercentage
            << '_' << logicNumber
            << " done"
            << '\n';
}