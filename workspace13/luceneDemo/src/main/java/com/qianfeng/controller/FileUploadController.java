package com.qianfeng.controller;


import com.qianfeng.service.FileNameMappingService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.scheduling.annotation.Async;
import org.springframework.util.FileCopyUtils;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import java.io.*;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;

import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.*;

@CrossOrigin(origins = "http://localhost:8080")
    @RestController
    public class FileUploadController {

        @Value("${file.upload-dir}")
        private String uploadDir;
        private final FileNameMappingService fileNameMappingService;

        @Autowired
        public FileUploadController(FileNameMappingService fileNameMappingService) {
            this.fileNameMappingService = fileNameMappingService;
        }


    @Async
    @PostMapping("/upload")
    public String uploadFile(@RequestPart("file") MultipartFile file,
                             @RequestPart("image") MultipartFile image,
                             @RequestParam("fileName") String fileName) {
        if (file.isEmpty() || image.isEmpty()) {
            return "上传失败，请选择文件和图片";
        }

        if (!file.getOriginalFilename().endsWith(".txt")) {
            return "只能上传.txt格式的文件";
        }

        // 校验 fileName 是否已输入
        if (fileName == null || fileName.trim().isEmpty()) {
            return "您未输入文章标题";
        }

        String finalFileName = fileName + ".txt";
        String imagePath = fileName + ".jpg";

        try {
            // 保存文档
            String documentPath = uploadDir + File.separator + finalFileName;
            File directory = new File(uploadDir);
            if (!directory.exists()) {
                directory.mkdirs();
            }
            Path docPath = Paths.get(documentPath);
            Files.write(docPath, file.getBytes());

            // 保存图片
            String imageSavePath = uploadDir + File.separator + imagePath;
            Path imgPath = Paths.get(imageSavePath);
            Files.write(imgPath, image.getBytes());

            System.out.println("Uploaded file name: " + finalFileName + ", Image name: " + imagePath);
        } catch (IOException e) {
            e.printStackTrace();
            return "上传失败：" + e.getMessage();
        }

        return "上传成功";
    }

    @Async
    @DeleteMapping("/delete/{fileName}")
    public ResponseEntity<String> deleteFile(@PathVariable("fileName") String fileName) {
        if (fileName == null || fileName.trim().isEmpty()) {
            return ResponseEntity.badRequest().body("文件名不能为空");
        }

        String finalFileName = fileName.trim() + ".txt";
        String imagePath = fileName.trim() + ".jpg";

        try {
            // 删除文档
            Path docPath = Paths.get(uploadDir, finalFileName);
            Files.deleteIfExists(docPath);

            // 删除图片
            Path imgPath = Paths.get(uploadDir, imagePath);
            Files.deleteIfExists(imgPath);

            return ResponseEntity.ok("文件删除成功");
        } catch (IOException e) {
            e.printStackTrace();
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body("文件删除失败");
        }
    }


    @GetMapping("/bookstore")
    public List<FileDetails> listFiles() {
        File directory = new File(uploadDir);
        File[] fileList = directory.listFiles((dir, name) -> name.endsWith(".txt"));
        List<FileDetails> fileDetailsList = new ArrayList<>();

        if (fileList != null) {
            for (File file : fileList) {
                try {
                    String fileName = file.getName();
                    String baseName = fileName.substring(0, fileName.lastIndexOf('.'));

                    String imagePathsmall = baseName + ".jpg";
                    String imagePath = "http://localhost:8088/images/" + imagePathsmall;
                    System.out.println("imagepath是："+imagePath);

//                    File imageFile = new File(directory, imagePath);
//                    String imageBase64 = ""; // 初始化图片的Base64表示
//                    if (imageFile.exists()) {
//                        // 将图片文件转换为Base64编码的字符串
//                        byte[] imageBytes = Files.readAllBytes(imageFile.toPath());
//                         imageBase64 = Base64.getEncoder().encodeToString(imageBytes);
//                        // 构造Data URL
//                        imagePath = "data:image/jpeg;base64," + imageBase64;
//                    } else {
//                        imagePath = "No image available";
//                    }

                    byte[] fileContent = FileCopyUtils.copyToByteArray(new FileInputStream(file));
                    String content = new String(fileContent, StandardCharsets.UTF_8);
                    long fileSize = file.length();
                    long lastModified = file.lastModified();

                    // 格式化日期
                    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                    String formattedDate = sdf.format(new Date(lastModified));

                    // 只取文件内容的前30个字符
                    content = content.substring(0, Math.min(content.length(), 30));

                    FileDetails fileDetails = new FileDetails(fileName, content, fileSize, formattedDate, imagePath);
                    fileDetailsList.add(fileDetails);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        // 打印文件详情
        for (FileDetails fileDetails : fileDetailsList) {
            System.out.println(fileDetails.toString());
        }
        return fileDetailsList;
    }


    @RestController
    @CrossOrigin(origins = "http://localhost:8080")
    public class FileController {

        @GetMapping(value = "/{fileName}")
        public ResponseEntity<Map<String, Object>> readFile(@PathVariable String fileName) {
            String filePath = uploadDir + "/" + fileName;
            String  imagePath1 = fileName.substring(0, fileName.length() - 4);
            String imagePathshort = imagePath1 + ".jpg";
            String imagePath="http://localhost:8088/images/"+imagePathshort;


            System.out.println("imagePath:"+imagePath);

            try {
                // 读取文件内容
                List<String> lines = Files.readAllLines(Paths.get(filePath));
                StringBuilder content = new StringBuilder();
                for (String line : lines) {
                    content.append(line).append("\n");
                }

                // 获取文件大小
                long fileSize = Files.size(Paths.get(filePath));

                // 构建响应体
                Map<String, Object> response = new HashMap<>();
                response.put("fileName", fileName);
                response.put("content", content.toString());
                response.put("fileSize", fileSize);
                response.put("imagePath",imagePath);

                return ResponseEntity.ok().body(response);
            } catch (IOException e) {
                e.printStackTrace();
                return ResponseEntity.notFound().build(); // 如果文件不存在或读取失败，返回404
            }
        }

    }


    public class FileDetails {
        private String fileName;
        private String content;
        private long fileSize;
        private String lastModified; 
        private String imagePath;

        public FileDetails(String fileName, String content, long fileSize, String lastModified, String imagePath) {
            this.fileName = fileName;
            this.content = content;
            this.fileSize = fileSize;
            this.lastModified = lastModified;
            this.imagePath = imagePath;
        }

        public void setFileSize(long fileSize) {
            this.fileSize = fileSize;
        }

        @Override
        public String toString() {
            return "FileDetails{" +
                    "fileName='" + fileName + '\'' +
                    ", content='" + content + '\'' +
                    ", fileSize=" + fileSize +
                    ", lastModified='" + lastModified + '\'' +
                    ", imagePath='" + imagePath + '\'' +
                    '}';
        }

        public String getImagePath() {
            return imagePath;
        }

        public void setImagePath(String imagePath) {
            this.imagePath = imagePath;
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

        public String getLastModified() {
            return lastModified;
        }

        public void setLastModified(String lastModified) {
            this.lastModified = lastModified;
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
            return decimalFormat.format(size) + "" + units[index];
        }

        // 更新 toJsonString 方法以包含 lastModified
        public String toJsonString() {
            return "{\"fileName\": \"" + fileName + "\", \"content\": \"" + content +
                    "\", \"fileSize\": " + fileSize + ", \"lastModified\": \"" + lastModified +
                    "\", \"formattedFileSize\": \"" + getFormattedFileSize() + "\"}";
        }
    }
    }

