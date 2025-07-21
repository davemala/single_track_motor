#include <iostream>
#include <cmath>
#include <algorithm>

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