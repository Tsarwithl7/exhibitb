package com.qianfeng.test.deeplearning4j.examples.nlp.word2vec;//package com.qianfeng.test.deeplearning4j.examples.nlp.word2vec;

import org.deeplearning4j.models.paragraphvectors.ParagraphVectors;
import org.deeplearning4j.text.documentiterator.LabelsSource;
import org.deeplearning4j.text.sentenceiterator.CollectionSentenceIterator;
import org.deeplearning4j.text.sentenceiterator.SentenceIterator;
import org.deeplearning4j.text.tokenization.tokenizer.preprocessor.CommonPreprocessor;
import org.deeplearning4j.text.tokenization.tokenizerfactory.DefaultTokenizerFactory;
import org.deeplearning4j.text.tokenization.tokenizerfactory.TokenizerFactory;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.*;
import java.util.*;
import java.util.stream.Collectors;

public class Doc2VecMultiFileTest {
    private static final Logger log = LoggerFactory.getLogger(Doc2VecMultiFileTest.class);
    private static final String VECTOR_OUTPUT_PATH = "S:\\project\\word2vecpath";
    private static final String Label_Output_Path = "S:\\project\\wordandlabelpath";

    private static final String UPLOAD_DIR_PATH = "S:\\project\\filedir";

    public static class CustomFileIterator {
        private final File directory;
        private List<String> filePaths;
        private int currentIndex;

        public CustomFileIterator(String directoryPath) {
            this.directory = new File(directoryPath);
            this.filePaths = new ArrayList<>();
            this.currentIndex = 0;
            init();
        }

        private void init() {
            File[] files = directory.listFiles((dir, name) -> name.endsWith(".txt"));
            if (files != null) {
                for (File file : files) {
                    filePaths.add(file.getAbsolutePath());
                }
            }
        }

        public boolean hasNext() {
            return currentIndex < filePaths.size();
        }

        public File nextFile() {
            if (!hasNext()) {
                return null;
            }
            return new File(filePaths.get(currentIndex++));
        }


        public static void main(String[] args) throws Exception {
            CustomFileIterator fileIterator = new CustomFileIterator(UPLOAD_DIR_PATH);
            Map<String, String> labelToContentMap = new HashMap<>();

            while (fileIterator.hasNext()) {
                File file = fileIterator.nextFile();
                String content = new String(java.nio.file.Files.readAllBytes(file.toPath()));
                labelToContentMap.put(file.getName(), content);
            }

            LabelsSource source = new LabelsSource(new ArrayList<>(labelToContentMap.keySet()));
            List<String> contents = new ArrayList<>(labelToContentMap.values());
            SentenceIterator sentenceIterator = new CollectionSentenceIterator(contents);


            List<String> labels = new ArrayList<>();
            List<String> fileContents = new ArrayList<>();

            while (fileIterator.hasNext()) {
                File file = fileIterator.nextFile();

                labels.add(file.getName());
                System.out.println("这篇文章长度是"+file.length());
                fileContents.add(new String(java.nio.file.Files.readAllBytes(file.toPath())));
            }

            TokenizerFactory token = new DefaultTokenizerFactory();
            token.setTokenPreProcessor(new CommonPreprocessor());

            ParagraphVectors vec = new ParagraphVectors.Builder()
                    .minWordFrequency(1)
                    .iterations(5)
                    .epochs(1)
                    .layerSize(50)
                    .learningRate(0.025)
                    .labelsSource(source)
                    .windowSize(5)
                    .iterate(sentenceIterator)
                    .trainWordVectors(false)
                    .tokenizerFactory(token)
                    .build();

            vec.fit();
            log.info("训练完成");

            writeDocVectors(vec, labelToContentMap,VECTOR_OUTPUT_PATH);
            writeDocLabels(labelToContentMap,Label_Output_Path);

        }


    public static void writeDocVectors(ParagraphVectors vectors, Map<String, String> labelToContentMap, String outputPath) throws IOException {
        // 假设calculateTFIDFAndExtractKeywords方法已经实现
        Map<String, List<String>> docKeywords = calculateTFIDFAndExtractKeywords(labelToContentMap);

        for (String label : vectors.getLabelsSource().getLabels()) {
            INDArray vector = vectors.getWordVectorMatrix(label);
            if (vector == null) {
                log.info("未找到标签 " + label + " 的向量。跳过...");
                continue;
            }

            StringBuilder vectorStr = new StringBuilder();
            for (int j = 0; j < vector.length(); j++) {
                vectorStr.append(vector.getDouble(j));
                if (j < vector.length() - 1) {
                    vectorStr.append(" ");
                }
            }

            String content = labelToContentMap.get(label);
            if (content != null) {
                String individualOutputPath = outputPath + "/" + label.replaceAll("[^a-zA-Z0-9\\.\\-]", "_") + ".txt";
                try (BufferedWriter bufferedWriter = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(individualOutputPath), "UTF-8"))) {
                    // 获取关键词
                    List<String> keywords = docKeywords.getOrDefault(label, Collections.emptyList());
//                    String keywordsStr = String.join(", ", keywords);

                    // 写入关键词标签，原文内容和向量
                    bufferedWriter.write(keywords.get(0) + "\t" + "\t" +content + "\t" + vectorStr);
                    System.out.println(keywords.get(0) + "\t" + content + "\t" + vectorStr);
                } catch (IOException e) {
                    log.error("写入文件 " + individualOutputPath + " 时发生错误", e);
                }
            }
        }
    }


        public static void writeDocLabels(Map<String, String> labelToContentMap, String outputPath) throws IOException {
            // 保留calculateTFIDFAndExtractKeywords方法的调用
            Map<String, List<String>> docKeywords = calculateTFIDFAndExtractKeywords(labelToContentMap);

            for (Map.Entry<String, String> entry : labelToContentMap.entrySet()) {
                String label = entry.getKey();
                String content = entry.getValue();

                String individualOutputPath = outputPath + "/" + label.replaceAll("[^a-zA-Z0-9\\.\\-]", "_") + ".txt";
                try (BufferedWriter bufferedWriter = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(individualOutputPath), "UTF-8"))) {
                    // 获取并写入第一个关键词，如果有的话
                    List<String> keywords = docKeywords.getOrDefault(label, Collections.emptyList());
                    String keyword = keywords.isEmpty() ? "" : keywords.get(0);

                    // 仅写入关键词（如果存在）和内容，中间用制表符\t分隔
                    bufferedWriter.write(keyword + "\t" + content);
                    System.out.println(keyword + "\t" + content);
                } catch (IOException e) {
                    System.err.println("写入文件 " + individualOutputPath + " 时发生错误: " + e.getMessage());
                    e.printStackTrace();
                }
            }
        }






        public static Map<String, List<String>> calculateTFIDFAndExtractKeywords(Map<String, String> labelToContentMap) {
            // 停用词列表
            Set<String> stopWords = Set.of("a","b","c","d","e","f","g","h","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","about","above","across","after","afterwards","again","against","all","almost","alone","along","already","also","although","always","am","among","amongst","amoungst","amount","an","and","another","any","anyhow","anyone","anything","anyway","anywhere","are","around","as","at","back","became","because","become","becomes","becoming","been","before","beforehand","behind","being","below","beside","besides","between","beyond","bill","both","bottom","but","by","can","cannot","cant","co","computer","con","could","couldnt","cry","de","describe","detail","done","down","due","during","each","eg","eight","either","eleven","else","elsewhere","empty","enough","etc","even","ever","every","everyone","everything","everywhere","except","few","fifteen","fify","fill","fire","first","five","for","former","formerly","forty","found","four","from","front","full","further","had","has","hasnt","he","hence","her","here","hereafter","hereby","herein","hereupon","hers","herself","him","himself","his","how","however","hundred","i","ie","if","in","inc","indeed","interest","into","is","it","its","itself","keep","last","latter","latterly","least","less","ltd","made","many","may","me","meanwhile","might","mill","mine","more","moreover","most","mostly","move","much","must","my","myself","name","namely","neither","never","nevertheless","next","nine","no","nobody","none","noone","nor","not","nothing","now","nowhere","of","off","often","on","once","one","only","onto","or","other","others","otherwise","our","ours","ourselves","out","over","own","part","per","perhaps","please","put","rather","re","same","seemed","seeming","seems","serious","several","she","should","show","side","since","sincere","six","sixty","so","some","somehow","someone","something","sometime","sometimes","somewhere","still","such","system","ten","than","that","the","their","them","themselves","then","thence","there","thereafter","thereby","therefore","therein","thereupon","these","they","thick","thin","third","this","those","though","three","through","throughout","thru","thus","to","together","too","top","toward","towards","twelve","twenty","two","un","under","until","up","upon","us","very","via","was","we","well","were","what","whatever","when","whence","whenever","where","whereafter","whereas","whereby","wherein","whereupon","wherever","whether","which","while","whither","who","whoever","whole","whom","whose","why","will","with","within","without","would","yet","you","your","yours","yourself","yourselves","be","have","do","say","get","make","go","know","take","see","come","think","look","want","give","use","find","tell","ask","work","seem","feel","try","leave","call","play","run","like","live","believe","hold","bring","happen","write","provide","sit","stand","lose","pay","meet","include","continue","set","learn","change","lead","understand","watch","follow","stop","create","speak","read","allow","add","spend","grow","open","walk","win","offer","remember","love","consider","appear","buy","wait","serve","die","send","expect","build","stay","fall","cut","reach","kill","remain","suggest","raise","pass","sell","require","report","decide","pull","return","explain","hope","develop","carry","break","receive","agree","support","hit","cover");

            Map<String, Map<String, Double>> tf = new HashMap<>();
            Map<String, Integer> df = new HashMap<>();
            Map<String, Double> idf = new HashMap<>();
            final int totalDocuments = labelToContentMap.size();

            // 计算TF和DF
            for (Map.Entry<String, String> entry : labelToContentMap.entrySet()) {
                String label = entry.getKey();
                String[] words = entry.getValue().toLowerCase().split("\\W+");

                Map<String, Double> wordCounts = new HashMap<>();
                for (String word : words) {
                    String stemmedWord = stemWord(word); // 这里模拟词干提取
                    if (!stopWords.contains(stemmedWord) && !stemmedWord.isEmpty()) {
                        wordCounts.put(stemmedWord, wordCounts.getOrDefault(stemmedWord, 0.0) + 1.0);
                    }
                }
                tf.put(label, wordCounts);

                for (String word : new HashSet<>(Arrays.asList(words))) {
                    String stemmedWord = stemWord(word);
                    if (!stopWords.contains(stemmedWord) && !stemmedWord.isEmpty()) {
                        df.put(stemmedWord, df.getOrDefault(stemmedWord, 0) + 1);
                    }
                }
            }

            // 计算IDF
            for (String word : df.keySet()) {
                idf.put(word, Math.log(totalDocuments / (double) df.get(word) + 1.0));
            }

            // 计算TF-IDF并选择关键词
            Map<String, List<String>> docKeywords = new HashMap<>();
            for (Map.Entry<String, Map<String, Double>> entry : tf.entrySet()) {
                String label = entry.getKey();
                Map<String, Double> wordsTf = entry.getValue();
                Map<String, Double> tfIdfScores = new HashMap<>();

                for (Map.Entry<String, Double> wordEntry : wordsTf.entrySet()) {
                    String word = wordEntry.getKey();
                    double score = wordEntry.getValue() * idf.getOrDefault(word, 0.0);
                    tfIdfScores.put(word, score);
                }

                List<String> keywords = tfIdfScores.entrySet().stream()
                        .sorted(Map.Entry.<String, Double>comparingByValue().reversed())
                        .limit(1)
                        .map(Map.Entry::getKey)
                        .collect(Collectors.toList());

                docKeywords.put(label, keywords);
            }

            return docKeywords;
        }

        // 简单的模拟词干提取方法
        private static String stemWord(String word) {
            // 优化处理以 "ing"、"ed" 或 "s" 结尾的单词，确保单词长度足够且不会因为去除后缀而变为空字符串
            if ((word.endsWith("ing") || word.endsWith("ed")) && word.length() > 3) {
                return word.substring(0, word.length() - 3);
            } else if (word.endsWith("s") && word.length() > 1) {
                return word.substring(0, word.length() - 1);
            }
            return word;
        }



    }
}
