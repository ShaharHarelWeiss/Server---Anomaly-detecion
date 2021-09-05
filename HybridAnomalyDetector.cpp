
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
	SimpleAnomalyDetector();
	this->circle_th = 0.5;
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

void HybridAnomalyDetector::learnHelper(string f1, string f2, float corl, Point** arr, int len) {
	SimpleAnomalyDetector::learnHelper(f1, f2, corl, arr, len);
	if (corl < this->getThreshhold() && corl >= 0.5) {
		Circle c = findMinCircle(arr, len);
		Point* b = new Point(c.center.x , c.center.y);
		float computed_th = c.radius * 1.1;
		Line line = linear_reg(arr, len);
		correlatedFeatures foundCF = {true ,f1, f2, corl, line, computed_th, b};
		//SimpleAnomalyDetector::getNormalModel().push_back(foundCF);
		this->cf.push_back(foundCF);
	}
}
int HybridAnomalyDetector::isAnomalous(float x, float y, correlatedFeatures c) {
	if (!c.is_circle)
		return SimpleAnomalyDetector::isAnomalous(x, y, c);
	else {
		float d = getDistance(x , y ,c.center);
		if (d > c.threshold) 
			return 1;
		//return (getDistance(x, y, c.center) > c.threshold);
		else 
			return 0;
	}
}
float HybridAnomalyDetector::getDistance(float x ,float y, Point* a) {
	return sqrt(pow(a->x - x, 2) + pow(a->y - y , 2));
}
