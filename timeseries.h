

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

class TimeSeries {
	// map data of rows
	std::map<string, vector<float>>data_list;
	//name of columns
	std::vector<string> colname;

public:
	TimeSeries(const char* CSVfileName);
	TimeSeries(const TimeSeries &ts); // copy constructor
	~TimeSeries();
	vector<string> getKeys() const;
	vector<float> getCol(string feature) const;

};

#endif /* TIMESERIES_H_ */
