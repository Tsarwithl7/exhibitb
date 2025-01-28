<!--<template>-->
<!--  <img alt="Vue logo" 01_自己写一个App组件="./assets/logo.png">-->
<!--  <HelloWorld msg="Welcome to Your Vue.js App"/>-->
<!--</template>-->

<!--<script>-->
<!--import HelloWorld from './components/HelloWorld.vue'-->

<!--export default {-->
<!--  name: 'App',-->
<!--  components: {-->
<!--    HelloWorld-->
<!--  }-->
<!--}-->
<!--</script>-->

<!--<style>-->
<!--#app {-->
<!--  font-family: Avenir, Helvetica, Arial, sans-serif;-->
<!--  -webkit-font-smoothing: antialiased;-->
<!--  -moz-osx-font-smoothing: grayscale;-->
<!--  text-align: center;-->
<!--  color: #2c3e50;-->
<!--  margin-top: 60px;-->
<!--}-->
<!--</style>-->
<template>
  <div class="app">
  <img alt="Vue logo" src="./assets/logo.png">
  <GuaComponent></GuaComponent>
  <GuaComponent></GuaComponent>
  <MovieComponent v-for="movie in MovieComponent" :key="movie.id" :title="movie.title" :rating="movie.ratings"></MovieComponent>  <!--表单渲染-->
  <TableComponent></TableComponent>
<!--  <PersonComponent></PersonComponent>-->

<h1>你好啊！</h1>
<!--  <PersonComponent></PersonComponent>-->

</div>
</template>

<script>
     import MovieComponent from './components/MovieComponent'
     import GuaComponent from './components/GuaComponent'
     import TableComponent from "@/components/TableComponent";
     import axios from "axios";
     // import PersonComponent from "@/components/Person";


   export default {
     name:'App',//组件名
     data:function (){
       return{
         MovieComponent:[
           {id:1,title:"jack",ratings:8.7},
           {id:1,title:"jack2",ratings:8.7},
           {id:1,title:"jack3",ratings:8.7},
           {id:1,title:"jack4",ratings:8.7},

         ],
         tableData:[]


       }
     },


     created:function(){/*属于组件自己的生命周期的函数*/
       console.log("App组件被创建了！")
       axios.get("http://localhost:8088/sku").then( (response)=> {
         console.log(response)
         this.tableData=response.data
       })

     },
     mounted:function () {
       console.log("App组件被挂载")
     },

     components:{
       TableComponent,
       MovieComponent,
       // PersonComponent,
       GuaComponent,
         //组测组件
     }

   }
</script>

<style>
   .app{
     background-color: #ddd;
     box-shadow:0 0 10px;
     border-radius: 10px;
     padding: 20px;
     text-align: center;
     margin: 0 auto;
     width: 80%}

</style>