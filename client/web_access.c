#include "main.h"

int max_curl_buf;
int curl_writer_point;

int curl_writer(char *data, size_t size, size_t nmemb, char *writerData)
{
	static 
	int real_size;
	
	real_size = size*nmemb;
	
	if(max_curl_buf <= curl_writer_point + real_size)
		real_size = max_curl_buf - curl_writer_point - 1;
	
	if(real_size <= 0)
		return 0;
	
	memcpy(writerData + curl_writer_point, data, real_size);
	curl_writer_point += real_size;
	writerData[curl_writer_point] = 0;
	//printf("output:%s¥n", data);
	
	return 1;
}

int fetch_url(char *url, char *dest, int size)
{
#ifdef USE_LIBCURL
	CURL *curl;
	CURLcode res;
	
	max_curl_buf = size;
	curl_writer_point = 0;
	
	curl = curl_easy_init();
	if(curl) 
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, dest);

		res = curl_easy_perform(curl);
		
		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	else
	{
		return 0;
	}
	
	return 1;
#else
	return 0;
#endif
}
