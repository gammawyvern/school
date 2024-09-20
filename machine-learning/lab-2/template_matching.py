from PIL import Image, ImageOps;
import numpy as np;
import sys;

########################################
# Load all images and their info
########################################

if len(sys.argv) != 3:
    print("Usage: python template_matching.py <main_image_path> <template_path>");
    sys.exit();

main_image_path = sys.argv[1];
template_path = sys.argv[2];

try:
    main_image = Image.open(main_image_path).convert("L");
    template_image = Image.open(template_path).convert("L");
except Exception as e:
    print(f"Error opening images: {e}");
    sys.exit();

########################################
# Setup numpy matrices for faster and 
# easier calculations 
########################################

main_array = np.array(main_image);
template_array = np.array(template_image);

main_height, main_width = main_array.shape;
template_height, template_width = template_array.shape;

output_size = (main_height - (template_height - 1),
               main_width - (template_width - 1));

output_array = np.full(output_size, None, dtype=object)

########################################
# Calculate output matrix and 
# important matrix values
########################################

# Avoid double for loop, chad behavior
# Single indents only here
for y, x in np.ndindex(output_array.shape):
    main_slice = main_array[y:y+template_height, x:x+template_width];
    output_array[y, x] = np.sum(np.abs(main_slice - template_array));

min_value = np.min(output_array);
max_value = np.max(output_array);
normalized_output = (output_array - min_value) / (max_value - min_value); 
normalized_output = (255 * normalized_output).astype(np.uint8);

min_index = np.unravel_index(np.argmin(normalized_output, axis=None), normalized_output.shape)
flipped_coords = (min_index[1], min_index[0]);
print(f"Closest match: {min_value} @ {flipped_coords}");

image_map = Image.fromarray(normalized_output, mode='L')
image_map.show();

