from mnist_op import mnist_loader
import argparse
import network


def build_arg_parser():
    parser = argparse.ArgumentParser(description='MNIST data')
    parser.add_argument('--input-dir', dest='input_dir', type=str,
                        default='../../data', help='Directory for storing data')
    return parser


if __name__ == "__main__":
    args = build_arg_parser().parse_args()
    training_data, validation_data, test_data = mnist_loader.load_data_wrapper(args.input_dir)
    training_data = list(training_data)
    print("Using 30 hidden neurons")
    net = network.Network([784, 30, 10])
    net.SGD(training_data, 30, 10, 3.0, test_data=test_data)
    print("Using 100 hidden neurons")
    training_data, validation_data, test_data = mnist_loader.load_data_wrapper(args.input_dir)
    training_data = list(training_data)
    net = network.Network([784, 100, 10])
    net.SGD(training_data, 30, 10, 3.0, test_data=test_data)
