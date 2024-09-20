from PIL import Image, ImageTk, ImageDraw;

import tkinter as tk;
from tkinter import Label;

import matplotlib.pyplot as plt;

# Simple image display function for tkinter
def show_images(images: list, name: str):
    image_window = tk.Tk();
    image_window.title(name);
    tk_images = []; # Have to do this because stupid garbage collection

    for image in images:
        tk_image = ImageTk.PhotoImage(image);
        tk_images.append(tk_image); # Silly
        window_image = Label(image_window, image=tk_image);
        window_image.pack(side=tk.LEFT, padx=5, pady=5);

    image_window.mainloop();

# Load base image
image_path = "/home/keag/GVSU/CIS378/Lab1/Core_Keeper.jpg"
image = Image.open(image_path);

########################################
# Scale image
########################################

scaled_images = [
    image.resize((int(0.5 * image.size[0]), int(0.5 * image.size[1]))),
    image.resize((int(1   * image.size[0]), int(1   * image.size[1]))),
    image.resize((int(2   * image.size[0]), int(2   * image.size[1]))),
];
show_images(scaled_images, "Scaled Images");

########################################
# Color channel images
########################################

color_channels = image.split();
color_channels = {
    "Red": color_channels[0],
    "Green": color_channels[1],
    "Blue": color_channels[2],
}

# Show each color channel
show_images([image] + list(color_channels.values()), "Color Channels of Image");

########################################
# Histograms
########################################

for channel in color_channels:
    width, height = color_channels[channel].size;
    histogram_data = [0] * 256;

    for x in range(width):
        for y in range(height):
            value = color_channels[channel].getpixel((x, y));
            histogram_data[value] += 1;

    plt.bar(range(256), histogram_data, width=1, color=channel, alpha=0.7)
    plt.title(f"Histogram for {channel} Channel")
    plt.xlabel('Pixel Value')
    plt.ylabel('Frequency')
    plt.show()

########################################
# Binned Histograms
# Copied my same code from above, I'm lazy 
########################################

bin_size = 4;
for channel in color_channels:
    width, height = color_channels[channel].size;
    histogram_data = [0] * (256 // bin_size);

    for x in range(width):
        for y in range(height):
            value = color_channels[channel].getpixel((x, y));
            histogram_data[value // bin_size] += 1;

    # Trying histogram edge plot thing from ChatGPT 
    bin_edges = [bin_size * edge for edge in range((256 // bin_size) + 1)];

    # Like wtf is '[:-1]'? Why is there a negative?
    plt.bar(bin_edges[:-1], histogram_data, width=bin_size, color=channel, alpha=0.7)
    plt.title(f"Binned Histogram for {channel} Channel")
    plt.xlabel('Pixel Value')
    plt.ylabel('Frequency')
    plt.show()

########################################
# Create my own image 
########################################

my_image_size = (300, 300);
my_image = Image.new("RGB", my_image_size, "White");
drawer = ImageDraw.Draw(my_image);

# Draw 2 gradiants over image
scale_to_color = lambda val, max_val: (val / max_val) * 255;
for line in range(my_image_size[1]):
    vert_color = (
        int(scale_to_color(line, my_image_size[1])),
        int(255),
        int(255),
    );

    hori_color = (
        int(255),
        int(255),
        int(scale_to_color(line, my_image_size[0])),
    );

    drawer.line([(0, line), (my_image_size[0], line)], fill=vert_color, width=1);
    drawer.line([(line, 0), (line, my_image_size[1])], fill=hori_color, width=1);

# Draw cool repeating circles in image
width, height = my_image_size;
ellipse_ratio = 20;
ellipse_size = (width // ellipse_ratio, height // ellipse_ratio);
for row in range(0, my_image_size[0], ellipse_size[0]):
    for col in range(0, my_image_size[1], ellipse_size[1]):
        drawer.ellipse([(row, col), (row + ellipse_size[0], col + ellipse_size[1])], outline="teal");

# Print to window with tk
show_images([my_image], "My Own Image");
