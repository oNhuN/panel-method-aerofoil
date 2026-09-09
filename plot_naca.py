import matplotlib.pyplot as plt
import pandas as pd

def drawContour():
    df = pd.read_csv("points.csv")
    plt.plot(df['x'], df['y'], linestyle = "-")


def plotPanel():
    df = pd.read_csv("panel.csv")
    
    plt.figure(figsize=(10, 6))
    drawContour()
    plt.quiver(df['mid_x'], df['mid_y'], df['nor_x'], df['nor_y'], width = 0.002)
    plt.axis('equal')
    
    plt.show()

 

if __name__ == "__main__":
    plotPanel()