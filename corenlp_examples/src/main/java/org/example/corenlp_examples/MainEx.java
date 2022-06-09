package org.example.corenlp_examples;

public class MainEx {
    public static void main(String...args) {
        String text = "Good evening. I want a vegetarian pizza.";
        NplPipeline.init();
        NplPipeline.findIntent(text);
    }
}
