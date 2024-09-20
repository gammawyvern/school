import math;
from matplotlib import pyplot as plt;
from scipy.spatial.distance import euclidean;
from scipy.spatial.distance import cityblock;

def euclid_dist(point_one, point_two):
    x1, y1 = point_one;
    x2, y2 = point_two;

    x_dist = abs(x2 - x1);
    y_dist = abs(y2 - y1);

    return math.sqrt((x_dist ** 2) + (y_dist ** 2));

def manhattan_dist(point_one, point_two):
    x1, y1 = point_one;
    x2, y2 = point_two;

    x_dist = abs(x2 - x1);
    y_dist = abs(y2 - y1);

    return x_dist + y_dist;

point_one = (2, 2);
point_two = (5, 5);

########################################
# Euclidean distances 
########################################

print(f"My euclidean distance: {euclid_dist(point_one, point_two)}");
print(f"SciPy euclidean distance: {euclidean(point_one, point_two)}");
print();

point_one_2 = (2 * point_one[0], 2 * point_one[1]);
point_two_2 = (2 * point_two[0], 2 * point_two[1]);

print(f"My euclidean distance (Points scaled by 2): {euclid_dist(point_one_2, point_two_2)}");
print(f"SciPy euclidean distance (Points scaled by 2): {euclidean(point_one_2, point_two_2)}");
print();

########################################
# Manhattan distances 
########################################

print(f"My manhattan distance: {manhattan_dist(point_one, point_two)}");
print(f"SciPy manhattan distance: {cityblock(point_one, point_two)}");
print();

print(f"My manhattan distance (Points scaled by 2): {manhattan_dist(point_one_2, point_two_2)}");
print(f"SciPy manhattan distance (Points scaled by 2): {cityblock(point_one_2, point_two_2)}");
print();

########################################
# Show points plotted
########################################

plt.grid();
plt.plot(point_one[0], point_one[1], marker="o");
plt.plot(point_two[0], point_two[1], marker="o");
plt.show();

