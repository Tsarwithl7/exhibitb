package com.qianfeng.service;

import com.qianfeng.controller.IndexController;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.KnnVectorQuery;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

@Service
public class KNNIndexSearcherService {

    private final Directory directory;
    private final IndexReader reader;
    private final IndexSearcher searcher;




    public KNNIndexSearcherService(@Value("${INDEX_DIR}") String indexPath) throws IOException {
        this.directory = FSDirectory.open(Path.of(indexPath));
        this.reader = DirectoryReader.open(directory);
        this.searcher = new IndexSearcher(reader);
    }


    public List<IndexController.IndexDetails> searchKNN(String query, int k) throws IOException {
        IndexService indexService=new IndexService();
        List<IndexService.InputAndVectorDetails> inputVectors = indexService.trainAndReturnInputVectors(query);
        List<IndexController.IndexDetails> Knnlist = new ArrayList<>();

        // 步骤 1: 创建查询向量

        IndexService.InputAndVectorDetails inputAndVectorDetails = inputVectors.get(0);


        // 步骤 2: 构建 Lucene 查询
        KnnVectorQuery knnQuery = new KnnVectorQuery("knnvectorField",inputAndVectorDetails.getVector(),10);

        long startTime = System.nanoTime();
        // 步骤 3: 执行搜索
        TopDocs topDocs = searcher.search(knnQuery, k);
        long knnsearchtime = System.nanoTime() - startTime;


        // 获取搜索结果
        List<Document> resultDocs = new ArrayList<>();
        for (ScoreDoc scoreDoc : topDocs.scoreDocs) {
            Document doc = searcher.doc(scoreDoc.doc);
            resultDocs.add(doc);
            System.out.println("Document ID: " + doc.get("fileName"));
            System.out.println("Document Content: " + doc.get("content"));
            System.out.println("Score: " + scoreDoc.score);
            String fileName = doc.get("fileName");
            String content = doc.get("content");
            long fileSize = Long.parseLong(doc.get("fileSize"));

            String baseName = fileName.substring(0, fileName.lastIndexOf('.'));

            String imagePathsmall = baseName + ".jpg";
            String imagePath = "http://localhost:8088/images/" + imagePathsmall;
            System.out.println("imagepath是："+imagePath);



            IndexController.IndexDetails indexDetails = new IndexController.IndexDetails(fileName, content, fileSize,imagePath,knnsearchtime);
            Knnlist.add(indexDetails);
        }

        return Knnlist;
    }



    public void close() throws IOException {
        reader.close();
        directory.close();
    }
}
