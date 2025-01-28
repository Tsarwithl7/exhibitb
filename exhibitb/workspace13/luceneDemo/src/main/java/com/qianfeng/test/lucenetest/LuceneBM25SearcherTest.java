package com.qianfeng.test.lucenetest;

import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.similarities.BM25Similarity;
import org.testng.annotations.Test;


import java.nio.file.Paths;



public class LuceneBM25SearcherTest {
    private static final String INDEX_DIR = "S:\\project\\luceneDir2";
    public static void main(String[] args) {
        try {
            Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
            IndexReader reader = DirectoryReader.open(directory);
            IndexSearcher searcher = new IndexSearcher(reader);


            searcher.setSimilarity(new BM25Similarity());


            QueryParser parser = new QueryParser("content", new StandardAnalyzer());
            String queryForSearch="every year";
            Query query = parser.parse(queryForSearch);


            TopDocs topDocs = searcher.search(query, 10);


            System.out.println("Top documents for "+queryForSearch+":");
            for (ScoreDoc sd : topDocs.scoreDocs) {
                Document doc = searcher.doc(sd.doc);
                String docId = doc.get("filename");
                String content=doc.get("content");
                float score = sd.score;

                System.out.printf("DocId: %s, %s, Score: %.4f\n", docId, content, score);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
