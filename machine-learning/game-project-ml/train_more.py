import torch
import torchvision
import torchvision.transforms as transforms
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import matplotlib.pyplot as plt
import numpy as np
import multiprocessing

from torchvision.datasets import ImageFolder
from torch.utils.data import DataLoader

import os

class Net(nn.Module):
    def __init__(self):
        super().__init__()
        self.conv1 = nn.Conv2d(3, 32, 5)
        self.conv2 = nn.Conv2d(32, 64, 5)
        self.pool = nn.MaxPool2d(2, 2)
        self.fc1 = nn.Linear(207936, 256)
        self.fc2 = nn.Linear(256, 64)
        self.fc3 = nn.Linear(64, 10)

    def forward(self, x):
        x = self.pool(F.relu(self.conv1(x)))
        x = self.pool(F.relu(self.conv2(x)))
        x = torch.flatten(x, 1)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x

def main():
    transform = transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))
    ])

    batch_size = 4

    train_dataset = ImageFolder(root="./datasets/train/", transform=transform)
    train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)

    validate_dataset = ImageFolder(root="./datasets/validate/", transform=transform)
    validate_loader = DataLoader(validate_dataset, batch_size=batch_size, shuffle=False)

    data_iter = iter(train_loader)
    images, labels = next(data_iter)

    net = Net()
    PATH = os.path.join(os.getcwd(), "./game_guesser.pth")
    net.load_state_dict(torch.load(PATH))

    criterion = nn.CrossEntropyLoss()
    optimizer = optim.SGD(net.parameters(), lr=0.001, momentum=0.9)

    for epoch in range(2):
        print(f"Epoch: {epoch}")

        running_loss = 0.0
        for i, data in enumerate(train_loader, 0):
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
    print(f"Finished training new Neural Network")
    print()

    print(f"Testing Trained Neural Network")
    outputs = net(images)
    
    _, predicted = torch.max(outputs, 1)

    correct = 0
    total = 0
    with torch.no_grad():
        # for data in test_loader:
        for data in validate_loader:
            images, labels = data
            outputs = net(images)
            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()
    print(f"Accuracy of the network: {100 * correct // total}%")

    classes = train_dataset.classes
    correct_pred = { classname: 0 for classname in classes }
    total_pred = { classname: 0 for classname in classes }

    with torch.no_grad():
        # for data in test_loader:
        for data in validate_loader:
            images, labels = data
            outputs = net(images)
            _, predictions = torch.max(outputs, 1)

            for label, prediction in zip(labels, predictions):
                if label == prediction:
                    correct_pred[classes[label]] += 1
                total_pred[classes[label]] += 1

    for classname, correct_count in correct_pred.items():
        accuracy = 100 * float(correct_count) / total_pred[classname]
        print(f"Accuracy for class: {classname:10s}\t{accuracy:.1f}%")
    print()

if __name__ == "__main__":
    multiprocessing.freeze_support()
    main()

