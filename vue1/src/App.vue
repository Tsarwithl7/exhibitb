<template>
  <div id="app" class="menu-container" :style="backgroundStyle">
    <router-link to="/manage" class="menu-button">现有书本管理</router-link>
    <router-link to="/" class="menu-button">返回首页</router-link>
    <router-link to="/bookstore" class="menu-button">查看书城</router-link>
    <router-view></router-view>
  </div>


<!--这是我的侧边栏-->
<!--  <div id="app">-->
<!--    <el-row class="tac">-->
<!--      <el-col :span="12">-->
<!--        <h5>默认颜色</h5>-->
<!--        <el-menu-->
<!--            default-active="2"-->
<!--            class="el-menu-vertical-demo"-->
<!--            @open="handleOpen"-->
<!--            @close="handleClose">-->
<!--          &lt;!&ndash; 这里是提供的sider标签内容 &ndash;&gt;-->
<!--        </el-menu>-->
<!--      </el-col>-->
<!--      <el-col :span="12">-->
<!--        <h5>自定义颜色</h5>-->
<!--        <el-menu-->
<!--            default-active="2"-->
<!--            class="el-menu-vertical-demo"-->
<!--            @open="handleOpen"-->
<!--            @close="handleClose"-->
<!--            background-color="#545c64"-->
<!--            text-color="#fff"-->
<!--            active-text-color="#ffd04b">-->
<!--          &lt;!&ndash; 这里是提供的另一个sider标签内容 &ndash;&gt;-->
<!--        </el-menu>-->
<!--      </el-col>-->
<!--    </el-row>-->
<!--    <div class="main-content">-->
<!--      <router-view></router-view>-->
<!--    </div>-->
<!--  </div>-->


</template>



<script>

export default {
  name: 'App',

  computed: {
    backgroundStyle() {
      return {
        backgroundImage: `url(${this.backgroundImage})`,
        backgroundSize: 'cover',
        backgroundRepeat: 'no-repeat',
        backgroundAttachment: 'fixed',

      };
    },
  },

  components: {
    // MainContent,

  },
  methods: {
    handleOpen(key, keyPath) {
      console.log(key, keyPath);
    },
    handleClose(key, keyPath) {
      console.log(key, keyPath);
    }
  }
};

function debounce(fn, delay) {
  let timer = null;
  return function() {
    const context = this;
    const args = arguments;
    clearTimeout(timer);
    timer = setTimeout(function() {
      fn.apply(context, args);
    }, delay);
  };
}

const _ResizeObserver = window.ResizeObserver;
window.ResizeObserver = class ResizeObserver extends _ResizeObserver {
  constructor(callback) {
    super(debounce(callback, 16));
  }
}



</script>


<!--<style scoped>-->
<!--#app {-->
<!--  padding: 20px;-->
<!--  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;-->
<!--}-->



<!--.router-link {-->
<!--  display: inline-block;-->
<!--  margin: 0 10px;-->
<!--  padding: 8px 16px;-->
<!--  text-decoration: none;-->
<!--  color: #4A4A4A;-->
<!--  background-color: #EFEFEF;-->
<!--  border: 1px solid #CCCCCC;-->
<!--  border-radius: 4px;-->
<!--  transition: color 0.3s ease, background-color 0.3s ease, border-color 0.3s ease;-->
<!--}-->

<!--.router-link:hover, .router-link:focus {-->
<!--  color: #FFFFFF;-->
<!--  background-color: #3273DC;-->
<!--  border-color: #3273DC;-->
<!--}-->

<!--.router-view {-->
<!--  margin-top: 20px;-->
<!--}-->

<!--/* Optional: Add some additional global styles for body to improve the overall look */-->
<!--body {-->
<!--  margin: 0;-->
<!--  font-size: 16px;-->
<!--  background-color: #F5F5F5;-->
<!--  color: #333;-->
<!--}-->
<!--</style>-->
<style scoped>
.menu-button {
  display: inline-block; /* 让它们像块级元素一样排列，但仍然在同一行 */
  margin: 0.5rem; /* 添加一些外边距 */
  padding: 0.5rem 1rem; /* 内边距，根据需要调整大小 */
  border: 1px solid #ddd;
  border-radius: 4px; /* 轻微的圆角边框 */
  background-color: #f7f7f7; /* 背景色 */
  text-decoration: none; /* 去除下划线 */
  color: #333; /* 文字颜色 */
  font-size: 1rem; /* 字体大小 */
  transition: background-color 0.3s, color 0.3s; /* 过渡动画 */
}

.menu-button:hover, .menu-button:focus {
  background-color: #e7e7e7; /* 鼠标悬停或聚焦时的背景色 */
  color: #000; /* 鼠标悬停或聚焦时的文字颜色 */
  outline: none; /* 移除聚焦时的轮廓 */
}

#app {
  position: relative;
  padding: 20px;
  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  overflow: hidden; /* 确保伪元素不会超出容器边界 */
}
/*.menu-container {*/
/*  display: flex; !* 使用Flexbox进行布局 *!*/
/*  justify-content: center; !* 水平居中 *!*/
/*  align-items: center; !* 垂直居中 *!*/
/*  padding: 1rem; !* 容器的内边距 *!*/
/*}*/
#app {
  text-align:  left;
  padding: 1rem; /* 容器的内边距 */
}

#app::before {
  content: "";
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background-image: url('/public/9609986dcb066d33610502bf8ae9235e.jpg'); /* 更正后的背景图片路径 */
  /*background-size: 100% 100%;*/
  background-size: cover;/* 或者使用一个固定的尺寸 */
  background-repeat: repeat;
  background-attachment: fixed;
  opacity: 0.28; /* 更正后的透明度设置 */
  z-index: -1; /* 确保背景图片在内容下方 */
}

.router-link {
  display: inline-block;
  margin: 0 10px;
  padding: 8px 16px;
  text-decoration: none;
  color: #4A4A4A;
  background-color: #EFEFEF;
  border: 1px solid #CCCCCC;
  border-radius: 4px;
  transition: color 0.3s ease, background-color 0.3s ease, border-color 0.3s ease;
}

.router-link:hover, .router-link:focus {
  color: #FFFFFF;
  background-color: #3273DC;
  border-color: #3273DC;
}

.router-view {
  margin-top: 20px;
}

body {
  margin: 0;
  font-size: 16px;
  background-color: #F5F5F5;
  color: #333;
}
</style>