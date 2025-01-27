package com.qianfeng.test.deeplearning4j.examples.nlp.word2vec;

import java.util.ArrayList;
import java.util.List;

public class TrainDataSet {

    /**
     * 特征集合
     **/
    public List<double[]> features = new ArrayList<>();
    /**
     * 标签集合
     **/
    public List<Integer> labels = new ArrayList<>();
    /**
     * 特征向量维度
     **/
    public int featureDim;

    public int size() {
        return labels.size();
    }

    public double[] getFeature(int index) {
        return features.get(index);
    }

    public int getLabel(int index) {
        return labels.get(index);
    }

    public void addData(double[] feature, int label) {
        if (features.isEmpty()) {
            featureDim = feature.length;
        } else {
            if (featureDim != feature.length) {
                throwDimensionMismatchException(feature.length);
            }
        }
        features.add(feature);
        labels.add(label);
    }

    public void throwDimensionMismatchException(int errorLen) {
        throw new RuntimeException("DimensionMismatchError: 应该传入维度为 " + featureDim + " 的特征向量 , 但传入了维度为 " + errorLen + " 的特征向量");
    }

}
