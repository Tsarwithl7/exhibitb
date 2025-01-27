package com.qianfeng.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.messaging.simp.SimpMessagingTemplate;

import java.io.IOException;
import java.nio.file.*;
import java.util.ArrayList;
import java.util.List;

@Service
public class IndexWatcherService {

    private final WatchService watchService;
    private final Path dirPath;
    private final SimpMessagingTemplate messagingTemplate;

    @Autowired
    public IndexWatcherService(@Value("${INDEX_DIR}") String indexDir, SimpMessagingTemplate messagingTemplate) throws IOException {
        this.dirPath = Paths.get(indexDir);
        this.watchService = FileSystems.getDefault().newWatchService();
        this.dirPath.register(watchService, StandardWatchEventKinds.ENTRY_CREATE, StandardWatchEventKinds.ENTRY_DELETE);
        this.messagingTemplate = messagingTemplate;
        startWatching();
    }


    public List<Path> getAllIndexes() {
        List<Path> indexPaths = new ArrayList<>();
        try (DirectoryStream<Path> stream = Files.newDirectoryStream(dirPath)) {
            for (Path entry : stream) {

                if (Files.isDirectory(entry)) {
                    indexPaths.add(entry);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return indexPaths;
    }
    private void startWatching() {
        Thread watchThread = new Thread(() -> {
            try {
                WatchKey key;
                while ((key = watchService.take()) != null) {
                    for (WatchEvent<?> event : key.pollEvents()) {
                        // 这里根据事件类型进行过滤，比如只处理文件创建或删除事件
                        // 将变化通知到前端
                        messagingTemplate.convertAndSend("/topic/fileChanges", "File changed: " + event.context());
                    }
                    key.reset();
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            } catch (Exception e) {
                e.printStackTrace();
            }
        });
        watchThread.start();
    }
}
