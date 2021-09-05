
#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
	float circle_th;
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
protected:
	virtual void learnHelper(string f1, string f2, float corl, Point** arr, int len) override;
	virtual int isAnomalous(float x, float y, correlatedFeatures c) override;
	float getDistance(float x ,float y, Point* a);

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
