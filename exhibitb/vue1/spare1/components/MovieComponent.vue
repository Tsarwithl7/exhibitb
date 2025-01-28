<template>
 <div>
    <h1>{{title}}</h1>
    <span>{{rating}}</span>
    <button @click="fun">点击收藏</button>
<!--    <h1>{{message}}</h1>-->
 </div>
</template>

<script>
export default {
  name:'MovieA',
  props:["title","rating"] , /*自定义属性进行渲染*/
  data:function () {
    return{

    }
  },

  created:function(){/*属于组件自己的生命周期的函数*/
    console.log("Movie组件被创建了！")
  },

  methods:{
    fun(){
     alert("收藏成功")
    }
  }



}
</script>