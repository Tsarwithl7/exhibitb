package com.qianfeng.test.lucenetest;

import org.apache.lucene.document.Document;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.search.KnnVectorQuery;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.nio.file.Paths;

/*在Lucene 9.x版本中，使用HNSW算法进行最近邻搜索确实涉及到KnnVectorField，这是Lucene用于存储和检索高维向量的数据结构。KnnVectorField与HNSW算法的整合使得在Lucene中实现高效的向量搜索成为可能。

当您使用KnnVectorField添加向量到索引中，并通过KnnVectorQuery进行查询时，您实际上是在使用HNSW算法。HNSW算法的实现细节被封装在Lucene内部，对用户来说是透明的。这意味着，通过正确使用这些API，您实际上已经在利用HNSW算法的优势了。

使用HNSW接口的关键点
索引向量时使用KnnVectorField：在索引文档时，通过为每个文档添加KnnVectorField，您实际上是在准备数据以便使用HNSW算法进行搜索。KnnVectorField接受一个浮点数数组作为向量和一个向量相似性函数，这决定了如何计算向量之间的相似性。

查询时使用KnnVectorQuery：执行最近邻搜索时，KnnVectorQuery允许您指定一个查询向量和想要检索的最近邻的数量k。内部使用HNSW算法来快速找到最相似的向量。

因此，虽然在使用上述方法时代码中没有显式提到“HNSW”，通过使用KnnVectorField和KnnVectorQuery，您已经在利用Lucene提供的HNSW接口了。这个接口背后的实现细节确保了向量搜索的效率和准确性，而无需用户直接与HNSW算法的复杂性打交道。

总结
您的indexDocVectors方法通过向索引中添加KnnVectorField，以及searchHnsw方法通过执行KnnVectorQuery，实际上都使用了Lucene提供的HNSW接口。这表明您的代码已经在利用HNSW算法的优势进行最近邻搜索，即使这一点在API调用中并不显眼。
*
* */

public class LuceneHnswSearcherTest {
    private static final Logger log = LoggerFactory.getLogger(LuceneHnswSearcherTest.class);
    private static final String INDEX_DIR = "S:\\luceneDir2";

    public static void searchHnsw(float[] queryVector, int k) {
        try (Directory directory = FSDirectory.open(Paths.get(INDEX_DIR));
             DirectoryReader reader = DirectoryReader.open(directory)) {
            IndexSearcher searcher = new IndexSearcher(reader);

            // 使用KnnVectorQuery进行最近邻搜索
            KnnVectorQuery knnQuery = new KnnVectorQuery("vector", queryVector, k);
            TopDocs topDocs = searcher.search(knnQuery, k);
            System.out.println("HNSW method initiated!");
            System.out.println("Top " + k + " closest hits:");
            for (ScoreDoc sd : topDocs.scoreDocs) {
                Document doc = searcher.doc(sd.doc);
                // 打印文档ID和匹配得分
                System.out.println("DocId: " + doc.get("docId") + " content: "+doc.get("content")+", Score: " + sd.score);
            }
        } catch (IOException e) {
            log.error("HNSW Search error: ", e);
        }
    }

    public static void main(String[] args) {

        float[] queryVector = {0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f,0.5f,0.3f, 0.4f,0.1f, 0.2f, 0.3f, 0.4f,0.1f, 0.2f};
        int k = 10;
        searchHnsw(queryVector, k);
    }
}
