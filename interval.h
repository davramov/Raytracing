#ifndef INTERVAL_H
#define INTERVAL_H

// Class representing a numeric interval [min, max].
class interval {
public:
    double min, max;

    // Default constructor, initializes an empty interval.
    interval() : min(+infinity), max(-infinity) {} // Default interval is empty

    // Constructor with specified min and max values.
    interval(double _min, double _max) : min(_min), max(_max) {}

    // Interval constructor that takes two intervals as input (for two bounding boxes)
    interval(const interval& a, const interval& b)
        : min(fmin(a.min, b.min)), max(fmax(a.max, b.max)) {}


    // Check if the interval contains a specific value.
    bool contains(double x) const {
        return min <= x && x <= max;
    }

    // Check if the interval surrounds a specific value (excluding the boundaries).
    bool surrounds(double x) const {
        return min < x && x < max;
    }

    // Clamp values within the interval range.
    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    double size() const {
        return max - min;
    }

    interval expand(double delta) const {
        auto padding = delta / 2;
        return interval(min - padding, max + padding);
    }


    // Static constants representing an empty interval and the universe interval.
    static const interval empty, universe;
};

// Static instances representing an empty interval and the universe interval.
const static interval empty(+infinity, -infinity);
const static interval universe(-infinity, +infinity);

interval operator+(const interval& ival, double displacement) {
    return interval(ival.min + displacement, ival.max + displacement);
}

interval operator+(double displacement, const interval& ival) {
    return ival + displacement;
}


#endif