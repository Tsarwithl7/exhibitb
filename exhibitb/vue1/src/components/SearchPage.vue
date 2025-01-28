<template>
  <div id="app">
    <div class="w" id="header-2013">

      <div id="search-2013">
        <div class="i-search ld">
          <ul id="shelper" class="hide"></ul>
          <form @submit.prevent="query">
            <div class="form">
              <input type="text" class="text" v-model="search.queryString">
              <input type="button" value="搜索" class="button" @click="query">
            </div>
            <input type="hidden" v-model="search.price">
            <input type="hidden" v-model="search.page">
          </form>
        </div>
      </div>

    </div>
  </div>
</template>

<script>
export default {
  name: 'SearchPage',
  data() {
    return {
      search: {
        queryString: '',
        price: '',
        page: 1
      }
    }
  },
  methods: {
    query() {
      // 这里实现查询逻辑
      console.log('执行查询', this.search);
    },
    filter(key, value) {
      this.search[key] = value;
      this.query();
    },
    changePage(p) {
      this.search.page += p;
      this.query();
    }
  }
};
</script>

<style scoped>

</style>
