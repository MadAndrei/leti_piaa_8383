#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

class Vertex {
public:
    Vertex(Vertex *par, char symb = 'A'): symbol(symb), parent(par), start(par->start) {
        par->nextBor[symb] = this;
        answer = par->answer;
        arr = par->arr;
    }

    Vertex (std::vector<std::pair<int,int>>* mainAnswer, std::vector<int>* mainArr):
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
            answer->emplace_back(ind - (*arr)[leafInd] + 2, leafInd+1);
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

    int countVert() {
        int maxEdge = 0;
        for (auto i: nextBor) maxEdge += (i!=nullptr);
        for (auto i: nextBor) if (i) maxEdge = std::max(maxEdge, i->countVert());
        return maxEdge;
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
    std::vector<std::pair<int,int>>* answer;
    std::vector<int>* arr;
};

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

void read(std::string& text, Vertex* suffAuto, std::vector<int>* arr) {
    std::cin >> text;

    int n; std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::string p; std::cin >> p;
        addString(suffAuto, p, i);
        arr->push_back(p.length());
    }
}

void getAnswer(std::string& text, Vertex* suffAuto, std::vector<std::pair<int,int>>* answer) {
    int ind = 0;
    std::cout << "WORK:\n";
    Vertex* curVertex = suffAuto;
    for (char c: text) {
        std::cout << "current symbol: " << c << '\n'; 
        curVertex = curVertex->go(c-'A');
        curVertex->check(ind);
        ind++;
    }
    std::sort(answer->begin(), answer->end());
}

void writeAnswer(Vertex* suffAuto, std::vector<int>* arr, std::vector<std::pair<int,int>>* answer, std::string& text) {
    std::cout << '\n';
    std::cout << "SUFFIX AUTO:\n";
    suffAuto->printVertex("");
    std::cout << "ANSWER:\n";
    std::cout << "maximum edge = " << suffAuto->countVert() << '\n';
    for (auto i: (*answer)) {
        for (int j = i.first-1; j < i.first-1 + (*arr)[i.second-1]; j++) {
            text[j] = '$';
        }
    }
    std::cout << "cut string: ";
    for (auto i: text) 
        if (i != '$')
            std::cout << i;
    std::cout << "\n";
    for (auto j:(*answer))
        std::cout << j.first << ' ' << j.second << '\n';
}

int main() {
    auto* answer = new std::vector<std::pair<int,int>>;
    auto* arr = new std::vector<int>;
    auto* suffAuto = new Vertex(answer, arr);
    std::string text;

    read(text, suffAuto, arr);

    getAnswer(text, suffAuto, answer);

    writeAnswer(suffAuto, arr, answer, text);
}