#include "utils.h"
#include <vector>
#include <iostream>
#include <algorithm>

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
                if (t + 2 + static_cast<int>(k.back().size()) < D) {
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

    long long actual_score = 0;
    int current_day = 0;
    std::vector<bool> scanned(B, false);

    for (int i = 0; i < Y.size(); i++) {
        int lib_id = Y[i];
        if (current_day + T[lib_id] > D) {
            break;
        }
        current_day += T[lib_id];
        if (current_day >= D) {
            break;
        }
        int available_days = D - current_day;
        if (available_days <= 0) {
            continue;
        }
        long long max_books = static_cast<long long>(available_days) * M[lib_id];
        int num_books_to_ship = std::min(static_cast<int>(k[i].size()), static_cast<int>(max_books));
        
        for (int j = 0; j < num_books_to_ship; j++) {
            int book_id = k[i][j];
            if (!scanned[book_id]) {
                scanned[book_id] = true;
                actual_score += S[book_id];
            }
        }
    }

    std::cerr << "Actual score: " << actual_score << std::endl;
    return 0;
}