import argparse
from mnist_op import mnist_loader
import network

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
    net.SGD(training_data, 30, 10, 0.1, lambda_val=5.0, evaluation_data=validation_data,
            monitor_evaluation_accuracy=True, monitor_training_accuracy=True)

    print("with 100 neurons in hidden layer")
    training_data, validation_data, test_data = mnist_loader.load_data_wrapper(args.input_dir)
    training_data = list(training_data)

    net = network.Network([784, 100, 10], cost=network.CrossEntropyCost)
    net.large_weight_initializer()
    net.SGD(training_data, 30, 10, 0.1, lambda_val=5.0, evaluation_data=validation_data,
            monitor_evaluation_accuracy=True, monitor_training_accuracy=True)
