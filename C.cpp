#include "utils.h"

#define ADD 90
#define POW 1.0

int main() {
    std::cout.tie(nullptr);
    std::ios::sync_with_stdio(false);

    Problem P = read_input();
    int B = P.B, L = P.L, D = P.D;
    std::vector<int> S = P.S;
    std::vector<int> N = P.N, T = P.T, M = P.M;
    std::vector<std::vector<int>> books = P.books;
    std::vector<std::vector<int>> libraries = P.libraries;

    std::priority_queue<std::pair<double, int>> q;
    std::vector<double> score_lib(L, 0);
    double mean_score = 0;
    
    for (int i = 0; i < B; i++) {
        mean_score += S[i];
    }
    mean_score /= B;
    
    for (int i = 0; i < L; i++) {
        for (int b : books[i]) {
            score_lib[i] += S[b];
        }
        score_lib[i] -= (0.09 * mean_score) * double(T[i]);
        score_lib[i] /= (ADD + std::pow(double(T[i]), POW));
        q.push({score_lib[i], i});
    }
    
    int t = 0;
    int SCORE = 0;
    std::vector<int> Y;
    std::vector<bool> seen(B, false);
    std::vector<bool> seenL(L, false);
    std::vector<std::vector<int>> k;
    
    while (t < D) {
        std::pair<double, int> a;
        int lib;
        
        while (!q.empty()) {
            a = q.top();
            lib = a.second;
            if (t + T[lib] + 1 < D && !seenL[lib] && std::abs(a.first - score_lib[a.second]) < 1e-5) {
                break;
            }
            q.pop();
        }
        
        if (q.empty()) {
            break;
        }
        q.pop();
        Y.push_back(lib);
        k.push_back({});
        seenL[lib] = true;
        
        for (int b : books[lib]) {
            if (!seen[b]) {
                seen[b] = true;
                for (int l : libraries[b]) {
                    score_lib[l] -= double(S[b]) / (ADD + std::pow(double(T[l]), POW));
                    if (!seenL[l]) {
                        q.push({score_lib[l], l});
                    }
                }
                k.back().push_back(b);
                SCORE += S[b];
            }
        }
        t += T[lib];
    }

    output_solution(Y, k);
    std::cerr << SCORE << std::endl;
    return 0;
}