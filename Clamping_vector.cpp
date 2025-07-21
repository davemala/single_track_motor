#include <vector>
#include <algorithm>

void Clamping_vector(std::vector<double>& f, double min, double max) {
    // The aim is to clamp a vector between min and max
    for (size_t i = 0; i < f.size(); ++i) {
        if (f[i] > max) {
            f[i] = max;
        }
        if (f[i] < min) {
            f[i] = min;
        }
    }
}