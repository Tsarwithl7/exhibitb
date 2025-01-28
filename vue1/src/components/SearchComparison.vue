<template>
  <div class="search-comparison">
    <h1>不同检索方法的比较</h1>
    <div>
      <label for="searchQuery" class="custom-file-upload">搜索词：</label>
      <input v-model="searchQuery" class="inputsize" id="searchQuery" placeholder="输入搜索词或短语" />
      <button @click="toggleHighlight">Highlight</button> 
      <button @click="toggleView">查看图表</button> 

    </div>

      <button v-if="isChartVisible" style="width: 150px;"   @click="generateTfidfBm25Chart">
        相似度比较
      </button>

      <button v-if="isChartVisible" style="width: 150px;"  @click="generateSearchingTime"> 检索时间比较</button>

      <div v-if="isChartVisible" id="main" style="width: 2400px; height: 950px;"></div>
      <div v-if="isChartVisible && isAnalysisOneVisible" style="width: 2400px; height: 890px;"></div>



    <el-table v-else :data="searchResults.flat()" class="el-table" style="width: 100%" :row-style="rowStyle">
      <el-table-column prop="fileName" label="文件名"></el-table-column>
      <el-table-column label="内容">
        <template #default="{ row }">
          {{ row.content && row.content.length > 30 ? row.content.slice(0, 30) + "..." : row.content || '内容未知' }}
        </template>
      </el-table-column>
      <el-table-column prop="wordCount" label="词数"></el-table-column>

      <el-table-column prop="tfidf" label="TFIDF">
        <template v-slot="scope">

          {{ scope.row.tfidf !== 0 ? scope.row.tfidf.toFixed(2) : '—' }}
        </template>
      </el-table-column>

      <el-table-column prop="bm25" label="BM25">
        <template v-slot="scope">

          {{ scope.row.bm25 !== 0 ? scope.row.bm25.toFixed(2) : '—' }}
        </template>
      </el-table-column>

      <el-table-column prop="bm25SearchTime" label="BM25搜索时间(ns)">
        <template v-slot="scope">

          {{ scope.row.bm25SearchTime !== 0 ? scope.row.bm25SearchTime.toFixed(2) : '—' }}
        </template>
      </el-table-column>

      <el-table-column prop="tfidfSearchTime" label="TFIDF搜索时间(ns)">
        <template v-slot="scope">

          {{ scope.row.tfidfSearchTime !== 0 ? scope.row.tfidfSearchTime.toFixed(2) : '—' }}
        </template>
      </el-table-column>

      <el-table-column prop="knnsearchtime" label="KNN搜索时间(ns)">
        <template v-slot="scope">

          {{ scope.row.knnsearchtime !== 0 ? scope.row.knnsearchtime.toFixed(2): '—' }}
        </template>
      </el-table-column>

      <el-table-column prop="defaultsearchtime" label="默认搜索时间(ns)">
        <template v-slot="scope">

          {{ scope.row.defaultsearchtime !== 0 ? scope.row.defaultsearchtime.toFixed(2) : '—' }}
        </template>
      </el-table-column>

      <el-table-column label="对应方法准确率">
        <template #default="scope">

          <div v-if="scope.row.precisiondefault !== 0">
            Default方法准确率: {{ scope.row.precisiondefault.toFixed(2)}}
          </div>
<!--          <div v-else>-->
<!--            Default方法准确率: —-->
<!--          </div>-->
          <div v-if="scope.row.precisionknn !== 0">
            KNN方法准确率: {{ scope.row.precisionknn.toFixed(2) }}
          </div>
          <div v-if="scope.row.precisiontfidf !== 0">
            TFIDF方法准确率: {{ scope.row.precisiontfidf.toFixed(2) }}
          </div>
          <div v-if="scope.row.precisionbm25 !== 0">
            BM25方法准确率: {{ scope.row.precisionbm25.toFixed(2) }}
          </div>
        </template>
      </el-table-column>
      <el-table-column label="对应方法召回率">
        <template #default="scope">
          <div v-if="scope.row.recalldefault !== 0">
            Default方法召回率: {{ scope.row.recalldefault.toFixed(2) }}
          </div>
          <div v-if="scope.row.recallknn !== 0">
            KNN方法召回率: {{ scope.row.recallknn.toFixed(2) }}
          </div>
          <div v-if="scope.row.recalltfidf !== 0">
            TFIDF方法召回率: {{ scope.row.recalltfidf.toFixed(2) }}
          </div>
          <div v-if="scope.row.recallbm25 !== 0">
            BM25方法召回率: {{ scope.row.recallbm25.toFixed(2) }}
          </div>
        </template>
      </el-table-column>
    </el-table>

  </div>
  <br>
  <br>
  <br>
    <div>请输入预期的搜索词：</div>
  <br>
  <el-select v-model="selectedOptions" filterable multiple placeholder="请选择">
    <el-option
        v-for="item in options"
        :key="item.value"
        :label="item.label"
        :value="item.value">
    </el-option>
  </el-select>
  <button @click="fetchSearchResults">搜索</button>
  <br>
  <br>
  <br>
  <br>
  <br>



</template>

<script>
import * as echarts from 'echarts';

import { ElTable, ElTableColumn } from 'element-plus';
import axios from 'axios';

export default {
  mounted() {
    this.fetchAllDocumentNames();
    if (this.isChartVisible) {
      this.initECharts();
    }
  },

  components: {
    ElTable,
    ElTableColumn,
  },
  data() {
    return {
      searchQuery: '',
      searchResults: [],
      highlightFiles: {}, //
      selectedOptions: [], // 修改此处
      options: [], // 用于存储从后端获取的所有fileName
      isChartVisible: false,
      isAnalysisOneVisible: false, // 新增状态控制变量
      // value: ''


    };
  },
  methods: {
    initECharts() {
      this.$nextTick(() => {
        if (this.myChart) {
          this.myChart.dispose();
        }
        const dom = document.getElementById('main');
        if (dom) {
          this.myChart = echarts.init(dom);

          // 创建一个Map来存储每个fileName及其累计的wordCount
          const fileNamesMap = new Map();
          this.searchResults.forEach(result => {
            if (!fileNamesMap.has(result.fileName)){

              fileNamesMap.set(result.fileName, result.wordCount);
            }
          });


          const fileNames = Array.from(fileNamesMap.keys());
          const dataValues = Array.from(fileNamesMap.values());

          this.myChart.setOption({
            title: {
              text: '字数对比'
            },
            tooltip: {},
            xAxis: {
              type: 'category',
              data: fileNames,
              axisLabel: {
                interval: 0, // 显示所有标签
                rotate: 45, // 标签倾斜显示
                margin: 30,  // 增加标签与轴线的距离
                opacity: 0.9,
                textStyle: {  // 添加字体样式
                  color: '#333', // 文本颜色
                  fontSize: 14,  // 字体大小
                  fontWeight: 'bold'  // 字体粗细
                }
              }
            },
            yAxis: {
              type: 'value'
            },
            grid: {
              left: '3%',
              right: '4%',
              bottom: '20%', // 增加底部边距以显示完整的横轴标签
              containLabel: true
            },
            series: [{
              name: '词数',
              type: 'bar',
              data: dataValues
            }]
          });
        } else {
          console.error('DOM element for echarts not found');
        }
      });
    },

    generateSearchingTime() {
      this.isAnalysisOneVisible = true;

      const filteredResults = this.searchResults.filter(result =>
          result.bm25SearchTime !== 0 || result.tfidfSearchTime !== 0 || result.knnsearchtime !== 0 || result.defaultsearchtime !== 0
      );

      console.log("Time filtered results:", filteredResults);

      const aggregatedResults = filteredResults.reduce((acc, curr) => {
        acc.tfidf += curr.tfidfSearchTime || 0;
        acc.knn += curr.knnsearchtime || 0;
        acc.bm25 += curr.bm25SearchTime || 0;
        acc.default += curr.defaultsearchtime || 0;
        return acc;
      }, { tfidf: 0, knn: 0, bm25: 0, default: 0 });

      const methods = ['TFIDF', '最近邻搜索', 'BM25', '默认搜索'];
      const times = [
        aggregatedResults.tfidf,
        aggregatedResults.knn,
        aggregatedResults.bm25,
        aggregatedResults.default
      ];

      const sortedData = methods.map((method, index) => ({
        method,
        time: times[index]
      })).sort((a, b) => a.time - b.time);

      const sortedMethods = sortedData.map(data => data.method);
      const sortedTimes = sortedData.map(data => data.time);

      console.log("Sorted Methods:", sortedMethods);
      console.log("Sorted Times:", sortedTimes);

      this.$nextTick(() => {
        const dom = document.getElementById('main');
        if (dom) {
          if (this.myChart) {
            this.myChart.dispose();
          }
          this.myChart = echarts.init(dom);
          const option = {
            title: {
              text: '搜索时间比较 单位(ns)',
              left: 'center',
              top: '20',
              textStyle: {
                fontSize: 31,
                fontWeight: 'bold'
              }
            },
            tooltip: {
              trigger: 'axis'
            },
            grid: {
              bottom: 300
            },
            legend: {
              data: ['搜索时间']
            },
            toolbox: {
              show: true,
              itemSize: 50,
              iconStyle: {
                normal: {
                  borderColor: '#666',
                  borderWidth: 1
                },
                emphasis: {
                  borderColor: '#333'
                }
              },
              feature: {
                dataView: { show: true, readOnly: false },
                magicType: { show: true, type: ['line', 'bar'] },
                restore: { show: true },
                saveAsImage: { show: true }
              }
            },
            xAxis: {
              type: 'category',
              data: sortedMethods,
              axisLabel: {
                interval: 0,
                rotate: 0,
                margin: 20,
                opacity: 0.9,
                textStyle: {
                  color: '#333',
                  fontSize: 30,
                  fontWeight: 'bold'
                }
              }
            },
            yAxis: {
              type: 'value',
              textStyle: {
                color: '#333',
                fontSize: 30,
                fontWeight: 'bold'
              }
            },
            series: [
              {
                name: '搜索时间',
                type: 'bar',
                data: sortedTimes,
                barWidth: '30%',
                label: {
                  show: true,
                  position: 'top',
                  textStyle: {
                    fontSize: 20,
                    fontWeight: 'bold'
                  }
                }
              },
              {
                name: '搜索时间',
                type: 'line',
                data: sortedTimes,
                label: {
                  show: true,
                  position: 'top',
                  textStyle: {
                    fontSize: 20,
                    fontWeight: 'bold'
                  }
                }
              }
            ]
          };
          this.myChart.setOption(option);
        }
      });
    }
,
    generateTfidfBm25Chart() {
      this.isAnalysisOneVisible = true;

      const filteredResults = this.searchResults.filter(result => result.tfidf !== 0 || result.bm25 !== 0);
      console.log("Filtered Results:", filteredResults);

      const resultsByFileName = filteredResults.reduce((acc, curr) => {
        if (!acc[curr.fileName]) {
          acc[curr.fileName] = { tfidf: 0, bm25: 0 };
        }
        if (curr.tfidf !== 0) acc[curr.fileName].tfidf = curr.tfidf;
        if (curr.bm25 !== 0) acc[curr.fileName].bm25 = curr.bm25;
        return acc;
      }, {});

      console.log("Aggregated Results by FileName:", resultsByFileName);

      const fileNames = Object.keys(resultsByFileName);
      const tfidfValues = fileNames.map(name => resultsByFileName[name].tfidf);
      const bm25Values = fileNames.map(name => resultsByFileName[name].bm25);

      console.log("File Names:", fileNames);
      console.log("TFIDF Values:", tfidfValues);
      console.log("BM25 Values:", bm25Values);

      const sortedData = fileNames.map((fileName, index) => ({
        fileName,
        tfidf: tfidfValues[index],
        bm25: bm25Values[index]
      })).sort((a, b) => a.tfidf - b.tfidf);

      const sortedFileNames = sortedData.map(data => data.fileName.slice(0, -4));
      const sortedTfidfValues = sortedData.map(data => data.tfidf);
      const sortedBm25Values = sortedData.map(data => data.bm25);

      console.log("Sorted File Names:", sortedFileNames);
      console.log("Sorted TFIDF Values:", sortedTfidfValues);
      console.log("Sorted BM25 Values:", sortedBm25Values);

      const numDocuments = this.searchResults.length;
      console.log("numDocsLength:", numDocuments);
      const idfValues = sortedBm25Values.map(bm25 => bm25 / (1.2 + 1));
      const avgIdfValue = idfValues.reduce((acc, val) => acc + val, 0) / idfValues.length;

      const k1 = 1.5;
      const n = 1;
      const asymptoticValue = n * avgIdfValue * (k1 + 1);
      const asymptoticLine = new Array(sortedBm25Values.length).fill(asymptoticValue);

      console.log("IDF Values:", idfValues);
      console.log("Average IDF Value:", avgIdfValue);
      console.log("Asymptotic Line Value:", asymptoticValue);

      this.$nextTick(() => {
        const dom = document.getElementById('main');
        if (dom) {
          if (this.myChart) {
            this.myChart.dispose();
          }
          this.myChart = echarts.init(dom);
          const option = {
            title: {
              text: 'TFIDF 和 BM25 比较',
              left: 'center',
              top: '20',
              textStyle: {
                fontSize: 36,
                fontWeight: 'bold'
              }
            },
            tooltip: {
              trigger: 'axis'
            },
            grid: {
              bottom: 300
            },
            legend: {
              data: ['TFIDF', 'BM25', '拟合直线'],
              textStyle: {
                fontSize: 20,
                fontWeight: 'bold'
              }
            },
            toolbox: {
              show: true,
              itemSize: 50,
              iconStyle: {
                normal: {
                  borderColor: '#666',
                  borderWidth: 1
                },
                emphasis: {
                  borderColor: '#333'
                }
              },
              feature: {
                dataView: { show: true, readOnly: false },
                magicType: { show: true, type: ['line', 'bar'] },
                restore: { show: true },
                saveAsImage: { show: true }
              }
            },
            xAxis: {
              type: 'category',
              data: sortedFileNames,
              axisLabel: {
                interval: 0,
                rotate: 15,
                margin: 20,
                opacity: 0.9,
                textStyle: {
                  color: '#333',
                  fontSize: 20,
                  fontWeight: 'bold'
                }
              }
            },
            yAxis: {
              type: 'value',
              axisLabel: {
                textStyle: {
                  color: '#333',
                  fontSize: 20,
                  fontWeight: 'bold'
                }
              },
              axisLine: {
                lineStyle: {
                  width: 3
                }
              }
            },
            series: [
              {
                name: 'TFIDF',
                type: 'line',
                data: sortedTfidfValues,
                lineStyle: {
                  width: 4
                }
              },
              {
                name: 'BM25',
                type: 'line',
                data: sortedBm25Values,
                lineStyle: {
                  width: 4
                }
              },
              {
                name: '拟合直线',
                type: 'line',
                data: asymptoticLine,
                lineStyle: {
                  color: 'red',
                  type: 'dashed',
                  width: 4
                }
              }
            ]
          };
          this.myChart.setOption(option);
        }
      });
    }
,




    toggleView() {
      this.isChartVisible = !this.isChartVisible;
      if (this.isChartVisible) {
        this.initECharts();
      } else if (this.myChart) {
        this.myChart.dispose();
        this.myChart = null;
      }
    },

    generateRandomColor() {
      return '#' + Math.floor(Math.random()*16777215).toString(16);
    },
    async fetchSearchResults() {
      if (!this.searchQuery.trim()) {
        alert('请输入搜索词');
        return;
      }

      const params = new URLSearchParams({
        query: this.searchQuery,
        expectedResults: this.selectedOptions.join(','), // 将数组转换为逗号分隔的字符串
      });

      try {
        const response = await axios.get(`http://localhost:8088/compareall?${params.toString()}`);
        console.log("API响应数据:", response.data);
        this.searchResults = response.data;
      } catch (error) {
        console.error('搜索出错:', error);
        this.searchResults = [];
      }
    },

    async fetchAllDocumentNames() {
      try {
        const response = await axios.get(`http://localhost:8088/all`);
        console.log("获取所有文档的响应:", response.data);
        // 更新options数组
        this.options = response.data.map(doc => ({
          value: doc.fileName,
          label: doc.fileName
        }));
      } catch (error) {
        console.error('获取文档名称出错:', error);
        this.options = []; // 出错时清空options数组
      }
    },

    toggleHighlight() {
      if (Object.keys(this.highlightFiles).length) {
        // 如果已经设置了高亮，再次点击取消高亮
        this.highlightFiles = {};
      } else {
        // 遍历搜索结果并为每个不同的fileName分配颜色
        this.searchResults.flat().forEach((item) => {
          if (!this.highlightFiles[item.fileName]) {
            this.highlightFiles[item.fileName] = this.generateRandomColor();
          }
        });
      }
    },

    rowClassName({row}) {
      if (this.highlightFileName && row.fileName === this.highlightFileName) {
        return 'highlight-row'; // 返回一个类名，用于高亮行
      }
      return '';
    },
    rowStyle({row}) {
      const color = this.highlightFiles[row.fileName];
      return color ? { backgroundColor: color } : {};
    },
  },
};
</script>

<style scoped>
h1{
  font-size: 40px;
}
.search-comparison {
  margin: 20px;
}

.chart-container {
  display: flex;
  justify-content: center; /* 水平居中 */
  align-items: center; /* 垂直居中 */
  height: 100%;
}

.el-table{
  border-radius: 4px;
  overflow: hidden;
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
  font-size: 21px;
}
.inputsize{
  font-size:21px;
}

.highlight-row {
  background-color: yellow; /* 选择一个醒目的颜色来高亮行 */
}

.search-comparison {
  margin: 20px;
  display: flex;
  flex-direction: column;
align-items: center;
width: 100%;
}
#main {
  margin-top: 50px;
}

</style>
