package com.qianfeng.service;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class NewConceptEnglishSplitter {

    public static void main(String[] args) {
        String inputFilePath = "C:\\Users\\30405\\Desktop\\新概念3.txt"; // 输入文件路径
        String outputDirectory = "S:\\project\\filedir2"; // 输出目录路径

        // 创建输出目录
        File directory = new File(outputDirectory);
        if (!directory.exists()) {
            directory.mkdirs();
        }

        try (BufferedReader reader = new BufferedReader(new FileReader(inputFilePath))) {
            String line;
            StringBuilder lessonContent = new StringBuilder();
            String lessonTitle = null;
            boolean isFirstLesson = true;

            while ((line = reader.readLine()) != null) {
                if (line.matches("LESSON\\d+\\s+.+")) {
                    // 保存上一课的内容
                    if (!isFirstLesson) {
                        saveLessonToFile(outputDirectory, lessonTitle, lessonContent.toString());
                    }
                    isFirstLesson = false;

                    // 开始新的一课
                    lessonTitle = line.split("\\s+", 2)[1].trim();
                    lessonContent = new StringBuilder();
                }
                lessonContent.append(line).append(System.lineSeparator());
            }

            // 保存最后一课的内容
            if (lessonTitle != null) {
                saveLessonToFile(outputDirectory, lessonTitle, lessonContent.toString());
            }

            System.out.println("All lessons have been saved successfully.");

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void saveLessonToFile(String directoryPath, String title, String content) {
        // 去掉文件名中的非法字符
        String fileName = title.replaceAll("[\\\\/:*?\"<>|]", "") + ".txt";
        File file = new File(directoryPath + fileName);

        try (BufferedWriter writer = new BufferedWriter(new FileWriter(file))) {
            writer.write(content);
            System.out.println("Saved lesson: " + title);
        } catch (IOException e) {
            System.err.println("Failed to save lesson: " + title);
            e.printStackTrace();
        }
    }
}
