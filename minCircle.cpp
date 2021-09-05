#include "minCircle.h"




Circle findMinCircle(Point** points,size_t size){
	return findMinCircleHelper(points, {}, size);
}

Circle findMinCircleHelper(Point** copy, vector<Point> set, size_t size) {
	if (size == 0) {
		return default_min_circle(set);
	}
	if (set.size() == 3) {
		return circle_from_three_points(set[0], set[1], set[2]);
	}
	// take last point of the array
	Point temp = *copy[size - 1];
	Circle minCirlce = findMinCircleHelper(copy, set, size - 1);
	if (is_inside(minCirlce, temp)) {
		return minCirlce;
	}
	// else point is not inside circle
	// push p into set , and call recursively for copy \ {p} - without last point
	set.push_back(temp);
	return findMinCircleHelper(copy , set, size - 1);
}
// find min circle for |set| == 3 or |copy| == 0
// we have either 0,1,2,3
Circle default_min_circle(vector<Point> & p) {
	if(p.size() == 0) {
		return Circle(Point(0, 0) , 0);
	}
	// p has only one point -form circle from this point
	else if (p.size() == 1) {
		return Circle(p[0] , 0);
	}
	// p hase 2 points - form a circle
	else if (p.size() == 2) {
		Point center = Point((p[0].x + p[1].x) / (float)2.0 ,
			(p[0].y + p[1].y) / (float)2.0);
		float radius = (distance(p[0], p[1])) / 2.0;
		return Circle(center, radius);
	}
	// else p has 3 points 
	// check if can form a circle with combination of 2 points from 3
	// else return circle from 3 points
}
// distance between point a and point b
float distance(const Point& a, const Point& b) 
{ 
    return sqrt((b.x - a.x) * (b.x - a.x) 
                + (b.y - a.y) * (b.y - a.y)); 
} 
bool is_inside(const Circle& c, const Point& p) 
{ 	float dist = distance(c.center, p);
    return dist <= c.radius; 
}
Circle circle_from_three_points(const Point& p1, const Point& p2, const Point& p3) {
	double px = ((p3.y - p1.y) * (pow((p2.x - p1.x),2) + pow((p2.y - p1.y),2)) - 
    (p2.y - p1.y)* (pow((p3.x - p1.x),2) + pow((p3.y - p1.y),2))) / 
    (2.0 * (((p2.x - p1.x) * (p3.y - p1.y)) - ((p2.y - p1.y) * (p3.x - p1.x))));
    double py = ((p2.x - p1.x) * (pow((p3.x - p1.x),2) + pow((p3.y - p1.y),2)) - 
    (p3.x - p1.x) *(pow((p2.x - p1.x),2) + pow((p2.y - p1.y),2))) / 
    (2.0 * (((p2.x - p1.x) * (p3.y - p1.y)) - ((p2.y - p1.y) * (p3.x - p1.x))));
    px = px + p1.x; 
    py = py + p1.y;
    float radius = sqrt(((px - p1.x) * (px - p1.x)) + ((py - p1.y) * (py - p1.y))); 
    return {Point(px,py), radius };
}