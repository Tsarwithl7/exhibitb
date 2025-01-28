package com.qianfeng.test.deeplearning4j.examples.nlp.word2vec;

/**
 * @Author: Zhidong Huang
 * @Date: 2024/3/1 14:54
 */

import org.deeplearning4j.models.embeddings.loader.WordVectorSerializer;
import org.deeplearning4j.models.word2vec.Word2Vec;
import org.deeplearning4j.text.sentenceiterator.LineSentenceIterator;
import org.deeplearning4j.text.sentenceiterator.SentenceIterator;
import org.deeplearning4j.text.tokenization.tokenizer.preprocessor.CommonPreprocessor;
import org.deeplearning4j.text.tokenization.tokenizerfactory.DefaultTokenizerFactory;
import org.deeplearning4j.text.tokenization.tokenizerfactory.TokenizerFactory;
import org.slf4j.Logger;

import org.slf4j.LoggerFactory;

import java.io.File;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;
import java.util.Set;

/**
 * Created by zhidong  on 2/28/2024.
 */
public class Word2VecTest {

    private static Logger log = LoggerFactory.getLogger(Word2VecTest.class);
    private static String outputPath = "S:\\project\\word2vec.txt";
    public static void main(String[] args) throws Exception {
        //输入文本文件的目录
//        File inputTxt = new File("S:\\project\\filedir\\doc0faf9af2-616d-42f5-91c5-62cbecb39ad0.txt");
        File inputTxt = new File("S:\\project\\raw_sentences2.txt");
        log.info("开始加载数据...."+inputTxt.getName());
        System.out.println(inputTxt);
        //加载数据
        SentenceIterator iter = new LineSentenceIterator(inputTxt);
        //切词操作
        TokenizerFactory token = new DefaultTokenizerFactory();
        //去除特殊符号及大小写转换操作
        token.setTokenPreProcessor(new CommonPreprocessor());
        log.info("训练模型....");
        List<String> stopWords = List.of("a","did","b","c","d","e","f","g","h","j","k","l","m","n","o","p","q","r","sent","s","t","used","u","v","w","x","y","z","went","about","above","across","after","afterwards","again","against","all","almost","alone","along","already","also","although","always","am","among","amongst","amoungst","amount","an","and","another","any","anyhow","anyone","anything","anyway","anywhere","are","around","as","at","back","became","because","become","becomes","becoming","been","before","beforehand","behind","being","below","beside","besides","between","beyond","bill","both","bottom","but","by","can","cannot","cant","co","computer","con","could","couldnt","cry","de","describe","detail","done","down","due","during","each","eg","eight","either","eleven","else","elsewhere","empty","enough","etc","even","ever","every","everyone","everything","everywhere","except","few","fifteen","fify","fill","fire","first","five","for","former","formerly","forty","found","four","from","front","full","further","had","has","hasnt","he","hence","her","here","hereafter","hereby","herein","hereupon","hers","herself","him","himself","his","how","however","hundred","i","ie","if","in","inc","indeed","interest","into","is","it","its","itself","keep","last","latter","latterly","least","less","ltd","made","many","may","me","meanwhile","might","mill","mine","more","moreover","most","mostly","move","much","must","my","myself","name","namely","neither","never","nevertheless","next","nine","no","nobody","none","noone","nor","not","nothing","now","nowhere","of","off","often","on","once","one","only","onto","or","other","others","otherwise","our","ours","ourselves","out","over","own","part","per","perhaps","please","put","rather","re","same","seemed","seeming","seems","serious","several","she","should","show","side","since","sincere","six","sixty","so","some","somehow","someone","something","sometime","sometimes","somewhere","still","such","system","ten","than","that","the","their","them","themselves","then","thence","there","thereafter","thereby","therefore","therein","thereupon","these","they","thick","thin","third","this","those","though","three","through","throughout","thru","thus","to","together","too","top","toward","towards","twelve","twenty","two","un","under","until","up","upon","us","very","via","was","we","well","were","what","whatever","when","whence","whenever","where","whereafter","whereas","whereby","wherein","whereupon","wherever","whether","which","while","whither","who","whoever","whole","whom","whose","why","will","with","within","without","would","yet","you","your","yours","yourself","yourselves","be","have","do","say","get","make","go","know","take","see","come","think","look","want","give","use","find","tell","ask","work","seem","feel","try","leave","call","play","run","like","live","believe","hold","bring","happen","write","provide","sit","stand","lose","pay","meet","include","continue","set","learn","change","lead","understand","watch","follow","stop","create","speak","read","allow","add","spend","grow","open","walk","win","offer","remember","love","consider","appear","buy","wait","serve","die","send","expect","build","stay","fall","cut","reach","kill","remain","suggest","raise","pass","sell","require","report","decide","pull","return","explain","hope","develop","carry","break","receive","agree","support","hit","cover","was", "been", "were", "baby-sat", "bore", "born", "beaten", "became", "become", "began", "begun", "bent", "bet", "bitten", "blown", "broke", "broken", "brought", "built", "burnt/burned", "bought", "could", "cast", "caught", "chose", "come", "cost", "cut", "dealt", "dug", "did", "done", "drew", "dreamt", "drank", "driven", "eaten", "fell", "fallen", "fed", "felt", "fought", "found", "flown", "forbidden", "forgot", "forgiven", "froze", "got", "given", "gone", "grown", "hung/hanged", "had", "heard", "hidden", "hit", "held", "hurt", "kept", "knew", "laid", "led", "learned", "left", "lent", "let", "lain", "lit/lighted", "lost", "made", "might", "meant", "met", "mistaken", "misunderstood", "paid", "proved/proven", "put", "quit/quitted", "read", "rebuilt", "retold", "rid/ridded", "ridden", "risen", "run", "sawed/sawn", "said", "seen", "sought", "sold", "sent", "set", "sewn/sewed", "shaken", "shone/shined", "shot", "shown", "sung", "sunk", "sat", "slept", "smelt", "sown/sewed", "spoken", "spelled/spelt", "spent", "spat/spit", "sprung", "stood", "stolen", "swept", "swum", "taken", "taught", "torn", "well", "very", "really", "quite", "too", "often", "always", "never", "sometimes", "here", "there", "now", "yesterday", "today", "tomorrow", "early", "late", "fast", "slowly", "hard", "softly", "loudly", "quietly", "carefully", "easily", "well", "badly", "quickly", "slowly", "beautifully", "nicely", "happily", "sadly", "brightly", "clearly", "loudly", "angrily", "gently", "patiently", "suddenly","just","told", "thought", "thrown", "understood", "woken", "worn", "wet/wetted", "would", "won", "written","wrote");

// 在构建Word2Vec对象时，添加过滤器来过滤停用词
        Word2Vec vec = new Word2Vec.Builder()
                .minWordFrequency(4)
                .iterations(99)
                .layerSize(50)
                .seed(42)
                .windowSize(10)
                .iterate(iter)
                .tokenizerFactory(token)
                .stopWords(stopWords)
                .build();
        log.info("配置模型....");
        vec.fit();
        log.info("输出词向量....");
        WordVectorSerializer.writeWordVectors(vec, outputPath);
        log.info("相似的词:");
        //获取相似的词
        Collection<String> lst = vec.wordsNearest("day", 10);
        System.out.println(lst);
        //获取某词对应的向量
        log.info("向量获取:");
        double[] wordVector = vec.getWordVector("day");
        System.out.println(Arrays.toString(wordVector));
    }
}

