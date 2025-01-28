<template>
  <el-table
      :data="tableData"
      style="width: 100%"
      :row-class-name="tableRowClassName">
    <el-table-column
        prop="id"
        label="编号"
        width="180">
    </el-table-column>
    <el-table-column
        prop="name"
        label="姓名"
        width="180">
    </el-table-column>
    <el-table-column
        prop="image"
        label="图片地址">
    </el-table-column>
  </el-table>
</template>

<style>
.el-table .warning-row {
  background: oldlace;
}

.el-table .success-row {
  background: #f0f9eb;
}
</style>

<script>
import axios from "axios";

export default {
  methods: {
    tableRowClassName({rowIndex}) {
      if (rowIndex === 1) {
        return 'warning-row';
      } else if (rowIndex === 3) {
        return 'success-row';
      }
      return '';
    }
  },
  created:function(){/*属于组件自己的生命周期的函数*/
    console.log("App组件被创建了！")
    axios.get("http://localhost:8088/sku").then( (response)=> {
      console.log(response)
      this.tableData=response.data
    })

  },

  data() {
    return {
      tableData: []
    }
  }
}
</script>