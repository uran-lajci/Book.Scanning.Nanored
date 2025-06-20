#include "utils.h"

#define TIME_MUL 0.028
#define D_POW 1.55
#define DC 0.26

int main() {
    std::cout.tie(nullptr);
    std::ios::sync_with_stdio(false);

    Problem P = read_input();
    int B = P.B, L = P.L, D = P.D;
    std::vector<int> S = P.S;
    std::vector<int> N = P.N, T = P.T, M = P.M;
    std::vector<std::vector<int>> books = P.books;
    std::vector<std::vector<int>> libraries = P.libraries;

    std::vector<std::vector<int>> inLib(B);
    std::priority_queue<std::pair<double, int>> q;
    std::vector<double> score_lib(L, 0);
    double sm = 0, smt = 0;
    std::vector<int> last_book(L);
    long long mean_score = 0;
    long long mean_time = 0;
    
    for (int i = 0; i < L; i++) {
        sm += M[i];
        smt += M[i] * T[i];
        std::sort(books[i].begin(), books[i].end(), [&S](int b0, int b1) {
            return S[b0] > S[b1];
        });
        int n = std::min(N[i], M[i] * (D - T[i]));
        last_book[i] = n;
        for (int j = 0; j < n; j++) {
            score_lib[i] += S[books[i][j]];
        }
        for (int j = 0; j < N[i]; j++) {
            inLib[books[i][j]].push_back(j);
        }
        mean_score += score_lib[i];
        mean_time += T[i];
    }
    
    int ratio = TIME_MUL * double(mean_score / mean_time);
    mean_time /= L;
    
    for (int i = 0; i < L; i++) {
        score_lib[i] = double(score_lib[i] - ratio * T[i]) / (DC * double(mean_time) + std::pow(double(T[i]), D_POW));
        q.push({score_lib[i], i});
    }

    int t = 0;
    int SCORE = 0;
    std::vector<int> Y;
    std::vector<bool> seen(B, false);
    std::vector<bool> seenL(L, false);
    std::vector<std::vector<int>> k;
    bool recomp = false;
    
    while (t < D) {
        std::pair<double, int> a;
        int lib;
        
        while (!q.empty()) {
            a = q.top();
            lib = a.second;
            if (t + T[lib] < D && !seenL[lib] && std::abs(a.first - score_lib[lib]) < 1e-5) {
                int n = M[lib] * (D - t - T[lib]);
                if (!recomp && N[lib] <= n) {
                    break;
                }
                recomp = true;
                score_lib[lib] = -ratio * T[lib];
                double m = 0, m2 = 0, nm = 0;
                
                for (int i = 0; i < N[lib]; i++) {
                    if (i >= n) {
                        break;
                    }
                    int b = books[lib][i];
                    if (seen[b]) {
                        n++;
                    }
                    else {
                        m += S[b];
                        m2 += S[b] * S[b];
                        nm++;
                    }
                }
                
                score_lib[lib] += m;
                m2 /= nm;
                m /= nm;
                double fac = std::min(70.0, std::max(1.0, (sm * (D - t) - 0.75 * smt) / L));
                fac = 65;
                score_lib[lib] -= fac * std::sqrt(m2 - m * m);
                last_book[lib] = n;
                score_lib[lib] /= (DC * double(mean_time) + std::pow(double(T[lib]), D_POW));
                
                if (std::abs(a.first - score_lib[lib]) < 1e-5) {
                    break;
                }
                q.push({score_lib[lib], lib});
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
                for (int j = 0; j < libraries[b].size(); j++) {
                    int l = libraries[b][j];
                    if (seenL[l] || inLib[b][j] >= last_book[l]) {
                        continue;
                    }
                    score_lib[l] -= double(S[b]) / (DC * double(mean_time) + std::pow(double(T[l]), D_POW));
                    q.push({score_lib[l], l});
                }
                if (t + T[lib] + k.back().size() / M[lib] < D) {
                    k.back().push_back(b);
                    SCORE += S[b];
                }
            }
        }
        
        if (k.back().empty()) {
            k.pop_back();
            Y.pop_back();
        }
        else {
            t += T[lib];
        }
    }

    output_solution(Y, k);
    std::cerr << SCORE << std::endl;
    return 0;
}