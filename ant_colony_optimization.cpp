#pragma comment(linker, "/STACK:200000000")
#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")

#include "bits/stdc++.h"

using namespace std;


typedef int Distance;

const int kZeroDistance = 0;
const int kMinWeight = 1e2;
const int kMaxWeight = 1e3;
const int kInfDistance = 1e9;

typedef vector<vector<pair<int, Distance>>> Graph;

struct Path {
    vector<int> topology;
    Distance distance;
};


istream& operator>>(istream& in, Graph& graph) {
    int vertexNumber, edgesNumber;
    in >> vertexNumber >> edgesNumber;
    graph.clear();
    graph.resize(vertexNumber);
    while (edgesNumber--) {
        int from, to;
        Distance dist;
        in >> from >> to >> dist;
        graph[from].emplace_back(to, dist);
    }
    return in;
}


Graph ReadGraphFromFile(string filename) {
    ifstream fin(filename.data());
    fin.sync_with_stdio(false);
    fin.tie(NULL);
    Graph res;
    fin >> res;
    return res;
}


Graph GenerateRandomGraph(int size, double density, int seed) {
    mt19937 gen(seed);
    uniform_real_distribution<double> probTransf(0.0, 1.0);
    uniform_int_distribution<Distance> weightTransf(kMinWeight, kMaxWeight);
    Graph res(size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (probTransf(gen) < density) {
                res[i].emplace_back(j, weightTransf(gen));
            }
        }
    }
    return res;
}


Graph GenerateSpecialGraph() {
    Graph g(91);
    int n;
    
    const int M = 30;
    const int W = (int)8e8;
    for (int i = 0; i < M; i++)
        g[i].push_back(make_pair(i + 1, 0));
    g[0].push_back(make_pair(M, W));
    n = M;
    
    for (int i = 0; i < M; i++)
    {
        g[n].push_back(make_pair(n + 2, W >> i));
        g[n].push_back(make_pair(n + 1, 0));
        g[n + 1].push_back(make_pair(n + 2, 0));
        n += 2;
    }
    n++;
    for (int i = 0; i < n; i++)
        random_shuffle(g[i].begin(), g[i].end());
    
    return g;
}


Path CalculateShortestPathsWithFordBellman(const Graph& graph,
                                           int source, int destination) {
    vector<int> parents(graph.size(), -1);
    vector<Distance> distances(graph.size(), kInfDistance);
    distances[source] = kZeroDistance;
    for (int iter = 1; ; ++iter) {
        bool wasModified = false;
        for (int i = 0; i < graph.size(); ++i) {
            for (const auto& edge : graph[i]) {
                if (distances[i] != kInfDistance) {
                    Distance upd = distances[i] + edge.second;
                    if (upd < distances[edge.first]) {
                        distances[edge.first] = upd;
                        parents[edge.first] = i;
                        wasModified = true;
                    }
                }
            }
        }
        if (!wasModified) {
            break;
        }
        if (iter == graph.size()) {
            return Path{{}, kInfDistance};
        }
    }
    Path res;
    for (int cur = destination; cur != -1; cur = parents[cur]) {
        res.topology.push_back(cur);
    }
    reverse(res.topology.begin(), res.topology.end());
    res.distance = distances[destination];
    return res;
}


class AntColonyOptimizationForSSSPP {
public:
    void Initialize(const Graph& graph, int source, int destination,
                    int seed,
                    double initialPheromoneRate, int iterationsNumber,
                    int antsNumber, int eliteAntsNumber,
                    double alphaParam, double betaParam,
                    double evaporationParam, Distance answerOrder) {
        this->graph = graph;
        this->source = source;
        this->destination = destination;
        this->seed = seed;
        this->initialPheromoneRate = initialPheromoneRate;
        this->iterationsNumber = iterationsNumber;
        this->antsNumber = antsNumber;
        this->eliteAntsNumber = eliteAntsNumber;
        this->alphaParam = alphaParam;
        this->betaParam = betaParam;
        this->evaporationParam = evaporationParam;
        this->answerOrder = answerOrder;
        
        gen = mt19937(seed);
        pheromone.clear();
        pheromone.resize(graph.size());
        for (int i = 0; i < graph.size(); ++i) {
            pheromone[i].assign(graph[i].size(), initialPheromoneRate);
        }
        bestPath = Path{{}, kInfDistance};
    }
    
    void Optimize() {
        for (int iter = 0; iter < iterationsNumber; ++iter) {
            vector<Path> antPaths;
            antPaths.reserve(antsNumber);
            for (int i = 0; i < antsNumber; ++i) {
                antPaths.push_back(SimulateAnt());
                if (antPaths.back().distance < bestPath.distance) {
                    bestPath = antPaths.back();
                }
            }
            for (int i = 0; i < graph.size(); ++i) {
                for (int j = 0; j < graph[i].size(); ++j) {
                    pheromone[i][j] *= (1.0 - evaporationParam);
                }
            }
            for (const auto& path : antPaths) {
                for (int i = 0, cur = source; i < path.topology.size(); ++i) {
                    pheromone[cur][path.topology[i]]
                        += 1.0 * answerOrder / path.distance;
                    cur = graph[cur][path.topology[i]].first;
                }
            }
            for (int i = 0, cur = source; i < bestPath.topology.size(); ++i) {
                pheromone[cur][bestPath.topology[i]]
                    += 1.0 * eliteAntsNumber * answerOrder / bestPath.distance;
                cur = graph[cur][bestPath.topology[i]].first;
            }
        }
    }
    
    const vector<vector<double>>& GetPheromone() const {
        return pheromone;
    }
    
    void SetPheromone(const vector<vector<double>>& pheromone) {
        this->pheromone = pheromone;
    }
    
    Path GetBestPath() const {
        Path res;
        res.topology.push_back(source);
        for (int i = 0, cur = source; i < bestPath.topology.size(); ++i) {
            cur = graph[cur][bestPath.topology[i]].first;
            res.topology.push_back(cur);
        }
        res.distance = bestPath.distance;
        return res;
    }
    
private:
    Path SimulateAnt() {
        vector<char> visited(graph.size(), false);
        visited[source] = true;
        Path res;
        res.distance = kZeroDistance;
        for (int cur = source; cur != destination; ) {
            vector<int> allowed;
            vector<double> probabilities;
            for (int index = 0; index < graph[cur].size(); ++index) {
                if (!visited[graph[cur][index].first]) {
                    allowed.push_back(index);
                    probabilities.push_back(
                        pow(pheromone[cur][index], alphaParam)
                            * pow(graph[cur][index].second, betaParam));
                }
            }
            if (allowed.empty()) {
                res.distance = kInfDistance;
                return res;
            }
            int toIndex = allowed[discrete_distribution<>
                (probabilities.begin(), probabilities.end())(gen)];
            visited[graph[cur][toIndex].first] = true;
            res.topology.push_back(toIndex);
            res.distance += graph[cur][toIndex].second;
            cur = graph[cur][toIndex].first;
        }
        return res;
    }
    
private:
    Graph graph;
    int source;
    int destination;
    
    int seed;
    double initialPheromoneRate;
    int iterationsNumber;
    int antsNumber;
    int eliteAntsNumber;
    double alphaParam;
    double betaParam;
    double evaporationParam;
    Distance answerOrder;
    
    mt19937 gen;
    vector<vector<double>> pheromone;
    Path bestPath;
};





int main() {
    const int kGraphSize = 100;
    const int kSeed = 28;
    const double kDensity = 0.7;
    
    Graph g = GenerateRandomGraph(kGraphSize, kDensity, kSeed);
    auto best = CalculateShortestPathsWithFordBellman(g, 0, kGraphSize - 1);
    cerr << best.distance << endl;
    
    
    AntColonyOptimizationForSSSPP algo;
    algo.Initialize(g, 0, kGraphSize - 1,
                    kSeed, kMaxWeight, 100, 100, 5, 1.0, -2.0, 0.55, 5000);
    algo.Optimize();
    cerr << algo.GetBestPath().distance;
    
    
    return 0;
}
