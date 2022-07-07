/*
 * this is the example from
 * https://blogs.oracle.com/javamagazine/post/how-to-program-machine-learning-in-java-with-the-tribuo-library
 */
package org.example.tribuoex;

import org.tribuo.MutableDataset;
import org.tribuo.Prediction;
import org.tribuo.classification.Label;
import org.tribuo.classification.LabelFactory;
import org.tribuo.classification.evaluation.LabelEvaluator;
import org.tribuo.classification.sgd.linear.LinearSGDTrainer;
import org.tribuo.classification.sgd.objectives.LogMulticlass;
import org.tribuo.datasource.IDXDataSource;
import org.tribuo.math.optimisers.AdaGrad;

import java.io.IOException;
import java.nio.file.Paths;
import java.util.List;

public class FirstTest {
    public static void main(String...args) throws IOException {

        /* download the data from http://yann.lecun.com/exdb/mnist/
         */
        String prefix = null;
        if (args.length == 0) {
            prefix = "./";
        } else {
            prefix = args[0];
        }
        var labelFactory = new LabelFactory();
        var trainDataSource = new IDXDataSource<>(Paths.get(prefix,"train-images-idx3-ubyte.gz"),
                Paths.get(prefix,"train-labels-idx1-ubyte.gz"), labelFactory);
        //to be shared across many different model training runs
        var trainDataset = new MutableDataset<>(trainDataSource);
        //create the trainer
        var trainer = new LinearSGDTrainer(new LogMulticlass(), new AdaGrad(0.5), 5, 42);
        //run the trainer
        System.out.println("Start the training....");
        var model = trainer.train(trainDataset);
        System.out.println("End training.");
        //test the data
        var testDataSource = new IDXDataSource<>(Paths.get(prefix,"t10k-images-idx3-ubyte.gz"),
                Paths.get(prefix,"t10k-labels-idx1-ubyte.gz"), labelFactory);
        Prediction<Label> prediction = model.predict(testDataSource.iterator().next());
        List<Prediction<Label>> batchPredictions = model.predict(testDataSource);
        //evaluate the model
        var evaluator = new LabelEvaluator();
        var evaluation = evaluator.evaluate(model, batchPredictions, testDataSource.getProvenance());
        var accuracy = evaluation.accuracy();
        System.out.println(evaluation);
    }
}
