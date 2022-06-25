/*
 * This the example from the article:
 * https://blogs.oracle.com/javamagazine/post/java-sentiment-analysis-domain-specific-phrases
 */
package org.example.corenlp_examples.building;

import edu.stanford.nlp.pipeline.StanfordCoreNLP;
import edu.stanford.nlp.sentiment.BuildBinarizedDataset;
import edu.stanford.nlp.sentiment.SentimentTraining;
import org.example.corenlp_examples.sentiment.NlpPipeline;

import java.io.*;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.nio.file.StandardOpenOption;
import java.util.Properties;

public class TrainOnDomainSpecific extends NlpPipeline {
    private String sampleFileName = "sample.txt";
    private String binarizedFileName = "binarized_trees.txt";
    private String localDirectory = null;
    private String outputFile = null;
    private String modelFileName = null;
    private String trainingFile = null;
    private boolean trainAlready = false;

    public TrainOnDomainSpecific() {
        URL res = TrainOnDomainSpecific.class.getClassLoader().getResource(sampleFileName);
        localDirectory = res.getFile().substring(0, res.getFile().lastIndexOf("/") + 1);
        outputFile = localDirectory + binarizedFileName;
        trainingFile = localDirectory + "trees/train.txt";
        modelFileName = localDirectory + "model.ser.gz";
    }
    private void binarizedFile() {
        if (!new File(outputFile).exists()) {
            try {
                FileOutputStream baos = new FileOutputStream(new File(outputFile));
                System.setOut(new PrintStream(baos));
            } catch (IOException e) {
                System.err.println("Could not open redirect file for binarized tree: " + e.getLocalizedMessage());
                System.exit(1);
            }
            BuildBinarizedDataset.main(new String[]{"-input", localDirectory + sampleFileName});
            //restore stdout
            System.setOut(new PrintStream(new FileOutputStream(FileDescriptor.out)));
        }
    }
    private void unzipAndAppendTrainingData() {
        String trainingDataFile = localDirectory + "training.zip";
        if (!new File(trainingDataFile).exists()) {
            try (InputStream in = new URL("https://nlp.stanford.edu/sentiment/trainDevTestTrees_PTB.zip").openStream();) {
                Files.copy(in, Paths.get(trainingDataFile), StandardCopyOption.REPLACE_EXISTING);
            } catch (IOException e) {
                System.err.println("Could not download zip file : " + e.getLocalizedMessage());
                e.printStackTrace();
                System.exit(2);
            }
            //unzip the file
            UnzipFile unzipFile = new UnzipFile(trainingDataFile, localDirectory);
            try {
                unzipFile.unzipFile();
            } catch (IOException e) {
                System.err.println("Could not unzip training data : " + e.getLocalizedMessage());
                e.printStackTrace();
                new File(trainingDataFile).delete();
                System.exit(3);
            }
            //append training data
            try {
                Files.write(Paths.get(trainingFile), new BufferedInputStream(new FileInputStream(new File(outputFile))).readAllBytes(), StandardOpenOption.APPEND);
            } catch (IOException e) {
                System.err.println("Could not append our training data : " + e.getLocalizedMessage());
                e.printStackTrace();
                new File(trainingDataFile).delete();
                System.exit(4);
            }
        }
    }
    private void trainTheNlp() {
        String devPath = localDirectory + "trees/dev.txt";
        if (!new File(modelFileName).exists()) {
            String[] trainingArguments = {"-numHid", "25", "-epochs", "100", "-devPath", devPath, "-trainPath", trainingFile, "-train", "-model", modelFileName};
            SentimentTraining.main(trainingArguments);
        }
        trainAlready = true;
    }
    public void trainingNlp() {
        binarizedFile();
        unzipAndAppendTrainingData();
        trainTheNlp();
    }

    public static void main(String... args) {
        TrainOnDomainSpecific domainSpecific = new TrainOnDomainSpecific();
        domainSpecific.trainingNlp();
        domainSpecific.init();
        domainSpecific.estimatingSentiment("I hape posted five stars.");
    }
    @Override
    public void init() {
        Properties props = new Properties();
        props.put("sentiment.model", modelFileName);
        props.setProperty("annotators", "tokenize, ssplit, parse, sentiment");
        pipeline = new StanfordCoreNLP(props);
    }
}
