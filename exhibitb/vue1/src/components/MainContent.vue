<template>
  <div class="main-content" >
    <h1>这是我的毕设首页</h1>
    <h2>基于Lucene的垂直搜索引擎设计与开发</h2>
    <p>这里是关于项目的介绍内容:</p>
    <!-- 更多内容 -->

    需求背景：
    Lucene是一个开放源代码的全文检索引擎工具包，但它不是一个完整的全文检索引擎，而是一个全文检索引擎的架构。在Java开发环境里Lucene是一个成熟的免费开源工具。
    完成一个基于Lucene的垂直搜索引擎，是《信息检索》课程的重要实验环节。
    随着向量表示和近似最近邻算法的发展，Lucene也在发布新的版本，Lucene最新版本9.0已经包含了这些新功能，在信息检索的性能方面有了很大的提高
    具体内容描述：设计一个基于Lucene的垂直搜索引擎，选择Lucene提供的不同技术路线，展示不同技术在存储和查询速度，查询准确率等方面的差异。为《信息检索》课程进行辅助教学。
    <br>
    <br>
    目标和要求:
    （1）	搜索引擎不包括数据采集部分，可以选择常见的文本数据集作为原始数据
    （2）	采用词包模型构建索引，分别采用TD-IDF和BM25模型建立相关度计算方法
    （3）	采用高维度数字向量建立索引，执行最近邻搜索。由此产生的实现使用了分层可导航小世界图算法，并满足了机器学习领域数据科学家对包含向量的文档进行索引的需求。
    （4）	对多种方法在性能和查准率方面进行对比。
    （5）	系统保留文档数据的扩展接口，辅助课程实验的完成
  </div>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
  <br>
</template>

<script>
export default {
  name: 'MainContent'
}
</script>

<style>

.main-content {
  flex-grow: 1;
  padding: 20px;
  overflow-y: auto;
  font-size: 26px;
}

h1 {
  font-size: 53px; /* 大标题的字体大小 */
}

h2 {
  font-size: 45px; /* 次级标题的字体大小 */
}

p {
  font-size: 28px; /* 段落文本的字体大小 */
}

</style>
