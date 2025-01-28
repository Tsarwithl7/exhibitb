<template>
  <h1>我的索引</h1>
  <div>
    <router-link to="/manage/upload">管理书本</router-link>
    <router-link to="/manageindex">管理索引</router-link>
    <router-view></router-view>



    <el-table
        :data="tableData"
        style="width: 100%"
        max-height="450">
      <el-table
          v-if="!isTableDataEmpty"
          :data="tableData"
          style="width: 100%"
          max-height="450">
        <!-- 表格列的内容 -->
      </el-table>
      <el-table-column
          fixed
          label="编号"
          width="120"
          type="index">
      </el-table-column>
      <el-table-column
          label="名称"
          width="180">
        <template v-slot="scope">
          {{ scope.row.indexName.substring(0, 11) + (scope.row.indexName.length > 11 ? '...' : '') }}
        </template>
      </el-table-column>
      <el-table-column
          label="索引名"
          width="300">
        <template v-slot="scope">
          {{ scope.row.indexName.length > 20 ? scope.row.indexName.substring(0, 20) + '...' : scope.row.indexName}}
        </template>
      </el-table-column>
      <el-table-column
          label="索引大小"
          width="120">
        <template v-slot="scope">
          {{ scope.row.size +"B"}}
        </template>
      </el-table-column>
      <el-table-column
          label="上次更改日期"
          width="500">
        <template v-slot="scope">
          {{ scope.row.lastModified}}
        </template>
      </el-table-column>
    </el-table>


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
</template>

<script>
import axios from 'axios';
export default {
  name: 'IndexPage', // 页面名称改为IndexPage
  data() {
    return {
      // 假设这是从后端获取的数据
      tableData: []

    }
  },
  computed: {
    isTableDataEmpty() {
      return this.tableData.length === 0;
    }
  },

  created() {
    this.fetchIndexes();
  },
  methods: {
    confirmIndexCreation() {
      this.checkIndexDirectory().then(isNotEmpty => {
        if (isNotEmpty) {
          this.$confirm('索引目录不为空，继续操作将清空目录并删除所有现有文档。是否继续?', '警告', {
            confirmButtonText: '继续',
            cancelButtonText: '取消',
            type: 'warning'
          }).then(() => {
            this.clearIndexDirectory().then(() => {
              this.$message.success('索引目录已清空');
              this.createIndex();
            });
          }).catch(() => {
            this.$message({
              type: 'info',
              message: '已取消操作'
            });
          });
        } else {
          this.createIndex();
        }
      });
    },

    fetchIndexes() {

      axios.get('http://localhost:8088/indexpage')
          .then(response => {
            this.tableData = response.data.map(file => ({
              indexName: file.fileName,
              size: `${file.fileSize}`,
              lastModified:file.lastModified
            }));
          })
          .catch(error => console.error('Error fetching indexes:', error));
    },
  }
}
</script>
