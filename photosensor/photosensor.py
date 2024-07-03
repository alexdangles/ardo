import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import math
intensity = []    # LED intensity
red = []    # Photoresistor value for red LED
green = []  # Photoresistor value for green LED
blue = []   # Photoresistor value for blue LED

# Operation functions

def get_data(file):
    data = np.genfromtxt(file, delimiter=',')
    # Split data for each color
    rgb = np.array_split(data, 3)
    for i in rgb[0]:
        intensity.append(int(i[1]))
    for i in rgb[0]:
        red.append(int(i[2]))
    for i in rgb[1]:
        green.append(int(i[2]))
    for i in rgb[2]:
        blue.append(int(i[2]))

# Use np.diff() to see where the data changes the greatest
# Principle here is to use the calculate the rate of change
# over each incremental intensity (5) for each lead
def differentiate(led, absolute=True):
    diff = np.diff(led)
    output = []
    output.append(0)  # First reference point (initialize)
    for i in diff:
        if (absolute == True): output.append(abs(i)/5)  # Absolute change in y over 5 unit increments
        else: output.append(i/5)
    return output

def get_pos_indices(lst):
    index = []
    for i in range(len(lst)-1):
        if i == 0:
            if lst[i] > 0:
                index.append(0)
        if lst[i] < 0 and lst[i+1] > 0:
            index.append(i+1)
    if len(index) == 0:
        index = [-1]
    return index

def calc_max_change(led):
    output = differentiate(led)
    max_index = output.index(max(output))
    return intensity[max_index] , max(output)  # Return (intensity value, p resistor value)

def calc_min_rise(led):
    output = differentiate(led, absolute=False)
    print(output)
    return intensity[min(get_pos_indices(output))]

def plotRGB():
    plt.xlabel("LED intensity")
    plt.ylabel("Photoresistor value")
    plt.plot(intensity, red, marker='o', color='r', label='red LED')
    plt.plot(intensity, green, marker='o', color='g', label='green LED')
    plt.plot(intensity, blue, marker='o', color='b', label='blue LED')
    plt.legend(loc='upper right')
    plt.show()

# In this case we measure responsivity (photoresistor value / light power (~ to wavelength))
# Calculate slope of linear releationship
def plotSens():
    plt.xlabel("Wavelength (nm)")
    plt.ylabel("Intensity")
    x = [635, 515, 470]
    y = [red[30], green[30], blue[30]]
    coef, b = np.polyfit(x, y, 1)
    poly1d_fn = np.poly1d(coef)
    plt.plot(x,y, 'yo', x, poly1d_fn(x), '--k')
    plt.xlim(0, 700)
    plt.ylim(0, 1024)
    plt.axline(xy1=(0, b), slope=coef, label=f'$y = {coef:.1f}x {b:+.1f}$')
    #plt.plot(, ,  marker='o', color='orange', label='Sensitivity')
    plt.legend(loc='upper right')
    plt.show()
    print("sensitivity is {0} photoresistor units per nm of light energy".format(abs(coef)))

# Helper functions

def exp(t, A, lbda):
    r"""y(t) = A \cdot \exp(-\lambda t)"""
    return A * np.exp(-lbda * t)

def sine(t, omega, phi):
    r"""y(t) = \sin(\omega \cdot t + phi)"""
    return np.sin(omega * t + phi)

def damped_sine(t, A, lbda, omega, phi):
    r"""y(t) = A \cdot \exp(-\lambda t) \cdot \left( \sin \left( \omega t + \phi ) \right)"""
    return exp(t, A, lbda) * sine(t, omega, phi)

def add_noise(y, loc=0, scale=0.01):
    noise = np.random.normal(loc=loc, scale=scale, size=y.shape)
    return y + noise

def find_peaks(x, y):
    peak_x = []
    peak_vals = []
    for i in range(len(y)):
        if i == 0:
            continue
        if i == len(y) - 1:
            continue
        if (y[i-1] < y[i]) and (y[i+1] < y[i]):
            peak_x.append(x[i])
            peak_vals.append(y[i])
    return np.array(peak_x), np.array(peak_vals)


# Main Script

get_data('data.csv')

# Determine the lowest R, G, and B LED intensity values that result in the maximum photoresistor value change.
print(calc_max_change(red))
print(calc_max_change(green))
print(calc_max_change(blue))

# Determine the minimum R, G, and B LED intensity values that register a photoresistor signal change.
print(calc_min_rise(red))
print(calc_min_rise(green))
print(calc_min_rise(blue))

# Plot the photoresistor readouts against the RGB intensities.
plotRGB()

# Plot peak sensitivity and print out coefficient as result.
plotSens()