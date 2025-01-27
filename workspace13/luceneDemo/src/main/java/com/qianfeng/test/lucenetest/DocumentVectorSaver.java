//package com.qianfeng.test.lucenetest;
//
//import java.io.BufferedWriter;
//import java.io.File;
//import java.io.FileWriter;
//import java.io.IOException;
//import java.nio.file.Files;
//import java.util.Arrays;
//
//public class DocumentVectorSaver {
//
//    private DocumentVectorGenerator vectorGenerator;
//
//    public DocumentVectorSaver(String modelPath) {
//        vectorGenerator = new DocumentVectorGenerator(modelPath);
//    }
//
//    public void processDocuments(String inputDir, String outputDir) throws IOException {
//        File dir = new File(inputDir);
//        File[] files = dir.listFiles((d, name) -> name.endsWith(".txt"));
//
//        if (files != null) {
//            for (File file : files) {
//                String content = new String(Files.readAllBytes(file.toPath()));
//                float[] vector = vectorGenerator.generateDocumentVector(content);
//                String vectorStr = Arrays.toString(vector);
//
//                // 将内容和向量保存到文件
//                saveDocumentVector(file.getName(), content, vectorStr, outputDir);
//            }
//        }
//    }
//
//    private void saveDocumentVector(String fileName, String content, String vectorStr, String outputDir) throws IOException {
//        File outputFile = new File(outputDir, fileName.replace(".txt", "_vector.txt"));
//        try (BufferedWriter writer = new BufferedWriter(new FileWriter(outputFile))) {
//            writer.write(content + "\t" + vectorStr);
//        }
//    }
//
//    public static void main(String[] args) throws Exception {
//        String modelPath = "path/to/your/word2vec/model"; // 修改为你的词向量模型路径
//        DocumentVectorSaver saver = new DocumentVectorSaver(modelPath);
//        saver.processDocuments("S:/filedir", "S:/word2vecpath1");
//    }
//}