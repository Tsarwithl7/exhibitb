package com.qianfeng.test.lucenetest;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.*;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class LuceneIndexer {

    public static void main(String[] args) throws Exception {
        String indexPath = "S:\\project\\luceneDir2"; // 索引存储路径
        Directory dir = FSDirectory.open(Paths.get(indexPath));
        Analyzer analyzer = new StandardAnalyzer();
        IndexWriterConfig iwc = new IndexWriterConfig(analyzer);

        try (IndexWriter writer = new IndexWriter(dir, iwc)) {
            String[] filePaths = getAllFilePaths("S:\\project\\word2vecpath");

            for (String filePath : filePaths) {
                System.out.println("filepath是:" + filePath);
                Document doc = new Document();
                try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
                    String line;
                    StringBuilder contentBuilder = new StringBuilder();
                    while ((line = br.readLine()) != null) {
                        contentBuilder.append(line).append("\n");
                    }
                    String content = contentBuilder.toString();

                    // 假设内容和向量通过"\t"分隔
                    String[] parts = content.split("\t");
                    if (parts.length == 2) {
                        // 获取不带后缀的文件名
                        String fileName = new File(filePath).getName();
                        int pos = fileName.lastIndexOf(".");
                        if (pos > 0) {
                            fileName = fileName.substring(0, pos);
                        }


                        doc.add(new StringField("filename", fileName, Field.Store.YES));

                        doc.add(new TextField("content", parts[0], Field.Store.YES));
                        doc.add(new StringField("vector", parts[1], Field.Store.YES));


                        // 获取文件的生成日期
                        long fileModified = new File(filePath).lastModified();
                        Date lastModifiedDate = new Date(fileModified);
                        System.out.println("文件最后修改时间是：" + lastModifiedDate);
                        System.out.println("fileModified 是："+fileModified);
                        doc.add(new StringField("filemodified", lastModifiedDate.toString(),Field.Store.YES));

                        // 获取文档大小（单位KB）
                        long fileSize = new File(filePath).length() / 1024;
                        doc.add(new StoredField("filesize", fileSize));
                        writer.addDocument(doc);
                    }
                }
            }
        }
    }

    private static String[] getAllFilePaths(String directoryPath) {
        File directory = new File(directoryPath);
        List<String> filePathsList = new ArrayList<>();
        if (directory.exists() && directory.isDirectory()) {
            getAllFiles(directory, filePathsList);
        }
        return filePathsList.toArray(new String[0]);
    }

    private static void getAllFiles(File directory, List<String> filePathsList) {
        File[] files = directory.listFiles();
        if (files != null) {
            for (File file : files) {
                if (file.isFile()) {
                    filePathsList.add(file.getAbsolutePath());
                } else if (file.isDirectory()) {
                    getAllFiles(file, filePathsList);
                }
            }
        }
    }
}
