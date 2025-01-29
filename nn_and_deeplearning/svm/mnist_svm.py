"""
mnist_svm
~~~~~~~~~

A classifier program for recognizing handwritten digits from the MNIST
data set, using an SVM classifier."""

#### Libraries
# My libraries
from mnist_op import mnist_loader

# Third-party libraries
from sklearn import svm
import argparse


def build_arg_parser():
    parser = argparse.ArgumentParser(description='MNIST data')
    parser.add_argument('--input-dir', dest='input_dir', type=str,
                        default='../../data', help='Directory for storing data')
    return parser


def svm_baseline(path="./"):
    training_data, validation_data, test_data = mnist_loader.load_data(path)
    # train
    clf = svm.SVC()
    clf.fit(training_data[0], training_data[1])
    # test
    predictions = [int(a) for a in clf.predict(test_data[0])]
    num_correct = sum(int(a == y) for a, y in zip(predictions, test_data[1]))
    print("Baseline classifier using an SVM.")
    print(str(num_correct) + " of " + str(len(test_data[1])) + " values correct.")


if __name__ == "__main__":
    args = build_arg_parser().parse_args()
    svm_baseline(args.input_dir)
