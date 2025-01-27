package com.qianfeng.dao;

import com.qianfeng.controller.IndexController;

import java.util.List;

public class SearchResults {
    private List<IndexController.IndexDetails> indexDetailsList;
    private String fileName;
    private String content;
    private int wordCount;
    public double tfidf;
    public double bm25;
    public long lastModified;
    private long searchTime;
    private double precision;
    private double recall;

    public long bm25SearchTime;
    public long tfidfSearchTime;
    public long knnsearchtime;
    public long defaultsearchtime;
    private int searchMethod;
    public double precisiondefault;
    public double precisionknn;
    public double precisiontfidf;
    public double precisionbm25;
    public double recalldefault;
    public double recalltfidf;
    public double recallbm25;
    public double recallknn;

    public SearchResults() {
    }

    public SearchResults(List<IndexController.IndexDetails> indexDetailsList, String fileName, String content, int wordCount, double tfidf, double bm25, long lastModified, long searchTime, double precision, double recall, long bm25SearchTime, long tfidfSearchTime, long knnsearchtime, long defaultsearchtime, int searchMethod, double precisiondefault, double precisionknn, double precisiontfidf, double precisionbm25, double recalldefault, double recalltfidf, double recallbm25, double recallknn) {
        this.indexDetailsList = indexDetailsList;
        this.fileName = fileName;
        this.content = content;
        this.wordCount = wordCount;
        this.tfidf = tfidf;
        this.bm25 = bm25;
        this.lastModified = lastModified;
        this.searchTime = searchTime;
        this.precision = precision;
        this.recall = recall;
        this.bm25SearchTime = bm25SearchTime;
        this.tfidfSearchTime = tfidfSearchTime;
        this.knnsearchtime = knnsearchtime;
        this.defaultsearchtime = defaultsearchtime;
        this.searchMethod = searchMethod;
        this.precisiondefault = precisiondefault;
        this.precisionknn = precisionknn;
        this.precisiontfidf = precisiontfidf;
        this.precisionbm25 = precisionbm25;
        this.recalldefault = recalldefault;
        this.recalltfidf = recalltfidf;
        this.recallbm25 = recallbm25;
        this.recallknn = recallknn;
    }

    public List<IndexController.IndexDetails> getIndexDetailsList() {
        return indexDetailsList;
    }

    public void setIndexDetailsList(List<IndexController.IndexDetails> indexDetailsList) {
        this.indexDetailsList = indexDetailsList;
    }

    public String getFileName() {
        return fileName;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }

    public int getWordCount() {
        return wordCount;
    }

    public void setWordCount(int wordCount) {
        this.wordCount = wordCount;
    }

    public double getTfidf() {
        return tfidf;
    }

    public void setTfidf(double tfidf) {
        this.tfidf = tfidf;
    }

    public double getBm25() {
        return bm25;
    }

    public void setBm25(double bm25) {
        this.bm25 = bm25;
    }

    public long getLastModified() {
        return lastModified;
    }

    public void setLastModified(long lastModified) {
        this.lastModified = lastModified;
    }

    public long getSearchTime() {
        return searchTime;
    }

    public void setSearchTime(long searchTime) {
        this.searchTime = searchTime;
    }

    public double getPrecision() {
        return precision;
    }

    public void setPrecision(double precision) {
        this.precision = precision;
    }

    public double getRecall() {
        return recall;
    }

    public void setRecall(double recall) {
        this.recall = recall;
    }

    public long getBm25SearchTime() {
        return bm25SearchTime;
    }

    public void setBm25SearchTime(long bm25SearchTime) {
        this.bm25SearchTime = bm25SearchTime;
    }

    public long getTfidfSearchTime() {
        return tfidfSearchTime;
    }

    public void setTfidfSearchTime(long tfidfSearchTime) {
        this.tfidfSearchTime = tfidfSearchTime;
    }

    public long getKnnsearchtime() {
        return knnsearchtime;
    }

    public void setKnnsearchtime(long knnsearchtime) {
        this.knnsearchtime = knnsearchtime;
    }

    public long getDefaultsearchtime() {
        return defaultsearchtime;
    }

    public void setDefaultsearchtime(long defaultsearchtime) {
        this.defaultsearchtime = defaultsearchtime;
    }

    public int getSearchMethod() {
        return searchMethod;
    }

    public void setSearchMethod(int searchMethod) {
        this.searchMethod = searchMethod;
    }

    public double getPrecisiondefault() {
        return precisiondefault;
    }

    public void setPrecisiondefault(double precisiondefault) {
        this.precisiondefault = precisiondefault;
    }

    public double getPrecisionknn() {
        return precisionknn;
    }

    public void setPrecisionknn(double precisionknn) {
        this.precisionknn = precisionknn;
    }

    public double getPrecisiontfidf() {
        return precisiontfidf;
    }

    public void setPrecisiontfidf(double precisiontfidf) {
        this.precisiontfidf = precisiontfidf;
    }

    public double getPrecisionbm25() {
        return precisionbm25;
    }

    public void setPrecisionbm25(double precisionbm25) {
        this.precisionbm25 = precisionbm25;
    }

    public double getRecalldefault() {
        return recalldefault;
    }

    public void setRecalldefault(double recalldefault) {
        this.recalldefault = recalldefault;
    }

    public double getRecalltfidf() {
        return recalltfidf;
    }

    public void setRecalltfidf(double recalltfidf) {
        this.recalltfidf = recalltfidf;
    }

    public double getRecallbm25() {
        return recallbm25;
    }

    public void setRecallbm25(double recallbm25) {
        this.recallbm25 = recallbm25;
    }

    public double getRecallknn() {
        return recallknn;
    }

    public void setRecallknn(double recallknn) {
        this.recallknn = recallknn;
    }

    @Override
    public String toString() {
        return "SearchResults{" +
                "indexDetailsList=" + indexDetailsList +
                ", fileName='" + fileName + '\'' +
                ", content='" + content + '\'' +
                ", wordCount=" + wordCount +
                ", tfidf=" + tfidf +
                ", bm25=" + bm25 +
                ", lastModified=" + lastModified +
                ", searchTime=" + searchTime +
                ", precision=" + precision +
                ", recall=" + recall +
                ", bm25SearchTime=" + bm25SearchTime +
                ", tfidfSearchTime=" + tfidfSearchTime +
                ", knnsearchtime=" + knnsearchtime +
                ", defaultsearchtime=" + defaultsearchtime +
                ", searchMethod=" + searchMethod +
                ", precisiondefault=" + precisiondefault +
                ", precisionknn=" + precisionknn +
                ", precisiontfidf=" + precisiontfidf +
                ", precisionbm25=" + precisionbm25 +
                ", recalldefault=" + recalldefault +
                ", recalltfidf=" + recalltfidf +
                ", recallbm25=" + recallbm25 +
                ", recallknn=" + recallknn +
                '}';
    }
}
