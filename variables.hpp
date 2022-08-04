#ifndef VARIABLES_HPP
#define VARIABLES_HPP

    const std::string buildPath{currentFolderPath() + getPathSeparator() + "build" + getPathSeparator()};
    const std::string filesPath{currentFolderPath() + getPathSeparator() + "files" + getPathSeparator()};
    const std::string inputFileName = "input.csv";
    const std::string outputFileName = "output.csv";
    const std::string resultFileName = "result.csv";
    const int maxRows{1'000'00};
    const int maxColumns{20};
    std::array<std::string, maxColumns> csvHeaders{};
    int rows;
    int columns;
    int currentColumn;
    int N{5};
    int indexCsvColumn; // input should not have index column, we'll add index to it in output later
    int dateCsvColumn;
    int timeCsvColumn;
    int openCsvColumn;
    int highCsvColumn;
    int lowCsvColumn;
    int closeCsvColumn;
    int volumeCsvColumn;
    double upMargin = 0.04;
    double downMargin = 0.02;
    double sum = 0;
    double average = 0;
    double transactionCount = 0;

#endif