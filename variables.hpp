#ifndef VARIABLES_HPP
#define VARIABLES_HPP

    const std::string buildPath{currentFolderPath() + getPathSeparator() + "build" + getPathSeparator()};
    const std::string filesPath{currentFolderPath() + getPathSeparator() + "files" + getPathSeparator()};
    const std::string resultsPath{currentFolderPath() + getPathSeparator() + "files" + getPathSeparator() + "results" + getPathSeparator()};
    const std::string inputFileName = "input.csv";
    const std::string outputFileName = "output.csv";
    // const std::string resultFileName = "result.csv";
    const std::string summaryFileName = "summary.csv";
    const int maxRows{1'000'000};
    const int maxColumns{20};
    std::array<std::string, maxColumns> csvHeaders{};
    int rows;
    int columns;
    int currentColumn;
    int N{5};
    int logicNumber;
    int indexCsvColumn; // input should not have index column, we'll add index to it in output later
    int dateCsvColumn;
    int timeCsvColumn;
    int openCsvColumn;
    int highCsvColumn;
    int lowCsvColumn;
    int closeCsvColumn;
    int volumeCsvColumn;
    int transactionsCount = 0; // to-do: if errors happened change it back to double
    double upMargin = 0.04;
    double downMargin = 0.02;
    int upMarginPercentage;
    int downMarginPercentage;
    double percentageSum = 0;
    double percentageAverage = 0;
    double logicScore = 0;

    // experiment cases
    int N_arr[] = {50};
    int upMarginPercentage_arr[] = {10};
    int downMarginPercentage_arr[] = {9};
    int logicNumber_arr[] = {2};

#endif