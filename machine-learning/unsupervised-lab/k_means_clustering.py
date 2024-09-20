import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.preprocessing import LabelEncoder
from sklearn.datasets import make_blobs
import pandas as pd

data = pd.read_csv("./mall_customer.csv")

label_encoder = LabelEncoder()
data['Genre'] = label_encoder.fit_transform(data['Genre'])

X = data[["CustomerID", "Genre", "Age", "Annual Income (k$)", "Spending Score (1-100)"]].values

# Display first 2 features
plt.scatter(X[:, 0], X[:, 1], s=50)
plt.title("Original Data Points")
plt.xlabel("CustomerID")
plt.ylabel("Gender")
plt.show()

n_clusters = 3
kmeans = KMeans(n_clusters=n_clusters)
kmeans.fit(X)

centroids = kmeans.cluster_centers_
labels = kmeans.labels_

# Display first 2 features
plt.scatter(X[:, 0], X[:, 1], c=labels, s=50, cmap='viridis')
plt.scatter(centroids[:, 0], centroids[:, 1], marker='*', s=200, c='red', label='Centroids')
plt.title("Clustered Data Points with Centroids")
plt.xlabel("CustomerID")
plt.ylabel("Gender")
plt.show()

