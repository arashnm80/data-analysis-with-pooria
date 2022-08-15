**created with collaboration with [pooryas98](https://github.com/pooryas98)**


**how to use:**
1. clone this repo for yourself:
```
git clone https://github.com/arashnm80/stock-and-crypto-price-analysis.git
```
2. remove previous result files (if there are any):
```
rm ./files/results/*
```
3. change experiment cases in `variables.hpp` to your desired ones, for example:
```
int N_arr[] = {5, 10};
int upMarginPercentage_arr[] = {1, 2, 3, 4, 5};
int downMarginPercentage_arr[] = {2, 4};
int logicNumber_arr[] = {1, 2};
```
4. build:
```
g++ -std=c++17 main.cpp -o app
```
5. run:
```
./app
```
