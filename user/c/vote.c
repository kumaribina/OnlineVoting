#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "ccgi.h"

#define MAX_NUMBER_CANDIDATE 20
#define DBFILE "../data/election.txt"
#define TEMPFILE "../data/temp.txt"
#define CANDIDATE_DBFILE "../data/candidate.txt"
#define DOVOTE_URL "../../cgi-bin/user/dovote.cgi"

struct election
{
    char eid[30];
    char ename[50];
    char description[100];
    char etype[15];
    char startDate[15];
    char endsDate[15];
    char resultDate[15];
    struct candidate
    {
        char candidateID[30];
        char voteCount[20];
        char isEmpty[3];
    } candidate[MAX_NUMBER_CANDIDATE];
    struct voter
    {
        char voterID[30];
        char votedCandidateID[30];
        char voteDateTime[15];
        char isEmpty[3];
    } voter[MAX_NUMBER_VOTER];
} election;

struct candidate
{
    char id[30];
    char firstName[50];
    char lastName[50];
    char gender[10];
    char mobile[20];
    char email[50];
    char fatherName[50];
    char dateOfBirth[15];
    char party[50];
    char symbol[20];
    char address[100];

} candidate;

int main()
{

    FILE *fp, *fpc;
    int i = 0;

    CGI_varlist *varlist;
    CGI_value *value;


    printf("%s%c%c\n",
           "Content-Type:text/html;charset=iso-8859-1", 13, 10);
    printf("<html>"
           "<head>");

    printf("<title>Vote</title>"
           "</head>"
           "<body>");

    fp = fopen(DBFILE, "rb");
    if (fp == NULL)
    {
        printf("<p>Oh dear, something went wrong with connecting database! %s</p>", strerror(errno));
        return 0;
    }

    fpc = fopen(CANDIDATE_DBFILE, "rb");
    if (fpc == NULL)
    {
        printf("<p>Oh dear, something went wrong with connecting database! %s</p>", strerror(errno));
        return 0;
    }

    if ((varlist = CGI_get_all(0)) == 0)
    {
        printf("<p>Oh dear, Empty request! Nothing Added.</p>");
        fclose(fp);
        return 0;
    }

    value = CGI_lookup_all(varlist, "selected");

    printf("<FORM ACTION=\"%s\" METHOD=\"POST\">", DOVOTE_URL);

    while (fread(&election, sizeof(election), 1, fp) == 1)
    {
        if (strcmp(value[0], election.eid) == 0)
        {
            
            // printf("<p>%s</p>", election.eid);
            printf("<h3>Election for : %s</h3>", election.ename);
            printf("<p>%s</p>", election.description);
            // printf("<p>%s</p>", election.etype);

            //Print the candidates info with header here
            printf("<table border=\"1\">");
            printf("<tr>"
                   "<th>Choose</th>"
                   "<th>Name</th>"
                   "<th>Gender</th>"
                   "<th>Party</th>"
                   "<th>Symbol</th>"
                   "<th>Vote Received</th>"
                   "</tr>");
            for (i = 0; i < MAX_NUMBER_CANDIDATE; i++)
            {
                if (strcmp(election.vote[i].isEmpty, "1") == 0)
                {
                    while (fread(&candidate, sizeof(candidate), 1, fpc) == 1)
                    {
                        if (strcmp(candidate.id, election.vote[i].candidateID) == 0)
                        {
                            printf("<tr>");
                            printf("<input type=\"hidden\" name=\"electionid\" value=\"%s\" />", election.eid);
                            printf("<td><input type=\"radio\" name=\"candidateid\" value=\"%s\" /></td>", election.vote[i].candidateID);
                            printf("<td>%s %s</td>", candidate.firstName, candidate.lastName);
                            printf("<td>%s</td>", candidate.gender);
                            printf("<td>%s</td>", candidate.party);
                            printf("<td>%s</td>", candidate.symbol);
                            printf("<td>%s</td>", election.vote[i].voteCount);
                            printf("</tr>");
                        }
                    }
                }
                rewind(fpc); //set the candidate file pointer to start
            }

            printf("</table>");
        }
       
    }
    

    printf("<input type=\"submit\" value=\"Vote\" name=\"operation\"/>");
    printf("<input type=\"submit\" value=\"Back\" name=\"operation\"/>");
    //print ends
    printf("</form>");
    printf("</body></html>");
    CGI_free_varlist(varlist);
    fclose(fp);
    fclose(fpc);
    return 0;
}