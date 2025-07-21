#include <vector>
#include <cstddef>
#include "Car.h" // Make sure this path matches where Car is defined

std::vector<double> ODE_Solver_Discretizzato(double h, const std::vector<double>& t, 
                                              const std::vector<double>& f_dot, double f_0, 
                                              const Car& car, int WhatToMod) {
    // The aim is to solve the Cauchy problem: d f / dx = f_dot,
    // f(xin)=f_0, in the total domain of t[xin, xfin]
    // h is the step size
    // t is the vector of xi

    // Solutions
    std::vector<double> f(t.size()); // Vector of f(xi)

    f[0] = f_0;  // initial condition 

    for (size_t i = 0; i < t.size() - 1; ++i) { // calculation loop
        f[i + 1] = f[i] + f_dot[i] * h;
        f[i + 1] = car.ModifyParameter(f[i + 1], WhatToMod);
    }

    return f;
}