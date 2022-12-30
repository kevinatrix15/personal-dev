#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct PositionSensorData
{
    double time{};
    double xPos{};
    double yPos{};
    double sensorVal{};
};

struct SensorData
{
    double time{};
    double sensorVal{};

    // TODO: make Doxygen stype doc
    // comparison operator enabling comparison between this time and
    // PositionSensorData time, used for interpolation
    bool operator< (const PositionSensorData& p) const{ return time < p.time; }
};

double getCSVKeyVal(istringstream& lineStreamRef)
{
    string entry;
    const char delim = ',';

    getline(lineStreamRef, entry, delim); // key, ignore
    getline(lineStreamRef, entry, delim); // val
    return stod(entry);
}

pair<double, double> getCSVPair(istringstream& lineStreamRef)
{
    string entry;
    const char delim = ',';

    double first, second;
    getline(lineStreamRef, entry, delim);
    first = stod(entry);
    getline(lineStreamRef, entry, delim);
    second = stod(entry);
    return make_pair(first, second);
}

vector<SensorData> readSensorData(const string& sensorFile)
{
    ifstream datFile(sensorFile, ios::in);
    string line;

    vector<SensorData> outData;

    // read each data line, assigning to relative struct members
    SensorData lineData;
    while (getline(datFile, line)) {
        istringstream lineStream(line);

        // read each entry, assigning to appropriate variables
        auto [time, sensorVal] = getCSVPair(lineStream);
        lineData.time = time;
        lineData.sensorVal = sensorVal;
        outData.emplace_back(lineData);
    }
    return outData;
}

void assignPositionData(const string& positionFile,
                        vector<PositionSensorData>& outputRef)
{
    ifstream datFile(positionFile, ios::in);

    string line;
    double timeStart, timeStep;
    
    // read two header lines and store start time and time step
    try {
        getline(datFile, line);

        // read the row and assign to line string
        istringstream lineStream(line);

        // read each entry, assigning to appropriate variables
        timeStart = getCSVKeyVal(lineStream);
        timeStep = getCSVKeyVal(lineStream);
    } catch (const exception& e) {
        cerr << "Failed reading position file header with: " << e.what() << endl;
    }

    // read each data line, assigning to relative struct members
    // NOTE: this leaves the sensor member unassigned at this point
    // TODO: add error handling
    size_t lineCount = 0U;
    PositionSensorData lineData;
    while (getline(datFile, line)) {
        // read the row and assign to line string
        istringstream lineStream(line);

        // read each entry, assigning to appropriate variables
        auto [xPos, yPos] = getCSVPair(lineStream);
        lineData.xPos = xPos;
        lineData.yPos = yPos;
        lineData.time = timeStart + static_cast<double>(lineCount) * timeStep;
        outputRef.emplace_back(lineData);
    }
}

double interpolateValue(const double interpTime,
                        const SensorData lowerData,
                        const SensorData upperData)
{
    // y0 + (x - x0) * (y1 - y0) / (x1 - x0);
    return lowerData.sensorVal +
        (interpTime - lowerData.time) * (upperData.sensorVal - lowerData.sensorVal)
        / (upperData.time - lowerData.time);
}

pair<SensorData, SensorData> getLowerUpperSensorData(const vector<SensorData>& data,
                                                     const PositionSensorData& target)
{
    if (data.empty())
        throw 0; // Can't help you if we're empty.

    // TODO: add custom comparison, or does it default to overloaded '<'??
    vector<SensorData>::const_iterator it = lower_bound(data.begin(), data.end(), target);

    if (it == data.end()) {
        // no data points with times higher than target
        return make_pair(data.back(), data.back());
    }
    else if (it == data.begin()) {
        // no data points with times lower than target
        return make_pair(data.front(), data.front());
    }
    return make_pair(*(it - 1), *(it));
}

void assignSensorData(const vector<SensorData> sensorData,
                      vector<PositionSensorData>& outputRef)
{
    for (auto& outData : outputRef) {
        // find nearest sensor time series data
        // NOTE: lower = upper when interp time < t_init || interp time > t_final
        const auto [lowerData, upperData] =
            getLowerUpperSensorData(sensorData, outData);

        // perform interpolation
        const auto interpVal = interpolateValue(outData.time, lowerData, upperData);

        // assign interpolated sensor data to output data
        outData.sensorVal = interpVal;
    }
}

vector<PositionSensorData> generateData(const string& positionFile,
                                        const string& sensorPath)
{
    vector<PositionSensorData> output;
    const auto sensorData = readSensorData(sensorPath);
    assignPositionData(positionFile, output);
    assignSensorData(sensorData, output);
}

int main(int, char**) {
    // read in the xy-position file
    // read in the sensor file
    // map sensor data to the fine xy-position time
    // generate output for plotting
}
