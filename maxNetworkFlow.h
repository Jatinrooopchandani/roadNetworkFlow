#pragma once
#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>
#include <memory>
#include <limits>
#include <stdexcept>

#include "edge.h"

using namespace std;

class NetworkFlow {
	
public:
	vector<int> path;
	int maxFlow = 0;

    NetworkFlow() = default;
    ~NetworkFlow() = default;

    // Delete copy constructor and assignment operator
    NetworkFlow(const NetworkFlow&) = delete;
    NetworkFlow& operator=(const NetworkFlow&) = delete;

    // Allow move operations
    NetworkFlow(NetworkFlow&&) = default;
    NetworkFlow& operator=(NetworkFlow&&) = default;

    int fordFulkerson(const std::vector<std::vector<int>>& graph, int source, int sink) {
        if (source < 0 || sink < 0 || source >= graph.size() || sink >= graph.size()) {
            throw std::invalid_argument("Invalid source or sink node");
        }

        int n = graph.size();
        std::vector<std::vector<int>> residualGraph(n, std::vector<int>(n, 0));

        // Copy the original graph to the residual graph
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                residualGraph[i][j] = graph[i][j];
            }
        }

        std::vector<int> parent(n);
        maxFlow = 0;

        // Augment the flow while there is a path from source to sink
        while (true) {
            std::fill(parent.begin(), parent.end(), -1);
            std::queue<int> q;
            q.push(source);
            parent[source] = -2;

            while (!q.empty()) {
                int u = q.front();
                q.pop();

                for (int v = 0; v < n; ++v) {
                    if (parent[v] == -1 && residualGraph[u][v] > 0) {
                        parent[v] = u;
                        q.push(v);
                    }
                }
            }

            // If there is no augmenting path, break
            if (parent[sink] == -1) {
                break;
            }

            int pathFlow = std::numeric_limits<int>::max();
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                pathFlow = std::min(pathFlow, residualGraph[u][v]);
            }

            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                residualGraph[u][v] -= pathFlow;
                residualGraph[v][u] += pathFlow;
            }

            maxFlow += pathFlow;
        }

        return maxFlow;
    }

    int findWeightTill(const std::vector<std::vector<EdgePtr>>& edgeMap, 
                       const std::vector<int>& parent, 
                       int source, 
                       int target) const {
        int res = 0;
        while (target != source) {
            if (parent[target] == -1) {
                throw std::runtime_error("Invalid path: parent node not found");
            }
            res += edgeMap[parent[target]][target]->weight();
            target = parent[target];
        }
        return res;
    }

    std::vector<int> optimizedPath(const std::vector<Edge>& edges, int source, int sink, int n) {
        if (source < 0 || sink < 0 || source >= n || sink >= n) {
            throw std::invalid_argument("Invalid source or sink node");
        }

        // Create a null edge shared pointer
        auto nullEdge = std::make_shared<Edge>();
        
        // Initialize edge map with null edges
        std::vector<std::vector<EdgePtr>> edgeMap(n, std::vector<EdgePtr>(n, nullEdge));

        // Fill the edge map
        for (const auto& edge : edges) {
            edgeMap[edge.from()][edge.to()] = std::make_shared<Edge>(edge);
        }

        std::vector<int> parent(n, -1);
        std::queue<int> q;
        q.push(source);

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            int weightTillCurrent = findWeightTill(edgeMap, parent, source, current);
            
            for (int i = 0; i < n; ++i) {
                if (edgeMap[current][i] != nullEdge) {
                    if (parent[i] == -1 || 
                        weightTillCurrent + edgeMap[current][i]->weight() < 
                        findWeightTill(edgeMap, parent, source, i)) {
                        parent[i] = current;
                        q.push(i);
                    }
                }
            }
        }

        // Reconstruct the path from source to sink
        std::vector<int> path;
        for (int at = sink; at != source; at = parent[at]) {
            if (at == -1) {
                throw std::runtime_error("No path found from source to sink");
            }
            path.push_back(at);
        }
        path.push_back(source);
        std::reverse(path.begin(), path.end());

        return path;
    }


	

};
};