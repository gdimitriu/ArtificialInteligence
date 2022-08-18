/*
 * This is an example from the article:
 * https://blogs.oracle.com/javamagazine/post/java-sentiment-analysis-stanford-corenlp
 * https://blogs.oracle.com/javamagazine/post/java-sentiment-analysis-multisentence-text-block
 */
package org.example.corenlp_examples.sentiment;

import edu.stanford.nlp.ling.CoreAnnotations;
import edu.stanford.nlp.neural.rnn.RNNCoreAnnotations;
import edu.stanford.nlp.pipeline.Annotation;
import edu.stanford.nlp.pipeline.StanfordCoreNLP;
import edu.stanford.nlp.sentiment.SentimentCoreAnnotations;
import edu.stanford.nlp.trees.Tree;
import edu.stanford.nlp.util.CoreMap;

import java.util.List;
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

    /**
     * getReviewSentiment
     * overall review sentiment
     * @param review
     * @param weight
     */
    public void getReviewSentiment(String review, float weight) {
        int sentenceSentiment;
        int reviewSentimentAverageSum = 0;
        int reviewSentimentWeightSum = 0;
        Annotation annotation = pipeline.process(review);
        List<CoreMap> sentences = annotation.get(CoreAnnotations.SentencesAnnotation.class);
        int numOfSentences = sentences.size();
        int factor = Math.round(numOfSentences*weight);
        if (factor == 0) {
            factor = 1;
        }
        int divisorLinear = numOfSentences;
        int divisorWeighted = 0;
        for (int i = 0; i < numOfSentences; i++) {
            Tree tree = sentences.get(i).get(SentimentCoreAnnotations.SentimentAnnotatedTree.class);
            sentenceSentiment = RNNCoreAnnotations.getPredictedClass(tree);
            reviewSentimentAverageSum = reviewSentimentAverageSum + sentenceSentiment;
            if (i == 0 || i == numOfSentences - 1) {
                reviewSentimentWeightSum = reviewSentimentWeightSum + sentenceSentiment * factor;
                divisorWeighted += factor;
            } else {
                reviewSentimentWeightSum = reviewSentimentWeightSum + sentenceSentiment;
                divisorWeighted += 1;
            }
        }
        System.out.println("Number of sentences\t\t" + numOfSentences);
        System.out.println("Adapted weighting factor:\t" + factor);
        System.out.println("Weighted average sentiment:\t" + Math.round((float) reviewSentimentWeightSum/divisorWeighted));
        System.out.println("Linear average sentiment:\t" + Math.round((float) reviewSentimentAverageSum/divisorLinear));
    }

    /**
     * get story sentiment
     * @param story
     */
    public void getStorySentiment(String story) {
        int sentenceSentiment;
        int reviewSentimentWeightedSum = 0;
        Annotation annotation = pipeline.process(story);
        List<CoreMap> sentences = annotation.get(CoreAnnotations.SentencesAnnotation.class);
        int divisorWeighted = 0;
        for (int i = 1; i <= sentences.size(); i++) {
            Tree tree = sentences.get(i - 1).get(SentimentCoreAnnotations.SentimentAnnotatedTree.class);
            sentenceSentiment = RNNCoreAnnotations.getPredictedClass(tree);
            reviewSentimentWeightedSum = reviewSentimentWeightedSum + sentenceSentiment * i;
            divisorWeighted += i;
        }
        System.out.println("Weighted average sentiment:\t" + (double)(2 * Math.floor((reviewSentimentWeightedSum/divisorWeighted)/2) + 1.0d));
    }
}
