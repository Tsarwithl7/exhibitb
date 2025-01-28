<template>
  <h1>我的书本</h1>
  <div>
    <router-link to="/manage/upload">管理书本</router-link>
    <router-link to="/indexpage">查看现有索引</router-link>
    <router-view></router-view>

    <el-table
        :data="tableData"
        style="width: 100%"
        max-height="450">
      <el-table-column
          fixed
          label="编号"
          width="120"
          type="index"
          index="1">
      </el-table-column>
      <el-table-column
          label="书本"
          width="180">
        <template v-slot="scope">
          {{ scope.row.fileName.substring(0, 22) + (scope.row.fileName.length > 22 ? '...' : '') }}
        </template>
      </el-table-column>

      <el-table-column
          label="内容"
          width="300">
        <template v-slot="scope">
          {{ scope.row.content.length > 20 ? scope.row.content.substring(0, 20) + '...' : scope.row.content}}
        </template>
      </el-table-column>
      <el-table-column
          label="文档大小"
          width="120">
        <template v-slot="scope">
          {{ scope.row.size +"B"}}
        </template>
      </el-table-column>
      <el-table-column
          fixed="right"
          label="操作"
          width="120">
        <template v-slot="scope">
          <el-button
              @click.prevent="deleteRow(scope.$index)"
              type="text"
              size="small">
            移除
          </el-button>
        </template>
      </el-table-column>
    </el-table>
    <br>

    <el-button
        type="primary"
        @click="confirmIndexCreation"
        style="margin-top: 20px;">
      生成索引
    </el-button>

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
</template>

<script>

import axios from 'axios';
export default {
  data() {
    return {

      tableData: [
      ]
    }
  },

  created() {
    this.fetchFiles();
  },
  methods: {
    confirmIndexCreation() {
      this.checkIndexDirectory().then(isNotEmpty => {
        if (isNotEmpty) { // 如果返回true，即目录不为空
          this.$confirm('索引目录不为空，继续操作将清空目录并删除所有现有文档。是否继续?', '警告', {
            confirmButtonText: '继续',
            cancelButtonText: '取消',
            type: 'warning'
          }).then(() => {
            this.clearIndexDirectory().then(() => {
              this.$message.success('索引目录已清空');
              this.createIndex(); // 继续生成索引的逻辑
            });
          }).catch(() => {
            this.$message({
              type: 'info',
              message: '已取消操作'
            });
          });
        } else {
          this.createIndex(); // 直接生成索引的逻辑
        }
      });
    },
    checkIndexDirectory() {
      return axios.get('http://localhost:8088/checkindex')
          .then(response => {
            console.log("responsedata是："+response.data)

            console.log("索引目录状态：", response.data ? "不为空" : "为空或不存在");
            return response.data; // true 或 false
          })
          .catch(error => {
            console.error('检查索引目录时出错:', error);
            this.$message.error('检查索引目录时出错');
          });
    },
    clearIndexDirectory() {
      return axios.post('http://localhost:8088/clearindex')
          .then(() => {

          })
          .catch(error => {
            console.error('清空索引目录时出错:', error);
            this.$message.error('清空索引目录时出错');
          });
    },
    createIndex() {

      axios.get('http://localhost:8088/manage/createindex')
          .then(() => {
            // 不需要使用响应数据时，不声明response参数
            localStorage.setItem('booksList', JSON.stringify(this.tableData));
            this.$message.success('索引生成成功');
            this.$router.push('/indexpage');
            // 生成索引成功后的逻辑
          })
          .catch(error => {
            console.error('生成索引时出错:', error);
            this.$message.error('生成索引时出错');
          });
    },
    fetchFiles() {
      axios.get('http://localhost:8088/bookstore') // 确保URL与后端服务匹配
          .then(response => {
            this.tableData = response.data.map(file => ({
              ...file,
              size: file.fileSize
            }));
          })
          .catch(error => console.error('Error fetching files:', error));
    },

    deleteRow: function (index) {

      const fileName = this.tableData[index].fileName; // 获取要删除的文件名
      var dotIndex = fileName.lastIndexOf(".");
      let s = fileName.substring(0, dotIndex);


      this.$confirm('是否确认移除该文件?', '提示', {
        confirmButtonText: '确定',
        cancelButtonText: '取消',
        type: 'warning'
      }).then(() => {
        axios.delete(`http://localhost:8088/delete/${encodeURIComponent(s)}`).then(() => {
          this.tableData.splice(index, 1);
          this.$message({
            type: 'success',
            message: '移除成功!'
          });
        }).catch(error => {
          console.error('删除失败:', error);
          this.$message({
            type: 'error',
            message: '移除失败!'
          });
        });
      }).catch(() => {

      });
    },
    truncatedContent(content) {
      const maxLength = 20; // 最大显示长度应为20
      return content.length > maxLength ? content.substring(0, maxLength) + '...' : content;
    }
  }
}
</script>

<!--<style scoped>-->

<!--/* 容器样式 */-->
<!--div {-->
<!--  padding: 20px;-->
<!--}-->
<!--/* 修改标题“我的书本”的字体大小 */-->
<!--h1 {-->

<!--}-->

<!--/* 表格样式 */-->
<!--.el-table {-->
<!--  border-radius: 4px;-->
<!--  overflow: hidden;-->
<!--  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);-->
<!--  font-size: 24px;  /* 修改表格中的字体大小 */-->
<!--}-->

<!--/* 表头和单元格样式 */-->
<!--.el-table th, .el-table td {-->
<!--  text-align: center;-->
<!--  padding: 10px 0;-->
<!--}-->

<!--/* 按钮样式 */-->
<!--.el-button {-->
<!--  margin-right: 10px;-->
<!--}-->

<!--/* 管理文件和查看现有索引链接样式 */-->
<!--.router-link {-->
<!--  display: inline-block;-->
<!--  margin-right: 20px;-->
<!--  padding: 10px 15px;-->
<!--  text-decoration: none;-->
<!--  color: #333;-->
<!--  background-color: #f2f2f2;-->
<!--  border: 1px solid #dcdcdc;-->
<!--  border-radius: 4px;-->
<!--  transition: all 0.3s ease;-->
<!--  font-size: 18px; /* 新增/修改的部分 */-->
<!--}-->

<!--.router-link:hover {-->
<!--  background-color: #e6e6e6;-->
<!--  border-color: #adadad;-->
<!--}-->

<!--/* 生成索引按钮样式 */-->
<!--.el-button[type="primary"] {-->
<!--  background-color: #409eff;-->
<!--  border-color: #409eff;-->
<!--  color: #fff;-->
<!--}-->

<!--.el-button[type="primary"]:hover {-->
<!--  background-color: #66b1ff;-->
<!--  border-color: #66b1ff;-->
<!--}-->

<!--/* 自定义滚动条样式（可选）*/-->
<!--.el-table::-webkit-scrollbar {-->
<!--  width: 6px;-->
<!--  height: 6px;-->
<!--}-->

<!--.el-table::-webkit-scrollbar-thumb {-->
<!--  background-color: rgba(0, 0, 0, 0.2);-->
<!--  border-radius: 3px;-->
<!--}-->

<!--.el-table::-webkit-scrollbar-track {-->
<!--  background-color: rgba(0, 0, 0, 0.1);-->
<!--}-->
<!--</style>-->

<style scoped>

/* 容器样式 */
div {
  padding: 20px;
}

/* 修改链接的字体大小 */
.router-link {
  display: inline-block;
  margin-right: 20px;
  padding: 10px 15px;
  text-decoration: none;
  color: #333;
  background-color: #f2f2f2;
  border: 1px solid #dcdcdc;
  border-radius: 4px;
  transition: all 0.3s ease;
  font-size: 18px; /* 新增/修改的部分 */
}

.router-link:hover {
  background-color: #e6e6e6;
  border-color: #adadad;
}

/* 修改按钮的字体大小 */
.el-button {
  margin-right: 10px;
  font-size: 16px; /* 新增/修改的部分 */
}

/* 生成索引按钮样式 */
.el-button[type="primary"] {
  background-color: #409eff;
  border-color: #409eff;
  color: #fff;
}

.el-button[type="primary"]:hover {
  background-color: #66b1ff;
  border-color: #66b1ff;
}

/* 表格样式 */
.el-table {
  border-radius: 4px;
  overflow: hidden;
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
  font-size: 21px;
}

/* 表头和单元格样式 */
.el-table th, .el-table td {
  text-align: center;
  padding: 10px 0;
}

/* 自定义滚动条样式（可选）*/
.el-table::-webkit-scrollbar {
  width: 6px;
  height: 6px;

}

.el-table::-webkit-scrollbar-thumb {
  background-color: rgba(0, 0, 0, 0.2);
  border-radius: 3px;
}

.el-table::-webkit-scrollbar-track {
  background-color: rgba(0, 0, 0, 0.1);
}
</style>
