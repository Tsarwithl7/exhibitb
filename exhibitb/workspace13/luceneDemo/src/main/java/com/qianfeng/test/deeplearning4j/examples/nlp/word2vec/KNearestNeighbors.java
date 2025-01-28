package com.qianfeng.test.deeplearning4j.examples.nlp.word2vec;

import java.util.HashMap;
import java.util.Map;
import java.util.PriorityQueue;

public class KNearestNeighbors {
    /**
     * 训练数据集
     **/
    TrainDataSet trainDataSet;
    /**
     * k值
     **/
    int k;
    /**
     * 距离公式
     **/
    DistanceType distanceType;

    /**
     * @param trainDataSet: 训练数据集
     * @param k:            k值
     */
    public KNearestNeighbors(TrainDataSet trainDataSet, int k, DistanceType distanceType) {
        this.trainDataSet = trainDataSet;
        this.k = k;
        this.distanceType = distanceType;
    }

    // 传入特征，返回预测值
    public int predict(double[] feature) {
        if (feature.length != trainDataSet.featureDim) {
            trainDataSet.throwDimensionMismatchException(feature.length);
        }
        PriorityQueue<Node> nodePriorityQueue = new PriorityQueue<>();
        for (int i = 0; i < trainDataSet.size(); i++) {
            nodePriorityQueue.add(new Node(trainDataSet.getLabel(i), calcDistance(trainDataSet.getFeature(i), feature)));
        }
        int cnt = 0;
        Map<Integer, Integer> map = new HashMap<>();
        int predictLabel = -1;
        int maxNum = -1;
        for (int i = 0; i < k && !nodePriorityQueue.isEmpty(); i++) {
            int label = nodePriorityQueue.poll().label;
            if (map.containsKey(label)) {
                map.replace(label, map.get(label) + 1);
            } else {
                map.put(label, 1);
            }
            if (map.get(label) > maxNum) {
                maxNum = map.get(label);
                predictLabel = label;
            }
            cnt++;
        }
        if (cnt != k || maxNum == -1) {
            throw new RuntimeException("predict fail");
        }
        return predictLabel;
    }

    // 计算距离
    private double calcDistance(double[] arr1, double[] arr2) {
        switch (distanceType) {
            case EuclideanDistance:
                return calcEuclideanDistance(arr1, arr2);
            case ManhattanDistance:
                return calcManhattanDistance(arr1, arr2);
            case ChebyshevDistance:
                return calcChebyshevDistance(arr1, arr2);
            default:
                break;
        }
        throw new RuntimeException("未知的distanceType: " + distanceType);
    }

    // 计算欧式距离
    private double calcEuclideanDistance(double[] arr1, double[] arr2) {
        double res = 0d;
        for (int i = 0; i < arr1.length; i++) {
            res += Math.pow(arr1[i] - arr2[i], 2);
        }
        return Math.sqrt(res);
    }

    // 计算曼哈顿距离
    private double calcManhattanDistance(double[] arr1, double[] arr2) {
        double res = 0d;
        for (int i = 0; i < arr1.length; i++) {
            res += Math.abs(arr1[i] - arr2[i]);
        }
        return res;
    }

    // 计算切比雪夫距离
    private double calcChebyshevDistance(double[] arr1, double[] arr2) {
        double res = 0d;
        for (int i = 0; i < arr1.length; i++) {
            res = Math.max(res, Math.abs(arr1[i] - arr2[i]));
        }
        return res;
    }

    private static class Node implements Comparable<Node> {
        int label;
        double distance;

        public Node(int label, double distance) {
            this.label = label;
            this.distance = distance;
        }

        @Override
        public int compareTo(Node o) {
            return Double.compare(distance, o.distance);
        }
    }

    public enum DistanceType {
        // 欧式距离
        EuclideanDistance,
        // 曼哈顿距离
        ManhattanDistance,
        // 切比雪夫距离
        ChebyshevDistance;
    }

}
