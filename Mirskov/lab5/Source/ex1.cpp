#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

class Vertex {
public:
    Vertex(Vertex *par, char symb = 'A'): symbol(symb), parent(par), start(par->start) {
        par->nextBor[symb] = this;
        answer = par->answer;
        arr = par->arr;
    }

    Vertex (std::set<std::pair<int,int>>* mainAnswer, std::vector<int>* mainArr):
    answer(mainAnswer), arr(mainArr) {
        start = this;
        link = this;
        parent = this;
    }

    Vertex* getLink() {
        if (link == nullptr)
            if (this == start || parent == start)
                link = start;
            else
                link = parent->getLink()->go(symbol);
        return link;
    }

    Vertex* go(char symb) {
        if (nextAuto[symb] == nullptr) {
            if (nextBor[symb] != nullptr)
                nextAuto[symb] = nextBor[symb];
            else
                nextAuto[symb] = this == start ? this : this->getLink()->go(symb);
        }
        return nextAuto[symb];
    }

    Vertex* next(char symb) {
        Vertex* newVertex =  nextBor[symb];
        if (newVertex == nullptr) {
            newVertex = new Vertex(this, symb);
        }
        return newVertex;
    }

    Vertex* fastLink() {
        if (f_link == nullptr) {
            Vertex* u = getLink();
            if (u == start)
                f_link = start;
            else
                f_link = (u->leaf) ? u : u->fastLink();
        }
        return f_link;
    }

    void check(int ind) {
        if (this == start) return;
        if (leaf) {
            answer->insert(std::make_pair(ind - (*arr)[leafInd] + 2, leafInd+1));
            std::cout << "found string with index " << leafInd << '\n';
        }
        fastLink()->check(ind);
    }

    void setLeaf() {
        leaf = true;
    }

    void setInd(int ind) {
        leafInd = ind;
    }

    char getSymbol() {
        return symbol + 'A';
    }

    void printVertex(std::string path) {
        if (this == start) std::cout << "this is root\n";
        if (this != start) path += symbol + 'A';
        std::cout << "path: " << path << '\n';
        std::cout << "is termenated = " << (leaf ? "true\n" : "false\n");
        if (link == start) std::cout << "suff link to root\n";
        else if (link) std::cout << "suff link to " << link->getSymbol() << '\n';
        std::cout << '\n';

        for (auto i: nextBor) if (i) i->printVertex(path);
    }

private:
    char symbol;
    Vertex* parent = this;
    Vertex* link = nullptr;
    Vertex* f_link = nullptr;
    Vertex* start = nullptr;
    std::vector<Vertex*> nextBor = std::vector<Vertex*>(26, nullptr);
    std::vector<Vertex*> nextAuto = std::vector<Vertex*>(26, nullptr);
    bool leaf = false;
    int leafInd;
    std::set<std::pair<int,int>>* answer;
    std::vector<int>* arr;
};

std::vector<std::pair<std::string, int>> cutString(std::string& s, int k, int sampleLen) {
    std::vector<std::pair<std::string, int>> cutStrings;
    int strLen = s.length();
    sampleLen--;
    int oneStringLen = (strLen - sampleLen) / k + sampleLen;
    if (sampleLen >= oneStringLen - 1) {
        std::cout << "k is too big\nno cut\n";
        cutStrings.push_back({s, 0});
        return cutStrings;
    }
    int plusOneCount = (strLen - sampleLen) % k;
    int ind = 0;
    for (int i = 0; i < plusOneCount; i++) {
        cutStrings.push_back(std::make_pair(s.substr(ind, oneStringLen + 1), ind));
        ind += oneStringLen + 1 - sampleLen;
    }
    for (int i = 0; i < k - plusOneCount; i++) {
        cutStrings.push_back(std::make_pair(s.substr(ind, oneStringLen), ind));
        ind += oneStringLen - sampleLen;
    }

    return cutStrings;
}

void addString(Vertex* suffAuto, std::string& s, int ind) {
    std::cout << "add string: " << s << '\n';
    Vertex* vertex = suffAuto;
    for (char c:s) {
        c -= 'A';
        if (vertex->next(c) == nullptr) {
            auto *newVertex = new Vertex(vertex, c);
        }
        vertex = vertex->next(c);
    }
    vertex->setLeaf();
    vertex->setInd(ind);
}

int read(std::string& text, Vertex* suffAuto, std::vector<int>* arr, int& k) {
    std::cin >> text;
    int maxSampleLen = 0;

    int n; std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::string p; std::cin >> p;
        maxSampleLen = std::max(maxSampleLen, (int)p.length());
        addString(suffAuto, p, i);
        arr->push_back(p.length());
    }

    std::cin >> k;

    return maxSampleLen;
}

void getAnswer(std::string& text, Vertex* suffAuto, std::set<std::pair<int,int>>* answer, int k, int maxSampleLen) {
    std::cout << "WORK:\n";
    Vertex* curVertex = suffAuto;
    for (auto pair: cutString(text, k, maxSampleLen)) {
        std::string textPart = pair.first;
        int ind = pair.second;
        std::cout << "work with " << textPart << '\n';
        for (char c: textPart) {
            std::cout << "current symbol: " << c << '\n'; 
            curVertex = curVertex->go(c-'A');
            curVertex->check(ind);
            ind++;
        }
    }
}

void writeAnswer(Vertex* suffAuto, std::vector<int>* arr, std::set<std::pair<int,int>>* answer, std::string& text) {
    std::cout << '\n';
    std::cout << "SUFFIX AUTO:\n";
    suffAuto->printVertex("");
    std::cout << "ANSWER:\n";
    for (auto j:(*answer))
        std::cout << j.first << ' ' << j.second << '\n';
}

int main() {
    auto* answer = new std::set<std::pair<int,int>>;
    auto* arr = new std::vector<int>;
    auto* suffAuto = new Vertex(answer, arr);
    std::string text;
    int k;
    int maxSampleLen = read(text, suffAuto, arr, k);

    getAnswer(text, suffAuto, answer, k, maxSampleLen);

    writeAnswer(suffAuto, arr, answer, text);
}