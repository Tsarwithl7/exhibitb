package com.qianfeng.controller;

import com.qianfeng.dao.SearchResults;
import com.qianfeng.service.IndexService;
import com.qianfeng.service.KNNIndexSearcherService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.time.Instant;
import java.time.temporal.ChronoUnit;
import java.util.*;
import java.util.stream.Collectors;

@CrossOrigin(origins = "http://localhost:8080")
@RestController
public class IndexController {

    @Value("${INDEX_DIR}")
    private String indexDir;

    private final IndexService indexService;
    private final KNNIndexSearcherService knnIndexSearcherService;

    @Autowired
    public IndexController(IndexService indexService,KNNIndexSearcherService knnIndexSearcherService) {
        this.indexService = indexService;
        this.knnIndexSearcherService=knnIndexSearcherService;
    }

    @GetMapping("/searchknn")
    public List<IndexDetails> searchKNN(
            @RequestParam String query,
            @RequestParam(defaultValue = "10") int k) {
        try {
            return knnIndexSearcherService.searchKNN(query, k);
        } catch (IOException e) {
            // Handle the exception, perhaps return an error response
            e.printStackTrace();
            return null;
        }
    }





    @GetMapping("/manage/createindex")
    @ResponseBody
    public String createIndex() {
        try {
            indexService.createIndex();
            return "Index created successfully.";
        } catch (Exception e) {
            e.printStackTrace();
            return "Failed to create index: " + e.getMessage();
        }
    }

    @GetMapping("/compareSearch")
    public List<IndexDetails> compareSearch(@RequestParam("query") String query) {
        try {
            return indexService.compareScoringMethods(query);
        } catch (IOException e) {
            e.printStackTrace();

            return List.of(); // Java 9+的简写
        }
    }

    @GetMapping("/calculatetfidf")
    public  List<IndexDetails> calculatetfidf(@RequestParam("query") String query){
        try {
            return indexService.calculate_tfidf(query);
        } catch (IOException e) {
            e.printStackTrace();

            return List.of(); // Java 9+的简写
        }
    }


    @GetMapping("/calculatebm25")
    public  List<IndexDetails> calculatebm25(@RequestParam("query") String query){
        try {
            return indexService.calculate_bm25(query);
        } catch (IOException e) {
            e.printStackTrace();

            return List.of(); // Java 9+的简写
        }
    }




    public static long[] calculateTimeRange(int hoursBefore) {
        // 获取当前时间戳
        long endTime = Instant.now().toEpochMilli();
        // 计算开始时间戳
        long startTime = Instant.now().minus(hoursBefore, ChronoUnit.HOURS).toEpochMilli();

        return new long[]{startTime, endTime};
    }


    @GetMapping("/search")
    public List<IndexController.IndexDetails> searchDocuments(
            @RequestParam(name = "queryText", required = false) String queryText,
            @RequestParam(name = "low" ) int low,
            @RequestParam(name = "high") int high,
            @RequestParam(name = "interval", required = false, defaultValue = "0") int interval,
            @RequestParam(name = "flag", required = false, defaultValue = "0") int flag) {


        List<IndexController.IndexDetails> indexDetailsList=new ArrayList<>();

        long[] timelist=calculateTimeRange(interval);

        try {

            List<IndexController.IndexDetails> results = indexService.searchDocuments(queryText, low, high,timelist[0],timelist[1],flag);



            for (IndexDetails result:results) {
                indexDetailsList.add(result);
            }

        } catch (Exception e) {
            e.printStackTrace();

        }
        return  indexDetailsList;
    }


    @PostMapping("/updateDocument")
    public String updateDocument(@RequestParam String fileName, @RequestParam String newContent) {
        try {
            indexService.updateDocumentContent(fileName, newContent);
            return "Document updated successfully.";
        } catch (Exception e) {
            e.printStackTrace();
            return "Failed to update document: " + e.getMessage();
        }
    }


//    查看目录是否为空
    @GetMapping("/checkindex")
    public Boolean checkIndexDirectory() {
        File folder = new File(indexDir);
        System.out.println("进入check方案！");
        // 检查文件夹是否存在并且是否为空
        if (folder.exists() && folder.isDirectory() && folder.list().length > 0) {
            System.out.println("1");
            return true;
        } else {
            System.out.println("0");
            return false;
        }
    }


/*从索引库中所有索引，以list形式放回
*
* */

    @GetMapping("/all")
    public List<IndexDetails> searchAllDocuments() {
        try {
            return indexService.searchAll();
        } catch (IOException e) {

            e.printStackTrace();
            return List.of();
        }
    }



static Set<String> expectedResultsStr = new HashSet<>(Arrays.asList("Smash-and-grab.txt", "A noble gangster.txt", "Fifty pence worth of trouble.txt","Flying cats.txt","Fifty pence worth of trouble.txt","The double life of Alfred Bloggs.txt"));

    @GetMapping("/compareall")
    @ResponseBody
        public List<SearchResults> compareAll(@RequestParam(name = "query") String query, @RequestParam(name = "expectedResults", required = false) String expectedResultsStr) {
        HashSet<String> expectedResults = new HashSet<>();
        if (expectedResultsStr != null && !expectedResultsStr.isEmpty()) {
            // 将逗号分隔的字符串转换为 HashSet
            expectedResults = Arrays.stream(expectedResultsStr.split(","))
                    .collect(Collectors.toCollection(HashSet::new));
        }

        try {
            return indexService.compareall(query, expectedResults); // 确保compareall方法接受HashSet作为参数
        } catch (Exception e) {
            e.printStackTrace();
            return new ArrayList<>();
        }
    }





        @GetMapping("/indexpage")
        public List<FileDetails> listAllFiles(@Value("${INDEX_DIR}") String indexDir) {

            File directory = new File(indexDir);
            File[] fileList = directory.listFiles();
            List<FileDetails> fileDetailsList = new ArrayList<>();

            if (fileList != null) {
                for (File file : fileList) {
                    String fileName = file.getName();
                    long fileSize = file.length();
                    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                    String lastModified = sdf.format(new Date(file.lastModified()));

                    String contentPreview = "Preview not available";

                    FileDetails fileDetails = new FileDetails(fileName, contentPreview, fileSize, lastModified);
                    System.out.println(fileDetails);
                    fileDetailsList.add(fileDetails);
                }
            }

            return fileDetailsList;
        }





    public static class FileDetails {


        @Override
        public String toString() {
            return "FileDetails{" +
                    "fileName='" + fileName + '\'' +
                    ", content='" + content + '\'' +
                    ", fileSize=" + fileSize +

                    ", lastModified='" + lastModified + '\'' +
                    '}';
        }

        private String fileName;
        private String content;
        private long fileSize;
        private String lastModified;

        public FileDetails(String fileName, String content, long fileSize) {
            this.fileName = fileName;
            this.content = content;
            this.fileSize = fileSize;
        }
        public FileDetails(String fileName, String content, long fileSize,String lastModified) {
            this.fileName = fileName;
            this.content = content;
            this.fileSize = fileSize;
            this.lastModified=lastModified;
        }

        public void setFileSize(long fileSize) {
            this.fileSize = fileSize;
        }

        public String getLastModified() {
            return lastModified;
        }

        public void setLastModified(String lastModified) {
            this.lastModified = lastModified;
        }

        public String getFileName() {
            return fileName;
        }

        public void setFileName(String fileName) {
            this.fileName = fileName;
        }

        public String getContent() {
            return content;
        }

        public void setContent(String content) {
            this.content = content;
        }

        public long getFileSize() {
            return fileSize;
        }

        public String getFormattedFileSize() {
            String[] units = {"B", "KB", "MB", "GB", "TB"};
            int index = 0;
            double size = fileSize;

            while (size >= 1024 && index < units.length - 1) {
                size /= 1024;
                index++;
            }

            DecimalFormat decimalFormat = new DecimalFormat("#.##");
            return decimalFormat.format(size) + " " + units[index];
        }
    }







        public static class IndexDetails {
        public IndexDetails(String fileName, String content, long fileSize) {
            this.fileName = fileName;
            this.content = content;
            this.fileSize = fileSize;
        }

        public IndexDetails(String fileName, String content, long fileSize, long knnsearchtime) {
            this.fileName = fileName;
            this.content = content;
            this.fileSize = fileSize;
            this.knnsearchtime = knnsearchtime;
        }

        public IndexDetails(String fileName, String content, long fileSize, String imagePath, long knnsearchtime) {
            this.fileName = fileName;
            this.content = content;
            this.fileSize = fileSize;
            this.imagePath=imagePath;
            this.knnsearchtime = knnsearchtime;
        }




        public IndexDetails() {
        }

        public IndexDetails(String fileName, String content, long last_modified, long wordCount, long fileSize, double tfidf, double bm25) {
            this.fileName = fileName;
            this.content = content;
            this.last_modified = last_modified;
            this.wordCount = wordCount;
            this.fileSize = fileSize;
            this.tfidf = tfidf;
            this.bm25 = bm25;

        }





   public IndexDetails(String fileName, String content, String imagePath, long last_modified, long wordCount, long fileSize, double tfidf, double bm25, String lastModified, long bm25SearchTime, long tfidfSearchTime, long knnsearchtime) {
            this.fileName = fileName;
            this.content = content;
            this.imagePath = imagePath;
            this.last_modified = last_modified;
            this.wordCount = wordCount;
            this.fileSize = fileSize;
            this.tfidf = tfidf;
            this.bm25 = bm25;
            this.lastModified = lastModified;
            this.bm25SearchTime = bm25SearchTime;
            this.tfidfSearchTime = tfidfSearchTime;
            this.knnsearchtime = knnsearchtime;
        }

            public IndexDetails(String fileName, String content, long last_modified, long wordCount, String imagePath, long fileSize, double v, double v1) {
            this.fileName=fileName;
            this.content=content;
            this.last_modified=last_modified;
            this.wordCount=wordCount;
            this.imagePath=imagePath;
            this.fileSize=fileSize;

            }

            public IndexDetails(String fileName, String content, long last_modified, String lastModified, long wordCount, String imagePath, long fileSize, double v, double v1) {
                this.fileName=fileName;
                this.content=content;
                this.last_modified=last_modified;
                this.lastModified=lastModified;
                this.wordCount=wordCount;
                this.imagePath=imagePath;
                this.fileSize=fileSize;
            }

            public long getFileSize() {
            return fileSize;
        }

        public void setFileSize(long fileSize) {
            this.fileSize = fileSize;
        }

        public double getTfidf() {
            return tfidf;
        }

        public void setTfidf(double tfidf) {
            this.tfidf = tfidf;
        }

        public double getBm25() {
            return bm25;
        }

        public void setBm25(double bm25) {
            this.bm25 = bm25;
        }

        public long getLast_modified() {
            return last_modified;
        }

        public void setLast_modified(long last_modified) {
            this.last_modified = last_modified;
        }

        public String getFileName() {
            return fileName;
        }

        public void setFileName(String fileName) {
            this.fileName = fileName;
        }

        public String getContent() {
            return content;
        }

        public void setContent(String content) {
            this.content = content;
        }

        public long getWordCount() {
            return wordCount;
        }

        public void setWordCount(long wordCount) {
            this.wordCount = wordCount;
        }

        public String getImagePath() {
            return imagePath;
        }

        public void setImagePath(String imagePath) {
            this.imagePath = imagePath;
        }

        public String getLastModified() {
            return lastModified;
        }

        public void setLastModified(String lastModified) {
            this.lastModified = lastModified;
        }

        public long getBm25SearchTime() {
            return bm25SearchTime;
        }

        public void setBm25SearchTime(long bm25SearchTime) {
            this.bm25SearchTime = bm25SearchTime;
        }

        public long getTfidfSearchTime() {
            return tfidfSearchTime;
        }

        public void setTfidfSearchTime(long tfidfSearchTime) {
            this.tfidfSearchTime = tfidfSearchTime;
        }

        public long getKnnsearchtime() {
            return knnsearchtime;
        }

        public void setKnnsearchtime(long knnsearchtime) {
            this.knnsearchtime = knnsearchtime;
        }

        public String fileName;
        public String content;
        public String imagePath;
        public long last_modified;
        public long wordCount;
        public long fileSize;
        public double tfidf;
        public double bm25;
        public String lastModified;

        public long bm25SearchTime;
        public long tfidfSearchTime;
        public long knnsearchtime;
        public long defaultsearchtime;
        private int searchMethod; // 搜索方式，比如"TF-IDF", "BM25", "KNN"
        public double precisiondefault;
        public double precisionknn;
        public double precisiontfidf;
        public double precisionbm25;
        public double recalldefault;
        public double recalltfidf;
        public double recallbm25;
        public double recallknn;


            public IndexDetails(String fileName, String content, String imagePath, long last_modified, long wordCount, long fileSize, double tfidf, double bm25, String lastModified, long bm25SearchTime, long tfidfSearchTime, long knnsearchtime, int searchMethod) {
            this.fileName = fileName;
            this.content = content;
            this.imagePath = imagePath;
            this.last_modified = last_modified;
            this.wordCount = wordCount;
            this.fileSize = fileSize;
            this.tfidf = tfidf;
            this.bm25 = bm25;
            this.lastModified = lastModified;
            this.bm25SearchTime = bm25SearchTime;
            this.tfidfSearchTime = tfidfSearchTime;
            this.knnsearchtime = knnsearchtime;
            this.searchMethod = searchMethod;
        }

        public IndexDetails(String fileName, String content, String imagePath, long last_modified, long wordCount, long fileSize, double tfidf, double bm25, String lastModified, long bm25SearchTime, long tfidfSearchTime, long knnsearchtime, long defaultsearchtime, int searchMethod) {
            this.fileName = fileName;
            this.content = content;
            this.imagePath = imagePath;
            this.last_modified = last_modified;
            this.wordCount = wordCount;
            this.fileSize = fileSize;
            this.tfidf = tfidf;
            this.bm25 = bm25;
            this.lastModified = lastModified;
            this.bm25SearchTime = bm25SearchTime;
            this.tfidfSearchTime = tfidfSearchTime;
            this.knnsearchtime = knnsearchtime;
            this.defaultsearchtime = defaultsearchtime;
            this.searchMethod = searchMethod;
        }

            public IndexDetails(String fileName, String content, String imagePath, long last_modified, long wordCount, long fileSize, double tfidf, double bm25, String lastModified, long bm25SearchTime, long tfidfSearchTime, long knnsearchtime, long defaultsearchtime, int searchMethod, double precisiondefault, double precisionknn, double precisiontfidf, double precisionbm25, double recalldefault, double recalltfidf, double recallbm25, double recallknn) {
                this.fileName = fileName;
                this.content = content;
                this.imagePath = imagePath;
                this.last_modified = last_modified;
                this.wordCount = wordCount;
                this.fileSize = fileSize;
                this.tfidf = tfidf;
                this.bm25 = bm25;
                this.lastModified = lastModified;
                this.bm25SearchTime = bm25SearchTime;
                this.tfidfSearchTime = tfidfSearchTime;
                this.knnsearchtime = knnsearchtime;
                this.defaultsearchtime = defaultsearchtime;
                this.searchMethod = searchMethod;
                this.precisiondefault = precisiondefault;
                this.precisionknn = precisionknn;
                this.precisiontfidf = precisiontfidf;
                this.precisionbm25 = precisionbm25;
                this.recalldefault = recalldefault;
                this.recalltfidf = recalltfidf;
                this.recallbm25 = recallbm25;
                this.recallknn = recallknn;
            }

            public double getPrecisiondefault() {
                return precisiondefault;
            }

            public void setPrecisiondefault(double precisiondefault) {
                this.precisiondefault = precisiondefault;
            }

            public double getPrecisionknn() {
                return precisionknn;
            }

            public void setPrecisionknn(double precisionknn) {
                this.precisionknn = precisionknn;
            }

            public double getPrecisiontfidf() {
                return precisiontfidf;
            }

            public void setPrecisiontfidf(double precisiontfidf) {
                this.precisiontfidf = precisiontfidf;
            }

            public double getPrecisionbm25() {
                return precisionbm25;
            }

            public void setPrecisionbm25(double precisionbm25) {
                this.precisionbm25 = precisionbm25;
            }

            public double getRecalldefault() {
                return recalldefault;
            }

            public void setRecalldefault(double recalldefault) {
                this.recalldefault = recalldefault;
            }

            public double getRecalltfidf() {
                return recalltfidf;
            }

            public void setRecalltfidf(double recalltfidf) {
                this.recalltfidf = recalltfidf;
            }

            public double getRecallbm25() {
                return recallbm25;
            }

            public void setRecallbm25(double recallbm25) {
                this.recallbm25 = recallbm25;
            }

            public double getRecallknn() {
                return recallknn;
            }

            public void setRecallknn(double recallknn) {
                this.recallknn = recallknn;
            }

            public int getSearchMethod() {
            return searchMethod;
        }

        public void setSearchMethod(int searchMethod) {
            this.searchMethod = searchMethod;
        }


        public long getDefaultsearchtime() {
            return defaultsearchtime;
        }

        public void setDefaultsearchtime(long defaultsearchtime) {
            this.defaultsearchtime = defaultsearchtime;
        }


    }







    //一键清空的功能
    @PostMapping("/clearindex")
    public ResponseEntity<String> clearIndexDirectory() {
        File folder = new File(indexDir);
        if (folder.exists() && folder.isDirectory()) {
            // 清空目录
            File[] files = folder.listFiles();
            if (files != null) {
                for (File file : files) {
                    try {
                        Files.deleteIfExists(file.toPath());
                    } catch (IOException e) {
                        e.printStackTrace();
                        // 使用兼容性更好的方法
                        return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body("清空索引目录时出错: " + e.getMessage());
                    }
                }
                return ResponseEntity.ok("索引目录已清空");
            } else {
                return ResponseEntity.ok("索引目录已经为空");
            }
        } else {
            return ResponseEntity.badRequest().body("索引目录不存在或不是一个目录");
        }
    }



}