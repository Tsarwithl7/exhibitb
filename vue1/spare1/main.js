
// import { createApp } from 'vue'
// import ElementPlus from 'element-plus'
// import 'element-plus/dist/index.css'
//
//
// import App from './App.vue'
//
// createApp(App).mount('#app')

// main.ts
import { createApp } from 'vue'
import ElementPlus from 'element-plus'
import 'element-plus/dist/index.css'
import App from '../备份/App.vue'
// import axios from "axios"
// axios.defaults.baseURL="http://localhost:8080"
// app.config.globalProperties.$http=axios
const app = createApp(App)

app.use(ElementPlus)
app.mount('#app')