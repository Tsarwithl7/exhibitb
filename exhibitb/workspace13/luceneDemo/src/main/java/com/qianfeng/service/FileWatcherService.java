package com.qianfeng.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.nio.file.*;

@Service
public class FileWatcherService {

    private final WatchService watchService;
    private final Path dirPath;
    private final SimpMessagingTemplate messagingTemplate;

    @Autowired
    public FileWatcherService(@Value("${file.upload-dir}") String uploadDir, SimpMessagingTemplate messagingTemplate) throws IOException {
        this.dirPath = Paths.get(uploadDir);
        this.watchService = FileSystems.getDefault().newWatchService();
        this.dirPath.register(watchService, StandardWatchEventKinds.ENTRY_CREATE, StandardWatchEventKinds.ENTRY_DELETE);
        this.messagingTemplate = messagingTemplate;
        startWatching();
    }

    private void startWatching() {
        Thread watchThread = new Thread(() -> {
            try {
                WatchKey key;
                while ((key = watchService.take()) != null) {
                    for (WatchEvent<?> event : key.pollEvents()) {
                        // 这里可以根据事件类型进行过滤，比如只处理文件创建或删除事件
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
