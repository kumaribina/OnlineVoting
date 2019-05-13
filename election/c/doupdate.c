#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "ccgi.h"

#define MAX_NUMBER_CANDIDATE 20
#define MAX_NUMBER_VOTER 30
#define DBFILE "../data/election.txt"
#define TEMPFILE "../data/temp.txt"
#define LIST_PATH "../../cgi-bin/election/list.cgi"
#define LIST_URL "http://localhost/cgi-bin/election/list.cgi"
#define ADD_URL "http://localhost/cgi-bin/election/add.cgi"

int update(CGI_varlist *varlist);

struct election
{
    char eid[30];
    char ename[50];
    char description[100];
    char etype[15];
    char startDate[15];
    char endsDate[15];
    char resultDate[15];
    struct ecandidate
    {
        char candidateID[30];
        char voteCount[20];
        char isEmpty[3];
    } ecandidate[MAX_NUMBER_CANDIDATE];
    struct evoter
    {
        char voterID[30];
        char votedCandidateID[30];
        char voteDateTime[15];
        char isEmpty[3];
    } evoter[MAX_NUMBER_VOTER];
} election;

int main(int argc, char **argv)
{
    CGI_varlist *varlist;
    CGI_value *value;

    printf("%s%c%c\n",
           "Content-Type:text/html;charset=iso-8859-1", 13, 10);
           
    if ((varlist = CGI_get_all(0)) == 0)
    {
        printf("No CGI data received\r\n");
        return 0;
    }

    value = CGI_lookup_all(varlist, "operation");
    if (strcmp(value[0], "Update") == 0)
    {
        update(varlist);
    }
    else if (strcmp(value[0], "Back") == 0)
    {
        printf("<META HTTP-EQUIV=refresh CONTENT=\"1;URL=%s\">\n", LIST_URL);
    }

    CGI_free_varlist(varlist);
    return 0;
}

int update(CGI_varlist *varlist)
{
    FILE *fp, *fpt;
    CGI_value *value;

    
    printf("<html>"
           "<head>");
    printf("<title>Update Election</title>"
           "</head>"
           "<body>");

    fp = fopen(DBFILE, "rb");
    if (fp == NULL)
    {
        printf("<p>Oh dear, something went wrong with connecting database! %s</p>", strerror(errno));
        return 0;
    }
    fpt = fopen(TEMPFILE, "wb");
    if (fpt == NULL)
    {
        printf("<p>Oh dear, something went wrong with connecting database! %s</p>", strerror(errno));
        return 0;
    }

    while (fread(&election, sizeof(election), 1, fp) == 1)
    {
        value = CGI_lookup_all(varlist, "eid");
        if (strcmp(value[0], election.eid) == 0)
        {
            strcpy(election.eid, CGI_lookup_all(varlist, "eid")[0]);
            strcpy(election.ename, CGI_lookup_all(varlist, "ename")[0]);
            strcpy(election.description, CGI_lookup_all(varlist, "description")[0]);
            strcpy(election.etype, CGI_lookup_all(varlist, "etype")[0]);
            strcpy(election.startDate, CGI_lookup_all(varlist, "startDate")[0]);
            strcpy(election.endsDate, CGI_lookup_all(varlist, "endsDate")[0]);
            strcpy(election.resultDate, CGI_lookup_all(varlist, "resultDate")[0]);

            fwrite(&election, sizeof(election), 1, fpt);
            }
            else
            {
                fwrite(&election, sizeof(election), 1, fpt);
            }
    }

    fclose(fpt);
    fclose(fp);
    remove(DBFILE);
    rename(TEMPFILE, DBFILE);
    printf("<p>Successfully Updated the election record.</p>");
    printf("<p>Click back to see the updated election record.</p>");
    printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", LIST_PATH);
    printf("<input type=\"submit\" value=\"Back\" name=\"operation\"/>");
    printf("</FORM>");
    printf("</body></html>");
    return 0;
}