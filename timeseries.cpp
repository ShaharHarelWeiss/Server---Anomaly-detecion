
#include "timeseries.h"


TimeSeries::TimeSeries(const char* CSVfileName) {
    std::ifstream file(CSVfileName);
    string line;
    std::vector<float> row;
    int i = 1;
    int j = 0;
    // get first line into column
    getline(file, line);
    stringstream ss(line);
    while (ss.good()) {
        string substr;
        getline(ss, substr, ',');
        this->colname.push_back(substr);
     }
     vector<float> x[this->colname.size()];
     // get line from csv file into string line
     while (getline(file, line)) {
         stringstream ss(line);
         while (ss.good()) {
            string substr;
            getline(ss, substr, ',');
            row.push_back(std::stof(substr));
         }
         for (j = 0; j < row.size(); j++) {
            x[j].push_back(row[j]);
         }
         row.clear();
    }
    // get columns into the map with key - name of col and value - data of col
    for (i = 0; i < this->colname.size(); i++) {
        this->data_list[this->colname[i]] = x[i];
    }

}
TimeSeries::TimeSeries(const TimeSeries &ts) {
    this->colname = ts.colname;
    this->data_list = ts.data_list;
}
TimeSeries::~TimeSeries() {
}
vector<string> TimeSeries::getKeys() const{
    return this->colname;
}
vector<float> TimeSeries::getCol(string key) const{
    vector<float> temp;
    for (auto it = this->data_list.begin(); it != data_list.end(); it++) {
        if (it->first.compare(key) == 0) {
            temp = it->second;
            return temp;
        }
    }
    cout << "key not found !!";
    return temp;
}
