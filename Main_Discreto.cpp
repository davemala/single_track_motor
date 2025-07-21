#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <math.h>

#include <algorithm>

#include <cstddef>

#include <fstream>

class Vettura2 {
public:
    // The following datas are from the design report 2024 and, mainly, in the
    // notation of Guiggiani's book

    // In this part, the data are still in the original unit of
    // measurement

    // GEOMETRY
    double p = 1540;
    double a = 818;
    double b = 722;
    double CoG_Z = 293;

    // WEIGHT
    double Tot_W = 285; // Without the driver?
    double Front_W = 127;
    double Rear_W = 158;

    // SPEED
    double V_Max = 72;
    double Speed = 0;
    double Omega = 0;
    double Omega_max = 72;
    double Omega_dot = 0;

    // HANDLING
    double SteeringWheelAngle = 0;
    double SteeringWheelAngle_Max = 150;
    double SteeringRatio = 5.07;
    double WheelAngle = 0;
    double WheelAngle_Max = 0;
    double RadiusWheel = 0.205;

    // GEOMETRIC PARAMETERS
    double x = 0;
    double y = 0;
    double teta = 0;

    // POWER UNIT
    double eff_engine = 90; // Engine efficiency
    double Tau_transmission = 10.6; // Transmission ratio from engine to wheel
    double Torque_Max = 100;

    // Constructor
    Vettura2() {
        // CONVERSIONS
        p /= 1000;
        a /= 1000;
        b /= 1000;
        CoG_Z /= 1000;
        V_Max /= 3.6;
        Omega_max = V_Max / RadiusWheel;
        Speed /= 3.6;
        Omega = Speed / RadiusWheel;
        SteeringWheelAngle = SteeringWheelAngle / 180 * M_PI;
        SteeringWheelAngle_Max = SteeringWheelAngle_Max / 180 * M_PI;
        WheelAngle = SteeringWheelAngle / SteeringRatio;
        WheelAngle_Max = SteeringWheelAngle_Max / SteeringRatio;
        eff_engine /= 100;
    }

    void Set_SteeringAngle(double SA) {
        if (SA > SteeringWheelAngle_Max) {
            SA = SteeringWheelAngle_Max;
        }
        SteeringWheelAngle = SA;
        WheelAngle = SteeringWheelAngle / SteeringRatio;
    }

    void Set_Speed(double sp) {
        if (sp > V_Max) {
            sp = V_Max;
        }
        Omega = sp / RadiusWheel;
        Speed = sp;
    }

    void Set_Omega(double Omega1) {
        if (Omega1 > Omega_max) {
            Omega1 = Omega_max;
        }
        Omega = Omega1;
        Speed = Omega * RadiusWheel;
    }

    double Get_Omega_dot(double C, double time) {
        if (C > Torque_Max) {
            C = Torque_Max;
        }
        double P = C * (Omega + exp(-10)) * Tau_transmission;

        Omega_dot = P * eff_engine / (Tot_W * pow(RadiusWheel, 2) * (Omega + exp(-10)));
        Set_Omega(Omega + Omega_dot * time);
        return Omega_dot;
    }

    double ModifyParameter(double value, int WhatToMod) {
        double value_out;

        switch (WhatToMod) {
            case 0:
                // Nulla
                value_out = value;
                break;
            case 1:
                Set_Speed(value);
                value_out = Speed;
                break;
            case 2:
                Set_SteeringAngle(value);
                value_out = SteeringWheelAngle;
                break;
            default:
                std::cout << "Comando sconosciuto nella modifica di un parametro della vettura" << std::endl;
                break;
        }
        return value_out;
    }

};

std::vector<double> createArray(size_t size, double initialValue) {
    return std::vector<double>(size, initialValue);
}

std::vector<double> Clamping_vector(std::vector<double>& f, double min, double max) {
    // The aim is to clamp a vector between min and max
    for (size_t i = 0; i < f.size(); ++i) {
        if (f[i] > max) {
            f[i] = max;
        }
        if (f[i] < min) {
            f[i] = min;
        }
    }
    return f;
}

std::vector<double> ODE_Solver_Discretizzato(double h, const std::vector<double>& t, 
                                              const std::vector<double>& f_dot, double f_0, 
                                              Vettura2& car, int WhatToMod) {
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

int main() {
    double tau = 50; // Tempo di previsione
    double h = 0.001; // Passo
    std::vector<double> t;
    for (double time = 0; time <= tau; time += h) {
        t.push_back(time);
    }
    size_t num_punti = t.size();

    Vettura2 car; // Oggetto Vettura
    
    //velocity
    std::cout << "Calcolo velocità\n";
    double v0 = 3; // Condizione iniziale
    car.Set_Speed(v0);
    auto input_funz = [](double t) { return t * t; }; // La coppia che arriva dalla "centralina"
    std::vector<double> input(num_punti);
    for (size_t i = 0; i < num_punti; ++i) {
        input[i] = input_funz(t[i]) * car.Torque_Max;
    }
    std::vector<double> acc = createArray(num_punti, 0);
    for (size_t i = 0; i < num_punti; ++i) {
        acc[i] = car.Get_Omega_dot(input[i], h) * car.RadiusWheel;
    }
    std::vector<double> v = ODE_Solver_Discretizzato(h, t, acc, v0, car, 1);
    v = Clamping_vector(v, 0, car.V_Max);

    //Teta
    std::cout << "Calcolo teta\n";
    double teta0 = 0; // Condizione iniziale
    auto delta_funz = [](double t) {
    return sin(t);
    };
    // Funzione di sterzata, esempio
    auto delta = createArray(num_punti, 0);
    for (size_t i = 0; i < num_punti; ++i) {
        delta[i] = delta_funz(t[i]);
    }
    delta = Clamping_vector(delta, -car.WheelAngle_Max, car.WheelAngle_Max);
    std::vector<double> der_teta = createArray(num_punti, 0);
    double L = 1.540;
    for (size_t i = 0; i < num_punti; ++i) {
        der_teta[i] = tan(delta[i]) * v[i] / L;
    }
    std::vector<double> teta = ODE_Solver_Discretizzato(h, t, der_teta, teta0, car, 0);

    // X
    std::cout << "Calcolo x\n";
    double x0 = 0; // Condizione iniziale
    std::vector<double> der_x = createArray(num_punti, 0);
    for (size_t i = 0; i < num_punti; ++i) {
        der_x[i] = v[i] * cos(teta[i]);
    }
    std::vector<double> x = ODE_Solver_Discretizzato(h, t, der_x, x0, car, 0);

    // Y
    std::cout << "Calcolo y\n";
    double y0 = 0; // Condizione iniziale
    std::vector<double> der_y = createArray(num_punti, 0);
    for (size_t i = 0; i < num_punti; ++i) {
        der_y[i] = v[i] * sin(teta[i]);
    }
    std::vector<double> y = ODE_Solver_Discretizzato(h, t, der_y, y0, car, 0);
    
    std::cout << "Grafici\n";

    // Open CSV file
    std::ofstream csv_file("simulation_output.csv");
    csv_file << "time,x,y,teta,v,delta,acc\n";

    // Write data
    for (size_t i = 0; i < num_punti; ++i) {
        csv_file << t[i] << "," << x[i] << "," << y[i] << "," << teta[i] << "," << v[i] << "," << delta[i] << "," << acc[i] << "\n";
    }
    csv_file.close();

    return 0;
}