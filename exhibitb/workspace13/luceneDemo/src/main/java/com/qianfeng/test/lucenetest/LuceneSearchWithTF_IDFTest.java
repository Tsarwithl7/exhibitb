package com.qianfeng.test.lucenetest;

import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;

import org.testng.annotations.Test;

import java.nio.file.Paths;


public class LuceneSearchWithTF_IDFTest {
    private static final String INDEX_DIR = "S:\\project\\luceneDir2";

    @Test
    public static void main(String[] args) {
        String queryString = "night";
        try {

            Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
            IndexReader reader = DirectoryReader.open(directory);
            IndexSearcher searcher = new IndexSearcher(reader);


            QueryParser parser = new QueryParser("content", new StandardAnalyzer());
            Query query = parser.parse(queryString);


            TopDocs topDocs = searcher.search(query, 10);


            System.out.println("Top documents for \"" + queryString + "\":");
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
