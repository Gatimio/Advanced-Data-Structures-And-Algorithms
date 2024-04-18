#ifndef _MY_CLOSEST_PAIR_POINTS_H_
#define _MY_CLOSEST_PAIR_POINTS_H_

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <fstream>

typedef struct {
    unsigned int ID;
    float x;
    float y;
} PointType;

bool compX(const PointType &a, const PointType &b) { return a.x < b.x; }

bool compY(const PointType &a, const PointType &b) { return a.y < b.y; }

float dist(const PointType &p1, const PointType &p2) {
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

float bruteForce(const std::vector<PointType> &points, int left, int right, PointType &p1, PointType &p2) {
    float minDist = std::numeric_limits<float>::infinity();
    for (int i = left; i < right; ++i) {
        for (int j = i + 1; j <= right; ++j) {
            float d = dist(points[i], points[j]);
            if (d < minDist) {
                minDist = d;
                p1 = points[i];
                p2 = points[j];
            }
        }
    }
    return minDist;
}

float stripClosest(std::vector<PointType> &strip, float d, PointType &p1, PointType &p2) {
    float minDist = d;
    std::sort(strip.begin(), strip.end(), compY);
    for (size_t i = 0; i < strip.size(); ++i) {
        for (size_t j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < minDist; ++j) {
            float d = dist(strip[i], strip[j]);
            if (d < minDist) {
                minDist = d;
                p1 = strip[i];
                p2 = strip[j];
            }
        }
    }
    return minDist;
}

float closeUtil(std::vector<PointType> &points, int left, int right, PointType &p1, PointType &p2) {
    const int SMALL_SIZE_THRESHOLD = 3;
    if (right - left <= SMALL_SIZE_THRESHOLD) {
        return bruteForce(points, left, right, p1, p2);
    }
    int mid = left + (right - left) / 2;
    PointType midPoint = points[mid];
    PointType pl1, pl2, pr1, pr2;
    float dl = closeUtil(points, left, mid, pl1, pl2);
    float dr = closeUtil(points, mid + 1, right, pr1, pr2);
    if (dl < dr) {
        p1 = pl1;
        p2 = pl2;
    } else {
        p1 = pr1;
        p2 = pr2;
    }
    if (p1.ID > p2.ID) {
        std::swap(p1, p2);
    }
    float d = std::min(dl, dr);
    std::vector<PointType> strip;
    strip.reserve(right - left + 1); 
    for (int i = left; i <= right; i++) {
        if (std::abs(points[i].x - midPoint.x) < d) {
            strip.emplace_back(points[i]);
        }
    }
    PointType strip_p1, strip_p2;
    float strip_d = stripClosest(strip, d, strip_p1, strip_p2);
    if (strip_d < d) {
        p1 = strip_p1;
        p2 = strip_p2;
    }
    if (p1.ID > p2.ID) {
        std::swap(p1, p2);
    }
    return std::min(d, strip_d);
}

float ClosestPairOfPoints(const std::vector<PointType> &points, PointType &p1, PointType &p2) {
    if (points.size() < 2) {
        throw std::runtime_error("Error: At least two points are required.");
    }
    std::vector<PointType> pointsCopy(points); 
    std::sort(pointsCopy.begin(), pointsCopy.end(), compX);
    float minDist = closeUtil(pointsCopy, 0, pointsCopy.size() - 1, p1, p2);
    minDist = std::round(minDist * 1000.0) / 1000.0;
    return minDist;
}

#endif
