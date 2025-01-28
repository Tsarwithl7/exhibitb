package com.qianfeng.test.oldtest;

import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.Explanation;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.store.FSDirectory;
import org.testng.annotations.Test;
import org.wltea.analyzer.lucene.IKAnalyzer;
//import org.wltea.analyzer.lucene.IKAnalyzer;


import java.io.IOException;
import java.nio.file.Paths;

public class TestWord2VecLuceneTfIdf {

    // 定义索引文件路径
    private static final String INDEX_DIR = "S:\\project\\luceneDir2";

      @Test
    public void testtf_idf() throws IOException, org.apache.lucene.queryparser.classic.ParseException {
        // 创建分词器
//        IKAnalyzer analyzer = new IKAnalyzer();
        StandardAnalyzer analyzer=new StandardAnalyzer();

        // 创建索引读取器
        IndexReader reader = DirectoryReader.open(FSDirectory.open(Paths.get(INDEX_DIR)));

        // 创建索引搜索器
        IndexSearcher searcher = new IndexSearcher(reader);

        // 构建查询
        QueryParser parser = new QueryParser("name", analyzer);
        Query query = parser.parse("example");

        // 执行查询
        TopDocs topDocs = searcher.search(query, 10);

        System.out.println("tf-idf value for the doc is ：");
        // 输出查询结果及对应的tf-idf值
        for (ScoreDoc scoreDoc : topDocs.scoreDocs) {
            Document doc = searcher.doc(scoreDoc.doc);
            double tfIdf = getTfIdf(searcher, query, scoreDoc.doc, "name");
            System.out.printf("%s (tf-idf=%.4f)\n", doc.get("name"), tfIdf);
//          System.out.printf("%s (tf-idf=%.4f)\n", tfIdf);
        }

        // 关闭资源
        reader.close();
    }

    /**
     * 计算文档中指定字段的tf-idf值
     *
     * @param searcher 索引搜索器
     * @param query    查询对象
     * @param docId    文档ID
     * @param field    字段名
     * @return tf-idf值
     * @throws IOException
     */
    public static double getTfIdf(IndexSearcher searcher, Query query, int docId, String field) throws IOException {


        Explanation explain = searcher.explain(query, docId);
        System.out.println("explain"+explain.toString());
//        double tf =Math.sqrt(explain.getDetails()[0].getDetails()[0].getValue().doubleValue());
//        double idf = (double) explain.getDetails()[1].getValue().doubleValue();
        double tf = Math.sqrt(explain.getDetails()[0].getValue().doubleValue());
        double tf1 = Math.sqrt(explain.getDetails()[0].getDetails()[0].getValue().doubleValue());
        double idf = explain.getValue().doubleValue();
        return tf * idf;
    }
}
