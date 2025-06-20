#include "utils.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include <cmath>
#include <cerrno>

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

    long long actual_score = 0;
    std::vector<int> activation_day(L, -1);
    long long signup_start = 0;

    for (int i = 0; i < Y.size(); i++) {
        int lib_id = Y[i];
        if (signup_start + T[lib_id] > D) {
            break;
        }
        activation_day[lib_id] = signup_start + T[lib_id];
        signup_start += T[lib_id];
    }

    std::vector<std::vector<int>> events(D);
    for (int i = 0; i < Y.size(); i++) {
        int lib_id = Y[i];
        if (activation_day[lib_id] == -1) continue;
        int a = activation_day[lib_id];
        if (a >= D) continue;
        int m = M[lib_id];
        const std::vector<int>& book_list = k[i];
        for (int idx = 0; idx < book_list.size(); idx++) {
            int day = a + idx / m;
            if (day >= D) break;
            events[day].push_back(book_list[idx]);
        }
    }

    std::vector<bool> scanned(B, false);
    for (int d = 0; d < D; d++) {
        for (int book_id : events[d]) {
            if (!scanned[book_id]) {
                scanned[book_id] = true;
                actual_score += S[book_id];
            }
        }
    }

    std::cerr << "Actual score: " << actual_score << std::endl;
    return 0;
}