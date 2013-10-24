#include "BayazitDecomposer.h"
#include <cmath>
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;
bool isReflex(const PolygonShape &p, const int &i);
void makeCCW(PolygonShape &poly);
void decomposePolygon(std::vector<PolygonShape>* polys, PolygonShape poly);
bool eq(const float &a, const float &b) {
    return abs(a - b) <= 1e-8;
}

int wrap(const int &a, const int &b) {
    return a < 0 ? a % b + b : a % b;
}

template<class T>
T& at(vector<T> v, int i) {
    return v[wrap(i, v.size())];
};
float area(const CCPoint &a, const CCPoint &b, const CCPoint &c) {
    return (((b.x - a.x)*(c.y - a.y))-((c.x - a.x)*(b.y- a.y)));
}

bool left(const CCPoint &a, const CCPoint &b, const CCPoint &c) {
    return area(a, b, c) > 0;
}

bool leftOn(const CCPoint &a, const CCPoint &b, const CCPoint &c) {
    return area(a, b, c) >= 0;
}

bool right(const CCPoint &a, const CCPoint &b, const CCPoint &c) {
    return area(a, b, c) < 0;
}

bool rightOn(const CCPoint &a, const CCPoint &b, const CCPoint &c) {
    return area(a, b, c) <= 0;
}

bool collinear(const CCPoint &a, const CCPoint &b, const CCPoint &c) {
    return area(a, b, c) == 0;
}

float sqdist(const CCPoint &a, const CCPoint &b) {
	float dx = b.x - a.x;
	float dy = b.y - a.y;
    return dx * dx + dy * dy;
}
void makeCCW(PolygonShape &poly) {
    int br = 0;

    // find bottom right point
    for (int i = 1; i < poly.size(); ++i) {
        if (poly[i].y < poly[br].y || (poly[i].y == poly[br].y && poly[i].x > poly[br].x)) {
            br = i;
        }
    }

    // reverse poly if clockwise
    if (!left(at(poly, br - 1), at(poly, br), at(poly, br + 1))) {
        reverse(poly.begin(), poly.end());
    }
}
bool isReflex(const PolygonShape &poly, const int &i) {
    return right(at(poly, i - 1), at(poly, i), at(poly, i + 1));
}

CCPoint intersection(const CCPoint &p1, const CCPoint &p2, const CCPoint &q1, const CCPoint &q2) {
    CCPoint i;
    float a1, b1, c1, a2, b2, c2, det;
    a1 = p2.y - p1.y;
    b1 = p1.x - p2.x;
    c1 = a1 * p1.x + b1 * p1.y;
    a2 = q2.y - q1.y;
    b2 = q1.x - q2.x;
    c2 = a2 * q1.x + b2 * q1.y;
    det = a1 * b2 - a2*b1;
    if (!eq(det, 0)) { // lines are not parallel
		i.x = (b2 * c1 - b1 * c2) / det;
        i.y = (a1 * c2 - a2 * c1) / det;
    }
    return i;
}

void swap(int &a, int &b) {
    int c;
    c = a;
    a = b;
    b = c;
}
void decomposePolygon(std::vector<PolygonShape>* polys, PolygonShape poly)
{
	CCPoint upperInt, lowerInt, p, closestVert;
    float upperDist, lowerDist, d, closestDist;
    int upperIndex, lowerIndex, closestIndex;
    PolygonShape lowerPoly, upperPoly;

    for (int i = 0; i < poly.size(); ++i) {
        if (isReflex(poly, i)) {
            upperDist = lowerDist = FLT_MAX;
            for (int j = 0; j < poly.size(); ++j) {
                if (left(at(poly, i - 1), at(poly, i), at(poly, j))
                        && rightOn(at(poly, i - 1), at(poly, i), at(poly, j - 1))) { // if line intersects with an edge
                    p = intersection(at(poly, i - 1), at(poly, i), at(poly, j), at(poly, j - 1)); // find the point of intersection
                    if (right(at(poly, i + 1), at(poly, i), p)) { // make sure it's inside the poly
                        d = sqdist(poly[i], p);
                        if (d < lowerDist) { // keep only the closest intersection
                            lowerDist = d;
                            lowerInt = p;
                            lowerIndex = j;
                        }
                    }
                }
                if (left(at(poly, i + 1), at(poly, i), at(poly, j + 1))
                        && rightOn(at(poly, i + 1), at(poly, i), at(poly, j))) {
                    p = intersection(at(poly, i + 1), at(poly, i), at(poly, j), at(poly, j + 1));
                    if (left(at(poly, i - 1), at(poly, i), p)) {
                        d = sqdist(poly[i], p);
                        if (d < upperDist) {
                            upperDist = d;
                            upperInt = p;
                            upperIndex = j;
                        }
                    }
                }
            }

            // if there are no vertices to connect to, choose a point in the middle
            if (lowerIndex == (upperIndex + 1) % poly.size()) {
                printf("Case 1: Vertex(%d), lowerIndex(%d), upperIndex(%d), poly.size(%d)\n", i, lowerIndex, upperIndex, (int) poly.size());
				p.x = (lowerInt.x + upperInt.x) / 2;
                p.y = (lowerInt.y + upperInt.y) / 2;

                if (i < upperIndex) {
                    lowerPoly.insert(lowerPoly.end(), poly.begin() + i, poly.begin() + upperIndex + 1);
                    lowerPoly.push_back(p);
                    upperPoly.push_back(p);
                    if (lowerIndex != 0) upperPoly.insert(upperPoly.end(), poly.begin() + lowerIndex, poly.end());
                    upperPoly.insert(upperPoly.end(), poly.begin(), poly.begin() + i + 1);
                } else {
                    if (i != 0) lowerPoly.insert(lowerPoly.end(), poly.begin() + i, poly.end());
                    lowerPoly.insert(lowerPoly.end(), poly.begin(), poly.begin() + upperIndex + 1);
                    lowerPoly.push_back(p);
                    upperPoly.push_back(p);
                    upperPoly.insert(upperPoly.end(), poly.begin() + lowerIndex, poly.begin() + i + 1);
                }
            } else {
                // connect to the closest point within the triangle
                printf("Case 2: Vertex(%d), closestIndex(%d), poly.size(%d)\n", i, closestIndex, (int) poly.size());

                if (lowerIndex > upperIndex) {
                    upperIndex += poly.size();
                }
                closestDist = FLT_MAX;
                for (int j = lowerIndex; j <= upperIndex; ++j) {
                    if (leftOn(at(poly, i - 1), at(poly, i), at(poly, j))
                            && rightOn(at(poly, i + 1), at(poly, i), at(poly, j))) {
                        d = sqdist(at(poly, i), at(poly, j));
                        if (d < closestDist) {
                            closestDist = d;
                            closestVert = at(poly, j);
                            closestIndex = j % poly.size();
                        }
                    }
                }

                if (i < closestIndex) {
                    lowerPoly.insert(lowerPoly.end(), poly.begin() + i, poly.begin() + closestIndex + 1);
                    if (closestIndex != 0) upperPoly.insert(upperPoly.end(), poly.begin() + closestIndex, poly.end());
                    upperPoly.insert(upperPoly.end(), poly.begin(), poly.begin() + i + 1);
                } else {
                    if (i != 0) lowerPoly.insert(lowerPoly.end(), poly.begin() + i, poly.end());
                    lowerPoly.insert(lowerPoly.end(), poly.begin(), poly.begin() + closestIndex + 1);
                    upperPoly.insert(upperPoly.end(), poly.begin() + closestIndex, poly.begin() + i + 1);
                }
            }

            // solve smallest poly first
            if (lowerPoly.size() < upperPoly.size()) {
				decomposePolygon(polys,lowerPoly);
                decomposePolygon(polys,upperPoly);
            } else {
                decomposePolygon(polys,upperPoly);
                decomposePolygon(polys,lowerPoly);
            }
            return;
        }
    }
    polys->push_back(poly);
}
std::vector<PolygonShape> DecomposePolygon(PolygonShape poly)
{
	std::vector<PolygonShape> polys;
	decomposePolygon(&polys,poly);
	return polys;
}
