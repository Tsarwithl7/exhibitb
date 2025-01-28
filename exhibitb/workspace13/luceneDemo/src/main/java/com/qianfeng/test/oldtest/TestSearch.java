package com.qianfeng.test.oldtest;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.*;
import org.apache.lucene.index.*;
import org.apache.lucene.queryparser.classic.MultiFieldQueryParser;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.*;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;

import org.testng.annotations.Test;
import org.wltea.analyzer.lucene.IKAnalyzer;


import java.io.IOException;
import java.nio.file.Paths;
import java.util.HashMap;

/**
 * @Author: Zhidong Huang
 * @Date: 2024/1/26 17:13
 */
public class TestSearch {

//    测试简单搜索

    @Test
    public void testIndexSearch() throws ParseException, IOException {

        //1、创建分词器
        Analyzer analyzer=new StandardAnalyzer();

        //2、创建查询对象
        QueryParser queryParser=new QueryParser("content",analyzer);

        //3、设置搜索关键词
        Query query=queryParser.parse("camping trip");

        //4、创建DIrectory目录对象，指定索引库的位置
        FSDirectory dir=FSDirectory.open(Paths.get("S:\\luceneDir2"));

        //5、创建输入流
        DirectoryReader openreader=DirectoryReader.open(dir);

        //6、创建搜索对象
        IndexSearcher indexSearcher=new IndexSearcher(openreader);

        //7、搜索并返回结果(搜索所得到 的消息，显示前10条)
        TopDocs topDocs=indexSearcher.search(query,10);

        //8、获取结果集
        System.out.println("总记录数："+topDocs.totalHits.value);

        //9、遍历结果
        ScoreDoc[] scoreDocs=topDocs.scoreDocs;
        if (scoreDocs!=null){
            for (ScoreDoc scoreDoc:scoreDocs) {
                int docId= scoreDoc.doc;
                Document doc = indexSearcher.doc(docId);
                System.out.println("====filename======"+doc.get("filename"));
                System.out.println("====content===="+doc.get("content"));
                System.out.println("====vector===="+doc.get("vector"));
                System.out.println("====knnvector===="+doc.get("knnvector"));
                System.out.println("====filesize===="+doc.get("filesize")+"KB");
                if (doc.get("filemodified") != null) {
                    System.out.println("====filemodified==="+doc.get("filemodified"));
                } else {
                    System.out.println("filemodified字段为空");
                }


                System.out.println("==================================================================");
            }

        }


        //10、关闭资源
         openreader.close();

      }



      /*
      * 控制多关键词查询
      *
      * */
      @Test
      public void testMultiWordsSearch() throws ParseException, IOException {

          //1、创建分词器
          Analyzer analyzer=new IKAnalyzer();

          //2、创建查询对象
          QueryParser queryParser=new QueryParser("name",analyzer);

          //3、设置搜索关键词
          Query query=queryParser.parse("name:华为 AND 手机");
          Query query1=queryParser.parse("name:华为 And 手机");

          //4、创建DIrectory目录对象，指定索引库的位置
          FSDirectory dir=FSDirectory.open(Paths.get("S:\\luceneDir"));

          //5、创建输入流
          DirectoryReader openreader=DirectoryReader.open(dir);

          //6、创建搜索对象
          IndexSearcher indexSearcher=new IndexSearcher(openreader);

          //7、搜索并返回结果(搜索所得到 的消息，显示前10条)
          TopDocs topDocs=indexSearcher.search(query,10);
          TopDocs topDocs1 = indexSearcher.search(query1, 10);

          //8、获取结果集
          System.out.println("总记录数："+topDocs.totalHits.value);
          System.out.println("另一种形式的总记录数："+topDocs1.totalHits.value);

          //9、遍历结果
          ScoreDoc[] scoreDocs=topDocs.scoreDocs;
          if (scoreDocs!=null){
              for (ScoreDoc scoreDoc:scoreDocs) {
                  int docId= scoreDoc.doc;
                  Document doc = indexSearcher.doc(docId);
                  System.out.println("====id======"+doc.get("id"));
                  System.out.println("====name===="+doc.get("name"));
                  System.out.println("====price===="+doc.get("price"));
                  System.out.println("====image===="+doc.get("image"));
                  System.out.println("====categoryName==="+doc.get("categoryName"));
                  System.out.println("====brandname======"+doc.get("brandname"));
                  System.out.println("==================================================================");
              }


          }

          //10、关闭资源
          openreader.close();

      }


      /*
      * 根据数值范围查询
      * e.g:根据价格查询100-1000元之间的商品
      * 未创建分词器，不需要
      * */

    @Test
    public void testRangeQuery() throws Exception {
        //1、创建查询对象
        Query query=IntPoint.newRangeQuery("price",100,1000);

        //2、创建Directory目录对象，指定索引库的位置
        FSDirectory open = FSDirectory.open(Paths.get("S:\\luceneDir"));

        //3、创建输入流对象
        IndexReader reader = DirectoryReader.open(open);

        //4、创建搜索对象
        IndexSearcher indexSearcher = new IndexSearcher(reader);

        //5、搜索并返回结果
        TopDocs topdocs = indexSearcher.search(query, 10);
        System.out.println("==========count======"+topdocs.totalHits.value);

        //6、获取结果
         ScoreDoc[] scoreDocs=topdocs.scoreDocs;
        //7、遍历结果集
        if (scoreDocs!=null){
            for (ScoreDoc scoreDoc:scoreDocs) {
                int docId= scoreDoc.doc;
                Document doc = indexSearcher.doc(docId);
                System.out.println("====id======"+doc.get("id"));
                System.out.println("====name===="+doc.get("name"));
                System.out.println("====price===="+doc.get("price"));
                System.out.println("====image===="+doc.get("image"));
                System.out.println("====categoryName==="+doc.get("categoryName"));
                System.out.println("====brandname======"+doc.get("brandname"));
                System.out.println("==================================================================");
            }


        }

    }

    /*
    * 组合查询：
    * 需求：
    * e.g 根据商品名字查询，查询华为手机关键字
    * 同时价格还在100-1000之间
    * */

    @Test
    public void testBoolQuery() throws Exception   {

        //1、创建分词器
        Analyzer analyzer=new IKAnalyzer();

        //2、根据商品名称进行查询对象
        QueryParser queryParser=new QueryParser("name",analyzer);

        //3、根据关键字进行查询
        Query query1=queryParser.parse("name:华为 AND 手机");
        Query query2=IntPoint.newRangeQuery("price",100,1000);

        //创建组合查询对象
        BooleanQuery.Builder booleanQuery=new BooleanQuery.Builder();

//       should等于or must等于and must_not等于not
        booleanQuery.add(query1,BooleanClause.Occur.MUST);
        booleanQuery.add(query2, BooleanClause.Occur.MUST);


        //4、创建DIrectory目录对象，指定索引库的位置
        FSDirectory dir=FSDirectory.open(Paths.get("S:\\luceneDir"));

        //5、创建输入流
        DirectoryReader openreader=DirectoryReader.open(dir);

        //6、创建搜索对象
        IndexSearcher indexSearcher=new IndexSearcher(openreader);

        //7、搜索并返回结果(搜索所得到 的消息，显示前10条)
        TopDocs topDocs=indexSearcher.search(booleanQuery.build(),10);
//        TopDocs topDocs1 = indexSearcher.search(query1, 10);

        //8、获取结果集
        System.out.println("结果记录数："+topDocs.totalHits.value);
//        System.out.println("另一种形式的总记录数："+topDocs1.totalHits.value);

        //9、遍历结果
        ScoreDoc[] scoreDocs=topDocs.scoreDocs;
        if (scoreDocs!=null){
            for (ScoreDoc scoreDoc:scoreDocs) {
                int docId= scoreDoc.doc;
                Document doc = indexSearcher.doc(docId);
                System.out.println("====id======"+doc.get("id"));
                System.out.println("====name===="+doc.get("name"));
                System.out.println("====price===="+doc.get("price"));
                System.out.println("====image===="+doc.get("image"));
                System.out.println("====categoryName==="+doc.get("categoryName"));
                System.out.println("====brandname======"+doc.get("brandname"));
                System.out.println("==================================================================");
            }


        }

        //10、关闭资源
        openreader.close();

    }

    @Test
    public void testSearchBoots() throws Exception {

        //1、创建分词器
        Analyzer analyzer=new IKAnalyzer();

        //设置从不同域内进行查询
        String[] fields={"name","categoryName","brandname"};

        //设置影响相关度排序的参数
        HashMap<String, Float> boots = new HashMap<>();
//        boots.put("categoryName",100000000f);
        boots.put("categoryName",1.0f);

        //创建多个域查询对象
        MultiFieldQueryParser multiFieldQueryParser = new MultiFieldQueryParser(fields, analyzer, boots);
        //设置查询的关键词
        Query query = multiFieldQueryParser.parse("手机");

        //4、创建DIrectory目录对象，指定索引库的位置
        FSDirectory dir=FSDirectory.open(Paths.get("S:\\luceneDir"));

        //5、创建输入流
        DirectoryReader openreader=DirectoryReader.open(dir);

        //6、创建搜索对象
        IndexSearcher indexSearcher=new IndexSearcher(openreader);

        //7、搜索并返回结果(搜索所得到 的消息，显示前10条)
        TopDocs topDocs=indexSearcher.search(query,10);
//        TopDocs topDocs1 = indexSearcher.search(query1, 10);

        System.out.println("yes");
        //8、获取结果集
        System.out.println("结果记录数："+topDocs.totalHits.value);
//        System.out.println("另一种形式的总记录数："+topDocs1.totalHits.value);

        //9、遍历结果
        ScoreDoc[] scoreDocs=topDocs.scoreDocs;
        if (scoreDocs!=null){
            for (ScoreDoc scoreDoc:scoreDocs) {
                int docId= scoreDoc.doc;
                Document doc = indexSearcher.doc(docId);
                System.out.println("====id======"+doc.get("id"));
                System.out.println("====name===="+doc.get("name"));
                System.out.println("====price===="+doc.get("price"));
                System.out.println("====image===="+doc.get("image"));
                System.out.println("====categoryName==="+doc.get("categoryName"));
                System.out.println("====brandname======"+doc.get("brandname"));
                System.out.println("==================================================================");
            }


        }

        //10、关闭资源
        openreader.close();

    }
}
