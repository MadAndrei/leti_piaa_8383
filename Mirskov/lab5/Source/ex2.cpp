#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

char joker;
int textSize;
int obrSize;

class Vertex {
public:
    Vertex(Vertex *par, char symb = 'A'): symbol(symb), parent(par), start(par->start) {
        par->nextBor[symb] = this;
        answer = par->answer;
        arr = par->arr;
        pos = par->pos;
        c_arr = par->c_arr;
    }

    Vertex (std::vector<std::pair<int,int>>* mainAnswer, std::vector<int>* mainArr, 
            std::vector<int>* mainc_arr, std::vector<int>* mainPos):
            answer(mainAnswer), arr(mainArr), pos(mainPos), c_arr(mainc_arr) {
        start = this;
        parent = this;
        link = this;
        f_link = this;
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

    Vertex* next(char symb, bool flag) {
        Vertex* newVertex =  nextBor[symb];
        if (newVertex == nullptr) {
            if (flag) std::cout << "create new vertex\n";
            newVertex = new Vertex(this, symb);
        }
        else 
            if (flag) std::cout << "go to old vertex\n";
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
        for (auto leafInd: leafInds) {
    		int ind_in_c = ind - (*pos)[leafInd] - (*arr)[leafInd] + 1;
            std::cout << "found new index " << ind_in_c << '\n';
    		if (ind_in_c >= 0 && ind_in_c + obrSize <= textSize) {
    			(*c_arr)[ind_in_c]++;
                std::cout << "inc c[" << ind_in_c << "]\n";
            }
            else std::cout << "not in range\n";
    	}
        fastLink()->check(ind);
    }

    void setLeaf() {
        leaf = true;
    }

    void setInd(int ind) {
        leafInds.push_back(ind);
    }

private:
    char symbol;
    Vertex* parent = this;
    Vertex* link = nullptr;
    Vertex* f_link = nullptr;
    Vertex* start = nullptr;
    std::vector<Vertex*> nextBor = std::vector<Vertex*>(26, nullptr);
    std::vector<Vertex*> nextAuto = std::vector<Vertex*>(26, nullptr);
    std::vector<int>* pos;
    std::vector<int>* c_arr;
    bool leaf = false;
    std::vector<std::pair<int,int>>* answer;
    std::vector<int>* arr;
    std::vector<int> leafInds;
};

void addString(Vertex* suffAuto, std::string& s, int ind) {
    Vertex* vertex = suffAuto;
    for (char c: s) {
        std::cout << "add new symbol " << c << "\n";
        c -= 'A';
        if (vertex->next(c,true) == nullptr) {
            auto *newVertex = new Vertex(vertex, c);
        }
        vertex = vertex->next(c,false);
    }
    vertex->setLeaf();
    vertex->setInd(ind);
}

int read(std::string& text, Vertex* suffAuto, std::vector<int>* arr, std::vector<int>* pos) {
    std::cin >> text;
    textSize = text.length();

    int n = 1;
    int k = 0;
    for (int i = 0; i < n; i++) {
        std::string p; std::cin >> p; obrSize = p.length();
        std::cin >> joker;
        std::string partP;
        for (int j = 0; j < obrSize; j++) {
            if (p[j] == joker) {
                if (!partP.empty()) {
                	(*pos).push_back(j - partP.length());
                    addString(suffAuto, partP, k);
                    arr->push_back(partP.length());
                    partP.clear();
                    k++;
                }
            }
            else {
                partP.push_back(p[j]);
            }
        }
        if (!partP.empty()) {
        	(*pos).push_back(obrSize - partP.length());
            addString(suffAuto, partP, k);
            arr->push_back(partP.length());
            k++;
        }
    }
    return k;
}

void getAnswer(std::string& text, Vertex* suffAuto, std::vector<std::pair<int,int>>* answer) {
    int ind = 0;
    Vertex* curVertex = suffAuto;
    for (char c: text) {
        std::cout << "work with symbol " << c << '\n';
        curVertex = curVertex->go(c-'A');
        curVertex->check(ind);
        ind++;
    }
    std::sort(answer->begin(), answer->end());
}

void writeAnswer(std::vector<std::pair<int,int>>* answer) {
    for (auto j:(*answer))
        std::cout << j.first << ' ' << j.second << '\n';
}

int main() {
    auto* answer = new std::vector<std::pair<int,int>>;
    auto* arr = new std::vector<int>;
    std::string text;

    std::vector<int>* c_arr = new std::vector<int>;
    std::vector<int>* pos = new std::vector<int>;

    auto* suffAuto = new Vertex(answer, arr, c_arr, pos);

    int k = read(text, suffAuto, arr, pos);
    (*c_arr) = std::vector<int>(textSize, 0);
    std::cout << "-------------------------\n";

    getAnswer(text, suffAuto, answer);

    std::cout << "result array c: ";
    for (auto i: (*c_arr)) std::cout << i << ' ';
    std::cout << '\n';

    std::cout << "answer:\n";
    for (int i = 0; i < textSize; i++) {
        if ((*c_arr)[i] == k) {
            std::cout << i+1 << '\n';
        }
    }
}