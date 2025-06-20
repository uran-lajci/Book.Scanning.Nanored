#include "utils.h"

#define MUL 5
#define ADD 5

int main() {
    std::cout.tie(nullptr);
    std::ios::sync_with_stdio(false);

    Problem P = read_input();
    int B = P.B, L = P.L, D = P.D;
    std::vector<int> S = P.S;
    std::vector<int> N = P.N, T = P.T, M = P.M;
    std::vector<std::vector<int>> books = P.books;
    std::vector<std::vector<int>> libraries = P.libraries;

    std::priority_queue<std::pair<int, int>> q;
    
    for (int i = 0; i < L; i++) {
        N[i] *= MUL;
    }
    
    for (int i = 0; i < B; i++) {
        if (libraries[i].size() == 2) {
            for (int l : libraries[i]) {
                N[l] += ADD;
            }
        }
    }
    
    for (int i = 0; i < L; i++) {
        q.push({N[i], i});
    }
    
    int t = 0;
    std::vector<int> Y;
    std::vector<bool> seen(B, false);
    std::vector<bool> seenL(L, false);
    std::vector<std::vector<int>> k;
    int score = 0;
    
    while (t < D) {
        std::pair<int, int> a;
        while (true) {
            a = q.top();
            if (a.first == N[a.second]) {
                break;
            }
            q.pop();
        }
        q.pop();
        int lib = a.second;
        Y.push_back(lib);
        k.push_back({});
        seenL[lib] = true;
        
        for (int b : books[lib]) {
            if (!seen[b]) {
                seen[b] = true;
                for (int l : libraries[b]) {
                    N[l] -= MUL;
                    if (libraries[b].size() == 2) {
                        N[l] -= ADD;
                    }
                    if (!seenL[l]) {
                        q.push({N[l], l});
                    }
                }
                if (t + 2 + k.back().size() < D) {
                    k.back().push_back(b);
                    score++;
                }
            }
        }
        
        if (k.back().empty()) {
            k.pop_back();
            Y.pop_back();
        }
        t += 2;
    }

    output_solution(Y, k);
    std::cerr << score * S[0] << std::endl;
    return 0;
}