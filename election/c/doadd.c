#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ccgi.h"
#include <errno.h>

#define MAX_NUMBER_CANDIDATE 20
#define MAX_NUMBER_VOTER 30
#define ELECTIONFILE "../data/election.txt"
#define VOTERFILE "../data/voter.txt"
#define TEMPFILE "../data/temp.txt"
#define LIST_URL "../../cgi-bin/election/list.cgi"

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

struct voter
{
    char id[30];
    char firstName[50];
    char lastName[50];
    char gender[10];
    char mobile[20];
    char email[50];
    char fatherName[50];
    char dateOfBirth[15];
    char address[100];
} voter;

int main_()
{

    CGI_varlist *varlist;
    CGI_value  *value;
    FILE *fp, *fpv;
    int i;

    printf("%s%c%c\r\n",
           "Content-Type:text/html;charset=iso-8859-1", 13, 10);
    printf("<html>"
           "<head>");
    printf("<style>table{font-family:arial,sans-serif;border-collapse:collapse;width:100%}"
           "td,th{border:1px solid #ddd;text-align:left;padding:8px}"
           "tr:nth-child(even){background-color:#ddd}"
           "</style>");
    printf("<title>Add Election</title>"
           "</head>"
           "<body>");

    if ((varlist = CGI_get_all(0)) == 0)
    {
        printf("<p>Oh dear, Empty request! Nothing Added.</p>");
        return 0;
    }

    fp = fopen(ELECTIONFILE, "a");
    if(fp == NULL)
    {
        printf("<p>Oh dear, something went wrong with connecting database! %s %s</p>", strerror(errno), ELECTIONFILE);
        return 0;
    }

    fpv = fopen(VOTERFILE, "a");
    if (fpv == NULL)
    {
        printf("<p>Oh dear, something went wrong with connecting database! %s %s</p>", strerror(errno), VOTERFILE);
        return 0;
    }

    strcpy(election.eid, CGI_lookup_all(varlist, "eid")[0]);
    strcpy(election.ename, CGI_lookup_all(varlist, "ename")[0]);
    strcpy(election.description, CGI_lookup_all(varlist, "decription")[0]);
    strcpy(election.etype, CGI_lookup_all(varlist, "etype")[0]);
    strcpy(election.startDate, CGI_lookup_all(varlist, "startDate")[0]);
    strcpy(election.endsDate, CGI_lookup_all(varlist, "endsDate")[0]);
    strcpy(election.resultDate, CGI_lookup_all(varlist, "resultDate")[0]);

    value = CGI_lookup_all(varlist, "candidateID");
    for (i = 0; value[i] != 0; i++)
    {

        strcpy(election.ecandidate[i].candidateID, value[i]);
        strcpy(election.ecandidate[i].voteCount, "0");
        strcpy(election.ecandidate[i].isEmpty, "1");
    }

    while (fread(&voter, sizeof(voter), 1, fpv) == 1)
    {
        strcpy(election.evoter[i].voterID, voter.id);
        strcpy(election.evoter[i].votedCandidateID, "0"); 
        strcpy(election.evoter[i].voteDateTime, "0");
        strcpy(election.evoter[i].isEmpty, "1");
    }

    fwrite(&election, sizeof(election), 1, fp);
    fclose(fp);
    fclose(fpv);
    CGI_free_varlist(varlist);  /* free variable list */
    printf("<p>Added the election with Election ID = %s and Name = %s .</p>", election.eid, election.ename);
    printf("<p>Click back to see the updated election record.</p>");
    printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", LIST_URL);
    printf("<input type=\"submit\" value=\"Back\" name=\"operation\"/>");
    printf("</FORM>");
    printf("</body></html>");
    return 0;
}