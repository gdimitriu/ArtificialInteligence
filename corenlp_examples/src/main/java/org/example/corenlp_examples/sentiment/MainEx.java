package org.example.corenlp_examples.sentiment;

import com.opencsv.CSVReader;
import com.opencsv.CSVReaderBuilder;
import com.opencsv.exceptions.CsvException;

import java.io.FileReader;
import java.io.IOException;
import java.net.URL;

public class MainEx {
    public static void main(String...args) {
        String text = "This is an excellent book. I enjoy reading it. I can read on Sundays. Today is only Tuesday. Can't wait for next Sunday. The working week is unbearably long. It's awful.";
        NlpPipeline nlpPipeline = new NlpPipeline();
        nlpPipeline.init();
        nlpPipeline.estimatingSentiment(text);

        String filename = "NlpBookReviews.csv";
        URL res = MainEx.class.getClassLoader().getResource(filename);
        try (CSVReader reader = new CSVReaderBuilder(new FileReader(res.getFile())).withSkipLines(1).build()) {
            String[]  row;
            while((row = reader.readNext()) != null) {
                System.out.println("Review: " + row[1] + "\t" + " Amazon rating: " + row[4] + "\t" + " Sentiment: " + nlpPipeline.findSentiment(row[1]));
            }
        } catch (IOException | CsvException ex) {
            ex.printStackTrace();
        }

        text = "I would recommend this book for anyone who wants an introduction to natural language processing. Just finished the book and followed the code all way. I tried the code from the resource web site. I like how it is organized. Well done.";
        nlpPipeline.getReviewSentiment(text,0.4f);

        text = "The weather in the morning was terrible. We decided to go to the cinema.  Had a great time.";
        nlpPipeline.getStorySentiment(text);
    }
}
