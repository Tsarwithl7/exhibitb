package com.qianfeng.test;

import com.qianfeng.controller.IndexController;
import com.qianfeng.service.IndexService;
import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.TokenStream;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.analysis.tokenattributes.CharTermAttribute;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.IntPoint;
import org.apache.lucene.document.LongPoint;
import org.apache.lucene.index.*;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.*;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.deeplearning4j.models.paragraphvectors.ParagraphVectors;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.StringReader;
import java.nio.file.Paths;
import java.text.ParseException;
import java.util.HashSet;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class IndexServiceTest {
    public ParagraphVectors vectors;
    private IndexService indexService;
    private static final String INDEX_DIR = "S:\\project\\indexdir";

    @BeforeEach
    void setUp() {
        indexService = new IndexService();
    }

    @AfterEach
    void tearDown() throws IOException {
        // 清理测试产生的数据，这里可以根据需要添加删除索引目录的逻辑
    }

    @Test
    public void testCalculateTFIDF() throws IOException {
        String input = "shop"; // 用你想测试的查询替换这里的字符串
        indexService.calculate_tfidf(input);
    }




    @Test
    void compareScoringMethods() throws IOException {
        String input = "shop";
        indexService.compareScoringMethods(input);
        indexService.compareScoringMethodsolderversion(input);


            // 测量compareScoringMethodsolderversion的执行时间
            long startTimeOldVersion = System.nanoTime();
            List<IndexController.IndexDetails> resultsOldVersion = indexService.compareScoringMethodsolderversion(input);
            long endTimeOldVersion = System.nanoTime();
            long durationOldVersion = endTimeOldVersion - startTimeOldVersion;

            // 测量compareScoringMethods的执行时间
            long startTimeNewVersion = System.nanoTime();
            List<IndexController.IndexDetails> resultsNewVersion = indexService.compareScoringMethods(input);
            long endTimeNewVersion = System.nanoTime();
            long durationNewVersion = endTimeNewVersion - startTimeNewVersion;

            // 打印两个方法的执行时间
            System.out.println("compareScoringMethodsolderversion time (ns):" + durationOldVersion);
            System.out.println("compareScoringMethods  time (ns):" + durationNewVersion);

            // 进一步比较两个方法的时间差异
            long timeDifference = durationOldVersion - durationNewVersion;
            System.out.println("time difference (ns):" + timeDifference);
    }




    @Test
    public void testTrainAndReturnInputVectors() throws IOException {
        String input = "This is a test sentence. Another test sentence here.";
        List<IndexService.InputAndVectorDetails> inputAndVectorDetails = indexService.trainAndReturnInputVectors(input);
        System.out.println(inputAndVectorDetails);
    }


    @Test
    void createIndexTest() {

        try {
            indexService.createIndex();
            Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
            IndexReader reader = DirectoryReader.open(directory);

            // 检查是否至少有一个文档被索引
            assertTrue(reader.numDocs() > 0, "应该至少有一个文档被索引");

            // 可以添加更多的断言来检查索引的文档是否包含特定的字段
            Document firstDoc = reader.document(0);
            assertNotNull(firstDoc.get("fileName"), "索引的文档应该包含fileName字段");
            assertNotNull(firstDoc.get("content"), "索引的文档应该包含content字段");
            // 更多的断言可以根据需要添加

            reader.close();
        } catch (IOException e) {
            fail("测试失败，原因：" + e.getMessage());
        }
    }

    @Test
    void deleteIndexTest(){
        try {
            indexService.deleteIndexTest();
        }
        catch (Exception e){
            e.printStackTrace();
        }
    }

    @Test
    void testAnalyzers(){
        Analyzer analyzer = new StandardAnalyzer();
// 尝试使用不同的Analyzer，例如EnglishAnalyzer
// Analyzer analyzer = new EnglishAnalyzer();
        try {
            TokenStream tokenStream = analyzer.tokenStream(null, new StringReader("Flying cats"));
            CharTermAttribute charTermAttr = tokenStream.addAttribute(CharTermAttribute.class);
            tokenStream.reset();
            while (tokenStream.incrementToken()) {
                String term = charTermAttr.toString();
                System.out.println(term);
            }
            tokenStream.end();
            tokenStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }




    @Test
    void printAllindexes() {
        try {
             indexService.printAllindexes();


        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Test
    void updateDocumentContent(){
        try {
            indexService.updateDocumentContent("doc1c1db4b6-b7c5-4836-979c-09da61971ad4.txt","Postcards always spoil my holidays. Last summer, I went to Italy. I visited museums and sat in public gardens. A friendly waiter taught me a few words of Italian. 'Then he lent me a book. I read a few lines, but I did not understand a word. Every day I thought about postcards. My holidays passed quickly, but I did not send any cards to my friends. On the last day I made a big decision. I got up early and bought thirty-seven cards. I spent the whole day in my room, but I did not write a single card!"
                   );
        }
        catch (Exception e){
            e.printStackTrace();
        }

    }
    @Test
    void Searchall(){
        try {
            indexService.searchAll();
        }
        catch (Exception e){
            e.printStackTrace();
        }

    }


    @Test
    public void updateDocumentContentByFileName(){
        try {
            indexService.updateDocumentContentByFileName("doc1c1db4b6-b7c5-4836-979c-09da61971ad4.txt","Last week I went to the theatre. I had a very good seat. The play was very interesting. I did not enjoy it. A young man and a young woman were sitting behind me. They were talking loudly. I got very angry. I could not hear the actors. I turned round. I looked at the man and the woman angrily. They did not pay any attention. In the end, I could not bear it. I turned round again. 'I can't hear a word!' I said angrily. ‘It’s none of your business,' the young man said rudely. 'This is a private conversation!'"
                    );
            } catch (Exception e) {
                e.printStackTrace();
            }
    }



    /*测试组合查询条件，多关键词
    *
    * */
          @Test
        public void testMultiWordsSearch() throws ParseException, IOException, org.apache.lucene.queryparser.classic.ParseException {
        //1、创建分词器，这里使用StandardAnalyzer标准分词器
        Analyzer analyzer = new StandardAnalyzer();

        //2、创建查询对象，假设我们在处理英文文档时，有一个"content"字段
        QueryParser queryParser = new QueryParser("content", analyzer);

        //3、设置搜索关键词，这里的搜索条件改为英文
        Query query = queryParser.parse("content:piano AND visitor");
//        Query query1 = queryParser.parse("name:tony AND lawyer");

        //4、创建Directory目录对象，指定索引库的位置
        FSDirectory dir = FSDirectory.open(Paths.get(INDEX_DIR));

        //5、创建输入流
        DirectoryReader openreader = DirectoryReader.open(dir);

        //6、创建搜索对象
        IndexSearcher indexSearcher = new IndexSearcher(openreader);

        //7、搜索并返回结果(搜索所得到的消息，显示前10条)
        TopDocs topDocs = indexSearcher.search(query, 10);
//        TopDocs topDocs1 = indexSearcher.search(query1, 10);

        //8、获取结果集
        System.out.println("Total Hits: " + topDocs.totalHits.value);
//        System.out.println("Total Hits for another query: " + topDocs1.totalHits.value);

        //9、遍历结果
        ScoreDoc[] scoreDocs = topDocs.scoreDocs;
        if (scoreDocs != null) {
            for (ScoreDoc scoreDoc : scoreDocs) {
                int docId = scoreDoc.doc;
                Document doc = indexSearcher.doc(docId);

                System.out.println("====fileName====" + doc.get("fileName"));
                System.out.println("====content====" + doc.get("content"));
                System.out.println("====fileSize====+"+doc.get("fileSize"));
                System.out.println("==================================================================");
            }
        }

        //10、关闭资源
        openreader.close();
    }

    @Test
    public void testwordsfilter() throws Exception {

        // 创建文本查询：比如搜索文档内容中包含特定文本
        Analyzer analyzer = new StandardAnalyzer();
        QueryParser queryParser = new QueryParser("content", analyzer);
        Query textQuery1 = new MatchAllDocsQuery();
//        Query textQuery2 = queryParser.parse(""); // 替换"特定文本"为搜索词
        // 字数范围查询：500到1000字之间
        Query wordCountQuery = IntPoint.newRangeQuery("wordCount", 50, 300);

        // 组合查询
        BooleanQuery.Builder booleanQuery = new BooleanQuery.Builder();

        booleanQuery.add(textQuery1, BooleanClause.Occur.MUST); // 必须满足文本条件
        booleanQuery.add(wordCountQuery, BooleanClause.Occur.MUST); // 必须满足字数条件


        // 执行查询
        FSDirectory open = FSDirectory.open(Paths.get(INDEX_DIR));
        IndexReader reader = DirectoryReader.open(open);
        IndexSearcher indexSearcher = new IndexSearcher(reader);
        TopDocs topDocs = indexSearcher.search(booleanQuery.build(), 10);

        System.out.println("Total Hits: " + topDocs.totalHits.value);
        for (ScoreDoc scoreDoc : topDocs.scoreDocs) {
            Document doc = indexSearcher.doc(scoreDoc.doc);
            System.out.println("====id======" + doc.get("id"));
            System.out.println("====fileName====" + doc.get("fileName"));
            System.out.println("====wordCount====" + doc.get("wordCount"));
            System.out.println("====content====" + doc.get("content"));
            // 打印其他你感兴趣的字段
        }
        reader.close();
    }




    @Test
    public void testtimeandwords() throws Exception {
        long now = System.currentTimeMillis();
        long twentyFourHoursAgo = now - (24 * 60 * 60 * 1000);
        long threeHoursAgo = now - (3 * 60 * 60 * 1000);

        // 创建时间范围查询：3到24小时之间最后被修改的文档
        Query timeQuery = LongPoint.newRangeQuery("last_modified", twentyFourHoursAgo, threeHoursAgo);

        // 创建文本查询：比如搜索文档内容中包含特定文本
        Analyzer analyzer = new StandardAnalyzer();
        QueryParser queryParser = new QueryParser("content", analyzer);
        Query textQuery = queryParser.parse("lucky"); // 替换"特定文本"为搜索词
        // 字数范围查询：500到1000字之间
        Query wordCountQuery = IntPoint.newRangeQuery("wordCount", 50, 300);

        // 组合查询
        BooleanQuery.Builder booleanQuery = new BooleanQuery.Builder();
        booleanQuery.add(timeQuery, BooleanClause.Occur.MUST); // 必须满足时间条件
        booleanQuery.add(textQuery, BooleanClause.Occur.MUST); // 必须满足文本条件
        booleanQuery.add(wordCountQuery, BooleanClause.Occur.MUST); // 必须满足字数条件


        // 执行查询
        FSDirectory open = FSDirectory.open(Paths.get(INDEX_DIR));
        IndexReader reader = DirectoryReader.open(open);
        IndexSearcher indexSearcher = new IndexSearcher(reader);
        TopDocs topDocs = indexSearcher.search(booleanQuery.build(), 10);

        System.out.println("Total Hits: " + topDocs.totalHits.value);
        for (ScoreDoc scoreDoc : topDocs.scoreDocs) {
            Document doc = indexSearcher.doc(scoreDoc.doc);
            System.out.println("====id======" + doc.get("id"));
            System.out.println("====fileName====" + doc.get("fileName"));
            System.out.println("====wordCount====" + doc.get("wordCount"));
            System.out.println("====content====" + doc.get("content"));
            // 打印其他你感兴趣的字段
        }
        reader.close();
    }

    @Test
    public void testCompareall() throws org.apache.lucene.queryparser.classic.ParseException, IOException {
        // 假设
        String queryText = "example query"; // 测试用的查询文本
        HashSet<String> expectedResults = new HashSet<>(); // 期望的结果集
        // 填充一些期望的结果，这应当与你的测试索引内容相匹配
        expectedResults.add("Smash-and-grab.txt");
        expectedResults.add("A noble gangster.txt");
        expectedResults.add("Fifty pence worth of trouble.txt");
        expectedResults.add("Flying cats.txt");
        indexService.compareall("shop",expectedResults);
    }
}
