"""expand_mnist.py
~~~~~~~~~~~~~~~~~~

Take the 50,000 MNIST training images, and create an expanded set of
250,000 images, by displacing each training image up, down, left and
right, by one pixel.  Save the resulting file to
../data/mnist_expanded.pkl.gz.

Note that this program is memory intensive, and may not run on small
systems.

"""

from __future__ import print_function

#### Libraries

# Standard library
import pickle
import gzip
import os.path
import random
import argparse
# Third-party libraries
import numpy as np


def build_arg_parser():
    parser = argparse.ArgumentParser(description='MNIST data')
    parser.add_argument('--input-dir', dest='input_dir', type=str,
                        default='../../data', help='Directory for storing data')
    return parser


if __name__ == "__main__":
    args = build_arg_parser().parse_args()
    print("Expanding the MNIST training set")
    file_expanded = args.input_dir + "/mnist_expanded.pkl.gz"
    file_orig = args.input_dir + "/mnist.pkl.gz"
    if os.path.exists(file_expanded):
        print("The expanded training set already exists.  Exiting.")
    else:
        f = gzip.open(file_orig, 'rb')
        data = pickle._Unpickler(f)
        data.encoding = "latin1"
        training_data, validation_data, test_data = data.load()
        f.close()
        expanded_training_pairs = []
        j = 0  # counter
        for x, y in zip(training_data[0], training_data[1]):
            expanded_training_pairs.append((x, y))
            image = np.reshape(x, (-1, 28))
            j += 1
            if j % 1000 == 0: print("Expanding image number", j)
            # iterate over data telling us the details of how to
            # do the displacement
            for d, axis, index_position, index in [
                (1, 0, "first", 0),
                (-1, 0, "first", 27),
                (1, 1, "last", 0),
                (-1, 1, "last", 27)]:
                new_img = np.roll(image, d, axis)
                if index_position == "first":
                    new_img[index, :] = np.zeros(28)
                else:
                    new_img[:, index] = np.zeros(28)
                expanded_training_pairs.append((np.reshape(new_img, 784), y))
        random.shuffle(expanded_training_pairs)
        expanded_training_data = [list(d) for d in zip(*expanded_training_pairs)]
        print("Saving expanded data. This may take a few minutes.")
        f = gzip.open(file_expanded, "w")
        pickle.dump((expanded_training_data, validation_data, test_data), f)
        f.close()
