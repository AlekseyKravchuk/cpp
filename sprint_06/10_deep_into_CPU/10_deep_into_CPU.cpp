#include "log_duration.h"

using namespace std;

// Функция анализирует данные телескопа, определяя, сколько сильных сигналов
// зафиксировано по каждому направлению.
vector<int> ComputeStatistics_v1(const vector<string>& measurements, int numOfSkyRegions) {
    int numOfMeasurements = measurements.size();
    vector<int> highSignalsNumPerSkyRegion(numOfSkyRegions);

    for (int skyRegionID = 0; skyRegionID < numOfSkyRegions; ++skyRegionID) {
        for (int dayNumber = 0; dayNumber < numOfMeasurements; ++dayNumber) {
            if (measurements[dayNumber][skyRegionID] >= 'K') {
                ++highSignalsNumPerSkyRegion[skyRegionID];
            }
        }
    }

    return highSignalsNumPerSkyRegion;
}

// Функция анализирует данные телескопа, определяя, сколько сильных сигналов зафиксировано по каждому направлению.
// в версии v2 меняем порядок циклов
vector<int> ComputeStatistics_v2(const vector<string>& measurements, int numOfSkyRegions) {
    int numOfMeasurements = measurements.size();
    vector<int> highSignalsNumPerSkyRegion(numOfSkyRegions);

    for (int dayNumber = 0; dayNumber < numOfMeasurements; ++dayNumber) {
        for (int skyRegionID = 0; skyRegionID < numOfSkyRegions; ++skyRegionID) {
            highSignalsNumPerSkyRegion[dayNumber] += (measurements[dayNumber][skyRegionID] >= 'K' ? 1 : 0);
        }
    }

    return highSignalsNumPerSkyRegion;
}

// Функция анализирует данные телескопа, определяя, сколько сильных сигналов зафиксировано по каждому направлению.
// в версии v3 избавляемся от избыточного прибавления нулей в тернарном операторе
vector<int> ComputeStatistics_v3(const vector<string>& measurements, int numOfSkyRegions) {
    int numOfMeasurements = measurements.size();
    vector<int> highSignalsNumPerSkyRegion(numOfSkyRegions);

    for (int dayNumber = 0; dayNumber < numOfMeasurements; ++dayNumber) {
        for (int skyRegionID = 0; skyRegionID < numOfSkyRegions; ++skyRegionID) {
            if (measurements[dayNumber][skyRegionID] >= 'K') {
                ++highSignalsNumPerSkyRegion[dayNumber];
            }
        }
    }

    return highSignalsNumPerSkyRegion;
}

string GetRandomString(int stringLength) {
    static std::mt19937 engine;
    std::uniform_int_distribution<int> distribution('A', 'Z');

    std::string generatedString(stringLength, ' ');
    for (char& ch : generatedString) {
        ch = char(distribution(engine));
    }

    return generatedString;
}

int main() {
    vector<string> data;
    int numOfObservations = 5000;
    int numOfSkyRegions = 5000;

    for (int i = 0; i < numOfObservations; ++i) {
        data.push_back(GetRandomString(numOfSkyRegions));
    }

    // ======================================================================
    vector<int> statistics1;
    {
        LOG_DURATION("ComputeStatistics v1"s);
        statistics1 = ComputeStatistics_v1(data, numOfSkyRegions);
    }
    cout << "Всего сильных сигналов: "s << accumulate(statistics1.begin(), statistics1.end(), 0) << endl;
    // ======================================================================

    cout << endl;

    // ======================================================================
    vector<int> statistics2;
    {
        LOG_DURATION("ComputeStatistics v2"s);
        statistics2 = ComputeStatistics_v2(data, numOfSkyRegions);
    }
    cout << "Всего сильных сигналов: "s << accumulate(statistics2.begin(), statistics2.end(), 0) << endl;
    // ======================================================================

    cout << endl;

    // ======================================================================
    vector<int> statistics3;
    {
        LOG_DURATION("ComputeStatistics v3"s);
        statistics2 = ComputeStatistics_v3(data, numOfSkyRegions);
    }
    cout << "Всего сильных сигналов: "s << accumulate(statistics3.begin(), statistics3.end(), 0) << endl;
    // ======================================================================
}