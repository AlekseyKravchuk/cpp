#include "log_duration.h"

using namespace std;

struct PositiveTemperatures {
    float sumOfPosTempsPerDay = 0.f;
    int64_t numOfPosTemps = 0;
};

vector<float> ComputeAvgTemp(const vector<vector<float>>& measurements) {
    if (measurements.empty()) {
        return {};
    }

    int numOfMeasurementsPerDay = measurements[0].size();
    vector<PositiveTemperatures> posTempsStatistics(numOfMeasurementsPerDay);

    for (size_t dayNumber = 0; dayNumber < measurements.size(); ++dayNumber) {
        for (int measureID = 0; measureID < numOfMeasurementsPerDay; ++measureID) {
            bool isPosTemp = measurements[dayNumber][measureID] > 0;
            posTempsStatistics[measureID].numOfPosTemps += isPosTemp ? 1 : 0;
            posTempsStatistics[measureID].sumOfPosTempsPerDay += isPosTemp ? measurements[dayNumber][measureID] : 0.f;
        }
    }

    vector<float> averages;
    averages.reserve(numOfMeasurementsPerDay);
    for (const auto& [sumOfPosTempsPerDay, numOfPosTemps] : posTempsStatistics) {
        averages.push_back(numOfPosTemps > 0 ? (static_cast<float>(sumOfPosTempsPerDay) / static_cast<float>(numOfPosTemps)) : 0);
    }

    return averages;
}

vector<float> GetRandomVector(int size) {
    static mt19937 engine;
    uniform_real_distribution<float> d(-100, 100);

    vector<float> res(size);
    for (int i = 0; i < size; ++i) {
        res[i] = d(engine);
    }

    return res;
}

int main() {
    int N = 5000;
    // vector<vector<float>> data = {
    //     {0, -1, -1},
    //     {1, -2, -2},
    //     {2, 3, -3},
    //     {3, 4, -4}};

    // vector<vector<float>> data = {
    //     {1,   4, -4},
    //     {0,   0, -5},
    //     {-1, -2, -2},
    //     {-2,  3, -3},
    //     {3,   4, -4}};
    // среднее для 0-го измерения (1+3) / 2 = 2 (не учитывам 0 и -1, -2)
    // среднее для 1-го измерения (4+3+4) / 3 = 3.666(6) (не учитывам -1, -2)
    // среднее для 2-го не определено (все температуры отрицательны), поэтому должен быть 0

    vector<vector<float>> data;
    data.reserve(N);

    for (int i = 0; i < N; ++i) {
        data.push_back(GetRandomVector(N));
    }

    vector<float> avg;
    {
        LOG_DURATION("ComputeAvgTemp"s);
        avg = ComputeAvgTemp(data);
    }

    cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
}