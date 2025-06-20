#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>
#include <cmath>

struct Problem {
    int B, L, D;
    std::vector<int> S;
    std::vector<int> N, T, M;
    std::vector<std::vector<int>> books;
    std::vector<std::vector<int>> libraries;
};

inline Problem read_input() {
    Problem P;
    std::cin >> P.B >> P.L >> P.D;
    P.S.resize(P.B);
    for (int i = 0; i < P.B; i++) {
        std::cin >> P.S[i];
    }

    P.N.resize(P.L);
    P.T.resize(P.L);
    P.M.resize(P.L);
    P.books.resize(P.L);
    P.libraries.resize(P.B);

    for (int i = 0; i < P.L; i++) {
        std::cin >> P.N[i] >> P.T[i] >> P.M[i];
        int b;
        for (int j = 0; j < P.N[i]; j++) {
            std::cin >> b;
            if (b < P.B) {
                P.libraries[b].push_back(i);
            }
            P.books[i].push_back(b);
        }
    }
    return P;
}

inline void output_solution(const std::vector<int>& Y, const std::vector<std::vector<int>>& k) {
    int A = Y.size();
    std::cout << A << "\n";
    for (int i = 0; i < A; i++) {
        std::cout << Y[i] << " " << k[i].size() << "\n";
        for (int b : k[i]) {
            std::cout << b << " ";
        }
        std::cout << "\n";
    }
}

#endif