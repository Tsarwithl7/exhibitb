<!--<template>-->
<!--  <div class="upload-page">-->

<!--    &lt;!&ndash; 文件上传部分，更新了 accept 属性允许上传 .txt 文件 &ndash;&gt;-->
<!--    <el-upload-->
<!--        class="upload-demo"-->
<!--        action="http://localhost:8088/upload"-->
<!--        name="files"-->
<!--        :on-preview="handlePreview"-->
<!--        :on-remove="handleRemove"-->
<!--        :before-remove="beforeRemove"-->
<!--        multiple-->
<!--        :limit="50"-->
<!--        :on-exceed="handleExceed"-->
<!--        :file-list="fileList"-->
<!--        accept=".txt"> &lt;!&ndash; 允许上传.txt文件 &ndash;&gt;-->
<!--      <el-button size="small" type="primary">点击上传</el-button>-->
<!--      <template #tip>-->
<!--        <div class="el-upload__tip">只能上传.txt文件</div>-->
<!--      </template>-->
<!--    </el-upload>-->

<!--    &lt;!&ndash; 输入框部分 &ndash;&gt;-->
<!--    <div class="input-section">-->
<!--      <el-input v-model="input" placeholder="请输入内容"></el-input>-->
<!--      <el-button type="primary" @click="submitInput">提交</el-button>-->
<!--    </div>-->
<!--  </div>-->
<!--</template>-->

<!--<script>-->
<!--export default {-->
<!--  data() {-->
<!--    return {-->
<!--      fileList: [],-->
<!--      input: ''-->
<!--    };-->
<!--  },-->
<!--  methods: {-->
<!--    handleRemove(file, fileList) {-->
<!--      this.$confirm('你真的希望删除该文件吗?', '提示', {-->
<!--        confirmButtonText: '确定',-->
<!--        cancelButtonText: '取消',-->
<!--        type: 'warning'-->
<!--      }).then(() => {-->
<!--        // 用户确认删除后的处理-->
<!--        const url = `http://localhost:8088/delete/${encodeURIComponent(file.name)}`;-->
<!--        fetch(url, { method: 'DELETE' })-->
<!--            .then(response => {-->
<!--              if (response.ok) {-->
<!--                this.$message({-->
<!--                  type: 'success',-->
<!--                  message: '删除成功!'-->
<!--                });-->
<!--                // 成功删除后，才从UI上移除文件-->
<!--                const index = fileList.indexOf(file);-->
<!--                if (index !== -1) {-->
<!--                  fileList.splice(index, 1);-->
<!--                }-->
<!--              } else {-->
<!--                this.$message({-->
<!--                  type: 'error',-->
<!--                  message: '删除失败!'-->
<!--                });-->
<!--              }-->
<!--            })-->
<!--            .catch(error => {-->
<!--              console.error('删除文件失败:', error);-->
<!--              this.$message({-->
<!--                type: 'error',-->
<!--                message: '删除失败!'-->
<!--              });-->
<!--            });-->
<!--      }).catch(() => {-->
<!--        // 用户取消删除，什么也不做，保留文件列表状态-->
<!--        this.$message({-->
<!--          type: 'info',-->
<!--          message: '已取消删除'-->
<!--        });-->
<!--      });-->
<!--    },-->

<!--    handlePreview(file) {-->
<!--      console.log(file);-->
<!--    },-->
<!--    handleExceed(files, fileList) {-->
<!--      alert(`最大上传数量为50个文件，你尝试上传了${files.length}个文件，总共选择了${files.length + fileList.length}个文件。`);-->
<!--    },-->
<!--    beforeRemove(file, fileList) {-->
<!--      return this.$confirm(`确定移除 ${file.name}？`);-->
<!--    },-->
<!--    submitInput() {-->
<!--      this.$confirm('是否确认提交?', '确认提示', {-->
<!--        confirmButtonText: '确定',-->
<!--        cancelButtonText: '取消',-->
<!--        type: 'warning'-->
<!--      }).then(() => {-->
<!--        const url = `http://localhost:8088/upload-text`;-->
<!--        const formData = new FormData();-->
<!--        formData.append("content", this.input);-->

<!--        fetch(url, {-->
<!--          method: 'POST',-->
<!--          body: formData-->
<!--        }).then(response => {-->
<!--          if (!response.ok) {-->
<!--            throw new Error(`HTTP error! status: ${response.status}`);-->
<!--          }-->
<!--          return response.json();-->
<!--        }).then(data => {-->
<!--          // 检查响应中是否包含fileName字段-->
<!--          if (data.fileName) {-->
<!--            // 服务器返回的文件名-->
<!--            const newFileName = data.fileName;-->
<!--            console.log("newfilename是："+newFileName)-->

<!--            // 添加到fileList中-->
<!--            this.fileList.push({ name:newFileName });-->

<!--            // 强制更新视图-->
<!--            this.$forceUpdate(); // 强制组件重新渲染-->

<!--            this.$message({-->
<!--              type: 'success',-->
<!--              message: '提交成功!'-->
<!--            });-->

<!--            this.input = ''; // 清空输入框内容-->
<!--          } else {-->
<!--            // 如果没有fileName，认为提交失败-->
<!--            this.$message({-->
<!--              type: 'error',-->
<!--              message: '提交失败!'-->
<!--            });-->
<!--          }-->
<!--        }).catch(error => {-->
<!--          console.error('提交失败:', error);-->
<!--          this.$message({-->
<!--            type: 'error',-->
<!--            message: '提交失败!'-->
<!--          });-->
<!--        });-->
<!--      }).catch(() => {-->
<!--        this.$message({-->
<!--          type: 'info',-->
<!--          message: '已取消提交'-->
<!--        });-->
<!--      });-->
<!--    }-->
<!--  }-->
<!--}-->
<!--</script>-->

<template>
  <div>
<!--    <input type="file" @change="handleFileChange" accept=".txt" >-->
<!--    <input type="file" @change="handleImageChange"  accept="image/jpeg" />-->
      <label for="text-file" class="file-input-label">上传文本文件 (.txt)</label>
      <input type="file" class="custom-file-upload" id="text-file" @change="handleFileChange" accept=".txt" />

      <label for="image-file" class="file-input-label">上传图片文件 (JPG)</label>
      <input type="file" class="custom-file-upload" id="image-file" @change="handleImageChange" accept="image/jpeg" />

      <!-- 其他元素 -->

    <input type="text" v-model="fileName" placeholder="请输入书本名" />
    <br>
    <br>

    <button :disabled="isSubmitDisabled" @click="submitForm">确认提交</button>
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
  <br>
</template>

<script setup>
import { ref, computed } from 'vue';

const file = ref(null);
const image = ref(null);
const fileName = ref('');

const handleFileChange = (event) => {
  file.value = event.target.files[0];
};

const handleImageChange = (event) => {
  image.value = event.target.files[0];
};

const isSubmitDisabled = computed(() => {
  return !file.value || !image.value || !fileName.value.trim();
});

const submitForm = async () => {
  if (!file.value || !image.value || !fileName.value.trim()) return;

  let formData = new FormData();
  formData.append('file', file.value);
  formData.append('image', image.value);
  formData.append('fileName', fileName.value.trim());

  try {
    const response = await fetch('http://localhost:8088/upload', {
      method: 'POST',
      body: formData,
    });
    if (!response.ok) {
      throw new Error('response was not ok');
    }
    const result = await response.text();
    alert(result); // Show success or error message from server
  } catch (error) {
    console.error('Upload failed:', error);
    alert('Upload failed');
  }
};
</script>
<style>
.file-input-label {
  display: inline-block;
  margin-top: 10px;
  margin-bottom: 5px;
  font-weight: bold;
  font-size: 31px; /* 设置标签的字体大小 */
}


input[type="file"] {
  display: block;
  margin-bottom: 50px;
}

/* 修改输入书本名的输入框的字体大小 */
input[type="text"] {
  font-size: 16px;
  width: 19%;
  padding: 8px 10px;
  margin-bottom: 10px;
}
.custom-file-upload {
  display: inline-block;
  padding: 12px 24px;
  cursor: pointer;
  /*color: #fff;*/
  /*background-color: #4CAF50;*/
  /*border-radius: 5px;*/
  /*border: 2px solid #ccc;*/
  font-size:25px;
}


button {
  font-size: 21px;
  padding: 10px 15px;
  background-color: #4CAF50;
  color: white;
  border: none;
  border-radius: 5px;
  cursor: pointer;
}

button:hover {
  background-color: #45a049;
}


input[type="file"], input[type="text"], button {
  transition: all 0.3s ease; /* 平滑过渡效果 */
}
</style>
