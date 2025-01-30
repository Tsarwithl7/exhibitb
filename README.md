Design of Vertical Search Engine based on Lucene

Project Background:
Lucene is an open-source full-text search engine toolkit, providing a robust architecture for building search engines rather than being a complete search engine itself. In the Java development environment, Lucene is a mature and widely used open-source tool.

Developing a vertical search engine based on Lucene is a key experimental component of the Information Retrieval course. With advancements in vector representations and approximate nearest neighbor algorithms, the latest version, Lucene 9.0, has introduced these new features, significantly improving information retrieval performance.

Project Description:
This project aims to design a vertical search engine using Lucene, exploring different techniques provided by Lucene to analyze the differences in storage efficiency, query speed, and retrieval accuracy. The goal is to support the Information Retrieval course by providing an educational tool that demonstrates various retrieval methodologies.

Implementation Details:
Data Source:
The search engine does not include data collection. Instead, commonly used text datasets will be selected as the raw data source.
Indexing and Ranking Models:

A Bag-of-Words (BoW) model will be used to construct the index.
Both TF-IDF and BM25 will be implemented for relevance scoring.
Vector-Based Indexing and Nearest Neighbor Search:

A high-dimensional vector index will be created to support nearest neighbor search.
This implementation will leverage the Hierarchical Navigable Small World (HNSW) graph algorithm, addressing the needs of data scientists and machine learning researchers who require vector-based document indexing.
Performance and Accuracy Comparison:

Multiple retrieval methods will be evaluated in terms of efficiency and precision, providing insights into their trade-offs.
Extensibility for Experimental Learning:

The system will retain document data extension interfaces, facilitating additional experiments and coursework enhancements for students.
This project serves as a practical demonstration of modern search techniques and their impact on retrieval performance and accuracy, offering valuable insights for students in the Information Retrieval course.
