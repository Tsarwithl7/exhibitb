    import { createRouter, createWebHistory } from 'vue-router';
    import Uploadfile from "../components/Uploadfile";
    import Blog from "../components/Blog";
    import MainContent from "../components/MainContent";
    import managefile from "../components/managefile"
    import indexpage from "../components/indexpage"
    import SearchPage from "../components/SearchPage";
    import Bookstore from "../components/Bookstore";
    import BookInfo from '../components/BookInfo';
    import manageindex from "../components/manageindex";
    import SearchComparison from "../components/SearchComparison";
    // import App from "../App";


    const routes = [
        // {path:'/searchcomparison',component: SearchComparison},
        {path: '/manageindex',component: manageindex},
        {path: '/bookstore',component: Bookstore},
        {path: '/',component: MainContent},
        {path: '/manage',component:managefile},
        { path: '/blog', component: Blog },
        {path: '/manage/upload',component:Uploadfile},
        {path: '/indexpage', component: indexpage },
        {path: '/searchpage',component: SearchPage},
        {
            path: '/:fileName',
            name: 'BookInfo',
            component: BookInfo,
        },
        {path: "/manage/createindex",component: indexpage},

        {
            path: '/compareall',
            name: 'SearchComparison',
            component: SearchComparison
        },
    ];

    const router = createRouter({
        history: createWebHistory(), // 使用HTML5 History模式
        routes,
    });

    export default router;