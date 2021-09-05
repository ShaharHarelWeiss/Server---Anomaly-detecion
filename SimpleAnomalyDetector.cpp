
#include "SimpleAnomalyDetector.h"
#define MAX_DEV_FIX 1.1

SimpleAnomalyDetector::SimpleAnomalyDetector() {
	// TODO Auto-generated constructor stub
	this->threshhold = 0.9;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
}


void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
	int i = 0, j = 0;
	// constant threshhold for corlation check
	//const float th = 0.9;
	//ctor of ts
	TimeSeries ts_copy(ts);
	//names of features - columns of TS
	vector<string> features = ts_copy.getKeys();
	int size_of_cols = features.size();
	// iterate over columns to check  for correlated features
	for (i = 0; i < size_of_cols; i++) {
		float maxCorl = 0;
		int j_maxCorl = 0;
		vector<float> tempX(ts_copy.getCol(features[i]));
		for(j = i + 1; j < size_of_cols; j++) {
			vector<float> tempY(ts_copy.getCol(features[j]));
			float corl = fabs(pearson(&tempX[0], &tempY[0], tempX.size()));
			if (corl > maxCorl) {
				maxCorl = corl;
				j_maxCorl = j;
			}
		}
		vector<float> tempY(ts_copy.getCol(features[j_maxCorl]));
		//check if two features are correltad
		Point** arr = getPoints(tempX, tempY, tempX.size());
		//maxCorl = 0.6;
		learnHelper(features[i], features[j_maxCorl], maxCorl ,arr, tempX.size());
		deletePoints(arr, tempX.size());
		}
}


vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	vector<AnomalyReport> ar;
	TimeSeries ts_copy(ts);
	size_t result = 0;
	for (int i = 0; i < this->cf.size(); i++) {
		//get the columns of current cf`
		vector<float> f1 = ts.getCol(this->cf[i].feature1);
		vector<float> f2 = ts.getCol(this->cf[i].feature2);
		//Point** arr = getPoints(f1, f2, f1.size());
		for (int k = 0; k < f1.size(); k++) {
			//float deviation = dev(*arr[k], this->cf[i].lin_reg);
			result = isAnomalous(f1[k], f2[k], this->cf[i]);
				if (result == 1) {
					//anomaly detected
					string desc = cf[i].feature1 + '-' + cf[i].feature2;
					// k is the time aka number of row in TS
					AnomalyReport anomaly(desc,k + 1);
					ar.push_back(anomaly);
					//break;
			}
		}
		//deletePoints(arr, f1.size());
	}		
	return ar;
}

int SimpleAnomalyDetector::isAnomalous(float x, float y, correlatedFeatures c){
	float newX = c.lin_reg.f(x);
	bool b = fabs(y - newX) > c.threshold;
	if (b) {
		return 1;
	}
	else
		return 0;
	
}

// learn help method
void SimpleAnomalyDetector::learnHelper(string f1, string f2, float corl, Point** arr, int len){
	if (corl >= this->threshhold) {
		Line line = linear_reg(arr, len);
		//max dev of current line and points
		float computed_th = fabs(maxDev(arr, len, line));
		correlatedFeatures foundCF = {false, f1, f2, corl, line, computed_th};
		this->cf.push_back(foundCF);
	}
}


//create array of points from two vectors
Point** SimpleAnomalyDetector::getPoints(vector <float> x, vector <float> y, int size) {
	Point** arr = new Point*[size];
	for (int k = 0; k < size; k++) {
		arr[k] = new Point(x[k], y[k]);
	}
	return arr;
}

//delete the Points array
void SimpleAnomalyDetector::deletePoints(Point** arr, int size) {
	for (int k = 0; k <size; k++) {
			delete arr[k];
	}
	delete [] arr;
}

//find the max dev of given array of points and line, multiply it by constant fixer
float SimpleAnomalyDetector::maxDev(Point** points, int size, Line l) {
	float max = 0;
	for (int i = 0; i < size; i++) {
		float current = dev(*points[i], l);
		if (current > max) {
			max = current;
		}
	}
	return MAX_DEV_FIX * max;
}