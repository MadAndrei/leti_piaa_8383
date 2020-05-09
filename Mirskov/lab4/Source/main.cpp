#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

vector<int> prefixFunction(string s) {
    cout << "work with " << s << '\n';
    int n = s.length();
    vector<int> pi(n,0);
    for (int i = 1; i < n; i++) {
        cout << "calculate pi[" << i << "]\n";
        int curLength = pi[i-1];
        if (curLength > 0) cout << "compare symbols " << curLength << ' ' << i << '\n';
        while (curLength > 0 && s[i] != s[curLength]) {
            cout << "not equal\n";
            curLength = pi[curLength - 1];
            if (curLength > 0) cout << "compare symbols " << curLength << ' ' << i << '\n';
        }
        if (curLength == 0) cout << "curLength = 0\n";
        else cout << "equal\n";
        cout << "compare symbols " << curLength << ' ' << i << '\n';
        if (s[curLength] == s[i]) {
            cout << "equal\n";
            curLength++;
        }
        else cout << "not equal\n";
        pi[i] = curLength;
    }
    cout << "pi: ";
    for (auto i:pi) cout << i << ' ';
    cout << "\n\n";
    return pi;
}

vector<string> cutString(string& s, int k, int sampleLen) {
    vector<string> cutStrings;
    int strLen = s.length();
    sampleLen--;
    int oneStringLen = (strLen - sampleLen) / k + sampleLen;
    if (sampleLen >= oneStringLen - 1) {
        cout << "k is too big\nno cut\n";
        cutStrings.push_back(s);
        return cutStrings;
    }
    int plusOneCount = (strLen - sampleLen) % k;
    int ind = 0;
    for (int i = 0; i < plusOneCount; i++) {
        cutStrings.push_back(s.substr(ind, oneStringLen + 1));
        ind += oneStringLen + 1 - sampleLen;
    }
    for (int i = 0; i < k - plusOneCount; i++) {
        cutStrings.push_back(s.substr(ind, oneStringLen));
        ind += oneStringLen - sampleLen;
    }

    return cutStrings;
}

set<int> findString(string& p, string& t) {
    vector<string> cutStrings = cutString(t, 2, p.length());
    cout << "strings for parallel algorithm:\n";
    for (auto& i:cutStrings) cout << i << '\n';
    cout << '\n';
    set<int> indexes;
    int curIndex = 0;
    for (string& partOfT:cutStrings) {
        string newString = p + '#' + partOfT;

        vector<int> pi = prefixFunction(newString);
        int pLength = p.length(), tLength = t.length();

        for (int i = pLength * 2; i < pLength + tLength + 1; i++)
            if (pi[i] == pLength)
                indexes.insert(i - 2 * pLength + curIndex);
        curIndex += partOfT.length() - p.length() + 1;
    }
    return indexes;
}

int cycle(string& a, string& b) {
    if (a.length() != b.length()) return -1;
    a += a;
    set<int> indexes = findString(b, a);
    if (indexes.empty()) return -1;
    return *indexes.begin();
}

int main() {
    string a, b;
    cin >> a >> b;

    // для первой задачи
    set<int> answer = findString(a,b);
    if (answer.empty()) cout << -1;
    else {
        auto answerEnd = answer.end();
        answerEnd--;
        for (auto i = answer.begin(); i != answerEnd; i++) {
            cout << *i << ',';
        }
        cout << *answerEnd;
    }
    cout << '\n';

    // для второй задачи
    // int answer = cycle(a,b);
    // cout << answer << '\n';
}