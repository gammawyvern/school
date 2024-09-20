from PIL import Image;
import numpy as np;
import sys;


# Command line support for num of pcs 
num_pcs = int(sys.argv[1]) if len(sys.argv) > 1 else 15

# Grayscale
img = Image.open("./input-image.bmp");

# Get mean with numpy
img_matrix = np.array(img);
img_mean = np.mean(img_matrix);

# Get covariance with mean
img_norm = img_matrix - img_mean;
img_covar = np.cov(img_norm, rowvar=False)

# Determine the eigenvectors/values from the covariance 
eig_val, eig_vec = np.linalg.eigh(img_covar);
eig_pairs = list(zip(eig_val, np.transpose(eig_vec))); 

# Sort and keep the 15 most important eigenvectors
eig_pairs_sorted = sorted(eig_pairs, key=lambda pair: pair[0], reverse=True);
key_eig_pairs = eig_pairs_sorted[0:num_pcs];

# Calculate the variance % of 15 chosen eigenvectors
total_eigen_sum = np.sum(eig_val);
key_eigen_sum = np.sum([pair[0] for pair in key_eig_pairs]);
variance_percent = key_eigen_sum / total_eigen_sum;

# Turn key vectors into numpy array
key_eig_vectors = np.array([pair[1] for pair in key_eig_pairs])
key_eig_vectors_transposed = np.transpose(key_eig_vectors)

# Compress image with key vectors
compressed_image = np.matmul(img_norm, key_eig_vectors_transposed);
lossy_compressed_image = np.matmul(compressed_image, key_eig_vectors) + img_mean;

# Display image
# lossy_compressed_image_int = lossy_compressed_image.astype(np.uint8)
compressed_image_pil = Image.fromarray(lossy_compressed_image)
# image_path = "compressed_" + str(num_pcs) + ".bmp";
# compressed_image_pil.save(image_path);
compressed_image_pil.show();

