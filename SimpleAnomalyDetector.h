


#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "timeseries.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	bool is_circle;
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
	Point* center;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
	float threshhold;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();

	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);


	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}
	void setCorrelationThreshold(float threshold){
		this->threshhold = threshold;
	}
	float getThreshhold() {
		return this->threshhold;
	}
protected:
	vector<correlatedFeatures> cf;
//helper functions
	Point** getPoints(vector <float> x, vector <float> y, int size);
	void deletePoints(Point** arr, int size);
	float maxDev(Point** points, int size, Line line);
	virtual void learnHelper(string f1, string f2, float corl, Point** arr, int len);
	virtual int isAnomalous(float x, float y,correlatedFeatures c);
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
