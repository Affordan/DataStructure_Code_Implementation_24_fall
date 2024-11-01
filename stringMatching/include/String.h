#include <algorithm>
#include <chrono>
#include <iostream>
#include <istream>
#include <iterator>
#include <ostream>
#include <random>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include <thread>        // 添加线程支持
#include <numeric>       // 用于 accumulate
using namespace std;

string getString(string promot = "");
vector<vector<int>> buildDfa(const string &s);
vector<int> buildRight(const string &s);
vector<int> buildNext(const string &pat, const vector<vector<int>> dfa);
template <typename Container> void print(Container con);

class String {
  protected:
    string pat;
    vector<vector<int>> dfa;
    /*
     * next state is actually the state simulation record of dfa
     * on pattern itself.
     */
    vector<int> next;
    vector<int> right;

  public:
    String() {
        pat = "";
    };
    String(const string &s);
    virtual ~String() = 0 {};

  public:
    int BruteForce(const string &s);
    virtual int DFAsearchIn(const string &text) {
        return -1;
    };
    virtual int DFAsearchIn(const string &text, int &count) {
        return -1;
    };
    virtual int BMsearchIn(const string &text) {
        return -1;
    };
    virtual int BMsearchIn(const string &text, int &count) {
        return -1;
    };
    void printNext() {
        print(next);
    };
    // int BMOptimizedSearch(const string& s);
    string getPat();

  private:
    // void printDFA() ;
};

class kmpString : public String {
  private:
  public:
    kmpString() {};
    kmpString(const string &s) : String(s) {
        dfa = buildDfa(s);
        next = buildNext(s, dfa);
    };
    virtual int DFAsearchIn(const string &text) override;
    virtual int DFAsearchIn(const string &text, int &count) override;
};

class bmString : protected String {
  public:
    bmString(const string &s) : String(s) {
        right = buildRight(s);
    };
    virtual int BMsearchIn(const string &text) override;
    virtual int BMsearchIn(const string &text, int &count) override;
};

inline String::String(const string &s) {
    this->pat = s;
}

inline int String::BruteForce(const string &text) {
    int M(pat.size()), N(text.size());
    int i(M - 1), j(M - 1);
    for (; i < N - M; i++) {
        for (j = M - 1; j >= 0; j--) {
            if (text.at(i + j) != pat.at(j))
                break;
        }
        if (j == 0)
            return i;
    }
    return -1;
}

int kmpString::DFAsearchIn(const string &text, int &count) {
    count = 0;
    size_t M = pat.size();
    size_t N = text.size();
    size_t i = 0;
    size_t j = 0;

    for (; i < N && j < M; i++) {
        count++;
        j = dfa[text[i]][j];
    }
    if (j == M)
        return static_cast<int>(i - M);
    return -1;
}

int kmpString::DFAsearchIn(const string &text) {
    int count = 0;
    return DFAsearchIn(text, count);
}

int bmString::BMsearchIn(const string &text, int &count) {
    count = 0;
    size_t M = pat.size();
    size_t N = text.size();
    size_t skip = 0;

    for (size_t i = 0; i < N - M; i += skip) {
        skip = 0;
        for (size_t j = M - 1; j < M; j--) {
            count++;
            if (text[i + j] != pat[j]) {
                skip = max<size_t>(1, j - right[text[i + j]]);
                break;
            }
        }
        if (skip == 0)
            return static_cast<int>(i);
    }
    return -1;
}

int bmString::BMsearchIn(const string &text) {
    int count = 0;
    return BMsearchIn(text, count);
}

string String::getPat() {
    if (pat.empty())
        throw runtime_error("string is empty,initialize it!\n");

    return pat;
}

string getString(string prompt) {

    string line;
    cout << prompt;

    // eof
    // while (!getline(cin, line))
    //{
    //	cin.clear();
    //	cout << prompt;
    //	//throw domain_error("End of file\n");
    //}

    getline(cin, line);
    return line;
}
vector<vector<int>> buildDfa(const string &s) {
    int M = s.size();
    int R = 256; // character type numbers
    vector<vector<int>> dfa(R, vector<int>(M, 0));
    dfa[s.at(0)][0] = 1; // transform to next state

    // X means the restart state
    for (int X = 0, i = 0; i < M; i++) {
        for (int c = 0; c < R; c++)
            dfa[c][i] = dfa[c][X];
        dfa[s.at(i)][i] = i + 1;
        X = dfa[s.at(i)][X];
    }
    return dfa;
}
vector<int> buildRight(const string &s) {
    int size = s.size();
    int R = 256; // all possible characters!
    vector<int> index(R, -1);
    for (int i = 0; i < size; i++) {
        index[s.at(i)] = i;
    }

    return index;
}

inline vector<int> buildNext(const string &pat, const vector<vector<int>> dfa) {
    int M = pat.size();
    vector<int> next(M, 0);
    next.at(0) = 0;
    for (int X = 0, i = 1; i < M; i++) {
        next.at(i) = dfa.at(pat.at(i)).at(X);
        X = dfa.at(pat.at(i)).at(X);
    }
    return next;
}

template <typename Container> inline void print(Container con) {
    using Type = ranges::range_value_t<decltype(con)>;
    copy(con.begin(), con.end(), ostream_iterator<Type>(cout, " "));
    cout << endl;
}
