

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include "anomaly_detection_util.h"
using namespace std;


class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------
//helper functions
Circle findMinCircle(Point** points,size_t size);
Circle findMinCircleHelper(Point** copy, vector<Point> set, size_t size);
Circle default_min_circle(vector<Point> & p);
float distance(const Point& a, const Point& b);
bool is_inside(const Circle& c, const Point& p);
Circle circle_from_three_points(const Point& p1, const Point& p2, const Point& p3);


// implement
#endif /* MINCIRCLE_H_ */
