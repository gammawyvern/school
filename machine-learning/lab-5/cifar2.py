import torch
import torchvision
import torchvision.transforms as transforms
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import matplotlib.pyplot as plt
import numpy as np
import multiprocessing

import os;

########################################
# Class for CNN
########################################

class Net(nn.Module):
    def __init__(self):
        super().__init__();
        self.conv1 = nn.Conv2d(3, 6, 5);
        self.pool = nn.MaxPool2d(2, 2);
        self.conv2 = nn.Conv2d(6, 16, 5);
        self.fc1 = nn.Linear(16 * 5 * 5, 120);
        self.fc2 = nn.Linear(120, 84);
        self.fc3 = nn.Linear(84, 2);

    def forward(self, x):
        x = self.pool(F.relu(self.conv1(x)));
        x = self.pool(F.relu(self.conv2(x)));
        x = torch.flatten(x, 1);
        x = F.relu(self.fc1(x));
        x = F.relu(self.fc2(x));
        x = self.fc3(x);
        return x;



########################################
# Helper functions 
########################################

def img_show(image):
    image = (image / 2) + 0.5;
    np_image = image.numpy()
    plt.imshow(np.transpose(np_image, (1, 2, 0)));
    plt.show();



########################################
# Loading and running data sets
########################################

def main():
    torchvision.datasets.CIFAR10.url="http://www.cs.toronto.edu/~kriz/cifar-10-python.tar.gz"

    transform = transforms.Compose(
        [transforms.ToTensor(),
        transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))]) 

    print("Loading Data");
    batch_size = 4;

    classes = ("plane", "car", "bird", "cat", "deer", "dog", "frog", "horse", "ship", "truck")
    my_classes = ("plane", "ship")

    trainset = torchvision.datasets.CIFAR10(root="./data", train=True,
        download=True, transform=transform)
    testset = torchvision.datasets.CIFAR10(root="./data", train=False,
        download=True, transform=transform)

    # Alter loaded in data
    class_mapping = {class_name: i for i, class_name in enumerate(my_classes)}
    trainset = [(image, class_mapping[classes[label]]) for image, label in trainset if classes[label] in my_classes]
    testset = [(image, class_mapping[classes[label]]) for image, label in testset if classes[label] in my_classes]

    trainloader = torch.utils.data.DataLoader(trainset, batch_size=batch_size,
        shuffle=True, num_workers=2)
    testloader = torch.utils.data.DataLoader(testset, batch_size=batch_size,
        shuffle=False, num_workers=2)

    print("Data Loaded");
    print();

    data_iter = iter(trainloader);
    images, labels = next(data_iter);

    print("Setting up Neural Network");
    net = Net();

    script_dir = os.path.dirname(os.path.abspath(__file__));
    PATH = os.path.join(script_dir, "net_2.pth");

    if os.path.exists(PATH):
        net.load_state_dict(torch.load(PATH));
        print(f"Loaded existing Neural Network at:\n\t\"{PATH}\"");
    else: 
        criterion = nn.CrossEntropyLoss();
        optimizer = optim.SGD(net.parameters(), lr=0.001, momentum=0.9);

        for epoch in range(2): # loop over the dataset multiple times
            running_loss = 0.0
            for i, data in enumerate(trainloader, 0):
                inputs, labels = data
                optimizer.zero_grad()
                outputs = net(inputs)
                loss = criterion(outputs, labels)
                loss.backward()
                optimizer.step()
                running_loss += loss.item()
                if i % 2000 == 1999: 
                    print(f"[{epoch + 1}, {i + 1:5d}] loss: {running_loss / 2000:.3f}")
                    running_loss = 0.0

        torch.save(net.state_dict(), PATH)
        print(f"Finished training new Neural Network");
    print();

    print(f"Testing Trained Neural Network");
    outputs = net(images);
    
    _, predicted = torch.max(outputs, 1);

    correct = 0;
    total = 0;
    with torch.no_grad():
        for data in testloader:
            images, labels = data;
            outputs = net(images);
            _, predicted = torch.max(outputs.data, 1);
            total += labels.size(0);
            correct += (predicted == labels).sum().item();
    print(f"Accuracy of the network on the 10000 test images: {100 * correct // total} %");

    correct_pred = {classname: 0 for classname in my_classes}
    total_pred = {classname: 0 for classname in my_classes}

    with torch.no_grad():
        for data in testloader:
            images, labels = data
            outputs = net(images)
            _, predictions = torch.max(outputs, 1)

            for label, prediction in zip(labels, predictions):
                if label == prediction:
                    correct_pred[my_classes[label]] += 1
                total_pred[my_classes[label]] += 1

    for classname, correct_count in correct_pred.items():
        accuracy = 100 * float(correct_count) / total_pred[classname]
        print(f'Accuracy for class: {classname:5s} is {accuracy:.1f} %')
    print();

########################################
# Script run protection
########################################

if __name__ == "__main__":
    multiprocessing.freeze_support();
    print("Lab Four");
    print();
    main();

