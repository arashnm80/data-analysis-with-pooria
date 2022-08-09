#include "headers.hpp"
#include "variables.hpp"

using namespace std;

void test();
void initializeVariables();
void getHeaders(string& firstLine);
void logic1(double** doubleArray);
void logic2(double** doubleArray);
void printSummary();

int main(){
    // optional function to change some variables
    initializeVariables();

    // create double array
    double** doubleArray = new double*[maxRows];
    for(int i{0}; i < maxRows; i++){
        doubleArray[i] = new double[maxColumns];
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
                doubleArray[rows][currentColumn] = stod(item);
                currentColumn++;
                item.clear();
            }else if('\r' != ch){
                item += ch;
            }
        }
        doubleArray[rows][currentColumn] = stod(item); // for last item in row which doesn't have comma
        rows++;
    }
    
    // close input file stream
    input.close();

    // try our logics
    cout << endl << "which logic you want to try? (enter from 1 to 2)" << endl;
    int logicNumber;
    cin >> logicNumber;
    cin.ignore();
    switch(logicNumber){
        case 1:
            logic1(doubleArray); // mixed transactions
            break;
        case 2:
            logic2(doubleArray); // serparate transactions
            break;    
    }

    // delete double array
    for(int i{0}; i < maxRows; i++){
        delete[] doubleArray[i];
    }
    delete[] doubleArray;

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
    cout << "do you want to change any variables? (type 1 for yes and 0 for no)" << endl;
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

void logic1(double** doubleArray){
    // open output file streams
    ofstream output;
    output.open(filesPath + outputFileName);
    ofstream result;
    result.open(filesPath + resultFileName);

    // check this logic for all items
    sum = 0;
    goodTransactionsCount = 0;
    badTransactionsCount = 0;
    result << "status,enter row,exit row,enter price,exit price,difference" << endl;
    for(int row{N}; row < rows; row++){
        double maxHigh = doubleArray[row - N][highCsvColumn];
        double minLow = doubleArray[row - N][lowCsvColumn];
        for(int i{row - N}; i < row; i++){
            if(doubleArray[i][highCsvColumn] > maxHigh){
                maxHigh = doubleArray[i][highCsvColumn];
            }
            if(doubleArray[i][lowCsvColumn] < minLow){
                minLow = doubleArray[i][lowCsvColumn];
            }
        }
        if(doubleArray[row][closeCsvColumn] > maxHigh){
            // enter transaction
            for(int i = row + 1; i < rows; i++){
                if(doubleArray[i][closeCsvColumn] > maxHigh * (1 + upMargin) 
                    && doubleArray[i][closeCsvColumn] > doubleArray[row][closeCsvColumn]){
                    result << "+," << row << ',' << i << ',' << doubleArray[row][closeCsvColumn] << ',' << doubleArray[i][closeCsvColumn] << ',' << doubleArray[i][closeCsvColumn] - doubleArray[row][closeCsvColumn] << endl;
                    sum += doubleArray[i][closeCsvColumn] - doubleArray[row][closeCsvColumn];
                    goodTransactionsCount++;
                    break;
                }else if(doubleArray[i][closeCsvColumn] < minLow * (1 - downMargin)
                    && doubleArray[i][closeCsvColumn] < doubleArray[row][closeCsvColumn]){
                    result << "-," << row << ',' << i << ',' << doubleArray[row][closeCsvColumn] << ',' << doubleArray[i][closeCsvColumn] << ',' << doubleArray[i][closeCsvColumn] - doubleArray[row][closeCsvColumn] << endl;
                    sum += doubleArray[i][closeCsvColumn] - doubleArray[row][closeCsvColumn];
                    badTransactionsCount++;
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
            output << ',' << doubleArray[row][column];
        }
        output << endl;
    }

    // close output file streams
    output.close();
    result.close();
}

void logic2(double** doubleArray){
    // open output file streams
    ofstream output;
    output.open(filesPath + outputFileName);
    ofstream result;
    result.open(filesPath + resultFileName);

    // check this logic for all items
    sum = 0;
    goodTransactionsCount = 0;
    badTransactionsCount = 0;
    result << "status,enter row,exit row,enter price,exit price,difference" << endl;
    for(int row{N}; row < rows; row++){
        double maxHigh = doubleArray[row - N][highCsvColumn];
        double minLow = doubleArray[row - N][lowCsvColumn];
        for(int i{row - N}; i < row; i++){
            if(doubleArray[i][highCsvColumn] > maxHigh){
                maxHigh = doubleArray[i][highCsvColumn];
            }
            if(doubleArray[i][lowCsvColumn] < minLow){
                minLow = doubleArray[i][lowCsvColumn];
            }
        }
        if(doubleArray[row][closeCsvColumn] > maxHigh){
            // enter transaction
            for(int i = row + 1; i < rows; i++){
                if(doubleArray[i][closeCsvColumn] > maxHigh * (1 + upMargin) 
                    && doubleArray[i][closeCsvColumn] > doubleArray[row][closeCsvColumn]){
                    result << "+," << row << ',' << i << ',' << doubleArray[row][closeCsvColumn] << ',' << doubleArray[i][closeCsvColumn] << ',' << doubleArray[i][closeCsvColumn] - doubleArray[row][closeCsvColumn] << endl;
                    row = i + 1; // all of difference bewteen logic 2 and 1 is in this part which is written 2 times in this for loop
                    sum += doubleArray[i][closeCsvColumn] - doubleArray[row][closeCsvColumn];
                    goodTransactionsCount++;
                    break;
                }else if(doubleArray[i][closeCsvColumn] < minLow * (1 - downMargin)
                    && doubleArray[i][closeCsvColumn] < doubleArray[row][closeCsvColumn]){
                    result << "-," << row << ',' << i << ',' << doubleArray[row][closeCsvColumn] << ',' << doubleArray[i][closeCsvColumn] << ',' << doubleArray[i][closeCsvColumn] - doubleArray[row][closeCsvColumn] << endl;
                    row = i + 1; // all of difference bewteen logic 2 and 1 is in this part which is written 2 times in this for loop
                    sum += doubleArray[i][closeCsvColumn] - doubleArray[row][closeCsvColumn];
                    badTransactionsCount++;
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
            output << ',' << doubleArray[row][column];
        }
        output << endl;
    }

    // close output file streams
    output.close();
    result.close();
}

void printSummary(){
    transactionsCount = goodTransactionsCount + badTransactionsCount;
    average = sum / transactionsCount;
    successRate = static_cast<double>(goodTransactionsCount) / transactionsCount * 100;
    cout << endl << "summary:" << endl;
    cout << "\t" << "sum: " << sum << endl
            << "\t" << "average: " << average << endl
            << "\t" << "successRate: " <<successRate << " %" << endl
            << "\t" << "transactionsCount: " << transactionsCount << endl;
}