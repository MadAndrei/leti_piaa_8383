#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>

class Edge {
public:
    char start;
    char end;
    int weight;
    bool operator<(Edge edge) const {
        if (this->start < edge.start) return true;
        if (this->start == edge.start && this->end < edge.end) return true;
        return this->start == edge.start && this->end == edge.end && this->weight < edge.weight;
    }
};

class Graph {
public:
    bool isEdge(char u, char v) {
        return this->data[u].count(v);
    }

    void addEdge(char u, char v, int w) {
        this->data[u][v] = w;
    }

    std::vector<Edge> getEdges(char u) {
        std::vector<Edge> edgesFromU;
        for (auto nextEdge:this->data[u]) {
            edgesFromU.push_back(Edge({u, nextEdge.first, nextEdge.second}));
        }
        return edgesFromU;
    }

    int getValue(char u, char v) {
        return this->data[u][v];
    }

    void addValue(char u, char v, int value) {
        this->data[u][v] += value;
    }

    void print() {
        for (auto& in: data) {
            for (auto& to: in.second) {
                std::cout << in.first << ' ' << to.first << ' ' << to.second << '\n';
            }
        }
    }
private:
    std::map<char, std::map<char, int>> data;
};

void read_graph(Graph* graph, std::set<std::pair<char,char>>& edges, int n) {
    char u, v;
    int w;
    for (int i = 0; i < n; i++) {
        std::cin >> u >> v >> w;
        edges.insert({u,v});
        graph->addEdge(u,v,w); // прямое ребро
        if (!graph->isEdge(v,u)) {
        	graph->addEdge(v,u,0); // обратное
        }
    }
}

bool bfs(Graph* graph, int start, int finish, std::map<char, char>& parents) {
    std::queue<char> q;
    q.push(start);
    parents[start] = start;
    char v;
    while (!q.empty()) {
        v = q.front();
        q.pop();
        for (Edge edge:graph->getEdges(v)) {
            char to = edge.end;
            int w = edge.weight;
            if (parents.count(to) == 0 && w > 0) {
                q.push(to);
                parents[to] = v;
                if (to == finish) break;
            }
        }
        if (parents.count(finish)) break;
    }
    return !q.empty();
}

int minWeightOnCurrentPath(Graph* graph, std::map<char, char>& parents, char finish) {
    char prevVert = finish;
    char curVert = parents[prevVert];
    int weight = graph->getValue(curVert, prevVert);
    std::vector<char> path;
    path.push_back(finish);
    while (prevVert != curVert) {
        path.push_back(curVert);
        weight = std::min(weight, graph->getValue(curVert, prevVert));
        curVert = parents[curVert];
        prevVert = parents[prevVert];
    }
    std::cout << "Found new available path\n";
    for (auto i = path.rbegin(); i != path.rend(); i++) {
        std::cout << (*i) << ' ';
    }
    std::cout << '\n' << "minimum weight on the path is " << weight << "\n";
    return weight;
}

void changeWeights(Graph* graph, std::map<char, char>& parents, char finish,
 int flow, std::map<std::pair<char,char>,int>& answer) {
    char prevVert = finish;
    char curVert = parents[prevVert];
    while (prevVert != curVert) {
        graph->addValue(curVert, prevVert, -flow);
        answer[{curVert, prevVert}] += flow;
        graph->addValue(prevVert, curVert, flow);
        curVert = parents[curVert];
        prevVert = parents[prevVert];
    }
    graph->print();
    std::cout << '\n';
}

void fordFulkerson(Graph* graph, int& maxFlow, char start, char finish, std::map<std::pair<char, char>, int>& answer) {
    bool isWayExist = true;
    while (isWayExist) {
        std::map<char, char> parents;
        isWayExist = bfs(graph, start, finish, parents);
        if (isWayExist) {
            int flow = minWeightOnCurrentPath(graph, parents, finish);
            maxFlow += flow;
            changeWeights(graph, parents, finish, flow, answer);
        }
    }
}

void writeAnswer(std::set<std::pair<char, char>>& edges, std::map<std::pair<char, char>, int> answer, int maxFlow) {
    std::cout << maxFlow << '\n';
    for (auto edge: edges) {
        char u = edge.first;
        char v = edge.second;
        std::cout << u << ' ' << v << ' ';
        if (edges.count({v,u})) {
            std::cout << answer[{u, v}] << '\n';
        }
        else {
            std::cout << answer[{u, v}] - answer[{v, u}] << '\n';
        }
    }
}

int main() {
    // ввод данных
    int n;
    std::cin >> n;
    char start, finish;
    std::cin >> start >> finish;

    auto* graph = new Graph;
    std::set<std::pair<char, char>> edges;
    read_graph(graph, edges, n);

    int maxFlow = 0;
    std::map<std::pair<char,char>,int> answer;
    fordFulkerson(graph, maxFlow, start, finish, answer);

    writeAnswer(edges, answer, maxFlow);

    delete graph;
}