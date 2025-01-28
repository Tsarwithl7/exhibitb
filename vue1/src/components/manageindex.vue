<template>
  <div>
    <h1>管理索引</h1>
    <div>
      <input type="text" v-model="searchText" placeholder="根据内容搜索...">
      <button @click="search">搜索</button>
    </div>
    <br>
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
          {{ scope.row.fileName.substring(0, 11) + (scope.row.fileName.length > 11 ? '...' : '') }}
        </template>
      </el-table-column>


      <el-table-column
          label="内容"
          width="300">
        <template v-slot="scope">
          <!-- 判断是否处于编辑状态 -->
          <div v-if="editingIndex === scope.$index">
            <el-input
                type="textarea"
                :rows="4"
                v-model="scope.row.content">
            </el-input>
          </div>
          <div v-else>
            {{ scope.row.content.length > 20 ? scope.row.content.substring(0, 20) + '...' : scope.row.content}}
          </div>
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
          width="135">
        <template v-slot="scope">
          <div v-if="editingIndex === scope.$index">
            <el-button @click.prevent="submitEdit(scope.$index, scope.row)" type="text" size="small">提交</el-button>
            <el-button @click.prevent="cancelEdit()" type="text" size="small">返回</el-button>
          </div>
          <div v-else>
            <el-button @click.prevent="editRow(scope.$index, scope.row)" type="text" size="small">编辑</el-button>
            <el-button @click.prevent="deleteRow(scope.$index)" type="text" size="small">移除</el-button>
          </div>
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
</template>

<script>
import axios from 'axios';

export default {
  name: 'ManageIndex',
  data() {
    return {
      searchText: '',
      tableData: [],
      searchClicked: false, // 标记搜索按钮是否被点击过
      editingIndex: -1, // 增加这一行来初始化编辑索引
    };
  },
  created() {
    axios.get('http://localhost:8088/all')
        .then(response => {
          this.tableData = response.data.map(item => ({
            fileName: item.fileName,
            content: item.content,
            size: item.wordCount,

          }));
        })
        .catch(error => {
          console.error('Error fetching index documents:', error);

        });
  },
  methods: {
    search() {
      this.searchClicked = true;

      if (this.searchText === '') {
        alert('搜索框为空');
        return;
      }


      axios.get(`http://your-api-url/search?query=${this.searchText}`)
          .then(response => {
            this.tableData = response.data;
          })
          .catch(error => {
            console.error('Error searching indexes:', error);
          });
    },
    editRow(index, row) {

      if (this.editingIndex === index) {
        this.editingIndex = -1; // 取消编辑状态
      } else {
        // 否则，设置当前行为编辑状态
        this.editingIndex = index;
      }
    },
    deleteRow(index) {
      this.tableData.splice(index, 1);
      // 如果当前编辑的行被删除，重置编辑状态
      if (this.editingIndex === index) {
        this.editingIndex = -1;
      }
    },
    submitEdit(index, row) {
      // 创建表单数据对象
      const formData = new FormData();
      formData.append('fileName', row.fileName);
      formData.append('newContent', row.content);

      axios.post('http://localhost:8088/updateDocument', formData, {
        headers: {
          'Content-Type': 'multipart/form-data'
        }
      })
          .then(response => {
            console.log(response.data); // 成功响应
            this.editingIndex = -1; // 退出编辑模式
            this.$message.success('Document updated successfully.');
          })
          .catch(error => {
            console.error(error);
            this.$message.error('Failed to update document.');
          });
    },


    cancelEdit() {
      this.editingIndex = -1; // 取消编辑状态
    },
  }
}
</script>

<style>
/* 添加样式以美化页面 */
input[type="text"] {
  padding: 5px;
  margin-right: 10px;
}

button {
  padding: 5px 10px;
  background-color: #007bff;
  color: #fff;
  border: none;
  cursor: pointer;
}

p {
  color: red;
}
</style>
