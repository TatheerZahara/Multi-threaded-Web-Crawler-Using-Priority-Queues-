# Multi-threaded-Web-Crawler-Using-Priority-Queues-
A skeleton of a multi-threaded web-crawler using priority queues. It can be integrated in a search engine for efficient  search results. Priority queues save the most commonly searched items/webpages, and multiple threads are used for fast traversal of queues.  This is just a skeleton, not the entire code. 

-wc.cpp is the main file. You can simply run it on a linux machine to see desired outputs. 


-The word document attached has a detailed explanation of our proposed methodology. Our methodology was inspired by previous research in multi-threading and priority queues for ordering most searched words and URLs. 

In short: Priority Queues are nested and used for sorting the most searched words and URLs. The main priority queue orders most searched word and each searched word starts another priority queue; this queue is used for ordering the most accessed URLs w.r.t. the searched word. 

Whenever, a word is searched in a web-browser, multiple threads traverse through the queues in different order to find desired results quickly and sort the queues too. 

