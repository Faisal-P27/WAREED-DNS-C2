
#pragma once



// Add a domain to the end of the list
void addDomainName(const char* domain);
// Get the next domain in the list
char* getNextDomain() ;
// Delete a domain from the list
int  deleteDomainName(const char* domain) ;

int ValidateNewCallBackDomain(char *NewDomain) ;

int DeleteCallBackDomain(char *DeleteDomain) ;
char* GetAllActiveDomains(int* DomainCount,int* DomainSize);
int SendSuccessDeleteCallBackDomain();
int GoSleep();