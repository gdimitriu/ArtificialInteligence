/*
 * This is an example from the article:
 * https://blogs.oracle.com/javamagazine/post/java-sentiment-analysis-stanford-corenlp
 */
package org.example.corenlp_examples.sentiment;

import edu.stanford.nlp.ling.CoreAnnotations;
import edu.stanford.nlp.neural.rnn.RNNCoreAnnotations;
import edu.stanford.nlp.pipeline.Annotation;
import edu.stanford.nlp.pipeline.StanfordCoreNLP;
import edu.stanford.nlp.sentiment.SentimentCoreAnnotations;
import edu.stanford.nlp.trees.Tree;
import edu.stanford.nlp.util.CoreMap;

import java.util.Properties;

public class NlpPipeline {
    protected StanfordCoreNLP pipeline;

    public void init() {
        Properties props = new Properties();
        props.setProperty("annotators","tokenize, ssplit, parse, sentiment");
        pipeline = new StanfordCoreNLP(props);
    }

    public void estimatingSentiment(String text) {
        int sentimentInt;
        String sentimentName;
        Annotation annotation = pipeline.process(text);
        for (CoreMap sentence : annotation.get(CoreAnnotations.SentencesAnnotation.class)) {
            Tree tree = sentence.get(SentimentCoreAnnotations.SentimentAnnotatedTree.class);
            sentimentInt = RNNCoreAnnotations.getPredictedClass(tree);
            sentimentName = sentence.get(SentimentCoreAnnotations.SentimentClass.class);
            System.out.println(sentimentName + "\t" + sentimentInt + "\t" + sentence);
        }
    }
    public String findSentiment(String text) {
        int sentimentInt = 2;
        String sentimentName = "NULL";
        if (text != null && text.length() > 0) {
            Annotation annotation = pipeline.process(text);
            CoreMap sentence = annotation.get(CoreAnnotations.SentencesAnnotation.class).get(0);
            Tree tree = sentence.get(SentimentCoreAnnotations.SentimentAnnotatedTree.class);
            sentimentInt = RNNCoreAnnotations.getPredictedClass(tree);
            sentimentName = sentence.get(SentimentCoreAnnotations.SentimentClass.class);
        }
        return sentimentName;
    }
}
