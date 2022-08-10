#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// верните {true, медиана}, если она существует,
// то есть вектор непустой,
// иначе - {false, 0}
// обратите внимание - вектор принимаем по значению,
// так как его придётся немного подпортить, чтобы вернуть ответ
pair<bool, double> CalcMedian(vector<double> samples) {
    if (samples.empty()) {
        return make_pair(false, 0.0);
    } else {
        double median;
        int len = samples.size();
        sort(samples.begin(), samples.end());

        if (len % 2 == 0) {
            median = (samples[len / 2] + samples[len / 2]) / 2;
            return make_pair(true, median);
        } else {
            median = samples[len / 2];
            return make_pair(true, median);
        }
    }
}

int main() {
    int size;
    cin >> size;

    vector<double> samples;
    for (int i = 0; i < size; ++i) {
        double sample;
        cin >> sample;
        samples.push_back(sample);
    }

    pair<bool, double> result = CalcMedian(samples);
    if (result.first) {
        cout << result.second << endl;
    } else {
        cout << "Empty vector"s << endl;
    }
}