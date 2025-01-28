<template>
  <div class="book-store" >
    <header class="store-header">
      <h1>图书检索系统</h1>
      <div class="search-bar">
        <input type="text" placeholder="搜索书籍" v-model="searchQuery">
        <button @click="setSearchModel('tfidf');performSearch()">TF-IDF搜索</button>
        <button @click="setSearchModel('bm25');performSearch()">BM25搜索</button>
        <button @click="setSearchModel('comparetwo');performSearch()">BM25和TF-IDF搜索比较</button>
        <button @click="setSearchModel('searchknn');performSearch()">最近邻搜索</button>
        <button @click="setSearchModel('compareall');performSearch()">所有方法的比较</button>

        <button @click="setSearchModel('default');performSearch()">搜索</button>
      </div>
    </header>

    <div class="filter-section">
      <div class="word-count-filter">
        <!--        <button @click="showAllBooks()">显示全部书籍</button>-->
        <button @click="resetFiltersAndSearch()">显示全部书籍</button>
        <button @click="handleWordCountFilter(0, 50)">50字以下</button>
        <button @click="handleWordCountFilter(50, 100)">50-100字</button>
        <button @click="handleWordCountFilter(100,100000)">100字以上</button>
      </div>
<!--      <div class="date-filter">-->
<!--        &lt;!&ndash;        <button @click="showAllBooks()">显示所有时间</button>&ndash;&gt;-->
<!--        <button @click="resetFiltersAndSearch()">显示所有时间</button>-->
<!--        <button @click="setDateFilter('past1Hour')">1小时内</button>-->
<!--        <button @click="setDateFilter('1to3Hours')">1-3小时内</button>-->
<!--        <button @click="setDateFilter('3to24Hours')">3-24小时内</button>-->
<!--        <button @click="setDateFilter('over24Hours')">24小时之后</button>-->
<!--      </div>-->
      <div class="time-filter">
        <input type="number" v-model="interval" placeholder="时间间隔（小时）">
        <input type="checkbox" v-model="flag" true-value="1" false-value="0"> 启用时间筛选（小时）
      </div>

    </div>

    <div class="books-list" v-if="books.length > 0">
      <div class="book-item" v-for="book in displayedBooks" :key="book.fileName">
        <img :src="book.imagePath || '/u=3967349716,315614459&fm=253&fmt=auto&app=138&f=JPEG.jpg'" :alt="book.fileName">
        <h2>{{ book.fileName?.replace(".txt", "") || "未知文件名" }}</h2>
        <p>文件大小: {{ book.fileSize + "B" }}</p>
        <p v-if="searchModel === 'comparetwo'">TF-IDF值: {{ book?.tfidf ?? '未知' }}</p>
        <p v-if="searchModel === 'comparetwo'">BM25值: {{ book?.bm25 ?? '未知' }}</p>
        <p v-if="searchModel === 'searchknn'">knn搜索用时(ns): {{ book?.knnsearchtime ?? '未知' }}</p>
        <p v-if="searchModel === 'default'">默认搜索用时(ns): {{ book?.defaultsearchtime ?? '未知' }}</p>
        <p v-if="searchModel === 'comparetwo'">TFIDF搜索用时(ns): {{ book?.tfidfSearchTime ?? '未知' }}</p>
        <p v-if="searchModel === 'comparetwo'">BM25搜索用时(ns): {{ book?.bm25SearchTime ?? '未知' }}</p>
        <p v-if="searchModel ==='tfidf'">TFIDF搜索用时(ns): {{ book?.tfidfSearchTime ?? '未知' }}</p>
        <p v-if="searchModel === 'bm25'">BM25搜索用时(ns): {{ book?.bm25SearchTime ?? '未知' }}</p>

<!--        <button @click="viewDetails(book.fileName)">查看详情</button>-->
        <button @click="viewDetails(book.fileName, searchQuery)">查看详情</button>
      </div>
    </div>
    <div v-else class="no-books-message">
      没有符合您的搜索条件的书籍。
    </div>


    <div class="pagination">
      <button :disabled="currentPage <= 1" @click="currentPage--">上一页</button>
      <button :disabled="currentPage >= totalPages" @click="currentPage++">下一页</button>
    </div>
  </div>
</template>

<script>
import axios from "axios";

export default {
  name: 'BookStore',
  data() {
    return {
      searchQuery: '',
      // searchModel: 'default',
      searchModel: '',
      currentPage: 1,
      pageSize: 6, // 每页显示的书籍数量
      wordCountFilter: {low: 0, high: 100000}, // 更新以跟踪字数过滤
      interval: 0, // 时间间隔，以小时计
      flag: 0, // 是否启用时间筛选的标志
      dateFilter: '',
      books: [], // 确保初始值为数组
      originalBooks: [],
    };
  },
  computed: {
    displayedBooks() {
      // 确保 this.books 是数组
      if (!Array.isArray(this.books)) {
        console.error('Expected books to be an array, but got:', typeof this.books);
        return [];
      }
      // 过滤逻辑保持不变
      return this.books.slice((this.currentPage - 1) * this.pageSize, this.currentPage * this.pageSize);
    },
    totalPages() {
      return Math.ceil(this.books.length / this.pageSize);
    },
  },
  methods: {
    resetFiltersAndSearch() {
      // 重置所有搜索条件
      this.searchQuery = '';
      this.wordCountFilter = {low: 0, high: 100000};

      this.dateFilter = '';

      this.searchModel = 'default';

      // 重新加载书籍列表，没有任何过滤条件
      this.fetchFiles();
    },



    fetchFiles() {

      axios.get('http://localhost:8088/bookstore').then(response => {
        // 在更新 this.books 之前，检查 response.data 是否为数组
        if (Array.isArray(response.data)) {
          this.books = response.data;
        } else {
          console.error('Data fetched is not an array:', response.data);
          this.books = []; // 防止类型错误
        }
      }).catch(error => {
        console.error('Error fetching books:', error);
        this.books = []; // 错误处理
      });
    },
    handleWordCountFilter(low, high) {
      console.log(low,high);

      // 更新字数过滤状态
      this.wordCountFilter = { low, high };
      // 根据字数过滤书籍，并考虑现有的搜索文本
      const url = `http://localhost:8088/search?queryText=${this.searchQuery}&low=${low}&high=${high}`;
      axios.get(url).then(response => {
        this.handleResponse(response);
      }).catch(this.handleError);
    },
    performSearch() {
      console.log("Flag:", this.flag); // 打印flag的值
      console.log("Interval:", this.interval); // 打印interval的值


      if (!this.searchQuery) {

        alert("请输入有效内容");
        return;
      }
      console.log("Performing search with model:", this.searchModel); // 调试输出
      let url = '';
      if (this.searchModel === 'comparetwo') {
        console.log("compare方法！")
        url = `http://localhost:8088/compareSearch?query=${encodeURIComponent(this.searchQuery)}`;
        if (this.flag === 1) {
          url += `&interval=${this.interval}&flag=1`;
        }

        axios.get(url).then(response => {
          this.handleResponse(response);
        }).catch(this.handleError);
      }
      if (this.searchModel === 'compareall') {
        console.log("compare all方法！")
        url = `http://localhost:8088/compareall?query=${encodeURIComponent(this.searchQuery)}`;

        axios.get(url).then(response => {
          this.handleResponse(response);
          this.$router.push({ name: 'SearchComparison' });
        }).catch(this.handleError);
      }
      else if (this.searchModel === 'bm25') {
        // 执行KNN搜索
        url = `http://localhost:8088/calculatebm25?query=${encodeURIComponent(this.searchQuery)}`;


        axios.get(url).then(response => {
          this.handleResponse(response);
        }).catch(this.handleError);
      }
      else if (this.searchModel === 'tfidf') {
        // 执行KNN搜索
        url = `http://localhost:8088/calculatetfidf?query=${encodeURIComponent(this.searchQuery)}`;

        axios.get(url).then(response => {
          this.handleResponse(response);
        }).catch(this.handleError);
      }

      else if (this.searchModel === 'searchknn') {
        // 执行KNN搜索
        url = `http://localhost:8088/searchknn?query=${encodeURIComponent(this.searchQuery)}&k=${this.pageSize}`;
        if (this.flag === 1) {
          url += `&interval=${this.interval}&flag=1`;
        }


        axios.get(url).then(response => {
          this.handleResponse(response);
        }).catch(this.handleError);
      }

      else if (this.searchModel === 'default') {
        console.log("普通方法！")
        // 保留原有逻辑，针对非比较模式的搜索
        url = `http://localhost:8088/search?queryText=${this.searchQuery}`;
        if (this.wordCountFilter.low !== null && this.wordCountFilter.high !== null) {
          url += `&low=${this.wordCountFilter.low}&high=${this.wordCountFilter.high}`;
        }

        if (this.flag == 1) {
          url += `&interval=${this.interval}&flag=1`;
        }
        console.log("url："+url)
      }
      axios.get(url).then(response => {
        this.handleResponse(response);
      }).catch(this.handleError);
    },
    handleResponse(response) {
      // 处理响应数据的通用逻辑
      console.log(0)
      if (Array.isArray(response.data)) {
        console.log(JSON.stringify(response.data, null, 2));
        this.books = response.data.map(book => ({
          ...book,
          imagePath: book.imagePath || '/u=3967349716,315614459&fm=253&fmt=auto&app=138&f=JPEG.jpg',
          tfidf: book.tfidf, // 假设后端返回的对象中包含tfidf和bm25
          bm25: book.bm25,
        }));
        if (this.searchModel === 'comparetwo') {
          console.log(2);
          console.log("BM25和TF-IDF搜索比较结果:");
          this.books.forEach(book => {
            console.log(`文件名: ${book.fileName}, TF-IDF值: ${book.tfidf}, BM25值: ${book.bm25}`);
          });
        }

      } else {
        console.error('Response data is not an array:', response.data);
        this.books = [];
        this.originalBooks = [];
      }
    },
    handleError(error) {
      // 错误处理的通用逻辑
      console.error('Error fetching data:', error);

    },

    setSearchModel(model) {
      console.log("Setting search model to:", model); // 调试输出
      this.searchModel = model;

    },

    viewDetails(fileName, searchQuery) {
      console.log(`Viewing details for ${fileName} with search query "${searchQuery}"`);
      this.$router.push({
        name: 'BookInfo',
        params: { fileName },
        query: { searchQuery: searchQuery }
      });
    },
  },
  created() {
    this.fetchFiles();
  },
}
</script>


<style>
.book-store {
  display: flex;
  flex-direction: column;
  align-items: center;
  width: 100%;
  max-width: 1200px;
  margin: 0 auto;
  padding: 20px;
}
.book-item img {
  width: 100%;
  height: 135px; /* 设置一个固定的高度 */
  /*height: auto; !* 保持图片的宽高比 *!*/
  /*object-fit: cover; !* 剪裁图片以覆盖整个容器 *!*/
  border-radius: 5px; /* 可选的：为图片添加圆角 */
  max-width: 150px; /* 限制最大宽度，可根据需要调整 */
}

.store-header {
  align-self: start;
  margin-bottom: 20px;
}

.search-bar {
  margin-top: 10px;
}

.search-bar input[type="text"] {
  padding: 10px;
  font-size: 16px;
}
.filter-section {
  display: flex;
  justify-content: space-between;
  width: 100%;
  margin-bottom: 20px;
}

.filter-section button {
  padding: 5px 10px;
  margin-right: 10px;
  cursor: pointer;
}



.books-list {
  display: flex;
  flex-wrap: wrap;
  justify-content: center;
  gap: 20px;
}

.book-item {
  width: calc(33.333% - 20px);
  text-align: center;
  border: 1px solid #eee;
  padding: 10px;
  border-radius: 5px;
}


.pagination {
  display: flex;
  justify-content: center;
  margin-top: 20px;
}

.pagination button {
  padding: 10px 20px;
  margin: 0 10px;
  border-radius: 5px;
  cursor: pointer;
}
.pagination button:disabled {
  opacity: 0.1;
  cursor: not-allowed;
}
</style>
