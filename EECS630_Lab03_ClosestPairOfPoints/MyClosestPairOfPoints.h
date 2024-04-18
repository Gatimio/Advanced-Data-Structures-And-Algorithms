#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <fstream>

typedef 
    struct 
    {
        unsigned int ID;    // the ID of the point
        float x;              // the x-coordinate of the point
        float y;              // the y-coordinate of the point
    }
PointType;

float distance(const PointType& p1, const PointType& p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return std::sqrt(dx * dx + dy * dy);
}

float bruteForce(const std::vector<PointType>& points, size_t start, size_t end, PointType& p1, PointType& p2) {
    float minDist = std::numeric_limits<float>::max();
    for (size_t i = start; i < end; ++i) {
        for (size_t j = i + 1; j < end; ++j) {
            float dist = distance(points[i], points[j]);
            if (dist < minDist) {
                minDist = dist;
                p1 = points[i];
                p2 = points[j];
            }
        }
    }
    return minDist;
}

float ClosestPairOfPoints(const std::vector<PointType>& points, PointType& p1, PointType& p2) {
    size_t n = points.size();
    if (n <= 1)
        return std::numeric_limits<float>::max(); // No points or only one point

    std::vector<PointType> sortedPoints = points;
    std::sort(sortedPoints.begin(), sortedPoints.end(), [](const PointType& a, const PointType& b) {
        return a.x < b.x;
    });

    if (n <= 3) // If there are 3 or fewer points, brute force is faster
        return bruteForce(sortedPoints, 0, n, p1, p2);

    // Divide the points into two halves
    size_t mid = n / 2;
    PointType midPoint = sortedPoints[mid];

    // Recursively find the closest pair in each half
    PointType closest1, closest2;
    float distLeft = ClosestPairOfPoints(std::vector<PointType>(sortedPoints.begin(), sortedPoints.begin() + mid), p1, p2);
    float distRight = ClosestPairOfPoints(std::vector<PointType>(sortedPoints.begin() + mid, sortedPoints.end()), closest1, closest2);

    float minDist = std::min(distLeft, distRight);

    // Merge the two sorted halves
    std::vector<PointType> strip;
    for (size_t i = 0; i < n; ++i) {
        if (std::abs(points[i].x - midPoint.x) < minDist)
            strip.push_back(points[i]);
    }

    // Find the closest pair in the strip
    size_t stripSize = strip.size();
    for (size_t i = 0; i < stripSize; ++i) {
        for (size_t j = i + 1; j < stripSize && (strip[j].y - strip[i].y) < minDist; ++j) {
            float dist = distance(strip[i], strip[j]);
            if (dist < minDist) {
                minDist = dist;
                p1 = strip[i];
                p2 = strip[j];
            }
        }
    }

    return minDist;
}
