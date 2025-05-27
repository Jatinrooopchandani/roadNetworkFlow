#pragma once
#include <iostream>
#include <memory>

class Edge {
public:
    Edge(int from = -1, int to = -1, int weight = 0)
        : from_(from), to_(to), weight_(weight) {}

    // Getters
    int from() const { return from_; }
    int to() const { return to_; }
    int weight() const { return weight_; }

    // Setters
    void setFrom(int from) { from_ = from; }
    void setTo(int to) { to_ = to; }
    void setWeight(int weight) { weight_ = weight; }

private:
    int from_;
    int to_;
    int weight_;
};

// Smart pointer type alias for Edge
using EdgePtr = std::shared_ptr<Edge>;