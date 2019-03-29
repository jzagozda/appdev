#include <stdio.h>
#include <curl/curl.h>
int main(int argc, char **argv){
	CURL *curl;
	CURLcode res;
	char id[20], email[40], poststring[100];
	if(argc < 3){
		printf("Usage: %s [ID] [email]\n", argv[0]);
		return 1; //program stops, as the user did not give enough information
	}
	sprintf(poststring, "ID=%s&email=%s", argv[1], argv[2]);

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, "http://www.cc.puv.fi/~e1900031/curl.php");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, poststring);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
			printf("Error in curl_easy_perform().\n");
		else
			printf("Post data in sent\n");
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	return 0;

}
