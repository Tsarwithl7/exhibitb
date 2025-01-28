package com.qianfeng.test;

import java.util.HashSet;
import java.util.Set;
import java.util.Arrays;

public class EvaluationMetrics {

    // 方法：计算准确率
    public static double calculatePrecision(Set<String> searchResults, Set<String> expectedResults) {
        Set<String> relevantDocumentsFound = new HashSet<>(searchResults);
        relevantDocumentsFound.retainAll(expectedResults); // 计算交集
        return relevantDocumentsFound.isEmpty() ? 0 : (double) relevantDocumentsFound.size() / searchResults.size();
    }

    // 方法：计算召回率
    public static double calculateRecall(Set<String> searchResults, Set<String> expectedResults) {
        Set<String> relevantDocumentsFound = new HashSet<>(searchResults);
        relevantDocumentsFound.retainAll(expectedResults); // 计算交集
        return expectedResults.isEmpty() ? 0 : (double) relevantDocumentsFound.size() / expectedResults.size();
    }

    // 测试方法
    public static void main(String[] args) {
        Set<String> expectedResults = new HashSet<>(Arrays.asList("doc1", "doc2", "doc3","doc5"));
        Set<String> searchResults = new HashSet<>(Arrays.asList("doc1", "doc2", "doc4"));

        double precision = calculatePrecision(searchResults, expectedResults);
        double recall = calculateRecall(searchResults, expectedResults);

        System.out.println("准确率: " + precision);
        System.out.println("召回率: " + recall);
    }
}
