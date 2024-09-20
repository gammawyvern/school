import numpy as np;

class Perceptron():

    def __init__(self, input_size):
        self.__weights = np.random.rand(input_size)
        self.__bias = np.random.rand()

    def run(self, inputs):
        r_out = np.dot(inputs, self.__weights) + self.__bias;
        return r_out / ((1 + (r_out ** 2)) ** 0.5);

    @property
    def weights(self):
        return np.copy(self.__weights);

    @property
    def bias(self):
        return self.__bias



p = Perceptron(2);
print()
print(f"Weights: {p.weights}");
print(f"Bias: {p.bias}");
print();

inputs = [0.4, 0.9];
print(f"Output for : {inputs}");
print(f"\t{p.run(inputs)}");
print();
