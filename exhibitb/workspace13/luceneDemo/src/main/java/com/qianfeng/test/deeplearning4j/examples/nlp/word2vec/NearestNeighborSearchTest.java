package com.qianfeng.test.deeplearning4j.examples.nlp.word2vec;

import org.deeplearning4j.models.paragraphvectors.ParagraphVectors;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.factory.Nd4j;

import java.io.*;
import java.util.*;

public class NearestNeighborSearchTest {

    public static void main(String[] args) throws IOException {
        String vectorFilePath = "S:\\project\\word2vecpath\\output.txt"; // 输出的文档向量文件路径
        String searchQueryDoc = "doc1"; // 假设要找的文档标签
        int topN = 10; // 找出最相似的N个文档

        // 加载文档向量
        Map<String, INDArray> docVectors = loadDocVectors(vectorFilePath);

        // 执行最近邻搜索
        List<Map.Entry<String, Double>> nearestDocs = findNearestDocuments(docVectors, searchQueryDoc, topN);

        System.out.println("the most simular with " + searchQueryDoc + " is:");
        for (Map.Entry<String, Double> entry : nearestDocs) {
            System.out.println(entry.getKey() + " =>similarity: " + entry.getValue());
        }
    }

    public static Map<String, INDArray> loadDocVectors(String filePath) throws IOException {
        Map<String, INDArray> docVectors = new HashMap<>();
        BufferedReader reader = new BufferedReader(new FileReader(filePath));
        String line;
        while ((line = reader.readLine()) != null) {
            String[] parts = line.split("\t");
            String label = parts[0];
            double[] vector = Arrays.stream(parts[1].split(" "))
                    .mapToDouble(Double::parseDouble)
                    .toArray();
            docVectors.put(label, Nd4j.create(vector));
        }
        reader.close();
        return docVectors;
    }

    public static List<Map.Entry<String, Double>> findNearestDocuments(Map<String, INDArray> docVectors, String queryDoc, int topN) {
        INDArray queryVector = docVectors.get(queryDoc);
        if (queryVector == null) {
            System.out.println("there doesnt exists query vector: " + queryDoc);
            return Collections.emptyList();
        }

        TreeMap<Double, String> distanceMap = new TreeMap<>();
        for (Map.Entry<String, INDArray> entry : docVectors.entrySet()) {
            if (!entry.getKey().equals(queryDoc)) {
                double cosSim = cosineSimilarity(queryVector, entry.getValue());
                distanceMap.put(cosSim, entry.getKey());
            }
        }

        List<Map.Entry<String, Double>> nearestDocs = new ArrayList<>();
        distanceMap.descendingMap().entrySet().stream().limit(topN).forEach(entry -> {
            nearestDocs.add(new AbstractMap.SimpleEntry<>(entry.getValue(), entry.getKey()));
        });

        return nearestDocs;
    }

    public static double cosineSimilarity(INDArray vectorA, INDArray vectorB) {
        // 手动计算点积
        double dotProduct = 0.0;
        for (int i = 0; i < vectorA.columns(); i++) {
            dotProduct += vectorA.getDouble(i) * vectorB.getDouble(i);
        }

        // 计算两个向量的模（L2范数）
        double normA = 0.0;
        double normB = 0.0;
        for (int i = 0; i < vectorA.columns(); i++) {
            normA += Math.pow(vectorA.getDouble(i), 2);
        }
        for (int i = 0; i < vectorB.columns(); i++) {
            normB += Math.pow(vectorB.getDouble(i), 2);
        }
        normA = Math.sqrt(normA);
        normB = Math.sqrt(normB);

        // 计算余弦相似度
        return dotProduct / (normA * normB);
    }
}
