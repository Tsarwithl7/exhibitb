import { createApp } from 'vue'
import ElementPlus from 'element-plus'
import 'element-plus/dist/index.css'
import App from './App.vue'
import router from "./router/index.js";
// import axios from "axios"
// axios.defaults.baseURL="http://localhost:8080"
// app.config.globalProperties.$http=axios
const app = createApp(App)

app.use(ElementPlus)
app.use(router); // 使用路由
app.mount('#app')