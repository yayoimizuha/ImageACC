#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;


void Print_array(const vector<vector<double>> &input_array) {
#ifdef DEBUG_FLAG
    auto output_flag = cout.flags();
    cout << setprecision(5);
    for (const auto &j: input_array) {
        for (const auto &k: j) {
            if (abs(k) < 1e-10) {
                cout << right << setw(9) << 0;
            } else {
                cout << right << setw(9) << k;
            }
            cout << ' ';
        }
        cout << endl;
    }
    cout.flags(output_flag);
    cout << endl;
#endif
}

vector<vector<double>> Transpose_array(const vector<vector<double>> &input_array) {
    auto array_length = input_array[0].size();
    for (const vector<double> &row: input_array) {
        if (array_length != row.size()) {
            cout << "Array is not square" << endl;
            return {{-1}};
        }
    }
    vector<vector<double>> return_array;
    return_array.resize(input_array[0].size());
    for (int i = 0; i < input_array.size(); ++i) {
        for (int j = 0; j < input_array[0].size(); ++j) {
            return_array[j].resize(input_array.size());
            return_array[j][i] = input_array[i][j];
        }
    }
    return return_array;
}

vector<vector<double>> Produce_array(const vector<vector<double>> &arrayA, const vector<vector<double>> &arrayB) {
    if (arrayA[0].size() != arrayB.size()) {
        cout << "Can't define array product." << endl;
    }
    vector<double> vecA;
    vecA.resize(arrayB.size());
    vector<double> vecB;
    vecB.resize(arrayB.size());

    double inner_product;
    vector<vector<double>> arrayC;
    arrayC.resize(arrayA.size());
    for (int i = 0; i < arrayA.size(); ++i) {
        arrayC[i].resize(arrayB[0].size());
        for (int j = 0; j < arrayB[0].size(); ++j) {
            inner_product = 0.0;
            for (int k = 0; k < arrayB.size(); ++k) {
                inner_product += arrayA[i][k] * arrayB[k][j];
            }
            arrayC[i][j] = inner_product;
        }
    }
    return arrayC;
}


vector<vector<double>> Identity_array(size_t size) {
    vector<vector<double>> return_array;
    return_array.resize(size);
    for (int i = 0; i < size; ++i) {
        return_array[i].resize(size);
        std::fill(return_array[i].begin(), return_array[i].end(), 0);
        return_array[i][i] = 1;

    }
    return return_array;
}

vector<vector<double>> Sweep_array(const vector<vector<double>> &input_array) {
    Print_array(input_array);
    vector<vector<double>> return_array(input_array.size());
    for (int i = 0; i < input_array.size(); ++i) {
        return_array[i].resize(input_array[i].size());
        copy(input_array[i].begin(), input_array[i].end(), return_array[i].begin());
    }
    for (int i = 0; i < return_array.size(); ++i) {
        vector<pair<double, int>> swap_array;
        for (int j = i; j < return_array.size(); ++j) {
            swap_array.emplace_back(abs(return_array[j][i]), j);
        }
        auto max_el = *max_element(swap_array.begin(), swap_array.end());
        if (max_el.first < 1e-7) {
            cout << "cannot divide" << endl;
            exit(-1);
        }
        if (max_el.second != i) {
            return_array[i].swap(return_array[max_el.second]);
        }

        auto mag = 1.0 / return_array[i][i];
        for (double &j: return_array[i]) {
            j *= mag;
        }
        for (int j = 0; j < return_array.size(); ++j) {
            if (j == i)continue;
            auto a = -return_array[j][i];
            for (int k = 0; k < return_array[0].size(); ++k) {
                return_array[j][k] += return_array[i][k] * a;
            }
        }
        Print_array(return_array);
    }
    for (auto &i: return_array) {
        for (auto &j: i) {
            if (abs(j) < 1e-20)j = 0;
        }
    }
    return return_array;
}

vector<vector<double>> Inverse_array(const vector<vector<double>> &input_array) {
    auto identity_array = Identity_array(input_array.size());
    vector<vector<double>> sweep_array;
    sweep_array.resize(input_array.size());
    for (auto &row: sweep_array)row.resize(input_array[0].size());
    copy(input_array.begin(), input_array.end(), sweep_array.begin());
    for (int i = 0; i < sweep_array.size(); ++i) {
        sweep_array[i].insert(sweep_array[i].end(), identity_array[i].begin(), identity_array[i].end());
    }
    auto return_array = Sweep_array(sweep_array);
    for (auto &i: return_array) {
        i.erase(i.cbegin(), i.cbegin() + static_cast<long long>(input_array[0].size()));
    }
    return return_array;
}
