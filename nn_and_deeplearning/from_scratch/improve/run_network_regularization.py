import argparse

from mnist_op import mnist_loader
import network
import matplotlib.pyplot as plt


def build_arg_parser():
    parser = argparse.ArgumentParser(description='MNIST data')
    parser.add_argument('--input-dir', dest='input_dir', type=str,
                        default='../../data', help='Directory for storing data')
    return parser


if __name__ == "__main__":
    args = build_arg_parser().parse_args()
    print("with 30 neurons in hidden layer")
    training_data, validation_data, test_data = mnist_loader.load_data_wrapper(args.input_dir)
    training_data = list(training_data)

    net = network.Network([784, 30, 10], cost=network.CrossEntropyCost)
    net.large_weight_initializer()
    epochs = 30
    evaluation_cost, evaluation_accuracy, training_cost, training_accuracy = (
        net.SGD(training_data[:1000], epochs, 10, 0.5, lambda_val=0.1, evaluation_data=test_data,
                monitor_evaluation_accuracy=True, monitor_training_accuracy=True, monitor_training_cost=True))
    axe = [*range(0, epochs, 1)]
    plt.figure()
    plt.plot(axe, evaluation_accuracy)
    plt.title("evaluation accuracy")
    plt.figure()
    plt.plot(axe, training_cost)
    plt.title("training cost")
    print("with full training data")
    training_data, validation_data, test_data = mnist_loader.load_data_wrapper(args.input_dir)
    training_data = list(training_data)

    net = network.Network([784, 30, 10], cost=network.CrossEntropyCost)
    net.large_weight_initializer()
    epochs = 30
    evaluation_cost, evaluation_accuracy, training_cost, training_accuracy = (
        net.SGD(training_data, epochs, 10, 0.5, lambda_val=5.0, evaluation_data=test_data,
                monitor_evaluation_accuracy=True, monitor_training_accuracy=True, monitor_training_cost=True))
    axe = [*range(0, epochs, 1)]
    plt.figure()
    plt.plot(axe, training_accuracy, "r--", axe, evaluation_accuracy, "b--")
    plt.title("accuracy for full training data")

    print("With full data and 100 neurons in hidden layer")
    training_data, validation_data, test_data = mnist_loader.load_data_wrapper(args.input_dir)
    training_data = list(training_data)

    net = network.Network([784, 100, 10], cost=network.CrossEntropyCost)
    net.large_weight_initializer()
    epochs = 30
    evaluation_cost, evaluation_accuracy, training_cost, training_accuracy = (
        net.SGD(training_data, epochs, 10, 0.5, lambda_val=5.0, evaluation_data=validation_data,
                monitor_evaluation_accuracy=True, monitor_training_accuracy=True, monitor_training_cost=True))
    axe = [*range(0, epochs, 1)]
    plt.figure()
    plt.plot(axe, training_accuracy, "r--", axe, evaluation_accuracy, "b--")
    plt.title("accuracy for full training data and 100 neurons")
    plt.show()
