import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

def drawContour():
    df = pd.read_csv("points.csv")
    plt.plot(df['x'], df['y'], linestyle = "-")


def plotPanel():
    df = pd.read_csv("panel.csv")
    
    plt.figure(figsize=(10, 6))
    drawContour()
    plt.quiver(df['mid_x'], df['mid_y'], df['nor_x'], df['nor_y'], width = 0.002) #vector arrow plot.
    plt.axis('equal')
    plt.savefig("naca_geometry.png", dpi=150)
    
    plt.show()

def plotPolar():
    df = pd.read_csv("polar.csv")
    plt.figure(figsize=(10, 6))
    plt.plot(df["angle_deg"], df["Cl"], '-o', label = "Panel method (N = 400)")
    theory = 2 * np.pi * np.deg2rad(df['angle_deg'])
    plt.plot(df["angle_deg"], theory, linestyle = '--', label= r'Thin-aerofoil theory, $2\pi\alpha$')
    plt.xlabel(r"$\alpha$ (degrees)")
    plt.ylabel("$C_l$")
    plt.legend()
    plt.title("NACA 0012, N = 400 panels")
    plt.savefig("cl_alpha_validation.png", dpi=150, bbox_inches="tight")
    plt.show()
    

if __name__ == "__main__":
    plotPanel()
    plotPolar()