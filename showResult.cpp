#include <iostream>
#include <vector>
#include <string>
#include <matplotlibcpp.h>


namespace plt = matplotlibcpp;

void showResults(const std::vector<double>& t, const std::vector<double>& v, const std::vector<double>& teta, const std::vector<double>& x, const std::vector<double>& y) {
    // Graphic parameters
    std::vector<double> figure_params = {100, 0, 1200, 800}; 
    double lineWidth = 2.0;
    std::string color = "blue";
    std::string lineStyle = "-";
    std::string marker = "o";    

    // PLOT
    plt::figure(1);
    plt::subplot(2, 2, 1);
    plt::plot(t, x);
    plt::title("Grafico X");
    plt::xlabel("t");
    plt::ylabel("x(t)");

    plt::subplot(2, 2, 2);
    plt::plot(t, y);
    plt::title("Grafico Y");
    plt::xlabel("t");
    plt::ylabel("y(t)");

    plt::subplot(2, 2, 3);
    plt::plot(t, teta);
    plt::title("Grafico Teta");
    plt::xlabel("t");
    plt::ylabel("teta(t)");

    plt::subplot(2, 2, 4);
    plt::plot(t, v, "k");
    plt::title("Grafico V-t");
    plt::xlabel("t");
    plt::ylabel("v(t)");

    // plot A PARTE XY
    plt::figure(2);
    plt::plot(x, y);
    plt::title("Grafico Y-X");
    plt::xlabel("x(t)");
    plt::ylabel("y(t)");

    // Set figure position (not directly supported in matplotlibcpp)
    // This part may need to be handled differently based on the environment
    // plt::gcf().set_position(figure_params); // Placeholder for setting figure position
    plt::show();
}