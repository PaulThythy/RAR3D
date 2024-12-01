#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <vector>
#include <string>
#include <utility>
#include <gnuplot-iostream.h>
#include <algorithm>

std::vector<std::pair<int, int>> createValencyHistogram(const std::vector<int> &data) {
    std::vector<int> histValencies;
    for(int i = 0; i < data.size(); i++) {
        if(histValencies.size() <= data[i])
            histValencies.resize(data[i]+1);

        histValencies[data[i]]++;
    }

    std::vector<std::pair<int, int>> dataValency;

    for (size_t i = 0; i < histValencies.size(); ++i) {
        dataValency.emplace_back(i, histValencies[i]);
    }

    return dataValency;
}

std::vector<std::pair<double, int>> createAreaHistogram(const std::vector<double> &data) {
    std::vector<int> histAreas;
    for(int i = 0; i < data.size(); i++) {
        if(histAreas.size() <= data[i])
            histAreas.resize(data[i]+1);

        histAreas[data[i]]++;
    }

    std::vector<std::pair<double, int>> dataArea;

    for (size_t i = 0; i < histAreas.size(); ++i) {
        dataArea.emplace_back(i, histAreas[i]);
    }

    return dataArea;
}

std::vector<std::pair<double, int>> createDihedralHistogram(const std::vector<double> &data) {
    std::vector<int> histDihedral;
    const int numBins = 36;
    histDihedral.resize(numBins, 0);

    for (double angle : data) {
        int binIndex = static_cast<int>((angle / M_PI) * numBins);
        if (binIndex >= 0 && binIndex < numBins) {
            histDihedral[binIndex]++;
        }
    }

    std::vector<std::pair<double, int>> dataDihedralAngles;

    for (int i = 0; i < numBins; ++i) {
        dataDihedralAngles.emplace_back((i + 0.5) * (M_PI / numBins), histDihedral[i]);
    }

    return dataDihedralAngles;
}

template <typename T>
void plotHistogram(const std::vector<std::pair<T, int>> &histogram, const std::string &title,
                   const std::string &xlabel, const std::string &ylabel, int windowId) {
    Gnuplot gp;
    gp << "set term wxt " << windowId << "\n";
    gp << "set title '" << title << "'\n";
    gp << "set xlabel '" << xlabel << "'\n";
    gp << "set ylabel '" << ylabel << "'\n";
    gp << "set style fill solid 0.5\n";
    gp << "plot '-' with boxes title ''\n";
    gp.send1d(histogram);
}

#endif // HISTOGRAM_H