from PIL import Image, ImageOps, ImageFilter;
import numpy as np;
import sys;

from PIL import ImageTk; 
import tkinter as tk;

########################################
# Validate Input / Load image / Setup
########################################

if len(sys.argv) != 2:
    print("Usage: python filter_image.py <image_path>");
    sys.exit();

image_path = sys.argv[1];

try:
    image = Image.open(image_path);
except Exception as e:
    print(f"Error opening image: {e}");
    sys.exit();

########################################
# Filter functions to apply a filter 
# matrix to all channels of an image.
########################################

def filter_channel(channel, filter_matrix):
    channel_array = np.array(channel);

    output_height = channel_array.shape[0] - (filter_matrix.shape[0] - 1);
    output_width = channel_array.shape[1] - (filter_matrix.shape[1] - 1);
    output_array = np.full((output_height, output_width), None, dtype=object)

    for y, x in np.ndindex(output_array.shape):
        channel_slice = channel_array[y: y + filter_matrix.shape[0], 
                                      x: x + filter_matrix.shape[1]];
        output_array[y, x] = np.sum((channel_slice * filter_matrix));

    return Image.fromarray(output_array.astype(np.uint8), mode='L');

def filter_image(image, filter_matrix):
    channels = image.split(); 
    filtered_channels = [filter_channel(channel, filter_matrix) for channel in channels]
    return Image.merge(image.mode, filtered_channels);

def create_filter(size, fn):
    array = np.array([[fn(x, y, size) for x in range(size)] for y in range(size)]);
    return array / np.sum(array);

########################################
# Filter / Blur code 
########################################

diagonal = lambda x, y, size: x - y == 0 or x - y == size - 1;

filtered_images = [image];
for size in [3, 9, 15]:
    d_filter = create_filter(size, diagonal);
    filtered_image = filter_image(image, d_filter); 
    filtered_images.append(filtered_image);

########################################
# Edge detection code
########################################

gray_image = image.convert("L");
edge_images = [gray_image];
edge_images.append(gray_image.filter(ImageFilter.FIND_EDGES));

########################################
# Printing to screen with Tkinter
########################################

def display_images(images, title):
    root = tk.Tk();
    root.title(title);

    for image_id, image in enumerate(images):
        tk_image = ImageTk.PhotoImage(image)
        label = tk.Label(root, image=tk_image)
        label.grid(row=0, column=image_id, padx=5, pady=5)
        label.image = tk_image

    root.mainloop()

display_images(filtered_images, "Blurred Images (3x3 | 9x9 | 15x15)");
display_images(edge_images, "Edge Detection");

