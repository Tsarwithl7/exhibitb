package com.qianfeng.service;

import org.springframework.stereotype.Service;

import java.util.concurrent.ConcurrentHashMap;

@Service
public class FileNameMappingService {
    private final ConcurrentHashMap<String, String> fileNameMapping = new ConcurrentHashMap<>();


    public void addMapping(String originalName, String uploadedName) {
        fileNameMapping.put(originalName, uploadedName);
    }


    public String getUploadedName(String originalName) {
        return fileNameMapping.get(originalName);
    }

    // 删除映射
    public void removeMapping(String originalName) {
        fileNameMapping.remove(originalName);
    }
}
