package com.qianfeng.test.lucenetest;

import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.KnnVectorField;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.search.KnnVectorQuery;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.TopDocs;
//import org.apache.lucene.search.knn.KnnVectorQuery;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Paths;
import java.util.List;

public class LuceneDoc2VecKnnTest {



    private static final Logger log = LoggerFactory.getLogger(LuceneDoc2VecKnnTest.class);
    private static final String VECTOR_FILE_PATH = "S:/word2vecpath/output.txt"; // 文档向量文件路径
    private static final String INDEX_DIR = "S:\\luceneDir2"; // Lucene索引存储路径

    public static void main(String[] args) {
        indexDocVectors();
        float[] queryVector = {0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f,0.5f,0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f}; // 示例查询向量
        int k = 5; // 返回最近邻的数量
        searchKnnVector(queryVector, k);
    }




    private static void indexDocVectors() {
        try {
            Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
            IndexWriterConfig config = new IndexWriterConfig(new StandardAnalyzer());
            try (IndexWriter writer = new IndexWriter(directory, config);
                 BufferedReader br = new BufferedReader(new FileReader(VECTOR_FILE_PATH))) {
                String line;
                while ((line = br.readLine()) != null) {
                    String[] parts = line.split("\t");
                    if (parts.length == 2) {
                        String docId = parts[0];
                        String[] vectorStrings = parts[1].split(" ");
                        float[] vector = new float[vectorStrings.length];
                        for (int i = 0; i < vectorStrings.length; i++) {
                            vector[i] = Float.parseFloat(vectorStrings[i]);
                        }

                        Document doc = new Document();
                        doc.add(new KnnVectorField("vector", vector));
                        doc.add(new StringField("docId", docId, Field.Store.YES));
                        writer.addDocument(doc);
                    }
                }
            }
            log.info("Document indexing completed successfully.");
        } catch (IOException e) {
            log.error("Failed to index documents: ", e);
        }
    }

    private static void searchKnnVector(float[] queryVector, int k) {
        try {
            Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
            IndexReader reader = DirectoryReader.open(directory);
            IndexSearcher searcher = new IndexSearcher(reader);
            KnnVectorQuery query = new KnnVectorQuery("vector", queryVector, k);
            TopDocs topDocs = searcher.search(query, k);

            log.info("Top " + k + " closest hits:");
            for (int i = 0; i < topDocs.scoreDocs.length; i++) {
                Document doc = searcher.doc(topDocs.scoreDocs[i].doc);
                String docId = doc.get("docId");
                float score = topDocs.scoreDocs[i].score;
                log.info((i + 1) + ". DocId: " + docId + ", 分数是: " + score);
            }
        } catch (IOException e) {
            log.error("Failed to search K-NN: ", e);
        }
    }
}
