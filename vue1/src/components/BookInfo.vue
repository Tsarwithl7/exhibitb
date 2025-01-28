<template>
  <div class="book-container">
    <img v-if="fileInfo.imagePath" :src="fileInfo.imagePath" alt="Document Image" class="document-image"/>
    <h1>{{ fileInfo.fileName.replace(".txt", "")}}</h1>
    <div class="book-details">
      <h3>文件详情：</h3>
      <p>大小: {{ fileInfo.formattedFileSize }}</p>
      <p>上次修改日期：2024/4/27</p>
    </div>
    <div class="book-content">
      <h3>内容：</h3>
      <p v-html="highlightedContent"></p>
    </div>
  </div>
</template>

<script>
import axios from 'axios';

export default {
  name: 'FileInfo',
  data() {
    return {
      fileInfo: {
        fileName: ""
      },
      searchQuery: ''
    };
  },
  created() {
    this.searchQuery = this.$route.query.searchQuery || '';
    console.log("Search query on BookInfo page:", this.searchQuery);
    this.fetchFileInfo();
  },
  computed: {
    highlightedContent() {
      // 检查是否有搜索查询
      if (!this.fileInfo.content || !this.searchQuery) {
        // 没有搜索查询，返回原始内容
        return this.fileInfo.content;
      }
      // 有搜索查询，进行高亮处理
      const searchTerms = this.searchQuery.split(/\s+/); // 分割搜索查询为单词数组
      return this.highlightSearchTerms(this.fileInfo.content, searchTerms);
    }
  },
  methods: {
    async fetchFileInfo() {
      const fileName = this.$route.params.fileName;
      const url = `http://localhost:8088/${fileName}`;
      try {
        const response = await axios.get(url);
        this.fileInfo = {
          fileName: response.data.fileName,
          content: response.data.content,
          formattedFileSize: this.formatFileSize(response.data.fileSize),
          imagePath: response.data.imagePath // 从响应中获取 imagePath
        };
        console.log("Fetched file info:", this.fileInfo); // 打印文件信息

      } catch (error) {
        console.error('读取文件信息失败:', error);
      }
    },

    highlightSearchTerms(text, searchTerms) {
      let highlightedText = text;
      // 对每个搜索词进行高亮处理
      searchTerms.forEach(term => {
        if (!term.trim()) {
          return;
        }
        highlightedText = highlightedText.replace(new RegExp(term, 'gi'), match => {
          return `<span class="highlight">${match}</span>`; // 使用highlight类高亮显示匹配项
        });
      });

      console.log("Highlighted text:", highlightedText); // 打印高亮文本
      return highlightedText;
    },

    formatFileSize(size) {
      if (size < 1024) return size + ' B';
      let i = Math.floor(Math.log(size) / Math.log(1024));
      let num = (size / Math.pow(1024, i)).toFixed(2);
      let r = 'KMGTPE';
      return `${num} ${r.charAt(i-1)}B`;
    }
  }
};
</script>

<style>
.book-container {
  max-width: 800px;
  margin: 20px auto;
  padding: 20px;
  border: 1px solid #eaeaea;
  border-radius: 10px;
  box-shadow: 0 2px 4px rgba(0,0,0,0.1);
}

.book-container h1, .book-container h2, .book-container h3 {
  color: #333;
}

.book-details, .book-content {
  background-color: #f9f9f9;
  padding: 15px;
  margin-top: 20px;
  border-radius: 5px;
}

.book-details p, .book-content p {
  color: #666;
}

.author {
  font-style: italic;
}

.highlight {
  background-color: yellow;
}
</style>

