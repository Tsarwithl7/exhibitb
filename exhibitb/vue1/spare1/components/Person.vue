<template>
  <div class="person">
     <h2>姓名：{{name}}</h2>
     <h2>年龄：{{age}}</h2>
     <button @click="changeName">修改姓名</button>
     <button @click="changeAge">修改年龄</button>
     <button @click="ShowTel">查看联系方式</button>
     <h2></h2>

  </div>
</template>

<script>
   export default {
    name:'PersonComponent',
     //组件由内部的data传递数据
     data(){
       return{
         name:'张三',
         age:18,
         tel:'1238342342343'
       }
     },
     methods:{
       changeName(){
       this.name='zhang-san'


       },
       changeAge(){
         this.age+=1;
       },


       ShowTel(){
         alert(this.tel)

       }
     }
   }

</script>

<style scoped>
.person{
  background-color: #87CEEB;
  box-shadow:0 0 10px;
  border-radius: 10px;
  padding: 20px;
}

button{
  margin: 0 5px;


}
</style>