package com.qianfeng.service;

import com.qianfeng.controller.IndexController;
import com.qianfeng.dao.SearchResults;
import com.qianfeng.test.deeplearning4j.examples.nlp.word2vec.Doc2VecMultiFileTest;
import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.en.EnglishAnalyzer;
import org.apache.lucene.document.*;
import org.apache.lucene.index.*;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.*;
import org.apache.lucene.search.similarities.BM25Similarity;
import org.apache.lucene.search.similarities.ClassicSimilarity;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.BytesRef;
import org.deeplearning4j.models.paragraphvectors.ParagraphVectors;
import org.deeplearning4j.text.documentiterator.LabelsSource;
import org.deeplearning4j.text.sentenceiterator.CollectionSentenceIterator;
import org.deeplearning4j.text.sentenceiterator.SentenceIterator;
import org.deeplearning4j.text.tokenization.tokenizer.preprocessor.CommonPreprocessor;
import org.deeplearning4j.text.tokenization.tokenizerfactory.DefaultTokenizerFactory;
import org.deeplearning4j.text.tokenization.tokenizerfactory.TokenizerFactory;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;
import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.time.Instant;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;
import java.util.*;

@Service
public class IndexService {

    private static final Logger log = LoggerFactory.getLogger(IndexService.class);
    private static final String UPLOAD_DIR = "S:\\project\\filedir";
    private static final String INDEX_DIR = "S:\\project\\indexdir";
    private ParagraphVectors vec;

    static Set<String> expectedResults = new HashSet<>(Arrays.asList("Smash-and-grab.txt", "A noble gangster.txt", "Fifty pence worth of trouble.txt","Flying cats.txt","Fifty pence worth of trouble.txt","The double life of Alfred Bloggs.txt"));

    public LabelsSource doc2vec() throws IOException {
        Doc2VecMultiFileTest.CustomFileIterator fileIterator = new Doc2VecMultiFileTest.CustomFileIterator(UPLOAD_DIR);
        Map<String, String> labelToContentMap = new HashMap<>();

        // 读取文件并映射标签到内容
        while (fileIterator.hasNext()) {
            File file = fileIterator.nextFile();
            String content = new String(java.nio.file.Files.readAllBytes(file.toPath()));
            labelToContentMap.put(file.getName(), content);
        }

        // 初始化句子迭代器和标签源
        LabelsSource source = new LabelsSource(new ArrayList<>(labelToContentMap.keySet()));
        List<String> contents = new ArrayList<>(labelToContentMap.values());
        SentenceIterator sentenceIterator = new CollectionSentenceIterator(contents);

        // 配置并训练段落向量模型
        TokenizerFactory tokenizerFactory = new DefaultTokenizerFactory();
        tokenizerFactory.setTokenPreProcessor(new CommonPreprocessor());

        this.vec = new ParagraphVectors.Builder()
                .minWordFrequency(1)
                .iterations(5)
                .epochs(1)
                .layerSize(50)
                .learningRate(0.025)
                .labelsSource(source)
                .windowSize(5)
                .iterate(sentenceIterator)
                .trainWordVectors(false)
                .tokenizerFactory(tokenizerFactory)
                .build();

        vec.fit();
        System.out.println(vec);
        System.out.println("训练完成");
        return source;
    }




public static String convertTimestampToDate(long timestamp) {

    Instant instant = Instant.ofEpochMilli(timestamp);

    LocalDateTime dateTime = LocalDateTime.ofInstant(instant, ZoneId.of("UTC"));
    DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
    return dateTime.format(formatter);
}



    public List<IndexController.IndexDetails> searchAll() throws IOException {
        List<IndexController.IndexDetails> detailsList = new ArrayList<>();

        Query query = new MatchAllDocsQuery();
        FSDirectory dir = FSDirectory.open(Paths.get(INDEX_DIR));
        IndexReader reader = DirectoryReader.open(dir);
        IndexSearcher indexSearcher = new IndexSearcher(reader);
        TopDocs topDocs = indexSearcher.search(query, Integer.MAX_VALUE);

        for (ScoreDoc scoreDoc : topDocs.scoreDocs) {
            Document doc = indexSearcher.doc(scoreDoc.doc);

            String fileName = doc.get("fileName");
            String content = doc.get("content");
            String imagePath = doc.get("imagePath");
            long fileSize= Long.parseLong(doc.get("fileSize"));
            long last_modified = Long.parseLong(doc.get("last_modified"));
            String lastModified = convertTimestampToDate(last_modified);
            String wordCountStr = doc.get("wordCount");
            long wordCount = 0;
            if (wordCountStr != null) {
                wordCount = Long.parseLong(wordCountStr);
            }
            IndexController.IndexDetails details = new IndexController.IndexDetails(
                    fileName,
                    content,
                    last_modified,
                    lastModified,
                    wordCount,
                    imagePath,

                    fileSize,

                    0.0, // tfidf
                    0.0// bm25

            );
            detailsList.add(details);
        }
        reader.close();
        return detailsList;
    }



    public static List<InputAndVectorDetails> trainAndReturnInputVectors(String input) throws IOException {
        List<String> sentences = Arrays.asList(input.split("\\.\\s*"));
        SentenceIterator sentenceIterator = new CollectionSentenceIterator(sentences);

        TokenizerFactory tokenizerFactory = new DefaultTokenizerFactory();
        tokenizerFactory.setTokenPreProcessor(new CommonPreprocessor());

        ParagraphVectors vec = new ParagraphVectors.Builder()
                .minWordFrequency(1)
                .iterations(5)
                .epochs(1)
                .layerSize(50)
                .learningRate(0.025)
                .iterate(sentenceIterator)
                .trainWordVectors(false)
                .tokenizerFactory(tokenizerFactory)
                .build();

        vec.fit();

        List<InputAndVectorDetails> detailsList = new ArrayList<>();
        for (String sentence : sentences) {
            INDArray vector = vec.inferVector(sentence);
            float[] vectorArray = vector.data().asFloat(); // 将INDArray转换为double数组
            detailsList.add(new InputAndVectorDetails(sentence, vectorArray));
        }

        return detailsList;
    }





    public void createIndex() throws IOException {
        LabelsSource labelsSource1 = doc2vec();


        HashMap<String, double[]> objectObjectHashMap = new HashMap<>();
        HashMap<String, float[]> floatVectorHashMap = new HashMap<>();



        List<String> labels = labelsSource1.getLabels();

        for (String l:labels) {
            double[] wordVector = vec.getWordVector(l);
            objectObjectHashMap.put(l,wordVector);
        }

        for (String l : labels) {
            double[] doubles = objectObjectHashMap.get(l);
            float[] floatArray = new float[doubles.length];
            for (int i = 0; i < doubles.length; i++) {
                floatArray[i] = (float) doubles[i];
            }
            floatVectorHashMap.put(l, floatArray);
        }

// 打印floatVectorHashMap中的值
        for (Map.Entry<String, float[]> entry : floatVectorHashMap.entrySet()) {
            String key = entry.getKey();
            float[] value = entry.getValue();
            System.out.println("Key::::: " + key);
            System.out.println("Value: " + Arrays.toString(value));
        }



        Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
        Analyzer analyzer = new EnglishAnalyzer();
        IndexWriterConfig config = new IndexWriterConfig(analyzer);
        try (IndexWriter indexWriter = new IndexWriter(directory, config)) {
            File dir = new File(UPLOAD_DIR);
            File[] files = dir.listFiles((d, name) -> name.endsWith(".txt"));
            for (File file:files) {
                System.out.println("file列表是:"+file.toString());
            }


            if (files != null) {
                for (File file : files) {
                    String filePath = file.getPath();
                    int lastIndex = filePath.lastIndexOf(".txt");
                    String pathBeforeTxt = filePath.substring(0, lastIndex);



                    String imagePath=pathBeforeTxt+".jpg";
                    Document document = new Document();
                    String fileName=file.getName();

                    // 为fileName，content，fileSize，lastModified，formattedFileSize字段创建索引
                    String content = new String(Files.readAllBytes(file.toPath()), StandardCharsets.UTF_8);



                    // 为fileName字段创建索引
                    document.add(new TextField("fileName", fileName, Field.Store.YES));
                    document.add(new StringField("imagePath", imagePath, Field.Store.YES));
                    // 为content字段创建索引并存储Term Vector
                    FieldType contentType = new FieldType();
                    contentType.setStored(true);
                    contentType.setIndexOptions(IndexOptions.DOCS_AND_FREQS_AND_POSITIONS);
                    contentType.setStoreTermVectors(true);
                    contentType.setStoreTermVectorPositions(true);
                    document.add(new Field("content", content, contentType));


                    int wordCount = content.split("\\s+").length; // 通过空白字符分割

                    document.add(new StoredField("fileSize", file.length()));
                    // 索引字数并存储
                    document.add(new IntPoint("wordCount", wordCount));
                    document.add(new StoredField("wordCount", wordCount));




                    long lastModifiedTimestamp = file.lastModified();

                    document.add(new LongPoint("last_modified", lastModifiedTimestamp));
                    document.add(new StoredField("last_modified", lastModifiedTimestamp));

                    document.add(new StringField("lastModified", new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date(file.lastModified())), Field.Store.YES));
                    // 格式化文件大小
                    document.add(new StringField("formattedFileSize", formatFileSize(file.length()), Field.Store.YES));
                    float[] floats = floatVectorHashMap.get(fileName);
                    if (floats != null) {
                        System.out.println("对应的值是：。。。。。。" + Arrays.toString(floats));
                    } else {
                        System.out.println("没有找到对应的值");
                    }
                    String vectorAsString = Arrays.toString(floats);
                    document.add(new StoredField("vectorAsString", vectorAsString));
                    document.add(new KnnVectorField("knnvectorField", floats));

                    indexWriter.addDocument(document);
                }
            }
        }
    }

    public void deleteIndexTest() throws Exception{
        //1. 创建分词器
        Analyzer analyzer = new EnglishAnalyzer();
        //2. 指定Directory对象, 指定索引库的位置
        Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
        //3. 创建config初始化对象
        IndexWriterConfig config = new IndexWriterConfig(analyzer);
        //4. 创建输出流对象
        IndexWriter indexWriter = new IndexWriter(directory, config);
        String fileName="doc42207f7b-5ffc-47e8-b2ab-663848285a50.txt";

        //5. 执行删除
        indexWriter.deleteDocuments(new Term("fileName",fileName));

        indexWriter.commit();
        //6. 释放资源
        indexWriter.close();
    }

    public void updateDocumentContent(String fileName, String newContent) throws Exception {
        // 1. 创建分词器
        Analyzer analyzer = new EnglishAnalyzer();
        // 2. 指定Directory对象, 指定索引库的位置
        Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
        // 3. 创建config初始化对象
        IndexWriterConfig config = new IndexWriterConfig(analyzer);
        // 4. 创建输出流对象（IndexWriter）
        try (IndexWriter indexWriter = new IndexWriter(directory, config)) {
            // 创建一个新的文档对象
            Document document = new Document();
            // 添加/更新 fileName 字段
            document.add(new StringField("fileName", fileName, Field.Store.YES));
            // 更新 content 字段
            FieldType contentType = new FieldType();
            contentType.setStored(true);
            contentType.setIndexOptions(IndexOptions.DOCS_AND_FREQS_AND_POSITIONS);
            contentType.setStoreTermVectors(true);
            contentType.setStoreTermVectorPositions(true);
            document.add(new Field("content", newContent, contentType));

            // 使用fileName作为标识符来更新文档。如果fileName存在，则先删除旧文档，再添加新文档。
            indexWriter.updateDocument(new Term("fileName", fileName), document);

            // 提交更改
            indexWriter.commit();
            // 注：不需要显式关闭IndexWriter，try-with-resources会自动处理
        } catch (IOException e) {
            e.printStackTrace();
            throw new Exception("Failed to update document due to an error: " + e.getMessage(), e);
        }
    }


    public void deleteDocumentByFileName(String fileName) {
        try {
            Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
            IndexWriterConfig config = new IndexWriterConfig(new EnglishAnalyzer());
            try (IndexWriter writer = new IndexWriter(directory, config)) {
                // 使用fileName创建一个Term
                Term term = new Term("fileName", fileName);
                // 删除匹配该Term的所有文档
                writer.deleteDocuments(term);
                // 提交更改
                writer.commit();
//                System.out.println("Document with fileName \"" + fileName + "\" has been deleted.");
            }
        } catch (IOException e) {
            e.printStackTrace();
            System.err.println("Failed to delete document due to an error: " + e.getMessage());
        }
    }


    public float calculateCustomTFIDFScore(IndexReader reader, String termText, int docId) throws IOException {
        Term term = new Term("content", termText);
        long docFreq = reader.docFreq(term);
        long numDocs = reader.numDocs();
        double idf = Math.log(1 + (numDocs / (double)(docFreq + 1))); // 计算IDF

        // 计算给定文档中的TF
        Terms terms = reader.getTermVector(docId, "content"); // 获取文档的词向量
        if (terms != null) {
            TermsEnum termsEnum = terms.iterator();
            PostingsEnum postings = null;
            while (termsEnum.next() != null) {
                if (termsEnum.term().utf8ToString().equals(termText)) {
                    postings = termsEnum.postings(postings, PostingsEnum.FREQS);
                    if (postings.nextDoc() != PostingsEnum.NO_MORE_DOCS) {
                        int freq = postings.freq(); // 词频
                        double tf = 1 + Math.log(freq); // 计算TF
                        return (float) (tf * idf);
                    }
                }
            }
        }
        return 0;
    }

    public static class InputAndVectorDetails {
        private String input;
        private float[] vector;

        public InputAndVectorDetails(String input, float[] vector) {
            this.input = input;
            this.vector = vector;
        }


        public String getInput() {
            return input;
        }

        public void setInput(String input) {
            this.input = input;
        }

        public float[] getVector() {
            return vector;
        }

        public void setVector(float[] vector) {
            this.vector = vector;
        }

        @Override
        public String toString() {
            return "Input: " + input + "\nVector: " + vector.toString();
        }
    }




    public List<IndexController.IndexDetails> compareScoringMethods(String input) throws IOException {
        List<IndexController.IndexDetails> indexDetailsList = new ArrayList<>();
        try (Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
             IndexReader reader = DirectoryReader.open(directory)) {
            IndexSearcher searcher = new IndexSearcher(reader);
            searcher.setSimilarity(new BM25Similarity()); // 设置一次，用于所有查询

            String[] terms = input.trim().split("\\s+");

            // 为每个词构建查询并进行搜索，而不是遍历每个文档
            for (String termText : terms) {
                Query query = new TermQuery(new Term("content", termText));

                long startTime = System.nanoTime();
                TopDocs topDocsBM25 = searcher.search(query, reader.numDocs());
                long bm25SearchTime = System.nanoTime() - startTime;


                for (ScoreDoc sd : topDocsBM25.scoreDocs) {
                    Document doc = searcher.doc(sd.doc);
                    String fileName = doc.get("fileName");
                    String content = doc.get("content");
                    String wordCount = doc.get("wordCount");
                    String formattedFileSize = doc.get("formattedFileSize");

                    String fileSize = doc.get("fileSize");



                    String baseName = fileName.substring(0, fileName.lastIndexOf('.'));

                    String imagePathsmall = baseName + ".jpg";
                    String imagePath = "http://localhost:8088/images/" + imagePathsmall;
                    System.out.println("imagepath是："+imagePath);


                    // 测量TF-IDF检索时间
                    startTime = System.nanoTime();
                    // 计算自定义TF-IDF得分
                    float customTFIDFScore = calculateCustomTFIDFScore(reader, termText, sd.doc);
                    long tfidfSearchTime = System.nanoTime() - startTime;

                    IndexController.IndexDetails indexDetail = new IndexController.IndexDetails();
                    indexDetail.setContent(content);
                    indexDetail.setFileSize(Long.parseLong(fileSize));
                    indexDetail.setWordCount(Long.parseLong(wordCount));
                    indexDetail.setFileName(fileName);
                    indexDetail.setTfidf(customTFIDFScore);
                    indexDetail.setImagePath(imagePath);
                    indexDetail.setBm25(sd.score); // 直接使用BM25得分

                    indexDetail.setBm25SearchTime(bm25SearchTime);
                    indexDetail.setTfidfSearchTime(tfidfSearchTime);



                    indexDetailsList.add(indexDetail);

                    // 输出结果，可根据需要调整
                    System.out.println("fileName: " + fileName + " content: " + content + " wordCount: " + wordCount +
                            " formattedFileSize: " + formattedFileSize + " customTFIDFScore: " + customTFIDFScore +" imagePath:"+imagePath+" fileSize:"+fileSize+
                            " bm25Score: " + sd.score);
                    // 输出检索时间，用于比较
                    System.out.println("Term: " + termText + " BM25 Search Time: " + bm25SearchTime + "ms, TF-IDF Search Time: " + tfidfSearchTime + "ms");
                }
            }
        }
        return indexDetailsList;
    }



    public List<IndexController.IndexDetails> calculate_bm25(String input) throws IOException {
        List<IndexController.IndexDetails> indexDetailsList = new ArrayList<>();

        try (Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
             IndexReader reader = DirectoryReader.open(directory)) {
            IndexSearcher searcher = new IndexSearcher(reader);

            searcher.setSimilarity(new BM25Similarity());

            String[] terms = input.trim().split("\\s+");
            for (String termText : terms) {
                Query query = new TermQuery(new Term("content", termText));
                long startTime = System.nanoTime();
                TopDocs topDocs = searcher.search(query, reader.numDocs());
                long searchTime = System.nanoTime() - startTime;

                for (ScoreDoc sd : topDocs.scoreDocs) {
                    Document doc = searcher.doc(sd.doc);
                    IndexController.IndexDetails indexDetail = createIndexDetail(doc, searcher, termText);
                    indexDetail.setBm25(sd.score);
                    indexDetail.setBm25SearchTime(searchTime);
                    indexDetailsList.add(indexDetail);
                }
            }
        }
        return indexDetailsList;
    }
// try (IndexReader indexReader = FSDirectory.open(INDEX_DIR)) {
//        IndexSearcher searcher=new IndexSearcher(indexReader);
//
//    }


//    public double calculatePrecision(List<Document> retrievedDocuments, List<Document> relevantDocuments) {
//        int relevantCount = 0;
//        for (Document doc : retrievedDocuments) {
//            if (relevantDocuments.contains(doc)) {
//                relevantCount++;
//            }
//        }
//        return relevantCount / (double) retrievedDocuments.size();
//    }



    public List<IndexController.IndexDetails> calculate_tfidf(String input) throws IOException {
        List<IndexController.IndexDetails> indexDetailsList = new ArrayList<>();
        try (Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
             IndexReader reader = DirectoryReader.open(directory)) {
            IndexSearcher searcher = new IndexSearcher(reader);





            searcher.setSimilarity(new ClassicSimilarity());

            String[] terms = input.trim().split("\\s+");
            for (String termText : terms) {
                Query query = new TermQuery(new Term("content", termText));
                long startTime = System.nanoTime();
                TopDocs topDocs = searcher.search(query, reader.numDocs());
                long searchTime = System.nanoTime() - startTime;

                for (ScoreDoc sd : topDocs.scoreDocs) {
                    Document doc = searcher.doc(sd.doc);
                    float customTFIDFScore = calculateCustomTFIDFScore(reader, termText, sd.doc);

                    IndexController.IndexDetails indexDetail = createIndexDetail(doc, searcher, termText);
                    indexDetail.setTfidf(customTFIDFScore);
                    indexDetail.setTfidfSearchTime(searchTime);
                    indexDetailsList.add(indexDetail);
                }
            }
        }

        return indexDetailsList;
    }





    private IndexController.IndexDetails createIndexDetail(Document doc, IndexSearcher searcher, String termText) throws IOException {
        String fileName = doc.get("fileName");
        String content = doc.get("content");
        String wordCount = doc.get("wordCount");
        String fileSize = doc.get("fileSize");
        String baseName = fileName.substring(0, fileName.lastIndexOf('.'));
        String imagePathsmall = baseName + ".jpg";
        String imagePath = "http://localhost:8088/images/" + imagePathsmall;

        IndexController.IndexDetails indexDetail = new IndexController.IndexDetails();
        indexDetail.setContent(content);
        indexDetail.setFileSize(Long.parseLong(fileSize));
        indexDetail.setWordCount(Long.parseLong(wordCount));
        indexDetail.setFileName(fileName);
        indexDetail.setImagePath(imagePath);

        return indexDetail;
    }




    private float calculateAvgFieldLength(IndexSearcher searcher) throws IOException {
        long totalLength = 0;
        int docCount = 0;

        IndexReader reader = searcher.getIndexReader();
        for (int i = 0; i < reader.maxDoc(); i++) {
            if (reader.hasDeletions()) {
                continue; // Skip deleted documents
            }

            Terms terms = reader.getTermVector(i, "content"); // Assuming 'content' is the field you're interested in
            if (terms != null) {
                TermsEnum termsEnum = terms.iterator();
                while (termsEnum.next() != null) {
                    totalLength += termsEnum.totalTermFreq();
                }
                docCount++;
            }
        }

        if (docCount > 0) {
            return (float) totalLength / docCount;
        } else {
            return 0;
        }
    }

    public float getDocumentLength(IndexSearcher searcher, String fileName) throws IOException {
        Query query = new TermQuery(new Term("fileName", fileName));
        TopDocs topDocs = searcher.search(query, 1);
        if (topDocs.totalHits.value == 0) {
            return 0;
        }
        int docId = topDocs.scoreDocs[0].doc;
        Terms terms = searcher.getIndexReader().getTermVector(docId, "content");
        if (terms == null) {
            return 0;
        }
        TermsEnum termsEnum = terms.iterator();
        long documentLength = 0;
        BytesRef term;
        while ((term = termsEnum.next()) != null) {
            documentLength += termsEnum.totalTermFreq();
        }
        return documentLength;
    }

    public float getTermFrequency(IndexSearcher searcher, int docId, String termText) throws IOException {
        Terms termVector = searcher.getIndexReader().getTermVector(docId, "content");
        if (termVector == null) {
            return 0;
        }
        TermsEnum termsEnum = termVector.iterator();
        BytesRef term = new BytesRef(termText);
        if (termsEnum.seekExact(term)) {
            return termsEnum.totalTermFreq();
        }
        return 0;
    }

    private float calculateBM25(float tf, int df, int N, float docLength, float avgdl) {
        float k1 = 1.2f;
        float b = 0.75f;
        float idf = (float)(Math.log((N - df + 0.5) / (df + 0.5) + 1));
        float termFrequencyComponent = tf / (tf + k1 * (1 - b + b * docLength / avgdl));
        return idf * termFrequencyComponent;
    }




    public List<IndexController.IndexDetails> compareScoringMethodsolderversion(String input) throws IOException {
        List<IndexController.IndexDetails> indexDetailsList = new ArrayList<>();
        try (Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
             IndexReader reader = DirectoryReader.open(directory)) {
            IndexSearcher searcher = new IndexSearcher(reader);
            searcher.setSimilarity(new BM25Similarity());
            float avgdl = calculateAvgFieldLength(searcher);
            int N = reader.numDocs();

            String[] terms = input.trim().split("\\s+");

            System.out.println("Starting compareScoringMethods...");

            for (String termText : terms) {
                Term term = new Term("content", termText);
                int df = reader.docFreq(term);

                System.out.println("Terms length: " + terms.length);


                for (int i = 0; i < reader.maxDoc(); i++) {


                    Document doc = reader.document(i);
                    String fileName = doc.get("fileName");
                    if (fileName == null) continue; // 跳过文件名为空的文档

                    // 获取文档长度
                    float docLength = getDocumentLength(searcher, fileName); // 使用fileName获取文档长度

                    // 获取词频
                    float tf = getTermFrequency(searcher, i, termText); // 获取特定文档中特定词项的频率

                    // 手动计算BM25得分
                    float bm25Score = calculateBM25(tf, df, N, docLength, avgdl);
                    float customTFIDFScore = calculateCustomTFIDFScore(reader, termText, i); // 自定义TF-IDF得分


                    if (customTFIDFScore > 0) {
                        String content = doc.get("content").substring(0, Math.min(doc.get("content").length(), 30));
                        int fileSize= Integer.parseInt(doc.get("fileSize"));
                        String wordCount = doc.get("wordCount");
                        String formattedFileSize = doc.get("formattedFileSize");


                        IndexController.IndexDetails indexDetail = new IndexController.IndexDetails();
                        indexDetail.setContent(content);
                        indexDetail.setWordCount(Long.parseLong(wordCount));
                        indexDetail.setFileName(fileName);
                        indexDetail.setTfidf(customTFIDFScore);
                        indexDetail.setFileSize(fileSize);
                        indexDetail.setWordCount(Long.parseLong(wordCount));
                        indexDetail.setBm25(bm25Score);

                        indexDetailsList.add(indexDetail);

                        // 输出结果，可根据需要调整
                        System.out.println("fileName: " + fileName + " content: " + content + " wordCount: " + wordCount +"fileSize: "+fileSize+
                                " formattedFileSize: " + formattedFileSize + " customTFIDFScore: " + customTFIDFScore +
                                " bm25Score: " + bm25Score);
                    }
                }
            }
        }
        return indexDetailsList;
    }

    public List<SearchResults> compareall(String queryText, HashSet<String> expectedResults) throws ParseException, IOException {
        List<SearchResults> lists = new ArrayList<>(4); // 将二维数组修改为使用SearchResults

        // 执行默认搜索
        Set<String> indexdefault = new TreeSet<>();

        Directory directory1 = FSDirectory.open(Paths.get(INDEX_DIR));
        IndexReader reader1 = DirectoryReader.open(directory1);
        IndexSearcher searcher1 = new IndexSearcher(reader1);

        Analyzer analyzer = new EnglishAnalyzer();
        Analyzer analyzer2 = new EnglishAnalyzer();

        long currentTimeMillis = System.currentTimeMillis();


        QueryParser contentQueryParser = new QueryParser("content", analyzer);
        QueryParser fileNameQueryParser = new QueryParser("fileName", analyzer2);


        // 根据输入决定查询类型
        Query textQuery = queryText == null || queryText.trim().isEmpty()
                ? new MatchAllDocsQuery()
                : contentQueryParser.parse(queryText);

        // 根据输入创建针对fileName的查询
        Query fileNameQuery = queryText == null || queryText.trim().isEmpty()
                ? new MatchAllDocsQuery()
                : fileNameQueryParser.parse(queryText);

        BooleanQuery booleanQuery = null;


        // 组合查询
        booleanQuery = new BooleanQuery.Builder()
                .add(textQuery, BooleanClause.Occur.SHOULD)
                .add(fileNameQuery, BooleanClause.Occur.SHOULD)
                .build();

        // 执行查询
        try (FSDirectory open = FSDirectory.open(Paths.get(INDEX_DIR));
             IndexReader reader = DirectoryReader.open(open)) {

            IndexSearcher indexSearcher = new IndexSearcher(reader);
            long l = System.nanoTime();
            TopDocs topDocs = indexSearcher.search(booleanQuery, 10);
            long l1 = System.nanoTime();

            long l2 = l1 - l;

            for (ScoreDoc scoreDoc : topDocs.scoreDocs) {
                Document doc = indexSearcher.doc(scoreDoc.doc);
                SearchResults searchResults = new SearchResults();


                String fileName = doc.get("fileName");
                indexdefault.add(fileName);/*将生成的姓名逐个加入set中*/

                String content = doc.get("content");
                String wordCount = doc.get("wordCount");

                searchResults.setFileName(fileName);
                searchResults.setContent(content);
                searchResults.setWordCount(Integer.parseInt(wordCount));
                searchResults.setDefaultsearchtime(l2);

                lists.add(searchResults);
            }

            System.out.println("=================");
            for (SearchResults searchResults : lists) {
                System.out.println(searchResults.toString());
            }


            double precision = calculatePrecision(indexdefault, expectedResults);
            double recall = calculateRecall(indexdefault, expectedResults);
            DecimalFormat decimalFormat = new DecimalFormat("#.###");
            String formatted=decimalFormat.format(recall);
            System.out.println("precision是：" + precision + " recall:" + recall);


            for (SearchResults s1 : lists) {
                if (s1.defaultsearchtime != 0) {
                    s1.setPrecisiondefault(precision);
                    s1.setRecalldefault(Double.parseDouble(formatted));
                }
            }


        }

        // 执行BM25
        Set<String> indexdefault1 = new TreeSet<>();

        try (Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
             IndexReader reader = DirectoryReader.open(directory)) {
            IndexSearcher searcher = new IndexSearcher(reader);
            searcher.setSimilarity(new BM25Similarity());

            String[] terms = queryText.trim().split("\\s+");


            for (String termText : terms) {
                Query query = new TermQuery(new Term("content", termText));

                long startTime = System.nanoTime();
                TopDocs topDocsBM25 = searcher.search(query, reader.numDocs());
                long bm25SearchTime = System.nanoTime() - startTime;


                for (ScoreDoc sd : topDocsBM25.scoreDocs) {
                    SearchResults searchResults=new SearchResults();
                    Document doc = searcher.doc(sd.doc);
                    String fileName = doc.get("fileName");
                    String content = doc.get("content");
                    String wordCount = doc.get("wordCount");
                    indexdefault1.add(fileName);

                    searchResults.setFileName(fileName);
                    searchResults.setContent(content);
                    searchResults.setWordCount((int) Long.parseLong(wordCount));
                    DecimalFormat df = new DecimalFormat("#.###");
                    String formattedNumber = df.format(sd.score);
                    searchResults.setBm25(Double.parseDouble(formattedNumber)); // 直接使用BM25得分

                    searchResults.setBm25SearchTime(bm25SearchTime);

                    lists.add(searchResults);
                }
            }
            double precision = calculatePrecision(indexdefault1, expectedResults);
            double recall = calculateRecall(indexdefault1, expectedResults);
            DecimalFormat decimalFormat = new DecimalFormat("#.###");
            String formatted=decimalFormat.format(recall);

            System.out.println("BM25 precision是：" + precision + " recall:" + recall);

            for (SearchResults s1 : lists) {
                if (s1.bm25SearchTime != 0) {
                    s1.setPrecisionbm25(precision);
                    s1.setRecallbm25(Double.parseDouble(formatted));
                }
            }

        }


        // 执行tfidf
        Set<String> indexdefault2 = new TreeSet<>();
       try (Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
         IndexReader reader = DirectoryReader.open(directory)) {
           IndexSearcher searcher = new IndexSearcher(reader);


           searcher.setSimilarity(new ClassicSimilarity());

           String[] terms = queryText.trim().split("\\s+");
           for (String termText : terms) {
               Query query = new TermQuery(new Term("content", termText));
               long startTime = System.nanoTime();
               TopDocs topDocs = searcher.search(query, reader.numDocs());
               long searchTime = System.nanoTime() - startTime;

               for (ScoreDoc sd : topDocs.scoreDocs) {
                   System.out.println("tfidf searchTime是："+searchTime);
                   SearchResults searchResults = new SearchResults();

                   Document doc = searcher.doc(sd.doc);
                   String fileName = doc.get("fileName");
                   String content = doc.get("content");
                   String wordCount = doc.get("wordCount");

                   indexdefault2.add(fileName);

                   float customTFIDFScore = calculateCustomTFIDFScore(reader, termText, sd.doc);
                   searchResults.setFileName(fileName);
                   searchResults.setContent(content);
                   searchResults.setWordCount(Integer.parseInt(wordCount));
                   DecimalFormat df = new DecimalFormat("#.###");
                   String formattedNumber = df.format(customTFIDFScore);

                   searchResults.setTfidf(Double.parseDouble(formattedNumber));
                   searchResults.setTfidfSearchTime(searchTime);


                   lists.add(searchResults);

               }
           }

           double precision = calculatePrecision(indexdefault1, expectedResults);
           double recall = calculateRecall(indexdefault1, expectedResults);
           DecimalFormat decimalFormat = new DecimalFormat("#.###");
           String formatted=decimalFormat.format(recall);

           System.out.println("tfidf precision是：" + precision + " recall:" + recall);



           for (SearchResults s1 : lists) {
               if (s1.tfidfSearchTime != 0) {
                   s1.setPrecisiontfidf(precision);
                   s1.setRecalltfidf(Double.parseDouble(formatted));
               }
           }

       }



        List<IndexService.InputAndVectorDetails> inputVectors = IndexService.trainAndReturnInputVectors(queryText);
        Set<String> indexdefault3 = new TreeSet<>();

    // 步骤 1: 创建查询向量

    IndexService.InputAndVectorDetails inputAndVectorDetails = inputVectors.get(0);


    // 步骤 2: 构建 Lucene 查询
    KnnVectorQuery knnQuery = new KnnVectorQuery("knnvectorField", inputAndVectorDetails.getVector(), 10);

    long startTime = System.nanoTime();

    // 步骤 3: 执行搜索
    TopDocs topDocs = searcher1.search(knnQuery, 5);
    long knnsearchtime = System.nanoTime() - startTime;


    // 获取搜索结果
    List<Document> resultDocs = new ArrayList<>();
    for (ScoreDoc scoreDoc : topDocs.scoreDocs) {
        SearchResults searchResults = new SearchResults();
        Document doc = searcher1.doc(scoreDoc.doc);
        resultDocs.add(doc);
        String fileName = doc.get("fileName");
        indexdefault3.add(fileName);

        String content = doc.get("content");
        String wordCount = doc.get("wordCount");
        indexdefault1.add(fileName);

        searchResults.setWordCount(Integer.parseInt(wordCount));
        searchResults.setFileName(fileName);
        searchResults.setContent(content);
        searchResults.setKnnsearchtime(knnsearchtime);
        lists.add(searchResults);
    }

        double precision = calculatePrecision(indexdefault3, expectedResults);

        double recall = calculateRecall(indexdefault3, expectedResults);
        DecimalFormat decimalFormat = new DecimalFormat("#.###");
        String formatted=decimalFormat.format(recall);


        System.out.println("knn precision是：" + precision + " recall:" + recall);



        for (SearchResults s1 : lists) {
            if (s1.knnsearchtime != 0) {
                s1.setPrecisionknn(precision);
                s1.setRecallknn(Double.parseDouble(formatted));
            }
        }


        return lists;
    }







//            Directory directory1 = FSDirectory.open(Paths.get(INDEX_DIR));
//            IndexReader reader1 = DirectoryReader.open(directory1);
//            IndexSearcher searcher1 = new IndexSearcher(reader1);
//
//
//            List<List<IndexController.IndexDetails>> lists = new ArrayList<>(4);
//            for (int i = 0; i < 4; i++) {
//                lists.add(new ArrayList<>()); // 添加空列表作为占位符
//            }
//            Analyzer analyzer = new EnglishAnalyzer();
//            Analyzer analyzer2 = new EnglishAnalyzer();
//
//            long currentTimeMillis = System.currentTimeMillis();
//
//
//            QueryParser contentQueryParser = new QueryParser("content", analyzer);
//            QueryParser fileNameQueryParser = new QueryParser("fileName", analyzer2);
//
//            List<IndexController.IndexDetails> indexdefault = new ArrayList<>();
//
//            // 根据输入决定查询类型
//            Query textQuery = queryText == null || queryText.trim().isEmpty()
//                    ? new MatchAllDocsQuery()
//                    : contentQueryParser.parse(queryText);
//
//            // 根据输入创建针对fileName的查询
//            Query fileNameQuery = queryText == null || queryText.trim().isEmpty()
//                    ? new MatchAllDocsQuery()
//                    : fileNameQueryParser.parse(queryText);
//
//            BooleanQuery booleanQuery = null;
//
//
//            // 组合查询
//            booleanQuery = new BooleanQuery.Builder()
//                    .add(textQuery, BooleanClause.Occur.SHOULD)
//                    .add(fileNameQuery, BooleanClause.Occur.SHOULD)
//                    .build();
//
//            // 执行查询
//            try (FSDirectory open = FSDirectory.open(Paths.get(INDEX_DIR));
//                 IndexReader reader = DirectoryReader.open(open)) {
//
//                IndexSearcher indexSearcher = new IndexSearcher(reader);
//                long l = System.nanoTime();
//                TopDocs topDocs = indexSearcher.search(booleanQuery, 10);
//                long l1 = System.nanoTime();
//
//                long l2 = l1 - l;
//
//                for (ScoreDoc scoreDoc : topDocs.scoreDocs) {
//                    Document doc = indexSearcher.doc(scoreDoc.doc);
//                    IndexController.IndexDetails details = new IndexController.IndexDetails();
//                    String fileName = doc.get("fileName");
//
//
//                    details.setFileName(fileName);
//                    details.setWordCount(Integer.parseInt(doc.get("wordCount")));
//                    details.setContent(doc.get("content"));
//                    details.setDefaultsearchtime(l2);
//
//                    String baseName = fileName.substring(0, fileName.lastIndexOf('.'));
//
//                    String imagePathsmall = baseName + ".jpg";
//
//                    System.out.println("imagePath是：" + imagePathsmall);
//
//                    details.setImagePath("http://localhost:8088/images/" + imagePathsmall);
//                    indexdefault.add(details);
//                }
//                lists.add(indexdefault);
//                System.out.println("lists的大小是：" + lists.size());
//                for (List<IndexController.IndexDetails> indexDetails : lists) {
//                    System.out.println("0:" + indexDetails.get(0).fileName);
////            System.out.println("1:"+indexDetails.get(1).fileName);
////            System.out.println("2:"+indexDetails.get(2).fileName);
////            System.out.println("3:"+indexDetails.get(3).fileName);
//                }
//
//
////        double pd= calculatePrecision(lists.get(0), expectedResults);
////        System.out.println("传统搜索的结果是：");
////        for (String element : searchResultsdefault) {
////            // 对每个元素执行操作
////            System.out.println(element);
////        }
//
//
//
//                List<IndexController.IndexDetails> indexDetailsListconvert = lists.get(0);
//                System.out.println("indexDetailsListconvert里面有：" + indexDetailsListconvert);
//                for (IndexController.IndexDetails indexDetails : indexDetailsListconvert) {
//                    System.out.println(indexDetails.fileName);
//                }
//
//
//                Set<String> searchResultsFileNames = new HashSet<>();
//                for (IndexController.IndexDetails details : indexDetailsListconvert) {
//                    searchResultsFileNames.add(details.fileName); // 提取文件名并添加到集合中
//                }
//                System.out.println("searchResultsFileNames的大小是：" + searchResultsFileNames.size());
//
//                double precision = calculatePrecision(searchResultsFileNames, expectedResults);
//                double recall = calculateRecall(searchResultsFileNames, expectedResults);
//                System.out.println("Precision is: " + precision + " Recall is: " + recall);
//
//            }
//            return lists;
//        }
//    }




//        System.out.println("pd:"+pd);
////
////            List<IndexController.IndexDetails> defaultResults = lists.get(1); // 获取BM25的搜索结果
////            for (IndexController.IndexDetails detail : defaultResults) {
////                detail.setPrecisiondefault(pd); // 为每个BM25搜索结果设置准确率
////            }
//
//        double rd = calculateRecall(searchResultsdefault,expectedResults);
//        System.out.println("rd:"+rd);
//
//            List<IndexController.IndexDetails> defaultResults1 = indexdefault; // 获取BM25的搜索结果
//            for (IndexController.IndexDetails detail : defaultResults1) {
//                detail.setRecalldefault(rd); // 为每个BM25搜索结果设置准确率
//                detail.setPrecisiondefault(pd);
//            }
//
//    } catch (IOException e) {
//        e.printStackTrace();
//    }
//
//
////    List<IndexController.IndexDetails> indexDetailstfidf = new ArrayList<>();
////
////    try (Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
////         IndexReader reader = DirectoryReader.open(directory)) {
////        IndexSearcher searcher = new IndexSearcher(reader);
////
////        searcher.setSimilarity(new BM25Similarity());
////
////        String[] terms = queryText.trim().split("\\s+");
////        for (String termText : terms) {
////            Query query = new TermQuery(new Term("content", termText));
////            long startTime = System.nanoTime();
////            TopDocs topDocs = searcher.search(query, reader.numDocs());
////            long searchTime = System.nanoTime() - startTime;
////
////            for (ScoreDoc sd : topDocs.scoreDocs) {
////                Document doc = searcher.doc(sd.doc);
////                String fileName = doc.get("fileName");
////                searchResultsbm25.add(fileName);
////                IndexController.IndexDetails indexDetail = createIndexDetail(doc, searcher, termText);
////                indexDetail.setBm25(sd.score);
////                indexDetail.setBm25SearchTime(searchTime);
////                indexDetailstfidf.add(indexDetail);
////            }
////            lists.add(indexDetailstfidf);
////            System.out.println("searchResultsbm25大小是：" + searchResultsbm25);
////            double pbm = calculatePrecision(searchResultsbm25, expectedResults);
////            System.out.println("pbm"+pbm);
////
////
////            List<IndexController.IndexDetails> bm25Results = lists.get(2); // 获取BM25的搜索结果
////            for (IndexController.IndexDetails detail : bm25Results) {
////                detail.setPrecisionbm25(pbm); // 为每个BM25搜索结果设置准确率
////            }
////
////            double rbm = calculateRecall(searchResultsbm25, expectedResults);
////            System.out.println("rbm"+rbm);
////            indexDetailstfidf.get(2).setRecalldefault(rbm);
////
////                List<IndexController.IndexDetails> bm25Results1 = indexDetailstfidf; // 获取BM25的搜索结果
////                for (IndexController.IndexDetails detail : bm25Results1) {
////                    detail.setRecallbm25(rbm); // 为每个BM25搜索结果设置准确率
////                    detail.setPrecisionbm25(pbm);
////                }
////
////        }
////    } catch (IOException e) {
////        e.printStackTrace();
////    }
//
//
//
//
//    List<IndexController.IndexDetails> indexDetailstfidf = new ArrayList<>();
//
//    try (Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
//         IndexReader reader = DirectoryReader.open(directory)) {
//        IndexSearcher searcher = new IndexSearcher(reader);
//
//        searcher.setSimilarity(new ClassicSimilarity());
//
//        String[] terms = queryText.trim().split("\\s+");
//        for (String termText : terms) {
//            Query query = new TermQuery(new Term("content", termText));
//            long startTime = System.nanoTime();
//            TopDocs topDocs = searcher.search(query, reader.numDocs());
//            long searchTime = System.nanoTime() - startTime;
//
//
//
//
//
//            for (ScoreDoc sd : topDocs.scoreDocs) {
//                Document doc = searcher.doc(sd.doc);
//                String fileName = doc.get("fileName");
//                searchResultstfidf.add(fileName);
//                IndexController.IndexDetails indexDetail = createIndexDetail(doc, searcher, termText);
//                indexDetail.setBm25(sd.score);
//                indexDetail.setBm25SearchTime(searchTime);
//                indexDetailstfidf.add(indexDetail);
//                System.out.println("===================");
//                System.out.println("BM25 searchtime是:"+searchTime);
//                System.out.println("===================");
//            }
//            lists.add(indexDetailstfidf);
//            System.out.println("searchResultsbm25大小是：" + searchResultstfidf);
//            double pbm = calculatePrecision(searchResultstfidf, expectedResults);
//            System.out.println("pbm"+pbm);
//
//
//            List<IndexController.IndexDetails> tfidf = lists.get(0); // 获取BM25的搜索结果
//            for (IndexController.IndexDetails detail : tfidf) {
//                detail.setPrecisiontfidf(pbm); // 为每个BM25搜索结果设置准确率
//            }
//
//            double rbm = calculateRecall(searchResultsbm25, expectedResults);
//            System.out.println("rbm"+rbm);
////            indexDetailstfidf.get(2).setRecalldefault(rbm);
//
//            List<IndexController.IndexDetails> bm25Results1 = indexDetailstfidf; // 获取BM25的搜索结果
//            for (IndexController.IndexDetails detail : bm25Results1) {
//                detail.setPrecisiontfidf(pbm); // 为每个BM25搜索结果设置准确率
//                detail.setRecalltfidf(rbm);
//            }
//
//        }
//    } catch (IOException e) {
//        e.printStackTrace();
//    }
//
//
//
//
//
//    List<IndexController.IndexDetails> indexDetailsListtfidf = new ArrayList<>();
//    try (Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
//         IndexReader reader = DirectoryReader.open(directory)) {
//        IndexSearcher searcher = new IndexSearcher(reader);
//
//
//        // 假设切换到使用TF-IDF的相似度模型
//        searcher.setSimilarity(new ClassicSimilarity());
//
//        String[] terms = queryText.trim().split("\\s+");
//        for (String termText : terms) {
//            Query query = new TermQuery(new Term("content", termText));
//            long startTime = System.nanoTime();
//            TopDocs topDocs = searcher.search(query, reader.numDocs());
//            long searchTime = System.nanoTime() - startTime;
//
//            for (ScoreDoc sd : topDocs.scoreDocs) {
//                Document doc = searcher.doc(sd.doc);
//                String fileName = doc.get("fileName");
//                float customTFIDFScore = calculateCustomTFIDFScore(reader, termText, sd.doc);
//                searchResultstfidf.add(fileName);
//                System.out.println("!!!filename是："+fileName+" customTFIDFScore:"+customTFIDFScore);
//                double ptf=  calculatePrecision(searchResultstfidf, expectedResults);


    // 方法：计算准确率
    public static double calculatePrecision(Set<String> searchResults, Set<String> expectedResults) {
        Set<String> relevantDocumentsFound = new HashSet<>(searchResults);
        relevantDocumentsFound.retainAll(expectedResults); // 计算交集
        System.out.println("searchResults是: " + searchResults);
        System.out.println("expectedResults是: " + expectedResults);
        System.out.println("最后的精准度是："+ (double) relevantDocumentsFound.size() / searchResults.size());

        System.out.println("relevantDocumentsFound是: " + relevantDocumentsFound);
        System.out.println("relevantDocumentsFound的大小是: " + relevantDocumentsFound.size());
        System.out.println("searchResults的大小是: " + searchResults.size());

        System.out.println("=====================================");
        return relevantDocumentsFound.isEmpty() ? 0 : (double) relevantDocumentsFound.size() / searchResults.size();
    }

    // 方法：计算召回率
    public static double calculateRecall(Set<String> searchResults, Set<String> expectedResults) {
        Set<String> relevantDocumentsFound = new HashSet<>(searchResults);
        relevantDocumentsFound.retainAll(expectedResults); // 计算交集
        System.out.println("最后的召回率是："+(double) relevantDocumentsFound.size() / expectedResults.size());
        return expectedResults.isEmpty() ? 0 : (double) relevantDocumentsFound.size() / expectedResults.size();
    }



    /*按照字数和内容进行组合查询！！！1
*
*
* */
public List<IndexController.IndexDetails> searchDocuments(String queryText, int low, int high, long startTime,long endTime,int flag) throws Exception {
    Analyzer analyzer = new EnglishAnalyzer();
    Analyzer analyzer2 = new EnglishAnalyzer();

    long currentTimeMillis = System.currentTimeMillis();


    QueryParser contentQueryParser  = new QueryParser("content", analyzer);
    QueryParser fileNameQueryParser = new QueryParser("fileName", analyzer2);
    // 新增时间范围查询
    Query timeRangeQuery = LongPoint.newRangeQuery("last_modified", startTime,endTime);

    List<IndexController.IndexDetails> indexDetailsList = new ArrayList<>();

    // 根据输入决定查询类型
    Query textQuery = queryText == null || queryText.trim().isEmpty()
            ? new MatchAllDocsQuery()
            : contentQueryParser.parse(queryText);

    // 根据输入创建针对fileName的查询
    Query fileNameQuery = queryText == null || queryText.trim().isEmpty()
            ? new MatchAllDocsQuery()
            : fileNameQueryParser.parse(queryText);

    // 字数范围查询
    Query wordCountQuery = IntPoint.newRangeQuery("wordCount", low, high);
    BooleanQuery booleanQuery = null;

    if (flag==0){
        // 组合查询
        booleanQuery = new BooleanQuery.Builder()
                .add(textQuery, BooleanClause.Occur.SHOULD)
                .add(fileNameQuery, BooleanClause.Occur.SHOULD)
                .add(wordCountQuery, BooleanClause.Occur.MUST)
                .build();

    }
    else
        if (flag==1){
            // 组合查询
             booleanQuery = new BooleanQuery.Builder()
                    .add(textQuery, BooleanClause.Occur.SHOULD)
                    .add(fileNameQuery, BooleanClause.Occur.SHOULD)
                    .add(wordCountQuery, BooleanClause.Occur.MUST)
                    .add(timeRangeQuery, BooleanClause.Occur.MUST) // 添加到组合查询中
                    .build();


        }




    // 执行查询
    try (FSDirectory open = FSDirectory.open(Paths.get(INDEX_DIR));
         IndexReader reader = DirectoryReader.open(open)) {

        IndexSearcher indexSearcher = new IndexSearcher(reader);
        long l = System.nanoTime();
        TopDocs topDocs = indexSearcher.search(booleanQuery, 10);
        long l1 = System.nanoTime();

        long l2 = l1 - l;

        for (ScoreDoc scoreDoc : topDocs.scoreDocs) {
            Document doc = indexSearcher.doc(scoreDoc.doc);
            IndexController.IndexDetails details = new IndexController.IndexDetails();
            String fileName = doc.get("fileName");


            details.setFileName(fileName);
            details.setWordCount(Integer.parseInt(doc.get("wordCount")));
            details.setContent(doc.get("content"));
            details.setDefaultsearchtime(l2);
            details.setFileSize(Long.parseLong(doc.get("fileSize")));

            String baseName = fileName.substring(0, fileName.lastIndexOf('.'));

            String imagePathsmall = baseName + ".jpg";

            System.out.println("imagePath是："+imagePathsmall);

            details.setImagePath("http://localhost:8088/images/"+imagePathsmall);
            indexDetailsList.add(details);
        }
    } catch (Exception e) {
        e.printStackTrace();

        throw e;
    }

    return indexDetailsList;
}


    public List<IndexController.IndexDetails> searchDocumentswithTime(
            String queryText,
            int low,
            int high,
            long timeLowerBound,
            long timeUpperBound) throws Exception {
        Analyzer analyzer = new EnglishAnalyzer();
        QueryParser queryParser = new QueryParser("content", analyzer);
        List<IndexController.IndexDetails> indexDetailsList = new ArrayList<>();

        // 根据输入决定查询类型
        Query textQuery = queryText == null || queryText.trim().isEmpty()
                ? new MatchAllDocsQuery()
                : queryParser.parse(queryText);

        // 字数范围查询
        Query wordCountQuery = IntPoint.newRangeQuery("wordCount", low, high);

        // 时间范围查询
        Query timeQuery = LongPoint.newRangeQuery("lastModified", timeLowerBound, timeUpperBound);

        // 组合查询
        BooleanQuery booleanQuery = new BooleanQuery.Builder()
                .add(textQuery, BooleanClause.Occur.MUST)
                .add(wordCountQuery, BooleanClause.Occur.MUST)
                .add(timeQuery, BooleanClause.Occur.MUST) // 添加时间查询
                .build();

        // 执行查询
        try (FSDirectory open = FSDirectory.open(Paths.get(INDEX_DIR));
             IndexReader reader = DirectoryReader.open(open)) {

            IndexSearcher indexSearcher = new IndexSearcher(reader);
            TopDocs topDocs = indexSearcher.search(booleanQuery, 10);

            for (ScoreDoc scoreDoc : topDocs.scoreDocs) {
                Document doc = indexSearcher.doc(scoreDoc.doc);
                IndexController.IndexDetails details = new IndexController.IndexDetails();

                details.setFileName(doc.get("fileName"));
                details.setWordCount(Integer.parseInt(doc.get("wordCount")));
                details.setContent(doc.get("content"));
                details.setLast_modified(Long.parseLong(doc.get("lastModified")));

                indexDetailsList.add(details);
            }
        } catch (Exception e) {
            e.printStackTrace();
            // 根据需要处理异常
            throw e;
        }

        return indexDetailsList;
    }












    private String formatFileSize(long fileSize) {
        String[] units = {"B", "KB", "MB", "GB", "TB"};
        int index = 0;
        double size = fileSize;
        while (size >= 1024 && index < units.length - 1) {
            size /= 1024;
            index++;
        }
        return String.format("%.2f %s", size, units[index]);
    }







    /*接受在某个field中搜索特定语句
    *
    * */
    public void updateDocumentContentByFileName(String fileName, String newContent) {
        try {
            Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
            IndexWriterConfig config = new IndexWriterConfig(new EnglishAnalyzer());
            try (IndexWriter writer = new IndexWriter(directory, config)) {
                writer.deleteDocuments(new Term("fileName", fileName));



                // 创建新的Document
                Document doc = new Document();
//                doc.add(new StringField("fileName", fileName, Field.Store.YES)); // 假设fileName是唯一标识符
                FieldType contentType = new FieldType();
                contentType.setStored(true);
                contentType.setIndexOptions(IndexOptions.DOCS_AND_FREQS_AND_POSITIONS);
                contentType.setStoreTermVectors(true); // 设置为与原索引中相同的storeTermVectors值

                // 使用上面的FieldType创建新的Field
                Field contentField = new Field("content", newContent, contentType);
//                doc.add(new StringField("fileName", fileName, Field.Store.YES)); // 假设fileName是文档的唯一标识符
                doc.add(contentField); // 添加更新后的内容字段


                // 使用Term来定位需要更新的Document
                writer.updateDocument(new Term("fileName", fileName), doc);
                writer.updateDocument(new Term("content", newContent), doc);

                System.out.println("Document with fileName \"" + fileName +" updated.");
            }
        } catch (IOException e) {
            e.printStackTrace();
            System.err.println("Failed to update document due to an error: " + e.getMessage());
        }
    }



    public void printAllindexes() {
        try {
            Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
            IndexReader reader = DirectoryReader.open(directory);

            System.out.println("Index contains " + reader.numDocs() + " documents.");

            for (int i = 0; i < reader.maxDoc(); i++) {
//                if (!reader.hasDeletions()) {
                    Document doc = reader.document(i);
                    System.out.println("Document " + i + ": ");
                    System.out.println("\tfileName: " + doc.get("fileName"));
                    System.out.println("\tcontent: " + doc.get("content"));
                    System.out.println("\tfileSize: " + doc.get("fileSize"));
                    System.out.println("\tlastModified: " + doc.get("lastModified"));
                    System.out.println("\tformattedFileSize: " + doc.get("formattedFileSize"));
//                    System.out.println("\tknnvectorField: " + doc.get("knnvectorField"));
                    System.out.println("\tvectorAsString: " + doc.get("vectorAsString"));

                    // 打印更多字段根据需要
//                }
            }

            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
            System.err.println("Failed to print documents from index due to an error: " + e.getMessage());
        }
    }



}

