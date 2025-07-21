import pandas as pd
import sys

import matplotlib.pyplot as plt

def show_results(t, v, teta, x, y):
    # Graphic parameters
    figure_params = [100, 0, 12, 8]  # inches for figsize
    line_width = 2.0
    color = "blue"
    line_style = "-"
    marker = "o"

    # PLOT
    plt.figure(1, figsize=(figure_params[2]/100, figure_params[3]/100))
    plt.subplot(2, 2, 1)
    plt.plot(t, x, linestyle=line_style, color=color, linewidth=line_width)
    plt.title("Grafico X")
    plt.xlabel("t")
    plt.ylabel("x(t)")

    plt.subplot(2, 2, 2)
    plt.plot(t, y, linestyle=line_style, color=color, linewidth=line_width)
    plt.title("Grafico Y")
    plt.xlabel("t")
    plt.ylabel("y(t)")

    plt.subplot(2, 2, 3)
    plt.plot(t, teta, linestyle=line_style, color=color, linewidth=line_width)
    plt.title("Grafico Teta")
    plt.xlabel("t")
    plt.ylabel("teta(t)")

    plt.subplot(2, 2, 4)
    plt.plot(t, v, "k", linewidth=line_width)
    plt.title("Grafico V-t")
    plt.xlabel("t")
    plt.ylabel("v(t)")

    # plot A PARTE XY
    plt.figure(2, figsize=(figure_params[2]/100, figure_params[3]/100))
    plt.plot(x, y, linestyle=line_style, color=color, linewidth=line_width)
    plt.title("Grafico Y-X")
    plt.xlabel("x(t)")
    plt.ylabel("y(t)")

    plt.show()

if __name__ == "__main__":
    csv_file = "simulation_output.csv"
    df = pd.read_csv(csv_file)

    # Assumes columns are named: time, x, y, teta, v, delta, acc
    t = df['time'].values
    x = df['x'].values
    y = df['y'].values
    teta = df['teta'].values
    v = df['v'].values

    show_results(t, v, teta, x, y)